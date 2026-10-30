# Start a parallel recovery worker on the local machine

1. The integrator starts a parent preflight in the authoritative checkout with `py -3.13 scripts/cmre_session.py start` and shares its receipt. Do not start a second integrator.
2. Pick the assigned issue shard and a unique worker name. In the authoritative checkout, claim at most three entries in that shard, for example `py -3.13 -m cmre next --c --claim worker-unique --shard 1/8 -n 3`. Use the shard assigned to you. The selector covers only currently eligible C work.
3. Read the local `docs/agent/C.md` and `docs/agent/PARALLEL.md`. Brief each claimed function, modify only assigned source and hints, compare against original instructions, and run the prescribed tests. Stop on the recorded failure limit or a known blocker.
4. Report pass or WIP with specific evidence to the integrator. A reviewer inspects the source independently; only the integrator accepts, manages shared records, and performs the parent closing check.

The proprietary binary and analysis must remain in the authoritative local checkout. The content in this Git repository is incomplete and is not the tool runtime. A separate worktree may be used for coordination or tooling, but do not run independent recovery claims or acceptance from it until the worktree adapter issue is complete. Creating separate worktrees and linking only `CM0102/Analysis` would split other authoritative state and is unsafe.

From any coordination worktree, `./worker.ps1 -AuthoritativeCheckout F:/src/cm0102-decompiled -Shard 1 -Owner worker-unique` checks the local target hash and previews that shard using the authoritative checkout. Add `-Action Claim` to make a real three-function claim there. The worker must then edit and test the claimed files in that checkout under the ownership rules above. This helper does not make recovery state worktree-isolated.
