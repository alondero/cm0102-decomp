# CM 01/02 recovery coordination

Coordinate faithful recovery of version 3.9.68. The authoritative local checkout contains the original executable, analysis, generated recovery records, and the tools required for validation. These remain local. This repository has a fresh history and must never receive game assets, original executable bytes, dumps, Ghidra exports, lifted instructions, local analysis, credentials, or diagnostic snapshots.

The pinned input has SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`. See [the execution plan](PLAN.md) and GitHub issues for the remaining work. This repository alone does not provide the proprietary inputs or the local recovery state.

Each function has one stable owner shard. Workers use the authoritative checkout, a unique claim, and bounded assignments; a single integrator reviews and accepts serially. Separate worktrees are not yet safe for normal recovery commands: the local evidence, claims, failure counters, and ledgers do not all share the same state. The worktree infrastructure issue must be completed and tested before claiming otherwise.

The checked-in source and tooling are an initial manually reviewed subset, not a complete or independently runnable snapshot. The two `fn_007a16*`/`fn_007a1710` memory-status files are unaccepted drafts; their local work records contain outstanding import and stack obligations. The authoritative checkout remains the source of truth for all acceptance records and validation. Additional files require review before publication; see the source migration issue.
