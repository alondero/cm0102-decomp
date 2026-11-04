# Run several agents at once

Use when more than one agent session (Claude Code, Codex, or another shell-capable
agent that reads `AGENTS.md`) works on recovery at the same time. Every role
follows `AGENTS.md`; this page assigns who may do what so sessions never collide.

## Roles

| Role | How many | Does | Never does |
| --- | --- | --- | --- |
| Integrator | exactly one | Preflight, tooling/harness changes, machine jobs, acceptance, closing check, handoff, commits | Translate while translators wait on review |
| Translator | as CPU allows | Own assigned candidates through input repair, source, tests, controls and an evidence bundle | Accept, block, edit repository tooling/tests/docs/headers or another owner's files |
| Reviewer | one per two to four translators | Review `needs-review` sources against the listing ([REVIEW.md](REVIEW.md)) and return a verdict | Accept, or edit the source under review |
| Lane owner | optional, one per lane | A separate lane: lift coverage, triage, a subsystem campaign | Touch the C queue |

Use a reviewer from a different model family from the translator where available.
Their errors are less correlated; measure this before relying on it.

## Candidate ownership

Apply the [delegation bounds](PLAYBOOK.md#delegation-only-when-the-user-authorizes-it).
Assign complete candidates with a named owner and writable scope. Shared-checkout
owners may edit their assigned C and ordinary input hints when the task authorizes
both. Discovery declarations, non-input ABI/return hint bindings, acceptance
policy and global tooling stay with the coordinator. Isolated worktrees retain the broker, write scope and size
limits in [WORKTREES.md](WORKTREES.md); ownership grants no new API.

The owner follows the [C loop](C.md) and [input-family loop](TRIAGE.md#input-family-loop)
through ordinary diagnosis and repair. A newly identified input field remains
owner work within its budget. A genuine unsupported ABI, native/import boundary,
exhausted retry allowance or worker budget produces a precise hold with retained
partial evidence. Keep cumulative failure history intact.

An acceptance-ready bundle freezes source and hints, pins the complete passing C
baseline, and accounts for every required computation mutation with its intended
observable failure. Include source/hint/report hashes, harness and input policy,
native review references, remaining limitations and lifetime attempts. Static
proposals and original-only observations carry their stage explicitly; their
filename or READY label alone never admits them to the acceptance queue.

The independent reviewer checks the frozen bundle. The coordinator performs
serial acceptance and the normal fresh repeat; routine baseline/control execution
belongs with the owner. Where a broker stage is unsupported, return one frozen
batch request through that protocol rather than inventing a per-function driver.

### Controller readiness

Use one controller for supported dispatch, execution, collection and review queues.
Before launching any child, check UTF-8 prompts, artifact schema and stage, source
and hint hashes, current claims/bindings, recorded execution environment and the
selected protocol's capabilities. Record command, exit status and receipt paths;
surface failed stages promptly and resume from retained successful stages.

Controller changes need focused deterministic tests before wider dispatch. Cover
encoding rejection before launch, stale or missing evidence, changed hashes and
environment, interrupted execution and proposal-versus-acceptance readiness.
Promote prototypes through the [tooling workflow](HARNESS.md#tooling-changes).
Existing `cmre work` state remains the scheduling authority; controller receipts
do not replace claims, independent review, acceptance or validation gates.

## Translator loop

```powershell
py -3.13 -m cmre next --c --claim codex-1 -n 3      # claims 3 for this agent (3 hours)
py -3.13 -m cmre brief fn_X                         # then write recovery/c/fn_X.c
py -3.13 -m cmre test fn_X --c                      # pass -> needs-review; fail -> translation-defect
py -3.13 -m cmre work release --owner codex-1 fn_X  # only when abandoning an unwritten claim
```

Owner names are unique per session (`claude-1`, `codex-1`, `gemini-1`). A written
source keeps the function out of every queue, so a claim only has to cover the
time until the source exists. `cmre work claims` lists live claims. Agents that
cannot share the claims file can split the queue statically with `--shard K/N`.
That selector covers only the C frontier. `cmre shards [--shard K] [--ids]` reports
the whole pinned inventory per shard of eight: accepted source, generated, blocked,
held, C-queue and unlifted entries plus outside-inventory exceptions. An empty queue
is not completion; only `COMPLETE` there is. `--write` saves a content-addressed
local report and `--since REPORT` shows per-shard deltas and inventory changes.
Follow [C.md](C.md) for translation; return `fn_X pass|wip: <detail>` with
the frozen bundle or precise hold artifact path.

## Integrator loop

1. Follow [parent preflight](PLAYBOOK.md#1-preflight): reuse its receipt on
   continuation; start once for a new parent. Translators inherit the receipt.
2. Feed reviewers from `cmre work list --state needs-review`; accept approved
   sources serially with `cmre accept fn_X --c`. Acceptance takes an exclusive,
   non-waiting OS lock: only the integrator accepts, so there is no contention.
3. Complete [recovery intake](HARNESS.md#recovery-intake) before dispatch and
   keep shared harness, headers and execution policies fixed for the batch.
   Freeze each candidate's source/hints for its baseline, controls and READY bundle.
   Prepare tooling in a separate lane and apply it at the next reviewed boundary.
4. Close with `cmre_session.py check` after translators stop.

## Limits

- Shared-checkout workers separate by file ownership. Same-machine isolated
  worktrees use the coordinator-mediated bounded C lane in [WORKTREES.md](WORKTREES.md).
  Workers receive ignored evidence snapshots and brokered validation; they never
  junction the recovery directory or own a separate acceptance ledger.
- `cmre test --c` takes no lock; CPU is the limit (24 logical processors here).
  Leave headroom for the integrator's acceptance reruns and do not add
  translators during a machine job. Revalidation pools are memory-limited on the
  64 GB host: 16 jobs hit `MemoryError` (2026-11-01). 8 jobs ran 5,598 audit tiers in
  53 minutes without one (2026-11-03).
  The validation budget is in [PLAYBOOK](PLAYBOOK.md#validation-budget).
- Measure before scaling: `cmre pilot` ([THROUGHPUT.md](THROUGHPUT.md)) per
  configuration. Report accepted bytes per hour and review corrections; more
  sessions without more reviewed acceptances is not progress.
- Tool-less chat models use task packets ([TASKS.md](TASKS.md)), not this page.
