# Championship Manager 01/02 faithful recovery

This project recovers the official 3.9.68 executable into buildable source that preserves its behavior and bugs. A validated standalone game precedes any modern port. The pinned original has SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`; its preferred image base is `0x00400000`.

## Local workspace and repository

The authoritative recovery checkout is on the same machine as the supplied game and local analysis. Its agents follow the local `AGENTS.md` and `docs/agent/PLAYBOOK.md`. Use `py -3.13` for Python. The executable, supplied assets, raw analysis, generated lifts, recovery records, hints, and evidence remain local and are not distributed by this repository.

This private repository contains the coordination plan and an initial reviewed subset of C and tooling. It is not a complete or independently runnable copy of the local workspace: substantial source and tooling still require publication review. A clean Git status does not mean those local-only or unreviewed files are backed up or synchronized. See [issue #22](https://github.com/alondero/cm0102-decomp/issues/22) for the remaining migration.

The local authoritative checkout also uses a Git exclude rule that hides untracked files by default. Review a file and its contents before explicitly adding it; never publish supplied assets, raw executable bytes, original instruction streams, local analysis, or credentials. The prior local Git history is retained separately and must not be pushed to this repository.

## Parallel work

See [PLAN.md](PLAN.md), [START-WORKER.md](START-WORKER.md), and the [issues](https://github.com/alondero/cm0102-decomp/issues). Eight stable shards divide function ownership. Workers claim bounded batches in the authoritative checkout, independent reviewers inspect them, and one integrator accepts results serially. The coordination worktrees use `worker.ps1` to reach the authoritative checkout; they do not yet isolate recovery state. The [worktree coordination issue](https://github.com/alondero/cm0102-decomp/issues/1) tracks that gap.

## Local checks

From the authoritative checkout, start a parent session with `py -3.13 scripts/cmre_session.py start`. The C workflow uses `py -3.13 -m cmre next --c`, `cmre brief`, `cmre test --c`, and `cmre accept --c`. Close the session with `py -3.13 scripts/cmre_session.py check --parent RECEIPT`; both pytest and `cmre verify` must pass. The local `docs/ROADMAP.md` defines the runtime, integration, subsystem, and standalone parity gates. Accepted C, lifts, composed calls, and live integration are separate measures.
