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

## Live pipeline dashboard

From the authoritative checkout, run `py -3.13 scripts/cmre_farm_dashboard.py serve`
and open [127.0.0.1:8042](http://127.0.0.1:8042/). The read-only browser page
discovers local farm runs, defaults to a live run when available, and refreshes
every five seconds. Use `serve --run RUN` to include a run directory outside
the default farm folder. `--port` changes the local port.

- **Pipeline flow and assignments** show the selected run's frontier, retries,
  active model workers by role and provider/model, gate re-tests, serial
  acceptance, and queued audits. Each assignment includes its function ID.
- **Capacity & scaling** shows applied worker limits and filters, pending control
  settings, the latest settings change, provider cooldowns, and when a frontier
  scan becomes eligible.
  An eligible scan still needs the dispatcher to look for work. An empty frontier
  plus idle translation slots indicates a supply constraint. Compare that with
  the gate, writer, and audit queues before changing concurrency. Use the farm's
  `set RUN` command to tune a running farm. Settings apply at the start of the
  next scheduler tick, which can be delayed by serial acceptance.
- **Supply inventory** shows workspace-wide authored, lifted, and unlifted body
  bytes and the largest evidence and harness blocker families. It refreshes
  independently every minute. The selected run's frontier also reflects its
  filters, claims, and parked work, so it can be empty with a large inventory.
- **Run activity** includes historical dispatches, provider changes, accepted
  functions, and supply transitions. The throughput chart uses original body
  bytes from translation acceptances. The workspace coverage cards report
  accepted C/legacy and lift-inclusive recovery separately.

The farm writes `RUN/live.json` under ignored local analysis storage while it
dispatches, ticks, and enters serial acceptance. Older runs show history or
dispatch-to-result records. The detailed stage data starts the next time the
updated farm runs. A stale signal keeps its last observations visibly marked
with their timestamp.

Expert sessions outside the farm can report their assignments at start and when
switching tasks. Reuse the same ID to renew an assignment and finish it at
handoff. Reports expire automatically after 30 minutes by default and appear
across all runs. Use a short label with function IDs or a blocker family.

```powershell
py -3.13 scripts/cmre_farm_dashboard.py report fixture-worker --model gpt-6.2-sol --work "Author fixtures for x87-live-stack" --minutes 45
py -3.13 scripts/cmre_farm_dashboard.py finish fixture-worker
```

The default role is `supply`. Use `--role coordinator` or
`--role "defect review"` for those assignments. External sessions without a
current report do not contribute to the reported count.

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
3. **`SUPPLY EMPTY` or `idle N` in `status` means: add supply now.** Run
   `lanes RUN` ([Lanes](#lanes-who-moves-work-the-farm-cannot-move)) and work
   its largest lane you own in the same turn. Supply work runs in parallel with
   the queue from the first minute, not after the queue empties.
4. **Apply fixes and inputs while the farm runs.** `adopt`, `dismiss` and `retry`
   are requests the running farm applies. A reviewed hint proposal is applied
   with `scripts/cmre_hint_apply.py` once its function is parked (no job in
   flight for it); the farm then re-gates the candidate it already holds.
5. **Resume the same RUN directory** across sessions. It remembers what is
   parked and which candidates wait for inputs; a new directory dispatches the
   whole held queue again.
6. **Validation budget.** Run `cmre_session.py check` once at the handoff after
   the final `stop`. Scope each harness release using the
   [validation budget](PLAYBOOK.md#validation-budget). The session decides when a
   full revalidation or composition run is warranted and records why
   ([ADR 0011](../adr/0011-engineering-decision-rights.md)).
7. **Write `docs/NEXT-SESSION-PROMPT.md` once**, at the handoff. It records
   state, not a live log.
8. **Report byte movement per supply item.** A lane that moves no bytes into
   `ready` or `lifted` after two attempts is reported and replaced with the next
   item, not reviewed again.
9. **Commit and push accepted work.** After the handoff check, commit the
   accepted sources and reviewed tooling with explicit paths and push them
   ([PUBLICATION.md](PUBLICATION.md)); during a run longer than a few hours,
   do the same at a quiet point while the farm keeps running. Adam's standing
   authorization covers this, so the session finishes with its work on
   `origin/main`.

## Lanes: who moves work the farm cannot move

The farm drives one stage by itself: lifted function → C candidate → gate →
accept → audit → fix. On 19 Nov its status read "0" for days while 317
functions sat parked and 952 unlifted functions waited for steps nobody owned.
Every state the farm cannot leave by itself now has a named lane:

```
py -3.13 scripts/cmre_farm.py lanes RUN          # --json for the full lists
```

It prints each lane's functions and original bytes, largest first, with the
owner, the next action and its five largest items (the parked detail's first
failing case for mismatches). Work the lane with the most bytes that you own:

| Lane | Owner | Moves by |
|---|---|---|
| `lift` | input lane + lift lane (automatic) | farm runs both; low coverage gets authored inputs and captures |
| `gate-mismatch`, `contract` | expert session | `cm-decompile` from the retained `RUN/src` candidate, then `cmre accept fn_X --c` |
| `input-needed`, `hint-evidence` | input lane | evidenced hints; the farm re-gates by itself |
| `budget-bound` | Adam | C exhausts the 200k budget; a raised budget changes shared hints of an accepted lift |
| `safeguard-blocker` | `cm-triage` | resolve the `BLOCKER:` the candidate declares, then `retry` |
| `worker-failure` | coordinator | `retry RUN fn_X --reason ...` |
| `open-defect` | defect lane | [Defect lane](#defect-lane) (`--defect-lane-workers`), or the [defect subagent](#defect-subagent) by hand |
| `boundary` | none | explicit unknown; a defect only with consumer evidence |

### Lift lane

When supply stalls, run `py -3.13 scripts/cmre_farm.py lanes RUN --actionable`.
It links the farm, lift, and latest session records. It lists up to five
unlifted functions with existing fixtures and no matching current gate
receipt, even if no runtime capture exists. Near-threshold lifts with a
matching receipt show the first uncovered instruction and its decoded
incoming edges. The receipt can come from the automatic lane or a manual gate
for a function with retained fixtures. Treat the edge as a starting point, not
proof that a particular input reaches it. The view never runs gates or modifies
the queue.
If either list contains an item with a scheduling hold, follow its `work show`
command and retry condition before running a gate or probing the uncovered edge.
An exhausted tool-error retry cap is reported with the retained error instead of
suggesting another manual gate.
Input proposals keep their evidence link and show any current work hold beside it.
Check the hold before applying a retained proposal.

`scripts/cmre_lift_lane.py once` takes every function with stored captures
(ADR 0009) and no accepted record whose capture set changed since its last
pass. It runs `cmre capture expand`, `cmre capture apply` (the unchanged
lifted gate) and `cmre accept` for each pass. The farm launches one pass every
`--lift-lane-minutes` (default 60; `set RUN --lift-lane-minutes 0` disables)
and rescans its frontier when a pass accepts, so new overnight captures reach
the C translators without an expert session. A busy writer lease leaves the
accept for the next pass; a refused accept and gate results stay until the
captures change; tool errors retry three times. State and the per-pass summary
are local evidence under `CM0102/Analysis/cmre/lift-lane/` and `RUN/lift-lane/`
(`lift-lane-start` / `lift-lane-done` events); never publish them.
`py -3.13 scripts/cmre_lift_lane.py status` shows the last pass.

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
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 0 --ladder opencode-go/hy4-flash-free,minimax-coding-plan/MiniMax-M3.1-Flash,openai/gpt-6.1-terra`.
   `--hours 0` keeps it running across sessions; a deadline left it idle for hours
   between sessions (20 Nov: exit 04:00, restart 07:49). Run 20261115T1250Z
   translate pass rates: Hy4 Flash 32%, MiniMax plan 23%, Terra 9% (later rung,
   harder functions); Hy4 Flash is free, so it leads.
   For every earlier run the handoff names, `py -3.13 scripts/cmre_farm.py defects OLD_RUN`
   lists its open defects; handle each as an `open-defect` event below with `OLD_RUN`.
   `adopt` and `dismiss` apply at once on a run with no live farm.
3. Run `py -3.13 scripts/cmre_farm.py wait RUN --minutes 60` as a **background**
   shell command and end your turn; OpenCode resumes you when it returns. Act on
   the reason it prints, in one short line per event:
   - `open-defect fn_X`: when `status` shows `defects lane:`, the farm's
     [defect lane](#defect-lane) takes it; do nothing. Otherwise send one
     `cm-defect` subagent the prompt `RUN fn_X` (at most 3 at once). Its last line decides: `ADOPT <path> <review_receipt> <note>` →
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
     DeepSeek model; also monthly) paused that provider until the time `status` shows.
     A limit whose message says to switch to a free model pauses only paid models,
     shown as `LIMITED provider (paid)`; `-free` models keep running, and their own
     limits pause only the free tier (`provider:free`). A `provider.no-route`
     ("Model unavailable") pauses only that model for one hour (`LIMITED provider/model`);
     the input lane still gates a spec the worker changed before the error. The farm
     requeues without spending attempts and falls through to an available provider.
     A failed attempt whose remaining rungs are all paused waits for them instead of
     parking, and a failure while the farm stops is left for the next run's scan.
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
4. Otherwise use `py -3.13 scripts/cmre_farm.py prepare-hand RUN fn_X --review-receipt PATH --claim-scoped`.
    It returns a claim-specific `source` under `RUN/hand/` without overwriting
    prior work. The command supports runs directly under `CM0102/Analysis/cmre/farm/`.
   For a nonstandard run, the coordinator prepares and edits the scratch copy
   manually; the generated worker has no edit permission there. Rerun
   `install-agents` before dispatching workers that need the
   new shell permission. If it reports a conflict, preserve the file and ask the
   coordinator to reconcile the retained work. Edit only that hand file and only
   for confirmed findings under the "Worker rules", then test with
    `py -3.13 scripts/cmre_farm.py test RUN fn_X SOURCE` (add `--case N`
   to explain a case). Five failed tests end the attempt.
5. Repeat `review RUN fn_X`. If `identity` or `open_findings` changed, evidence became unbound,
   or acceptance is quarantined, finish `UNRESOLVED` and request fresh review.
   A legacy/fresh-review branch may finish only after the complete current-code
   review and an unchanged identity, never on the old verdict alone.
    Finish with one line: `ADOPT <source> <review_receipt> <what changed>` after a pass,
   else `UNRESOLVED <first remaining mismatch or why>`.

## Run by hand (Claude Code, Codex)

1. `py -3.13 scripts/cmre_session.py start` (once per session).
2. `py -3.13 scripts/cmre_farm.py probe` sends one trivial prompt to each model.
   It uses the same OpenCode build as `run` (`--opencode`, default v2). An
   `UNAVAILABLE` line with an expired key or `Token refresh failed` needs Adam
   (OpenCode console for OpenCode Go keys, `opencode auth login` for OpenAI). The farm also retires a model on its first auth
   error and moves to the next rung, without spending an attempt.
3. Start the farm detached and leave it running:
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 0 --ladder opencode-go/hy4-flash-free,minimax-coding-plan/MiniMax-M3.1-Flash,openai/gpt-6.1-terra`.
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
   restores the old run-once behaviour for smoke tests. `run --only fn_A,fn_B`
   limits a trial run's frontier to the named functions; use your own `RUN` and
   leave functions the live farm is working on alone.

   `set RUN ...` changes workers, audit workers, size bounds, ladder, audit and
   confirmation models, `--include-held`, skip ranges and the deadline on the
   running farm. The settings live in `RUN/control.json` and apply again after a
   restart. A new `start` replaces any setting it names explicitly (for example
   `--ladder`) and keeps the other `set` values.

   A candidate that compiles and runs with no mismatch but whose test inputs
   cover too little of the function is **input-bound**: coverage counts original
   instructions the inputs reach, so no model can fix it. The farm parks it after
   one attempt without spending the next rung (`input-needed` event), keeps the
   candidate in `RUN/src/`, and lists it with `py -3.13 scripts/cmre_farm.py
   inputs RUN`. When that function's canonical hints change, the farm re-gates
   the retained candidate without a model call (`input-arrived`, `recheck`) and
   accepts it on a pass. Rechecks can run while all model providers are
   unavailable and stay eligible across restarts and temporary claim conflicts.
   A differential mismatch on the new corpus starts at the first available
   model rung. `retry RUN fn_X --reason "<what changed>"` requests a fresh
   gate for an input-bound candidate or queues a translation for other parked
   functions, for example after a harness release.

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
defect fixes, `x87-mutants/` retained ADR 0010 operand-mutant receipts,
`requests/` (including immutable
`requests/candidates/` proposals and `requests/rejected/` evidence), and `logs/`
(prompts and worker transcripts). Retain/archive these together with their
receipts and identities under the policy below. They are local evidence; never publish them.

### Transaction ownership and retention

The controller owns `RUN/transactions/`: each journal retains exact prior and
candidate C/hints bytes, SHA-256 input identities, request/resume identity,
acceptance evidence when available, and lifecycle phase **before** replacement.
The farm sends a typed local request to `cmre farm-accept`. Under the serial
writer lease, acceptance rechecks canonical source and hints, the canonical
gate, and any prior review or operand proof. Worker/model pass claims are not
acceptance evidence. Completed collection is
recorded only after terminal status; interrupted collection remains resumable.

Acceptance requests use schema 2 and name the controller's write-once
`dispatch/JOB/receipt.json`, created before launch or explicit adoption admission.
Acceptance derives prior hints from that snapshot and verifies the originating
audit's function, input identity, verdict and findings again after the fresh gate.
Schema-2 review claims retain their audit authority; fresh manual claims identify
inputs only and do not claim a machine audit verdict. These files are controller
authority, outside worker write scopes; hashes do not authenticate an arbitrary
local process with permission to rewrite the controller's evidence.

Historical schema-1 requests/claims and pending jobs without dispatch authority
fail closed. Preserve them; obtain a fresh explicit claim/adoption or redispatch
the candidate. Acceptance never manufactures a dispatch snapshot from already
installed inputs. Uncertain post-write outcomes still require ledger review.

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

For an accepted lift with no accepted C, the hint applier permits append-only
`c_hints` fixtures/captures and provenance without changing shared inputs or
generator policy. The existing effective C input prefix includes inherited
shared fixtures/captures and must remain intact. Use `--expected-sha256 absent`
only when the canonical hint file is absent; the tool rechecks that absence
under its writer lease and retains a prior-absence receipt. Remove `auto: true`
to claim an edited hint. Accepted-C and legacy records remain refused by this
tool: changes to their inputs require explicit parent review and the farm's
fresh identity-bound adoption protocol, not an automatic fixture append.

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
**that run**. Missing/hash-drift evidence is requeued for fresh audit.
An accept recorded before the farm stored identities is bound once, when the
ledger's `c.source_sha256` equals the current source and the latest audit is
still bound to intact snapshots of the current source and hints
(`identity-backfilled` event). Without that proof it is audited afresh; on
19 Nov, 25 such records had caused 362 repeat audits across restarts. Legacy
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
- A translation of 2,000 bytes or more with no `recovery/c` source,
  no earlier candidate and no resume candidate starts with the `cmre draft`
  output already in its scratch source. Repair that file in place and still
  check every statement against the listing; the first test run of a seeded
  draft is a build check, and its build failures are the first thing to fix,
  not a result. The next rung, and a retry after a usage limit, keep repairing
  the same lineage; when that lineage's source has shrunk below a quarter of
  the draft, the farm reseeds the draft. Outside the lineage, a retried
  candidate is replaced by the draft only when it is that small and declares
  itself unfinished. The draft is translation input, never evidence: the gate
  and audit decide as before.
- Never shape code generation to pass: no `register`, no optimize attributes,
  no volatile locals added only to change register allocation, no inline
  assembly. A frame layout or a pass at one optimization level is not evidence.
- Unknown meaning stays unnamed. Never invent logic, defaults or initialization.
- Five failed tests (build failures count, except those of a seeded draft)
  end the attempt. Report the first
  remaining mismatch on the final line; the farm escalates to a stronger model.
- A candidate that still needs work must keep its truthful
  `UNVALIDATED CANDIDATE`, `must not be accepted` or `BLOCKER:` statement.
  The farm refuses such a source; never delete a disclaimer merely to get it
  accepted. Resolve the boundary, or leave the candidate blocked for the parent
  input lane. A `BLOCKER:` statement parks the function as `safeguard-blocker`.
  A `must not be accepted` statement is gated first: a pass parks as
  `safeguard-blocker` (the worker says the gate cannot see the problem), and a
  fail is a failed attempt. A failed attempt with an `UNVALIDATED CANDIDATE` or
  `must not be accepted` statement escalates to the next rung and, at the top,
  parks as `worker-failure`. Before 22 Nov a failing `must not be accepted`
  candidate parked without a gate: 96 of 162 safeguard parks.

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
11. Unproven consumption (Adam, 2026-11-19): upper bits or register residue that
    differ from the original (a zero-extended byte where the original pushes a
    whole register; ECX/EDX/EDI left by an earlier call) are a finding only when
    listing VAs show the consumer reads those bits. Otherwise list the item under
    `"boundaries"` in the verdict: an explicit unknown boundary, kept out of the
    open-defect count and shown as `boundaries N` in `status`. Accepted functions
    audited before this rule are re-judged with `py -3.13 scripts/cmre_farm.py
    reaudit RUN fn_X... --reason "..."`.

Audit workers do not run `cmre mutate` (it outlives the worker shell's 2-minute
timeout). When triaging an open defect by hand, `cmre mutate fn_X` shows operator
mutants that survive the corpus; a surviving mutant on a line the listing proves
is a weak test, not a defect by itself.

### Defect lane

`scripts/cmre_defect_lane.py run RUN` runs the [defect subagent](#defect-subagent)
procedure headless, one open defect per job, largest accepted function first.
The farm keeps it running while `--defect-lane-workers` is above 0 (default 0;
`set RUN --defect-lane-workers 2` turns it on live, `--defect-lane-model` picks
the model; the lane's default is `openai/gpt-6.2-sol`). Each job's claim name
(`defect-lane-N-…`), review receipt and hand-source path are fixed before
dispatch. A final `ADOPT` or `DISMISS` line naming exactly those paths becomes
the farm's checked `adopt`/`dismiss` request; anything else is recorded as
unresolved. A defect gets two looks per (accepted source, hints, findings); a
change to any of them starts again. The lane takes a defect only after the farm
reports it open (its own automatic fix has finished), and skips quarantined
functions, functions with a queued request, and functions someone else claimed
for review in the last 6 hours, so a coordinator working a defect by hand is not
raced. Only closing text that no tool call follows counts as a result; a worker
killed at its time limit submits nothing. Provider limits pause the model
without spending an attempt.

- `py -3.13 scripts/cmre_defect_lane.py plan RUN` lists what it would work;
  `status RUN` gives counts. While `--defect-lane-workers` is above 0, `status`
  on the farm shows `defects lane: running N, adopt A, dismiss D, unresolved U`
  (adopt and dismiss count submitted requests).
- Smoke runs on a live farm: `run RUN --only fn_X --workers 1 --no-submit`
  records each adopt/dismiss command in `RUN/defect-lane/smoke/state.json`
  without queuing it, stops after one recorded result per defect, and leaves the
  live lane's state and attempts alone. Its `defect-smoke-…` claims and hand
  files are new immutable files under `RUN/reviews/` and `RUN/hand/`; nothing
  else in the run changes.
- With the lane on, the coordinator no longer sends `cm-defect` for
  `open-defect` events; `UNRESOLVED` rows in `status RUN` go to the handoff.
- Stopping: the lane obeys `RUN/STOP` and `RUN/defect-lane/STOP`; a hand-written
  `defect-lane/STOP` pauses it until deleted. State and worker logs are local
  evidence under `RUN/defect-lane/`; never publish them.

### Input lane

[ADR 0013](../adr/0013-input-lane-authored-entry-inputs.md). The farm keeps one
`scripts/cmre_input_lane.py run RUN` process beside its translators
(`--input-lane-workers`, default 4; `set RUN --input-lane-workers N` changes it
live, and 0 stops dispatch). It is the farm's own supply source. Each job asks
one ladder model to write a compact input spec for one unlifted function
(arena records, stack words, globals, `vary` axes) and gate it with
`scripts/cmre_input_probe.py`. The lane then re-gates the spec itself, appends
the fixtures that reach new instructions to the canonical hints under the writer
lease, and runs `cmre accept`. A refused accept restores the previous hints.
New lifts reach the C translators on the next frontier rescan.

- When a worker probe finishes with a pass or mismatch, the lane can collect it
  before the worker exits. It freezes the spec only if its hash and the probe's
  base-hint hash match, then stops the worker and runs its usual independent gate
  on that snapshot. A probe result is not acceptance evidence.
- Order: nearest the coverage threshold first, then larger. A never-gated
  function is gated once first, and if that gate passes it is accepted with no
  model call. Each function gets three attempts per canonical-hint state, one
  ladder rung each. Each later attempt starts from the best spec so far.
- Skipped: hard work holds, blocked records, lift `lift-blocked`, `fail` and
  `error`. A mismatch found with authored inputs is recorded in
  `RUN/input-lane/state.json` for triage and is never reshaped away.
- Stopping: the lane obeys `RUN/STOP` and `RUN/input-lane/STOP`, kills its
  running workers and finishes in-flight gates. At its deadline, or when an
  `--exit-when-empty` farm drains, the farm writes the lane's STOP itself and
  waits for it to exit. The farm removes only its own marker on a later
  relaunch (for example after `set --add-hours`). A hand-written
  `input-lane/STOP` pauses the lane until you delete it.
- Poor targets: functions whose uncovered code lies behind an import result,
  such as file open or allocation failure. The lane adds no import transcripts,
  so after three `no-new-reach` attempts it leaves them alone.
- Watch it with `cmre_farm.py status RUN` (`inputs: running N, accepted K fn /
  B`), `scripts/cmre_input_lane.py status RUN`, and `plan RUN --top 20` for the
  next candidates. Events, specs, proposals and worker logs are local evidence
  under `RUN/input-lane/`; never publish them.
- An expert session does not hand-author fixtures for a function the lane can
  reach. Improve the lane instead (prompt, probe spec format, ordering) and
  measure the change in bytes moved out of `unlifted`.

## Session throughput rules (measured 2026-11-21)

Review of the five coordinator sessions of 20–21 Nov: about 23 hours produced
+25 accepted C functions (12.7 KB). Run 20261115T1250Z was supply-empty for 47 of
its 131 hours; sessions spent most turns on single-function probes, reviews and
evidence edits. 70% of the remaining bytes are unlifted and need inputs, not
translation. So:

1. The farm never stops for the session: `--hours 0`, no stop for preflight,
   tests, tooling or reviews. Make `cmre/` changes in a worktree while a writer
   holds the lease.
2. Expert turns go to byte pools: inputs for unlifted and low-coverage
   functions (capture rounds, expander improvements, fixtures) and one harness
   family release per session. Hand-repair a single function only when it is
   the largest lane in `lanes RUN --actionable`.
3. Measure work in bytes moved by `py -3.13 -m cmre pipeline survey`. After
   about 45 minutes without bytes moved, record the blocker in one line and
   switch lanes.
4. Write the evidence note once at the handoff, not after each step.
5. When Adam is away, run a capture round ([CAPTURE.md](CAPTURE.md)): recorder
   at the main menu, a new game with every league selected (21 Nov: Brazil
   2002 start), then holiday with the stall-only driver. The lift lane picks up
   new captures by itself.

## Supply lane (Codex / GPT)

The farm only drains supply that the gate can test. Bytes stuck outside it are
the real limit (`py -3.13 -m cmre pipeline survey`): on 2026-11-13, 71 KB
ready, 654 KB held, 2.03 MB unlifted.

Measured 2026-11-16 22:00 at the end of run
`20261115T1250Z-sol-commandcode-supervised` (194 functions / 227 KB accepted;
supply empty from 19:58). `survey` bands: authored 195 fn / 158 KB (C written,
not accepted), lifted 185 fn / 229 KB (no C yet), unlifted 1,060 fn / 1.98 MB.
By work state: evidence-needed 207 fn / 216 KB, harness-blocker 158 fn / 150 KB,
translation-defect 10 fn / 3 KB, ready 3 fn / 16 KB (parked by the farm after
gate mismatches, so not dispatchable). Translation capacity is idle until supply
moves; these are the byte pools, largest first:

| Pool (work-state blockers) | Bytes | What releases it |
| --- | --- | --- |
| Open audit defects on accepted sources (`cmre_farm.py defects RUN`) | 156 KB, 115 fn | The defect subagent or by-hand fixes, then `adopt`/`dismiss` (below). By cause: x87/float 34 KB, call order and reloads 33 KB, stack residue 19 KB, register ABI 18 KB, entry-stack alias 17 KB (11 fn; plain C cannot bind a read ahead of the prologue, so these need an ABI entry decision, not a fix attempt) |
| Import out-parameters (`native-directory-output`, `file-search-import-outparams`, `shutdown-uninitialized-find-output`, `cleanup-file-enumeration-output`, `unwritten-*`) | 97 KB, 37 fn | Import stubs that write their output buffers (recorded results, item 1). Two accepted functions (fn_006be710, fn_0070d690) pass only by frame-size luck on this family |
| x87 live stack across `__ftol` (`x87-live-stack`, `original-live-x87-argument`, `live-x87-call`) | 60 KB, 7 fn (fn_00808690 26 KB, fn_00846920 13 KB, fn_0085fc50 11 KB, fn_00695240 5 KB) | A bridge like ADR 0010 for calls to `0x00945640` that keep deeper x87 values live across the call. Plain-C `__ftol` (ADR 0006) covers only an empty stack |
| `_CIpow` operand evidence (ADR 0010 condition 3; work blocker `x87-power-boundary`) | 12 accepted fn (10 kept local) | Applied 16 Nov. Breaking each operand left 10 of 12 farm accepts still passing (`diagnostics/x87-cipow-bridge-20261116/mutation-sweep/`), plus fn_004cda70 and fn_006eecb0 (bridged candidates staged). Needs evidenced `c_hints` fixtures until each mutant fails. fn_004d1600 and fn_007109f0 (24 KB) failed translation after the release |
| Gate mismatches with no blocker (`(none)` family) | 34 KB, 48 fn | Parked after mismatches on every ladder model: hand translation (`cm-decompile`) from the first failing case |
| Input-bound candidates (`cmre_farm.py inputs RUN`) | 34 KB, 53 fn (14 KB / 21 fn at coverage 0.5 or more) | Authored `c_hints` fixtures; the farm re-gates the retained candidate by itself |
| Stack-probe fault address (`callee-stack-probe-address`, `*stack-probe*`) | 22 KB, ~7 fn | A harness rule for a stack-exhaustion fault at the same original instruction whose address differs only by frame layout (`fn_006146b0`: 598 of 600 cases equal) |
| Virtual-entry inventory (`missing-real-virtual-entry`, `virtual-target-inventory-entry`) | 19 KB, 35 fn | Inventory entries for real vtable targets (the `fn_0051c020` seed flow of 15 Nov) |
| Exit and post-fatal continuation (`exit-return-stack-continuation`, `returning-exit*`) | 12 KB, 23 fn | A harness rule for execution after a call that does not return in the game |
| Unlifted | 1.98 MB, 1,060 fn | Captures from the running game (items 1 and 4) |

The import, x87 live-stack, stack-probe, virtual-entry and exit pools are each
one harness release, not per-function work: retrying those functions on any
model repeats the recorded failure. Sessions build these releases themselves
(Adam, 2026-11-16): an ADR, the change in a worktree with failing regression
tests first ([HARNESS.md](HARNESS.md), [ADR 0007](../adr/0007-harness-releases.md)),
the release's [validation gate](HARNESS.md#validation-scope) at a batch boundary,
then `retry` of the family. An independent reviewer clears the finished release before it
merges to `main` ([ADR 0011](../adr/0011-engineering-decision-rights.md)); nobody waits for
Adam to start or merge one. The fidelity rules still bound
every release: plain C calls original code across the boundary, and nothing
invents API results or engine behaviour. Expert sessions work this list in
order while the farm runs, and never coordinate the farm:

1. **Import-result recording** (authorized 2026-11-13). Extend the capture
   recorder ([CAPTURE.md](CAPTURE.md), ADR 0009) so a replay continues with
   the real results of `HeapAlloc`, `ReadFile` and other imports instead of
   seeded stub values. 181 captured functions (455 KB) stop below 0.6 coverage
   mostly on this. Recording needs the game running with the recorder attached.
   Recording drives the desktop: ask Adam whether he is away from the computer.
   When he is, run it unattended (approved 2026-11-16): attach at the main menu,
   create a game, play past any match on day one, then send the manager on
   holiday (manager menu; disabled on match days) so the assistant picks teams,
   plays matches and handles transfers. A click during holiday asks "Return
   from holiday?", so the driver acts only when the window stalls (the 16 Nov
   run's `ui.py` and `overnight.py` are in `CM0102/Runtime/host-capture-20261116/`).
   Posted clicks work; the first click after an Escape is often swallowed.
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
