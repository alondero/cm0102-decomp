# Parallel recovery plan

## Baseline

On 2026-10-30, the local `cmre status` reported 3,322 accepted C/legacy functions (1,347,928 / 4,309,884 inventory body bytes, 31.275%), 8,035 / 9,241 lift-inclusive functions (2,111,803 bytes, 48.999%), and zero integrated functions. The inventory also reports 3,898 compiler-generated entries, 23 blocked entries and two accepted C/legacy entries outside inventory. These populations overlap; do not add them as independent progress totals.

The current lifted-to-C queue, including held items and excluding generated entries, has 815 entries across shards 1–8: 102, 94, 97, 92, 115, 111, 100, 104. Of these, 149 are currently dispatchable. These are a starting snapshot, not the whole remaining inventory. Additional work enters as lifts are accepted or holds resolved.

## Ownership and scheduling

Fix the shard count at eight. For every pinned inventory entry with preferred-image VA `va`, compute `1 + crc32(ASCII lowercase 'fn_%08x' % va) % 8`. Each entry belongs to exactly one shard throughout its lifecycle, including held, unlifted, blocked, and compiler-generated entries. The `cmre next --c --shard K/8` selector applies only to the C frontier, not the whole inventory. Do not interpret an empty queue as a completed shard. Inventory splits, merges, missing entries and functions outside the inventory are handled by the inventory reconciliation issue.

For each shard, an owner coordinates independent workers. In the authoritative checkout, workers claim up to three functions from one family or subsystem with `py -3.13 -m cmre next --c --claim UNIQUE_OWNER --shard K/8 -n 3`, inspect their instructions with `cmre brief`, write their assigned C and hints only, run `cmre test fn_X --c`, and report pass or WIP with evidence. Reviewers independently inspect the original instructions and boundary contracts. The single integrator runs `cmre accept fn_X --c` serially, maintains the shared state and closing gate, and measures accepted bytes and review corrections. Stop or hold work according to the existing local workflow; never reset failure counts by switching workers.

Shard completion requires reconciliation of every pinned inventory entry in the shard: faithful C/legacy acceptance, or a verified compiler-generated disposition bound to its parent and regeneration path. Resolve all associated holds and blockers; record the versioned inventory identity, coverage counts, relevant receipts, and acceptance evidence. A lift, a passing draft, or a lack of ready items does not establish completion.

## Separate enabling and milestone work

Cross-cutting issues cover inventory and generated-code reconciliation; shared callback, register, stack, x87, import, and budget boundaries; same-machine isolated worktree coordination; runtime reference and replay; composed subsystems; hybrid integration; and the standalone faithful game. These issues are dependencies, not additional owners of function IDs. Parallelize their own independent evidence and implementation slices with assigned reviewers and a single integration owner.

The runtime, hybrid, subsystem, and standalone gates require the corresponding local roadmap evidence. A 100% C counter alone is not proof of a standalone game; original dependencies, complete game and season scenarios, match behavior, save/load, and continued progression all require parity evidence. Modernization follows the standalone gate.

## Publication and worktree boundaries

The original checkout's history contains local inspection material and must not be pushed wholesale. Keep supplied assets, raw instruction listings and byte dumps, lifted instructions, local analysis, diagnostic fixtures and logs out of this repository and its Git history. Never place secrets or local game data in issues or comments. A repository worktree does not by itself provide the local binary, analysis, shared claims, failure counters, or authoritative ledger. Until the worktree issue is complete, run recovery in the authoritative local checkout using claims and file ownership; worktrees may hold coordination or tooling changes only.

Use the number of open issues to monitor chunks, not as a weighted percentage. Each issue may contain a very different amount of work. Update it with accepted-function and accepted-byte deltas, blockers, and local receipt references without publishing prohibited material.
