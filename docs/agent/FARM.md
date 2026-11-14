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

Coordinator procedure:

1. `py -3.13 scripts/cmre_session.py start`, then `py -3.13 scripts/cmre_farm.py probe`.
   Drop an `UNAVAILABLE` model from `--ladder`; when every rung is down, tell Adam.
2. Resume the run named in `docs/NEXT-SESSION-PROMPT.md` when its `status` shows
   it running; otherwise start one (`RUN` = `CM0102/Analysis/cmre/farm/<UTC yyyymmddThhmmZ>`):
   `py -3.13 scripts/cmre_farm.py start RUN --workers 16 --audit-workers 4 --include-held --hours 10`.
   For every earlier run the handoff names, `py -3.13 scripts/cmre_farm.py defects OLD_RUN`
   lists its open defects; handle each as an `open-defect` event below with `OLD_RUN`.
   `adopt` and `dismiss` apply at once on a run with no live farm.
3. Run `py -3.13 scripts/cmre_farm.py wait RUN --minutes 60` as a **background**
   shell command and end your turn; OpenCode resumes you when it returns. Act on
   the reason it prints, in one short line per event:
   - `open-defect fn_X`: send one `cm-defect` subagent the prompt `RUN fn_X`
     (at most 3 at once). Its last line decides: `ADOPT <path> <note>` →
     `py -3.13 scripts/cmre_farm.py adopt RUN fn_X --source <path> --note "<note>"`;
     `DISMISS <reason>` → `py -3.13 scripts/cmre_farm.py dismiss RUN fn_X --reason "<reason>"`;
     `UNRESOLVED <why>` → leave it open and list it in the handoff.
   - `request-failed fn_X`: the hand fix failed the gate. One more `cm-defect`
     with the failure detail, then leave it open.
   - `model-unavailable`: the farm already moved to the next rung; probe once and
     tell Adam if a key needs renewal.
   - `model-limited`: a provider usage limit (OpenCode Go: 5 hours, shared by every
     DeepSeek model) paused that provider until the time `status` shows. The farm
     requeues without spending attempts and resumes on its own. Hold `cm-defect`
     work (DeepSeek) until then; tell Adam once, since Extra Usage is his call.
   - `supply-empty`: no testable function left. Let the run drain, then report the
     first supply-lane item below to Adam.
   - `timeout` (hourly): read `status` and apply step 5 of "Run by hand".
   - `farm-exit` or `farm not running`: read `RUN/farm.err`; start a new run when
     the session continues, otherwise go to step 4.
   Then wait again.
4. End: `py -3.13 scripts/cmre_farm.py stop RUN`, wait for `farm-exit`, run
   `py -3.13 scripts/cmre_session.py check`, write the handoff (below).

## Defect subagent

`cm-defect` (DeepSeek v4.1 Flash, 40 steps) resolves one open defect. It gets `RUN fn_X`.

1. Read `RUN/audit/fn_X.json`, `RUN/audit/fn_X.confirm.json` when present,
   `RUN/bundle/fn_X.md` (or run `py -3.13 -m cmre brief fn_X`), and `recovery/c/fn_X.c`.
2. Confirm each finding from the listing VA and instruction, or reject it. The
   "Audit checklist" below lists real defect classes; auditors also report
   false positives.
3. None confirmed: finish with `DISMISS <reason citing VAs>`.
4. Otherwise copy `recovery/c/fn_X.c` to `RUN/hand/fn_X.c` (the only file you may
   write), fix only the confirmed findings under the "Worker rules", and test with
   `py -3.13 scripts/cmre_farm.py test RUN fn_X RUN/hand/fn_X.c` (add `--case N`
   to explain a case). Five failed tests end the attempt.
5. Finish with one line: `ADOPT RUN/hand/fn_X.c <what changed>` after a pass,
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
   It runs in a hidden console that outlives the agent session, with
   `RUN/farm.out`, `RUN/farm.err` and `RUN/farm.pid`. `run` takes the same
   options in the foreground.

   Options: `--include-held` also takes held functions (each prompt carries
   the hold reason; the audit checks it). `--min-bytes/--max-bytes` bound the
   queue. `--ladder` is the model escalation order, cheapest first
   (default MiniMax M3.1 Flash → DeepSeek v4.1 Flash `#low` → DeepSeek v4.1
   Flash, DeepSeek via OpenCode Go; MiniMax M3 passed 3 of 13 on 2026-11-13 and
   is off the ladder). DeepSeek runs use v4.1 Flash; other models are Adam's call. `--audit-model` defaults to DeepSeek
   v4.1 Flash; when it is unavailable the strongest available rung audits.
   `--stall-minutes` (default 15) kills a worker whose transcript stops growing.
   OpenCode 2.x writes nothing while a step thinks, so a shorter limit kills real
   work. DeepSeek models take an effort variant (`opencode-go/deepseek-v4.1-flash#low`).
   `--opencode v2` (default) runs the npm OpenCode 2.x build with one temporary
   agent file per job in `.opencode/agents/` carrying its permissions; it has a
   256k output ceiling and the current credentials. `--opencode v1` runs the
   Chocolatey 1.x `opencode` on PATH, which caps output at 32k unless raised and
   reported the OpenCode Go key as expired on 2026-11-13 while 2.x worked.
   `--confirm-model` (default DeepSeek v4.1 Flash) is the second auditor; `''`
   disables it. `--skip-ranges` (default `0x00940000-0x00953000`, the x87
   runtime helpers behind 7 of 10 production defects) is never dispatched.
   `--verify-threads` (default 8) runs gate re-tests beside the dispatcher.
   Every job reads one generated `RUN/bundle/fn_X.md`: the brief with the
   listing, the draft, the rules and `cmre.h`.
4. Run `py -3.13 scripts/cmre_farm.py wait RUN --minutes 60` (it returns early on
   an open defect, an unavailable model, empty supply or the farm's exit), or
   check `py -3.13 scripts/cmre_farm.py status RUN` about once an hour. Status
   prints accepted functions and bytes, bytes per hour, parked items, audit
   results, open defects and pass rate per model. The farm needs no supervision
   between checks.
5. Act on the status, not on individual functions:
   - Pass rate of a model under 20% after 20 attempts: drop it from `--ladder`.
   - `pending 0` with free slots: supply ran out. Restart with `--include-held`,
     then work the supply lane below.
   - Workers barely load the machine (each is a waiting OpenCode session); gate
     re-tests are the CPU cost (24 threads here). Raise `--workers` toward 24
     while `status` shows no provider rate-limit failures; 500 KB/day needs about
     20 workers at the measured ~1 KB/h each.
6. End: `py -3.13 scripts/cmre_farm.py stop RUN` (running workers finish,
   nothing new starts; it logs `farm-exit`). When the process exits, run
   `py -3.13 scripts/cmre_session.py check` and write the handoff (below).

The run directory holds `state.json` (resumable: rerun `run` with the same
directory), `events.jsonl`, `src/` candidates, `bundle/` job inputs, `audit/`
verdicts, `hand/` defect fixes, `requests/` and `logs/` (prompts and worker
transcripts). It is local evidence; never publish it.

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
- Write only your scratch source. `c_hints` fixtures go in
  `recovery/hints/fn_X.json` under `"c_hints"`; run
  `py -3.13 scripts/cmre_hint_check.py recovery/hints/fn_X.json` first.
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
ready, 654 KB held, 2.03 MB unlifted. Expert sessions work this list in
order, through [HARNESS.md](HARNESS.md) for harness changes, and never
coordinate the farm:

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
   clears a whole family; then rerun the farm with `--include-held`.
3. **Fixture lane.** Record-table functions (`[0xae23bc]`, `[0xae23b4]`,
   `[0xb63c98]`, `[0xae2364]` chains) that play does not reach take authored
   `fixtures` in hints: a DeepSeek worker per function, evidence chains under
   `CM0102/Analysis/cmre/diagnostics/coverage-spike-20261103/specs/`.
4. **Expand and apply** after every capture session: `cmre capture expand`,
   then `cmre capture apply` and accept the passing lifts.

Each item's completion is bytes moved into `ready` or `lifted`, measured by
`survey` before and after.
