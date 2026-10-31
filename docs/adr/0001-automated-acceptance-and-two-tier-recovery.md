# ADR 0001: Automated acceptance gate and two-tier recovery

Status: accepted (2026-10-05, chosen by the project owner).

## Context

By 2026-10-04 the hand-built process had recovered 51 functions (1.65% of
4.31 MB of code) in about a week. Each slice needed roughly 20 bespoke stages
(candidate, audit, maintained, final-a/b, map, reviews), driven by about 140
uncommitted scripts in `CM0102/Analysis/`. A single map script was cloned per
slice. Status lived in README prose. This was too slow, too token-hungry for
agents, and not reproducible from a fresh clone.

## Decision

1. **One automated acceptance gate** for new work: `py -3.13 -m cmre accept <va>`.
   It lifts, builds and statically compares every instruction with the
   original. It then runs a seeded differential test in Unicorn against the
   original function (whole original image on both sides, callees original),
   repeats it on fresh machines, requires identical result digests, and writes
   one ledger record per function (`recovery/ledger/fn_<va>.json`).
   Existing hand-built ledgers stay frozen and are listed in
   `recovery/registry.json`.
2. **Two tiers of recovered source.**
   - Tier 1, `lifted-asm`: machine-lifted, labeled GAS assembly in
     `recovery/lifted/`. It counts as compiled + validated, not decompiled.
   - Tier 2, `c`: hand/agent-written C in `recovery/c/`. It must pass the same
     differential test at `-O0` and `-O2` under the `abi` comparison profile
     (superseded for new C by the `semantic` profile, ADR 0002).
     It sets `decompiled: true`.
3. **Tool-neutral core.** The `cmre` CLI, `AGENTS.md` and `docs/agent/PLAYBOOK.md`
   serve every agent runtime. Claude Code adapters (skills, subagents, guard
   hook) are thin wrappers.

## Consequences

- Fidelity rules are unchanged. Nothing is invented. Callees and imports stay
  explicit boundaries. Import stubs exist only inside tests and must be
  identical on both sides; they are never evidence of game behaviour.
- Coverage is reported per function. Low coverage blocks acceptance until
  hints (`recovery/hints/`) raise it. Pointer hints are inferred automatically
  where an exact fault/displacement relation proves them.
- Cases where the original reads its own instruction bytes as data are counted
  as `code_as_data_cases` and recorded. They are hazards for M4 relocation, not
  lifting mismatches. Above 2% of cases, the function is blocked for review.
- Tier-1 code is linked away from its original address to prove relocation.
  The rejected alternative was to link each routine at its original VA
  ("matching decompilation"). That is simpler but would hide relocation
  hazards that M4 hybrid integration must handle.
- M2 (runtime determinism), M4 (live integration) and M6 (standalone build)
  gates are unchanged by this decision.
