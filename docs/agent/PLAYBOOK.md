# Agent playbook

`AGENTS.md` holds the fidelity and preservation contract. This file routes work;
the selected workflow holds its procedure. Acceptance policy:
[ADR 0001](../adr/0001-automated-acceptance-and-two-tier-recovery.md) and
[ADR 0002](../adr/0002-semantic-c-profile.md).

## 1. Preflight

Run `py -3.13 scripts/cmre_session.py start` once per parent session (doctor,
status, full pytest or exact-input receipt reuse; prints a receipt path). Workers inherit it. If pytest fails,
record the failing node IDs: a recovery continuation starts with that defect;
an explicitly requested docs/tooling task may proceed and report it. Note
existing `git status --short` changes and leave unrelated work alone.

On continuation, run `py -3.13 scripts/cmre_session.py resume --parent RECEIPT`.
It requires a finished green preflight and prints the latest checkpoint for
that parent. It runs no tests and makes no new validation claim. Keep the same
parent receipt through context compactions and bounded batches.

## 2. Route the task

First matching row wins; an explicit user task outranks the queue.

| Task | Read next |
| --- | --- |
| Continue / make progress / translate functions (default) | [C translation](C.md) |
| Agent instructions, skills, scripts, guard, CLI, queue, harness | [Tooling](HARNESS.md) |
| Compare recovery throughput or model/context choices | [Measured recovery](THROUGHPUT.md) |
| Failing baseline tests | [Tooling](HARNESS.md) |
| One named blocked function | [Triage](TRIAGE.md) |
| Recover or plan a named subsystem / coherent source slice | [Subsystem recovery](SUBSYSTEM.md) |
| Review recovered code, evidence or a milestone claim | [Evidence review](REVIEW.md) |
| Composition, campaign manifests, primary-runtime checkpoints | [Campaign](CAMPAIGN.md) |
| Several concurrent agent sessions (any client) | [Parallel sessions](PARALLEL.md) |
| Tool-less or untrusted model workers (packets, leases) | [Task packets](TASKS.md) |
| REA setup or investigation | [REA](REA.md) |
| Runtime reference, live integration, milestone gate | Relevant gate in [ROADMAP](../ROADMAP.md) |
| Modern native port | [Port](PORT.md), then the relevant M6/M7 gate |
| Explicit lift campaign | [Batch](BATCH.md) |

A continuation means translating functions with the C loop until the user's
time or token budget is used. A short handoff is not a reason to open a new
investigation; the queue and `cmre brief` supply the next unit.

### Delegation (only when the user authorizes it)

Give each worker at most three distinct function IDs from one family or subsystem
and the C loop. Workers write only
`recovery/c/fn_X.c` for their IDs, run `cmre test --c`, and return
`fn_X pass|wip: <detail>`. The parent runs `cmre accept fn_X --c` serially.
Packets and leases ([TASKS.md](TASKS.md)) are for tool-less model workers only.
Start a fresh worker at the batch boundary; do not refill a worker indefinitely.
Give it the exact IDs, header path, representative and member delta paths, and
the selected workflow. After 30 assistant messages, finish the current bounded
attempt and return its result; the parent assigns any remaining IDs afresh.
Measure worker, coordinator and review costs through [THROUGHPUT.md](THROUGHPUT.md)
before changing model tier or increasing concurrency. Model names alone do not
establish throughput. The parent reviews source evidence and accepts serially.

### Throughput rules

- Use `cmre brief`, `show`, `explain --c`, `next --c` and `status` instead of
  ad-hoc `py -c` probes. If you need the same probe twice, add it to `cmre`
  with a test (tooling task) rather than a script under `CM0102/Analysis/`.
- Harness modules (`HARNESS_MODULES` in `cmre/recover.py`) are frozen during a
  recovery session. For tooling changes, inspect `cmre revalidate --plan`, batch
  fixes and follow [HARNESS.md](HARNESS.md). Global/unknown impact requires the
  full inventory; bounded input dispatch needs an explicit compatibility receipt.
- Report progress as the `Progress since` line from the closing check.
- Reuse the parent preflight receipt across continuations and workers. Use
  function tests within a batch and one closing check at the parent boundary;
  a worker completion, context compaction or successful acceptance is not a
  session boundary. Required tooling revalidation remains separate.
- At a continuation boundary, run
  `py -3.13 scripts/cmre_session.py checkpoint --parent RECEIPT --next "TASK" --functions fn_X fn_Y`.
  This records a bounded handoff under ignored analysis storage, with current
  work states, failure counts and retry obligations. It also retains the latest
  parent gate receipt and a bounded summary of its failures, including pytest
  node IDs; complete failures remain in the linked logs. It runs no closing gate.
  Select at most 20 relevant IDs; the work registry retains all other holds.
  Start the next context from that handoff and the selected workflow, then
  read only the evidence packets needed for its batch.
- `cmre work list` owns pending review and blocker reasons. Use `work set` to
  group a shared blocker and name its retry condition; return to held work only
  after that condition is resolved. The five-failure allowance is a ceiling,
  not a reason to repeat a known harness failure.

## 3. Close the session

Run `py -3.13 scripts/cmre_session.py check --parent RECEIPT` (pytest, `cmre verify`, status
write). Green means both pytest and verify exited 0; it prints accepted-C
progress against that parent, even if another session started later. A failed
baseline may be bound for a tooling fix; it cannot be reused as a green preflight.
The legacy command without `--parent` still selects the latest successful start.
Matching completed full pytest evidence may be reused; changed declared inputs
invalidate it. Integrity always runs. Differential inventory evidence is separate.
On failure report exact failures and
whether they existed at preflight. Commit only when requested; never push.

For continuation, replace `docs/NEXT-SESSION-PROMPT.md` with at most 40 lines:
next task and workflow, progress line, unresolved failures with commands,
any active job receipt/PID, and WIP function IDs. Keep history out of it;
archive detail under `CM0102/Analysis/` and link it.
`checkpoint --write-handoff` also replaces that file; add `--workflow HARNESS.md`
for tooling rather than recovery. Review the handoff for task-specific facts
outside the work registry. During an active supervised job, retain its receipt
and PID in the existing handoff; checkpoint publication requires the writer lease.
