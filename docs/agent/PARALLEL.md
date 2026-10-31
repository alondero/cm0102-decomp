# Run several agents at once

Use when more than one agent session (Claude Code, Codex, or another shell-capable
agent that reads `AGENTS.md`) works on recovery at the same time. Every role
follows `AGENTS.md`; this page assigns who may do what so sessions never collide.

## Roles

| Role | How many | Does | Never does |
| --- | --- | --- | --- |
| Integrator | exactly one | Preflight, tooling/harness changes, machine jobs, acceptance, closing check, handoff, commits | Translate while translators wait on review |
| Translator | as CPU allows | Claim, brief, write `recovery/c/fn_X.c`, `cmre test --c`, report one line | Accept, block, edit `cmre/`, `tests/`, `docs/`, `recovery/include/` or another agent's files |
| Reviewer | one per two to four translators | Review `needs-review` sources against the listing ([REVIEW.md](REVIEW.md)) and return a verdict | Accept, or edit the source under review |
| Lane owner | optional, one per lane | A separate lane: lift coverage, triage, a subsystem campaign | Touch the C queue |

Use a reviewer from a different model family from the translator where available.
Their errors are less correlated; measure this before relying on it.

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
Follow [C.md](C.md) for the translation itself; return `fn_X pass|wip: <detail>`.

## Integrator loop

1. `cmre_session.py start` once; translators inherit its receipt.
2. Feed reviewers from `cmre work list --state needs-review`; accept approved
   sources serially with `cmre accept fn_X --c`. Acceptance takes an exclusive,
   non-waiting OS lock: only the integrator accepts, so there is no contention.
3. Keep harness modules (`HARNESS_MODULES` in `cmre/recover.py`) frozen while
   translators run: a change alters every comparison in flight and its required
   revalidation holds the acceptance lock for hours. Batch harness changes and
   run the revalidation at the end of the day; resume an interrupted one with
   `--resume RECEIPT`.
4. Close with `cmre_session.py check` after translators stop.

## Limits

- Shared-checkout workers separate by file ownership. Same-machine isolated
  worktrees use the coordinator-mediated bounded C lane in [WORKTREES.md](WORKTREES.md).
  Workers receive ignored evidence snapshots and brokered validation; they never
  junction the recovery directory or own a separate acceptance ledger.
- `cmre test --c` takes no lock; CPU is the limit (24 logical processors here).
  Leave headroom for the integrator's acceptance reruns and do not add
  translators during a machine job.
- Measure before scaling: `cmre pilot` ([THROUGHPUT.md](THROUGHPUT.md)) per
  configuration. Report accepted bytes per hour and review corrections; more
  sessions without more reviewed acceptances is not progress.
- Tool-less chat models use task packets ([TASKS.md](TASKS.md)), not this page.
