# Agent architecture review, 2026-10-10

Evidence: repo commits `9402f1a` through `d750e6d`, `docs/HISTORY.md`, ADR 0001,
the current instructions/adapters, and the local Claude transcript
`3e8b398c-69f5-4dd9-aa8b-aba32133bdfb` (2026-10-05 through 2026-10-08).
The 2026-10-03 Codex recovery session provides earlier continuation context.
Transcripts stay outside the repository. Line numbers below refer to the Claude
JSONL transcript; they identify evidence rather than recovery specifications.

The machine harness is the successful architectural change: it replaced bespoke
per-slice stages with one acceptance gate. The latest session reports a large
batch reaching the current 9.111% body-byte coverage; live status confirms the
result. Preserve that gate and spend agent reasoning on shared blockers.

## Changes ranked by impact

| Priority | Observed failure | Change |
| --- | --- | --- |
| 1 | Full pytest remained red, yet the session and handoff said green (lines 2709-2716, 2977). Test names containing RED were treated as expected failures without pytest marking them so. | `cmre_session.py` records real exit codes; start/run refuse machine work after failing preflight; check writes status only after pytest and verify both succeed. The handoff names the actual failures. |
| 2 | Wrap-up requests were followed by another full revalidation; interrupted calls left uncertain jobs, followed by restarts and harness edits while work ran (2411-2927). Piping to tail obscured results, and redirected logs had UTF-16/console-encoding problems (2051-2058). | A supervised runner streams UTF-8 logs, records child PIDs before waiting, and retains finished/interrupted receipts. An OS lock excludes concurrent runner jobs. The batch workflow freezes harness code and defines restart checks. |
| 3 | Root instructions, numbered playbook sections, skills, worker prompts, and handoff duplicated commands and disagreed. The handoff prohibited hints edits although triage and the guard permit them. The session skill launched machine work before checking the failing baseline. | One router and one procedure per branch; skills are thin adapters. Explicit tasks precede queues. Hints ownership is stated at its write step. Workers inherit parent preflight and return bounded results. |
| 4 | The handoff suggested `next -n 1` to reproduce a known blocked inference defect and forecast acceptance gains without measurements. Its counts/test totals were already stale. | The handoff names the existing failing tests and reproducer, distinguishes observed facts from old hypotheses, and points to live status instead of caching coverage. |
| 5 | Guard refusals consumed repeated shell rewrites and Git cleanup attempts (2989-3157). Bare-python false positives also occurred in quoted searches (1682, 1792). | Claude guidance directs allowed file edits through structured tools and retains guard failures as regression evidence. Independent shell commands limit whole-command false positives. The existing user worktree edit to `cmre/guard.py` is preserved. |
| 6 | The playbook required full accepted-inventory refresh for every harness edit even though `HARNESS_MODULES` excludes guard, CLI and queue changes. | The tooling workflow distinguishes evidence-changing modules from instructions/adapters and requires full revalidation when evidence changes. `verify` is explicitly an integrity check, not proof of current harness identity. |

## Validation and limits

Before edits, doctor passed and full pytest exited 1: **87 passed, 1 skipped,
2 failed**. Both failures are the existing struct-promotion contracts in
`tests/test_cmre_infer.py`; this architecture task keeps them intact. The focused
runner suite exercises failure gating, both green-gate commands, status-write
gating, CLI option forwarding, actual lock exclusion/release, and UTF-8/PID/exit
receipts. All **12 runner tests pass**. The final session check exits 1: full
pytest has **99 passed, 1 skipped, the same 2 failures**, and cmre verify exits
0 (`ledgers verified`). Generated status is not rewritten on that failed gate.
Receipt: `CM0102/Analysis/cmre/sessions/20261010T101730096986Z-96684-check/receipt.json`.
Python compilation, Markdown link checks, and `git diff --check` also pass.

| Final review axis | Result |
| --- | --- |
| Standards | No unresolved findings |
| Spec | No unresolved findings |

Both reviews found a post-launch receipt-write cancellation gap. A subprocess
regression reproduced the surviving child, then passed after moving PID receipt
saving inside the cleanup boundary. The final reviews checked that correction.

The runner supervises `batch` and `revalidate`; direct CLI invocations retain
their existing behaviour and do not acquire its lock. A forcibly killed runner
can leave a receipt marked running; inspect the recorded child PID and log before
restarting. A successful batch can include blocked functions. Large jobs repeat
preflight so a stale earlier pass cannot launch work after a regression. The
issue #24 runner retains doctor/status and reuses full pytest only through a
completed successful step whose declared inputs and log hash still match;
see [HARNESS.md](HARNESS.md#reusing-full-pytest) for the current contract.

Deferred: repair shared struct inference with the existing tests, then perform
full revalidation before expanding recovery. A guard-parser repair should cover
both benign quoted prose and a compound command that actually writes a protected
path; the pre-existing guard edit needs that separate review. Atomic refresh of
accepted records remains a harness concern: supervision prevents competing jobs,
but cannot make the current unlink/re-accept sequence crash-safe.

Use a future session to compare time to first useful action, repeated refusals,
and unfinished job restarts. This review proves the runner's safety properties;
it does not measure a throughput gain or agent behaviour across new sessions.
