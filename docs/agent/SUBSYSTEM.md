# Recover a coherent subsystem slice

Use for a named subsystem or a request to turn accepted lifts into connected,
readable source. Follow parent [preflight and closure](PLAYBOOK.md). Read the
relevant M3/M5 gate in [ROADMAP.md](../ROADMAP.md); M2/M4 runtime obligations
remain distinct from local function acceptance.

1. **Bound the slice.** Use live `cmre status`, `next`, `show`, and the checked
   [recovery map](../RECOVERY-MAP.md). Select a small connected set with an
   observable input/state/output contract. Record exact function IDs, suspected
   boundaries, shared state, imports and original callees. Source-path strings
   are candidate attribution, not proof of subsystem ownership. Completion:
   a finite function list and a named observable result, with unknowns visible.
2. **Make an evidence packet.** Use the fields below. Put reviewable conclusions
   in a focused `docs/` note; raw dumps and exploratory code stay under ignored
   `CM0102/Analysis/`. Reuse a matching packet or REA snapshot instead of importing
   the game again. Use [REA.md](REA.md) only when a specific question needs it.
   Completion: each semantic claim cites original instructions or an identified
   runtime observation, with its confidence and limitations.
3. **Recover shared layouts before naming abstractions.** Collect base-relative
   offsets, access widths, array strides, aliasing, constructor/destructor use,
   and evidence from callers. Preserve unknown storage as explicitly unknown.
   A proposed field meaning needs evidence beyond a convenient name. Check
   packing, `sizeof`, and `offsetof` in the 32-bit recovery build before using
   a shared declaration. Proposed shared declarations go in `recovery/c/`;
   changing the common `recovery/include/` build contract routes through
   [HARNESS.md](HARNESS.md). Completion: observed layout facts and their consumers
   are listed; unknown initialization is preserved.
4. **Recover one routine at a time.** Unaccepted routines use [BATCH.md](BATCH.md)
   or [TRIAGE.md](TRIAGE.md); accepted lifts use [C.md](C.md) for readable source.
   Reconcile pseudocode with every original instruction, including indirect
   calls, switch tables, tail transfers and SEH/compiler support. Preserve
   x87 behavior, intermediate widths, overflow and incidental reads where they
   affect the contract. Completion: each routine has a real acceptance result
   or a bounded unresolved discrepancy; a declaration alone is not recovery.
5. **Review and check dependencies.** Follow [REVIEW.md](REVIEW.md). Re-run affected
   C contracts after a shared-layout change; follow harness revalidation rules
   when build/acceptance inputs change. Do not claim transitive recovery when
   accepted routines still call original code. Completion: per-function evidence
   and an explicit residual list for code, data, CRT and API boundaries.
6. **Advance only the demonstrated gate.** For a local slice, report accepted
   routines and remaining dependencies. For live replacement, first satisfy the
   M2/M4 evidence and reproduce the integration. M5 closure requires both local
   checks and whole-game replay checks. Completion: state exactly which gate the
   evidence supports; leave the others open and name the smallest next experiment.

## Packet fields

| Field | Required content |
| --- | --- |
| Target | Executable path and full SHA-256; address convention and preferred/actual module base |
| Question | Observable behavior and scope; milestone gate being investigated |
| Functions | Exact IDs/body ranges, inventory source, uncertain boundaries, status obtained from cmre |
| Contracts | Arguments/registers, return/cleanup ABI, read/write state, numeric and RNG order |
| Layouts | Offset/width/stride observations, source instruction sites, proven meanings and unknowns |
| Dependencies | Direct/indirect callees, callbacks, globals, original data, CRT and imported APIs; recovery state separately |
| Evidence | Raw local paths and hashes, REA Evidence IDs/profile/version where used, controlled runtime setup where used |
| Validation | Exact commands, corpora/seeds, exit-code receipts, exclusions and comparison limits |
| Result | Observed / inferred / unknown conclusions, unresolved divergence and next falsifiable experiment |

## Ownership when delegation is authorized

The parent supplies the packet, function IDs, allowed files and expected result.
Each worker owns one C source or one hint file; proposed shared-type changes
return to the parent. Do not edit harness inputs during a running acceptance job.
The parent schedules acceptance writers without overlap with machine jobs and
owns shared declarations and integration. Workers inherit completed preflight.
The reviewer receives the claimed gate and evidence, not merely an acceptance
summary. Preserve the best bounded attempt and its mismatch before handing off.
