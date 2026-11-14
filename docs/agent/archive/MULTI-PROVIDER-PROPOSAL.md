# Multi-provider design history and implementation status

The operating workflow is now [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md):
a 150-minute total campaign, two external workers initially, coordinator-owned
evidence and acceptance, and an explicitly requested end-of-run retro.
[PROVIDER-WORKERS.md](PROVIDER-WORKERS.md) documents the implemented process adapter.
[WORKTREES.md](WORKTREES.md) owns isolated source edits and brokered tests.

## Implemented and exercised

As of 2026-11-08:

| Capability | Implementation |
| --- | --- |
| Standalone worker launch, status, cancellation and collection | scripts/cmre_provider_worker.py |
| Process-tree cleanup, terminal receipts, scope/pin audits and model identity | Adapter, fake-process regressions and real scoped pilots |
| Claims, immutable packets, attempts and broker tests | Existing cmre worktree/task/work protocol |
| Independent evidence review and serial local acceptance | Coordinator through cmre worktree review/publish |
| Finite cohort timing and usage accounting | Existing cmre pilot |
| MiniMax and Go access | Both completed small reviewed C cohorts through OpenCode1.18.3 |

These pilots qualify the exercised configurations for bounded use. Different
functions, prompts and validation costs prevent a pure model comparison.
Private raw receipts, quota snapshots, native artifacts and account setup remain
local and are linked by the ignored next-session handoff.

## Remaining orchestration boundary

The single-worker adapter is not a campaign scheduler. It does not own global
deadlines, role transitions, selection, claims, review backpressure, bulk usage,
acceptance, skill invocation or retrospective decisions.

A future thin batch driver may automate deterministic launch/wait/collection
around the current broker and pilot records. Test process failures, global
deadlines, restart recovery, duplicate ownership, output validation and frozen
input checks before unattended use. Keep instruction review and acceptance with
the coordinator. At run completion the driver returns a bounded evidence bundle;
the coordinator applies the user-requested retro skill and updates the handoff.

This remaining tooling work is separate from the next recovery campaign.
The coordinator can run that campaign using current commands. Do not create a
parallel queue or postpone all recovery until a new framework exists.

## Historical references

The original 2026-10-23 proposal preceded the adapter and used shared-checkout
examples. Those examples and its unimplemented-launcher next-session request are
superseded. [PROVIDER-RESEARCH.md](PROVIDER-RESEARCH.md) retains the initial client
investigation; current account access, role qualification and consumption require
live evidence under the operating workflow.
