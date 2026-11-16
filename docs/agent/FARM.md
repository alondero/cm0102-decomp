# Recovery farm

The default route for decompilation sessions. `scripts/cmre_farm.py` is a
deterministic scheduler. It keeps many headless `opencode run` workers
translating one function each, re-tests every candidate itself, copies passes
into `recovery/c/`, and runs `cmre accept` serially. Accepted functions are
audited afterwards by a separate worker pool. No LLM coordinates the loop.

Policy (Adam, 2026-11-13): the differential gate plus the farm's mechanical
checks decide acceptance. Review is an asynchronous audit, not a gate. The
fidelity rules in AGENTS.md are unchanged. Why: per-function serial review
and an LLM coordinator cut throughput from 100–230 KB/day (19–28 Oct) to
~7 KB/day (8–13 Nov), with 8 overhead tasks per authored function.

Adam added `openai/gpt-6.1-terra` on 2026-11-15 as an ordinary farm lane,
alongside MiniMax and DeepSeek. It uses the same worker permissions, mechanical
checks, differential gates and serial acceptance. Terra is also the default
auditor, confirmation worker and defect fixer while the other quotas are scarce.
Confirmation remains a separate worker run, including when both auditors use Terra.

## Session rules (every orchestrating agent)

The farm runs for the whole session and the agent works the supply lane beside
it. These rules come from the 14–16 Nov sessions, where the farm was stopped and
drained eight times in 19 hours and then sat idle for nine.

1. **Start once, stop once.** `start` runs the farm under a supervisor that
   relaunches it if the process dies. The farm idles when supply is empty,
   rescans every two minutes and takes new supply by itself. Stop it only at the
   end of the session, or to integrate a harness release (edits under `cmre/`
   change the gate under running jobs); develop and test that release in a
   worktree first so the stop lasts one drain.
2. **Change settings with `set`, never with a restart.** `py -3.13
   scripts/cmre_farm.py set RUN --min-bytes 100` (also `--workers`, `--ladder`,
   `--audit-model`, `--confirm-model`, `--include-held`, `--max-bytes`,
   `--add-hours`) applies on the next tick. Running jobs finish on their old
   model; new jobs use the new setting. A drain costs 15–35 minutes.
3. **`SUPPLY EMPTY` or `idle N` in `status` means: add supply now.** Launch or
   continue supply-lane work (below) in the same turn. Supply work runs in
   parallel with the queue from the first minute, not after the queue empties.
4. **Apply fixes and inputs while the farm runs.** `adopt`, `dismiss` and `retry`
   are requests the running farm applies. A reviewed hint proposal is applied
   with `scripts/cmre_hint_apply.py` once its function is parked (no job in
   flight for it); the farm then re-gates the candidate it already holds.
5. **Resume the same RUN directory** across sessions. It remembers what is
   parked and which candidates wait for inputs; a new directory dispatches the
   whole held queue again.
6. **Validation budget.** `cmre_session.py check` runs once, at the handoff,
   after the final `stop`. Full revalidation and composition runs need Adam's
   request; they produced no recovery on 15–16 Nov (4 h 46 min plus 1 h 42 min).
7. **Write `docs/NEXT-SESSION-PROMPT.md` once**, at the handoff. It records
   state, not a live log.
8. **Report byte movement per supply item.** A lane that moves no bytes into
   `ready` or `lifted` after two attempts is reported and replaced with the next
   item, not reviewed again.

## OpenCode coordinator (default)

Adam runs sessions from OpenCode 2.x with the `cm-coordinator` agent (MiniMax
M3.1 Flash, so it keeps running through an OpenCode Go usage limit). The
coordinator owns the session; the farm owns every per-function
step. It never translates, audits or reads worker transcripts itself: an LLM
holding per-function state was the 8–13 Nov collapse. Setup, once per machine
and after `scripts/cmre_farm.py` changes: `py -3.13 scripts/cmre_farm.py
install-agents` writes `cm-coordinator` and `cm-defect` into the local
`.opencode/agents/`. Then open OpenCode in the repo, pick `cm-coordinator`, and
ask it to continue the decompilation.

The generated coordinator auto-allows direct `cmre` reads only: `status`,
`pipeline survey`, `work show`, `family`, `show`, and `job status` (with their
read arguments). Acceptance, retraction, work retry/block, source mutation and
other direct `cmre` commands have no automatic grant. Use the farm's checked
request protocol, not direct ledger/source mutation; the farm owns acceptance.

Coordinator procedure:

1. `py -3.13 scripts/cmre_session.py start`, then `py -3.13 scripts/cmre_farm.py probe`.
   Drop an `UNAVAILABLE` model from `--ladder`; when every rung is down, tell Adam.
2. Use the run named in `docs/NEXT-SESSION-PROMPT.md`: leave it alone when its
   `status` shows it running, otherwise start it again with the same directory
   (a first run uses `RUN` = `CM0102/Analysis/cmre/farm/<UTC yyyymmddThhmmZ>`):
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 10`.
   For every earlier run the handoff names, `py -3.13 scripts/cmre_farm.py defects OLD_RUN`
   lists its open defects; handle each as an `open-defect` event below with `OLD_RUN`.
   `adopt` and `dismiss` apply at once on a run with no live farm.
3. Run `py -3.13 scripts/cmre_farm.py wait RUN --minutes 60` as a **background**
   shell command and end your turn; OpenCode resumes you when it returns. Act on
   the reason it prints, in one short line per event:
   - `open-defect fn_X`: send one `cm-defect` subagent the prompt `RUN fn_X`
     (at most 3 at once). Its last line decides: `ADOPT <path> <review_receipt> <note>` →
     `py -3.13 scripts/cmre_farm.py adopt RUN fn_X --source <path> --review-receipt <review_receipt> --note "<note>"`;
     `DISMISS <review_receipt> <reason>` →
     `py -3.13 scripts/cmre_farm.py dismiss RUN fn_X --review-receipt <review_receipt> --reason "<reason>"`;
     `UNRESOLVED <why>` → leave it open and list it in the handoff.
   - `acceptance-quarantined fn_X`: preserve the run and canonical inputs.
     Ask for manual/current `cmre` ledger review at a stopped-farm boundary;
     do not queue another adoption or infer nonacceptance from unavailable evidence.
   - `request-failed fn_X`: the hand fix failed the gate. One more `cm-defect`
     with the failure detail, then leave it open. A missing/stale review receipt
     requires a fresh claimed review, not resubmission against a new baseline.
   - `hint-evidence-needed fn_X`: the worker retained a private input proposal and
     the function is parked. Give its receipt to the parent input lane for
     validation; an accepted proposal is applied with `scripts/cmre_hint_apply.py`
     while the farm runs, then `retry RUN fn_X --reason "<what was applied>"`.
     The farm has not applied or accepted it.
   - `fix-evidence-needed fn_X`: the origin review or automatic-fix dispatch
     inputs are missing/drifted. Preserve the concern and raw evidence; the farm
     requeues fresh review instead of accepting a stale fix. This is an evidence
     failure, not an ordinary failed model attempt or permission to restore newer work.
   - `model-unavailable`: the farm already moved to the next rung; probe once and
     tell Adam if a key needs renewal.
   - `model-limited`: a provider usage limit (OpenCode Go: 5 hours, shared by every
     DeepSeek model) paused that provider until the time `status` shows. The farm
     requeues without spending attempts and falls through to an available provider.
     Audits and confirmations also use an available ladder model; they wait only
     when all their lanes are unavailable. Tell Adam once, since Extra Usage is his call.
   - `farm-crash-loop`: the supervisor gave up after five restarts in an hour.
     Read `RUN/farm.err`, report the traceback to Adam, and start the same RUN
     again once the cause is fixed.
   - `timeout` (hourly): read `status` and apply step 5 of "Run by hand".
   - `farm-exit` or `farm not running`: the deadline passed or someone stopped it.
     When the session continues, start the same RUN again (`set RUN --add-hours N`
     extends a deadline before it passes); otherwise go to step 4.
   Supply events (`supply-empty`, `supply-resumed`, `input-needed`) do not end a
   wait: the farm keeps running and `status` shows `SUPPLY EMPTY` and `idle N`.
   Then wait again.
4. End: `py -3.13 scripts/cmre_farm.py stop RUN`, wait for `farm-exit`, run
   `py -3.13 scripts/cmre_session.py check`, write the handoff (below).

## Defect subagent

`cm-defect` (GPT-6.1 Terra, 40 steps) resolves one open defect. It gets `RUN fn_X`.

1. Explicitly claim the inputs you will review:
   `py -3.13 scripts/cmre_farm.py review RUN fn_X --claim NAME`, with a new unique
   name (letters/digits/underscore/hyphen, at most 80 characters). A successful claim returns
   `review_receipt` and immutable `source` / `hints` paths under
   `RUN/reviews/NAME/`; an existing claim is never overwritten. Read these
   claimed source/hints bytes. A claim freezes inputs; it does not certify a review.
   A returned hints path is absent when no canonical hints existed; that means no
   hints, for both bound and fresh-review-required claims.
   - `status: "bound"`: read the returned `review.audit_path`, `review.bundle`
     when non-null (otherwise run `py -3.13 -m cmre brief fn_X`), and
     `review.source_snapshot` / `review.hints_snapshot` (missing hints means none).
     Read `first_review` too when present: it locates the first audit preceding
     the confirmation. These paths are bound to the current accepted source/hints,
     verified snapshot bytes, and unchanged verdict/bundle hashes.
   - `status: "fresh-review-required"`: use `source`, `hints`, and a fresh brief
     for a complete listing review. `unbound_legacy_evidence` preserves the old
     run route (`audit/fn_X[.confirm].json`, `bundle/fn_X.md`) as historical leads,
     not findings about current code. A missing identity never becomes a bound verdict.
     If canonical C is missing, claim fails before creating a directory/receipt:
     obtain actual source first. Old missing-C receipts are invalid at admission,
     even when their old `c: null` identity matches the missing canonical file.
   - `status: "acceptance-quarantined"`: finish `UNRESOLVED` pending manual/current
     `cmre` ledger review; leave canonical inputs and retained evidence untouched.
     The CLI exits 2 with bounded JSON and creates no receipt.
   - `status: "conflict"`: the requested claim name already exists; exit 2 with
     bounded JSON. Preserve that claim. Choose a new name only for an actual new
     review, never to rebase an older fix or overwrite reviewed inputs.
   A fresh-review-required response with a new `review_receipt` exits 0 and
   permits the complete fresh review above. An input error/missing run/drift
   response has no receipt and exits 2 with bounded `reason`/`instruction` JSON:
   correct or obtain the inputs before reviewing. Expected statuses do not
   require interpreting a traceback.
   Independently verify every `open_findings` item in the claim. Retain
   `review_receipt`, `identity` and `open_findings` for the final check.
   Never guess job-number paths.
2. Confirm each finding from the listing VA and instruction, or reject it. The
   "Audit checklist" below lists real defect classes; auditors also report
   false positives. The target source/hints/bundle MUST be the supplied
   immutable snapshots. Callee C may be read ONLY for navigation, NEVER as
   signature/ABI/fidelity authority or proof. Every ABI finding must cite
   original instructions at preferred-image VAs for the pinned CM0102 3.9.68
   binary, SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
   A bound target review does not establish callee composition or live runtime
   equivalence; reading callee C does not silently add a global dependency hash.
3. None confirmed: after the identity check in step 5, finish with
   `DISMISS <review_receipt> <reason citing VAs>`.
4. Otherwise copy the claimed `source` to `RUN/hand/fn_X.c` (the only file you may
   write), fix only the confirmed findings under the "Worker rules", and test with
   `py -3.13 scripts/cmre_farm.py test RUN fn_X RUN/hand/fn_X.c` (add `--case N`
   to explain a case). Five failed tests end the attempt.
5. Repeat `review RUN fn_X`. If `identity` or `open_findings` changed, evidence became unbound,
   or acceptance is quarantined, finish `UNRESOLVED` and request fresh review.
   A legacy/fresh-review branch may finish only after the complete current-code
   review and an unchanged identity, never on the old verdict alone.
   Finish with one line: `ADOPT RUN/hand/fn_X.c <review_receipt> <what changed>` after a pass,
   else `UNRESOLVED <first remaining mismatch or why>`.

## Run by hand (Claude Code, Codex)

1. `py -3.13 scripts/cmre_session.py start` (once per session).
2. `py -3.13 scripts/cmre_farm.py probe` sends one trivial prompt to each model.
   It uses the same OpenCode build as `run` (`--opencode`, default v2). An
   `UNAVAILABLE` line with an expired key or `Token refresh failed` needs Adam
   (OpenCode console for OpenCode Go keys, `opencode auth login` for OpenAI). The farm also retires a model on its first auth
   error and moves to the next rung, without spending an attempt.
3. Start the farm detached and leave it running:
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 10`.
   A supervisor (`RUN/supervisor.pid`) runs the farm (`RUN/farm.pid`) in a hidden
   console and relaunches it 30 seconds after an unexpected exit (`farm-restart`
   event), keeping the original deadline. It leaves the caller's job object, through
   WMI when the job forbids breakaway, so it outlives the shell, agent session or
   OpenCode server that started it. Logs: `RUN/farm.out`, `RUN/farm.err`. `run`
   takes the same options in the foreground without the supervisor.

   The run is continuous: with no supply it idles (`supply-empty` event,
   `SUPPLY EMPTY` in `status`), fetches the queue again every `--rescan-minutes`
   (default 2) and dispatches what appears (`supply-resumed`). It ends on `stop`
   or the `--hours` deadline (`--hours 0`: no deadline). `--exit-when-empty`
   restores the old run-once behaviour for smoke tests.

   `set RUN ...` changes workers, audit workers, size bounds, ladder, audit and
   confirmation models, `--include-held`, skip ranges and the deadline on the
   running farm. The settings live in `RUN/control.json` and apply again after a
   restart.

   A candidate that compiles and runs with no mismatch but whose test inputs
   cover too little of the function is **input-bound**: coverage counts original
   instructions the inputs reach, so no model can fix it. The farm parks it after
   one attempt without spending the next rung (`input-needed` event), keeps the
   candidate in `RUN/src/`, and lists it with `py -3.13 scripts/cmre_farm.py
   inputs RUN`. When that function's canonical hints change, the farm re-gates
   the retained candidate without a model call (`input-arrived`, `recheck`) and
   accepts it on a pass. `retry RUN fn_X --reason "<what changed>"` returns any
   other parked function to the queue, for example after a harness release.

   Options: `--include-held` also takes held functions (each prompt carries
   the hold reason; the audit checks it). `--min-bytes/--max-bytes` bound the
   queue. `--ladder` is the model escalation order
   (default MiniMax M3.1 Flash → DeepSeek v4.1 Flash `#low` via Go → `#low` via
   Command Code → default effort via Go → default effort via Command Code →
   GPT-6.1 Terra via OpenAI;
   MiniMax M3 passed 3 of 13 on 2026-11-13 and is off the ladder). DeepSeek runs
   use v4.1 Flash. `--audit-model` defaults to `openai/gpt-6.1-terra`; when the
   preferred auditor is limited or retired, the last available ladder rung audits.
   `--stall-minutes` (default 15) kills a worker whose transcript stops growing.
   OpenCode 2.x writes nothing while a step thinks, so a shorter limit kills real
   work. DeepSeek models take an effort variant (`opencode-go/deepseek-v4.1-flash#low`).
   Command Code uses exactly `commandcode/deepseek/deepseek-v4.1-flash`
   (not `deepseek-v4.1-flash-fast`), at the same capability/effort tiers as Go.
   Its provider quota/availability is independent of Go. Workers use the same
   permissions, gate tests and serial acceptance; no separate installed agent is needed.
   `--opencode v2` (default) runs the npm OpenCode 2.x build with one temporary
   agent file per job in `.opencode/agents/` carrying its permissions; it has a
   256k output ceiling and the current credentials. `--opencode v1` runs the
   Chocolatey 1.x `opencode` on PATH, which caps output at 32k unless raised and
   reported the OpenCode Go key as expired on 2026-11-13 while 2.x worked.
   `--confirm-model` (default `openai/gpt-6.1-terra`) is the second auditor, with
   the same availability fallback; `''` disables it. `--skip-ranges` (default
   `0x00940000-0x00953000`, the x87
   runtime helpers behind 7 of 10 production defects) is never dispatched.
   `--verify-threads` (default 8) runs gate re-tests beside the dispatcher.
   Translation jobs read `RUN/bundle/fn_X.md`: the brief with the listing,
   draft, rules and `cmre.h`. Fix/audit/confirmation bundles are immutable per
   dispatch (`fn_X-N.md`); audit/confirmation verdicts are unique
   per job, including `fn_X-N.md` and `fn_X-N[.confirm].json`; the supported
   defect lookup is `review RUN fn_X`, not a stable-name alias.

   To conserve MiniMax/OpenCode Go quota, select Terra for all farm workers:
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 10 --ladder openai/gpt-6.1-terra --audit-model openai/gpt-6.1-terra --confirm-model openai/gpt-6.1-terra`.
   Explicit `--ladder` settings replace the defaults, including on resumed runs;
   add Terra to an older explicit list to enable its lane. Re-run `install-agents`
   to update the local `cm-defect` model. Model discovery is not a successful
   inference/authentication probe; the normal pre-run probe still checks that.

4. Run `py -3.13 scripts/cmre_farm.py wait RUN --minutes 60` (it returns early on
   a notable event: open defects, provider limits/unavailability, failed
   requests, quarantine, missing hint/fix evidence, a crash loop or exit), or
   check `py -3.13 scripts/cmre_farm.py status RUN` about once an hour. Status
   prints accepted functions and bytes, bytes per hour, running jobs, `queue`
   (known undispatched supply), `idle` (free translation slots), parked items
   with the input-needed count, audit results, open defects and pass rate per
   model. `pending` counts only requeued jobs; it is 0 in a healthy run. The farm
   needs no supervision between checks; spend the time on the supply lane.
5. Act on the status, not on individual functions:
   - Pass rate of a model under 20% after 20 attempts: `set RUN --ladder ...`
     without it.
   - `idle` above 0 or `SUPPLY EMPTY`: supply ran out. `set RUN --min-bytes 0
     --include-held` when a floor or the hold filter is on, then work the supply
     lane below. The farm stays up.
   - Workers barely load the machine (each is a waiting OpenCode session); gate
     re-tests are the CPU cost (24 threads here). `set RUN --workers 24` while
     `status` shows no provider rate-limit failures; 500 KB/day needs about
     20 workers at the measured ~1 KB/h each.
6. End: `py -3.13 scripts/cmre_farm.py stop RUN` (running workers finish,
   nothing new starts; it logs `farm-exit` and the supervisor exits). When the
   process exits, run `py -3.13 scripts/cmre_session.py check` and write the
   handoff (below).

The run directory holds `state.json` (resumable: rerun `run` with the same
directory), `events.jsonl`, `src/` candidates, `bundle/` job inputs, `audit/`
verdicts, `audit/snapshots/` dispatch inputs, `transactions/` durable candidate
journals, `reviews/` explicit immutable review claims, `hints/` private input
proposals/receipts, `fix/snapshots/` automatic-fix dispatch evidence, `hand/` manual
defect fixes, `requests/` (including immutable
`requests/candidates/` proposals and `requests/rejected/` evidence), and `logs/`
(prompts and worker transcripts). Retain/archive these together with their
receipts and identities under the policy below. They are local evidence; never publish them.

### Transaction ownership and retention

The controller owns `RUN/transactions/`: each journal retains exact prior and
candidate C/hints bytes, SHA-256 input identities, request/resume identity,
acceptance evidence when available, and lifecycle phase **before** replacement.
Worker/model pass claims are not acceptance evidence. Completed collection is
recorded only after terminal status; interrupted collection remains resumable.

Restart restores only bytes whose ownership is proven. After an interrupted
acceptance, matching authoritative acceptance evidence may complete bookkeeping;
only explicit authoritative **nonacceptance** permits rollback. Unavailable,
absent or mismatching ledger proof instead records `quarantined`, preserves all
current canonical inputs, and blocks further requests/acceptance for that
function. The real adapter currently has no complete ledger identity proof, so
this case requires manual/current `cmre` ledger review. No automatic unquarantine
or guessed success is supported. Quarantine is separate from a failed gate.
Quarantine is function-local: offline application drains every runnable
request/verifier/acceptance while retaining that function's blocked requests.
Unrelated adopts/dismissals still complete. An action whose own target is
quarantined returns bounded `acceptance-quarantined` JSON and exit 2, including
when acceptance becomes unknown during that action. It does not abort the
whole offline queue or roll back potentially accepted bytes.

Canonical hints are read-only to farm workers in both OpenCode configurations.
Each translation/fix job may edit only its scratch C and private
`RUN/hints/fn_X-N.json`, initialized from the dispatch hint bytes. The controller
checks proposal shape/shared-policy restrictions and retains changed proposals,
their bytes/hashes, and dispatch source/hints identity in a `.proposal.json`
receipt. A valid proposal produces `hint-evidence-needed`, not automatic canonical
application or acceptance. Worker tests still use canonical hints and do not
validate the private proposal. The parent validates it and applies it with
`scripts/cmre_hint_apply.py` while the farm runs (the function is parked, so no
job holds its inputs), then requests fresh verification under the reviewed
inputs with `retry`; an input-bound candidate is re-gated automatically. There is
no claimed real-worker mutation-provenance adapter. Unrelated canonical changes
are preserved and parked; provider/auth paths never restore canonical hints.
Private drafts replaced on redispatch are retained with their dispatch metadata.

Adopt/dismiss requests must carry `--review-receipt` from the actual prior
`review --claim NAME`, not a new claim created after a hand fix is reviewed.
The queue records that immutable receipt's hash and expected source/hints identity.
Admission verifies both snapshots, current inputs and the reviewed open-concern
set before taking the receipt bytes as the baseline. Acceptance rechecks the
concern set too. A queued request cannot rebase onto a newer acceptance or clear
newer unreviewed concerns.
Missing, changed, or stale proof is retained under `requests/rejected/` and
requires fresh review; legacy no-receipt requests fail closed.
At queue time, adopt copies the proposed output into a unique
`requests/candidates/` file and records its SHA-256. Admission and verification
check that identity. Later edits to the original `--source` path do not replace
the queued proposal. A missing/changed queue copy requires explicit resubmission;
its hash is not a substitute for the separate prior review-input receipt.

The controller owns immutable `RUN/audit/snapshots/N/` source/hints bytes and
identity/first-verdict metadata. Metadata bytes and bundle identity are pinned
at dispatch. Before applying any actionable verdict, the controller verifies
all required retained target source/hints, metadata, bundle and verdict evidence;
confirmation also verifies the first review's evidence. Missing/hash-drift
evidence preserves open concerns and requeues fresh review, even if the returned
verdict says `ok`. Older unbound metadata is not silently upgraded.
Auditors own only their unique verdict output. Their target inputs are the
supplied snapshots; optional callee C is navigation only under the original-only
ABI evidence rule above. Stale verdicts remain historical evidence and cannot
cause rollback, dismissal or a fix. A stale old result cannot invalidate a newer
bound review with intact evidence.
Automatic fix jobs also freeze their accepted starting source/hints, metadata
and bundle under `fix/snapshots/N/` and unique bundle paths. Their authorization
is the exact originating defect review (including confirmation evidence when
present), not just a source hash or a mutable latest verdict. Origin artifacts
are checked before dispatch; both origin and fix-dispatch artifacts are checked
before canonical installation and immediately before serial acceptance.
Evidence failure preserves current concerns and requeues fresh review. Only an
unaccepted installed source still matching its transaction may be rolled back;
newer unrelated canonical C/hints are never restored from a stale fix snapshot.
Provider retries get new dispatch identities and retain the prior inputs.
This does not grant extra automatic fix rounds beyond the existing run budget.
`review` without `--claim` and `status` are read-only; neither reconciles transactions.
`review --claim NAME` explicitly creates immutable inputs/receipt, not a latest alias.

On a supervised restart with audit workers, retained terminal verdict,
snapshot and dispatch-bound bundle evidence is checked for the functions in
**that run**. Missing/hash-drift evidence is requeued for fresh audit. Legacy
unbound findings stay OPEN as unresolved concerns until actually reviewed;
stale findings cannot authorize a fix or silently dismiss a backlog item.
Fresh auditors receive the unresolved concerns explicitly; a result for an older
concern set cannot clear newly added findings.
An offline adopt/dismiss has no audit workers and leaves unrelated legacy
audits/findings untouched. This is per-run reaudit work, not a whole-game
inventory revalidation request.

Journals, review claims, private hint proposals, snapshots, immutable audit bundles/verdicts, events and associated
receipts are required local acceptance evidence. They can contain original
listings, proprietary inputs and raw analysis: keep them under ignored local
analysis storage and out of published history/packages. **No journal/snapshot
pruning without a verified archive** retaining exact bytes, identities, phase,
raw verdicts, receipts and cross-referenced paths. Preserve unresolved
quarantines in place until reviewed; an ignored directory is not disposable.

## Open defects

A first audit's defect is `unconfirmed` until the second auditor
(`--confirm-model`) checks each finding against the listing: no confirmed
finding closes it as `disputed`; confirmed findings get one fix attempt. Why:
on 2026-11-13 the MiniMax M3 auditor flagged 8 of 8 functions, including false
positives, and DeepSeek `#low` passed real defects. `status` lists as open the
defects that the fix attempt did not resolve; the accepted source stays in place.

Resolve them through the defect subagent (OpenCode) or by hand with the same
steps, then `adopt` or `dismiss`. The running farm applies both requests: it
gates and accepts an adopted source, then audits it afresh. `cmre retract-c`
needs a witness showing a compared gate mismatch, so it cannot withdraw a
defect the gate cannot see; fix those.

## Handoff

Overwrite `docs/NEXT-SESSION-PROMPT.md` with at most 8 lines: the final
`status` line, `cmre status` totals, `cmre pipeline survey` band totals, open
defects, and the next supply-lane item. No hold lists or receipt hashes; the
farm state and `cmre work` already hold them.

## Worker rules

Workers read this section. Translation rules are in
[C.md](C.md), step 2 of "Loop" (the bulleted Rules list): read it completely
before writing. Then:

- Search with `rg` (it skips the ignored multi-GB `CM0102/` tree) or the
  built-in grep/glob tools; other shell commands are denied.
- Write only your scratch source and the private hint-proposal path given in
  your prompt (`RUN/hints/fn_X-N.json`). Canonical `recovery/hints/` is read-only.
  Put proposed fixtures under `"c_hints"`; run
  `py -3.13 scripts/cmre_hint_check.py <private-proposal-path>` first.
  Changed proposals finish `FARM-RESULT: evidence-needed <why>` for parent input
  validation/application; a worker test is not evidence for that proposal.
- Start from the draft path `cmre brief` prints, or from the source already
  in your scratch file. Check every statement against the listing.
- Never shape code generation to pass: no `register`, no optimize attributes,
  no volatile locals added only to change register allocation, no inline
  assembly. A frame layout or a pass at one optimization level is not evidence.
- Unknown meaning stays unnamed. Never invent logic, defaults or initialization.
- Five failed tests (build failures count) end the attempt. Report the first
  remaining mismatch on the final line; the farm escalates to a stronger model.

## Audit checklist

Auditors read this section. These are defects that passed the gate before
(mostly from October reviews). Check each against the listing and cite the
instruction VA for every finding:

1. Return: `void` only with direct callers that ignore EAX; wrappers that
   forward a callee's EAX keep it; width matches what callers read.
2. Calls: every argument, pushes made earlier in the routine, varargs, the
   thiscall receiver (ECX) for callees that read it, and call order.
3. Memory order: reads before or after SEH state stores and other stores as in
   the listing; re-reads after intervening stores; globals read once vs twice.
4. Widths and signedness: byte/word counters (`inc al` wraps at 127/255),
   `movsx` vs `movzx`, signed ±1 wraps (use `u32`), negative remainders as indexes.
5. Stack: `lea [esp+N]` offsets recalculated across pushes and callee cleanup;
   buffers that land in argument slots; adjacent buffers kept in one allocation.
6. Control flow: loop exits compare against the right saved value; jumps to the
   epilogue; code after a no-return call (for example `_longjmp`) is unreachable.
7. Aliasing at `-O2`: a field re-read after a store through another pointer type
   needs a volatile access; a non-volatile null load can be deleted.
8. Constants: table rows, format strings against their argument order,
   inclusive bounds and table bases at each switch.
9. Gate gaps: the gate counts a case as inconclusive when either side exhausts
   its budget, so C that never terminates can pass; the gate does not compare
   stack memory or the ECX passed to callees.
10. A held function: the accepted source resolves the recorded hold reason.

Audit workers do not run `cmre mutate` (it outlives the worker shell's 2-minute
timeout). When triaging an open defect by hand, `cmre mutate fn_X` shows operator
mutants that survive the corpus; a surviving mutant on a line the listing proves
is a weak test, not a defect by itself.

## Supply lane (Codex / GPT)

The farm only drains supply that the gate can test. Bytes stuck outside it are
the real limit (`py -3.13 -m cmre pipeline survey`): on 2026-11-13, 71 KB
ready, 654 KB held, 2.03 MB unlifted.

Measured 2026-11-16 after run `20261115T1250Z-sol-commandcode-supervised`: the C
queue holds 412 functions / 453 KB and **2 functions / 2.3 KB are unheld**. That
run took 158 of its 162 accepted functions (154 KB) from held functions whose
holds were stale, then parked 300. Re-gating the 300 retained candidates
(`cmre_farm.py reclassify RUN`) gave: 245 that fail the gate (227 / 344 KB on
real mismatches, 12 / 25 KB that do not build: x87 placeholders, `sqrt` link),
51 / 32.5 KB input-bound (20 / 13.4 KB already at coverage 0.5 or more), 1
passing, 3 with no candidate.
Unlifted: 1,060 functions / 1.98 MB. Translation capacity is idle until supply
moves; these are the byte pools, largest first:

| Pool (work-state blockers) | Held bytes | What releases it |
| --- | --- | --- |
| x87 call boundary (`x87-live-stack`, `x87-power-call-boundary`, `original-live-x87-argument`, 11 smaller) | 161 KB, ~30 fn | A policy-approved way for plain C to call an original routine that takes or returns values on the x87 stack (for example `0x00945ff0`) |
| Import out-parameters (`native-directory-output`, `file-search-import-outparams`, `shutdown-uninitialized-find-output`, `unwritten-*-output`, `readfile-import-outparams`) | 85 KB, ~60 fn | Import stubs that write their output buffers (recorded results, item 1) |
| Virtual-entry inventory (`missing-real-virtual-entry`, `virtual-target-inventory-entry`, `real-virtual-*`) | 27 KB, ~45 fn | Inventory entries for real vtable targets (the `fn_0051c020` seed flow of 15 Nov) |
| Stack-probe fault address (`callee-stack-probe-address`, `*stack-probe*`) | 25 KB, ~15 fn | A harness rule for a stack-exhaustion fault at the same original instruction whose address differs only by frame layout (`fn_006146b0`: 598 of 600 cases equal) |
| Exit and post-fatal continuation (`exit-return-stack-continuation`, `returning-exit*`, `original-post-fatal-*`) | 21 KB, ~35 fn | A harness rule for execution after a call that does not return in the game |
| Input-bound candidates (`cmre_farm.py inputs RUN`) | 32.5 KB, 51 fn | Authored `c_hints` fixtures; the farm re-gates the retained candidate by itself |

Each of the first five is one harness decision (ADR, [HARNESS.md](HARNESS.md),
frozen revalidation) for Adam, not per-function work: retrying those functions
on any model repeats the recorded failure. Expert sessions work this list in
order while the farm runs, and never coordinate the farm:

1. **Import-result recording** (authorized 2026-11-13). Extend the capture
   recorder ([CAPTURE.md](CAPTURE.md), ADR 0009) so a replay continues with
   the real results of `HeapAlloc`, `ReadFile` and other imports instead of
   seeded stub values. 181 captured functions (455 KB) stop below 0.6 coverage
   mostly on this. Recording needs the game running with the recorder attached.
   Ask Adam before driving the desktop, or run unattended only with his approval.
2. **Harness families.** `py -3.13 -m cmre work families` and the `survey`
   blocker counts name families that block 10–45 functions each
   (`missing-real-virtual-entry`, `dtor-heapfree-divergence`,
   `shutdown-uninitialized-find-output`, `indirect-delayed-save-as-argument`,
   `exit-return-stack-continuation`, `x87-live-stack`). One harness release
   clears a whole family; then `cmre_farm.py retry RUN fn_X ... --reason
   "<release>"` returns its parked functions to the running farm.
3. **Fixture lane.** Start with `cmre_farm.py inputs RUN`, highest coverage
   first: each listed function already has a candidate with no mismatch, so a
   fixture that lifts coverage past the threshold is accepted without another
   model attempt. Then record-table functions (`[0xae23bc]`, `[0xae23b4]`,
   `[0xb63c98]`, `[0xae2364]` chains) that play does not reach: authored
   `fixtures` in hints, a worker per function, evidence chains under
   `CM0102/Analysis/cmre/diagnostics/coverage-spike-20261103/specs/`. Proposals
   append fixtures and keep every existing one; a proposal that replaces
   fixtures is refused (15 Nov: 9 replaced with 2, 7 with 1).
4. **Expand and apply** after every capture session: `cmre capture expand`,
   then `cmre capture apply` and accept the passing lifts.

Each item's completion is bytes moved into `ready` or `lifted`, measured by
`survey` before and after.
