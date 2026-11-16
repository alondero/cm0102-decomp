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

The coordinator completes [recovery intake](HARNESS.md#recovery-intake) before
dispatch; workers inherit its frozen receipt. Authorized parallel batches use [candidate ownership](PARALLEL.md#candidate-ownership);
input repair uses the [input-family loop](TRIAGE.md#input-family-loop).

## 2. Route the task

First matching row selects the task workflow; an explicit user task outranks the queue.

| Task | Read next |
| --- | --- |
| Continue / make progress / decompile / use DeepSeek, MiniMax or other model credits (default) | [Recovery farm](FARM.md) |
| Translate one named function by hand, or a farm worker's rules | [C translation](C.md) |
| Agent instructions, skills, scripts, guard, CLI, queue, harness | [Tooling](HARNESS.md) |
| Failing baseline tests | [Tooling](HARNESS.md) |
| One named blocked function | [Triage](TRIAGE.md) |
| Recover or plan a named subsystem / coherent source slice | [Subsystem recovery](SUBSYSTEM.md) |
| Review recovered code, evidence or a milestone claim | [Evidence review](REVIEW.md) |
| Composition, campaign manifests, primary-runtime checkpoints | [Campaign](CAMPAIGN.md) |
| Planning remaining work, supply, accepted-byte targets | [Farm supply lane](FARM.md#supply-lane-codex--gpt) |
| Most remaining bytes are `unlifted`, or a lifted test is `low-coverage` | [Capture supply](CAPTURE.md) |
| Several concurrent agent sessions (any client) | [Parallel sessions](PARALLEL.md) |
| Git history, local-only files, or retiring worker directories | [Publication](PUBLICATION.md), then [workspace lifecycle](WORKTREES.md#workspace-lifecycle) |
| Tool-less or untrusted model workers (packets, leases) | [Task packets](TASKS.md) |
| REA setup or investigation | [REA](REA.md) |
| Runtime reference, live integration, milestone gate | Relevant gate in [ROADMAP](../ROADMAP.md) |
| Modern native port | [Port](PORT.md), then the relevant M6/M7 gate |
| Explicit lift campaign | [Batch](BATCH.md) |

A continuation means running the [farm](FARM.md) until the user's time or
token budget is used, while an expert session works its supply lane. A short handoff is not a reason to open a new
investigation; the queue and `cmre brief` supply the next unit.

### Worker allocation

Adam authorized MiniMax, DeepSeek and Codex/GPT use for decompilation
(2026-11-13). The [farm](FARM.md) is the allocation: cheap models translate and
audit, an expert model works the supply lane, and no allocation record or
dispatch receipt is needed. Older checkpoints with an `allocation` record still
resume; their workflow is archived under `archive/`.

### Delegated recovery (Claude Code subagents)

Outside the farm, give each `cm-decompiler` worker distinct function IDs. It
stops at a passing test and returns the ID; the parent accepts serially
(accept holds an exclusive lease). Packets and leases ([TASKS.md](TASKS.md))
are for tool-less model workers only.

### Throughput rules

- Use `cmre brief`, `show`, `explain --c`, `next --c` and `status` instead of
  ad-hoc `py -c` probes. If you need the same probe twice, add it to `cmre`
  with a test (tooling task) rather than a script under `CM0102/Analysis/`.
- Inspect JSON keys and project scalar status/exit/path fields before opening
  nested payloads. Keep fixture corpora and `inputs_before` behind artifact
  paths. For session receipts, use `py -3.13 scripts/cmre_session.py status RECEIPT`;
  it prints bounded recorded steps, PIDs and log paths without starting work.
  This view does not prove live process ownership or current-input validity.
  Check command help when syntax is unknown.
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
  Supply `--allocation ALLOCATION.json` to establish or update worker allocation;
  omission inherits this parent's latest finished checkpoint. Each checkpoint
  freezes its own `worker-allocation.json`; resume prints its mode and path.
  See [batch intake](archive/PROVIDER-CAMPAIGN.md#batch-intake) for the record format.
  Select at most 20 relevant IDs; the work registry retains all other holds.
  Start the next context from that handoff and the selected workflow, then
  read only the evidence packets needed for its batch.
- `cmre work list` discovers pending work; `cmre work show fn_X` gives one
  function's source acceptance, queue history and all holds. Use `work set` to
  group a shared blocker and name its retry condition; return to held work only
  after that condition is resolved. The five-failure allowance is a ceiling,
  not a reason to repeat a known harness failure.

### Validation budget

Run the cheapest level that answers the question. A higher level runs only when
its trigger has fired. This table is the single source for when validation runs.

| Level | Trigger | Runs | Does not run |
| --- | --- | --- | --- |
| Function | `cmre test --c`, then `cmre accept --c` | The differential test. Accept repeats it once in a fresh process as a determinism check. | pytest, `check`, `revalidate`, `status --write` |
| Batch | A handoff, or a batch of about 20 accepted functions or one hour, whichever comes first | `py -3.13 scripts/cmre_session.py check --parent RECEIPT`, once | Anything per function |
| Harness | An edit to a `HARNESS_MODULES` file in `cmre/recover.py` | `cmre revalidate --plan`, then one run with `--reuse` (or `--resume RECEIPT` after an interruption) | `--audit`, unless a milestone |
| Milestone | A named gate claim in [ROADMAP](../ROADMAP.md) | `cmre revalidate --audit`, and `compose-test` for each affected composition | Nothing is skipped here |

Why the levels sit where they do:

- Every accept changes `recovery/` files. The pytest reuse key covers all of
  `recovery/` (`cmre/session_evidence.py`), so every `check` after an accept reruns
  the full suite. Some tests read real recovery files, for example
  `tests/test_cmre_extended_indirect_transport.py`, so the key cannot be narrowed
  without proof. Run `check` once per batch.
- `check` and `revalidate` both hold the command lease. Run `check` after
  revalidation has finished, never during it.
- Revalidation pools: use `--jobs 8`; on 2026-11-03 it ran 5,598 audit tiers in 53
  minutes without a `MemoryError`. A 16-job run on the 64 GB host produced
  `MemoryError` in 7 tiers (2026-11-01). After an out-of-memory failure, resume
  with `--resume RECEIPT`; do not restart the whole selection.

Working rules:

- Harness edits belong outside the recovery run. The guard denies every `cmre/`
  edit while a writer holds the command lease (`cmre/guard.py`). Make experimental
  harness changes in a separate worktree ([WORKTREES.md](WORKTREES.md)), then bring
  back one tested batch for a single revalidation.
- Check a long job with `py -3.13 -m cmre job status RECEIPT` no more than hourly.
  Repeated status polling costs context without changing the result.
- A plan that lists a global harness change is the signal to stop and batch, not to
  start a second whole-inventory job.

## 3. Close the session

Run `py -3.13 scripts/cmre_session.py check --parent RECEIPT` (pytest, `cmre verify`, status
write). Green means both pytest and verify exited 0; it prints accepted-C
progress against that parent, even if another session started later. A failed
baseline may be bound for a tooling fix; it cannot be reused as a green preflight.
The legacy command without `--parent` still selects the latest successful start.
Matching completed full pytest evidence may be reused; changed declared inputs
invalidate it. Integrity always runs. Differential inventory evidence is separate.
On failure report exact failures and
whether they existed at preflight. After a green check, commit and push the
reviewed source-only batch under the standing authorization in AGENTS.md,
following [PUBLICATION.md](PUBLICATION.md) for scans and exact-commit/remote
checks. Supplied content and raw evidence remain local. `cmre task publish` remains
local acceptance, separate from Git publication.

For continuation, replace `docs/NEXT-SESSION-PROMPT.md` with at most 40 lines:
next task and workflow, progress line, unresolved failures with commands,
any active job receipt/PID, and WIP function IDs. Keep history out of it;
archive detail under `CM0102/Analysis/` and link it.
The linked pack is the index: current source/hint hashes, terminal or live job
receipts, independent verdicts, evidence stages, retained blockers and ordered
next commands. Reuse immutable artifacts instead of copying raw corpora into
the prompt. Completion: the prompt starts the next bounded task from that pack
without rediscovering a gate, claim or input witness.
`checkpoint --write-handoff` also replaces that file; add `--workflow HARNESS.md`
for tooling rather than recovery. Review the handoff for task-specific facts
outside the work registry. During an active supervised job, retain its receipt
and PID in the existing handoff; checkpoint publication requires the writer lease.
