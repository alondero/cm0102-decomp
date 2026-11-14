# Decompilation strategy and tool decisions

Reviewed 2026-10-14. Target identity and acceptance gates are authoritative in
[ROADMAP.md](ROADMAP.md); recovery policy is [ADR 0001](adr/0001-automated-acceptance-and-two-tier-recovery.md).
This review changes task structure and tool routing, not the acceptance harness.

## Deliverables in order

1. Recover original instructions as relocated, compiled, differentially validated
   routines. Machine lifts establish bounded function evidence.
2. Translate coherent slices into readable C with evidence for shared layouts,
   state, dependencies, and observed bugs. Validate each translation and then
   test the connected subsystem in a controlled original runtime.
3. Build a standalone faithful game from source, accounting for all executable
   logic and data dependencies. Close M6 through clean builds and whole-game
   scenarios, rather than extrapolating from function counts.
4. Freeze that reference and develop the native modern port under M7. Platform
   adapters, host architecture, presentation, and deliberate behavior changes
   each have their own comparison scope.

The current `cmre` inventory covers local recovery, not subsystem closure. Use
live `py -3.13 -m cmre status`; keep lifted assembly, readable C, legacy recovery,
and live integration distinct. An 80% lift target does not mean 80% readable C,
or that the game can run without its original executable. The unresolved runtime
replay work remains necessary even while static recovery advances.

## What decomp.wtf contributes

[decomp.wtf](https://decomp.wtf/) is a local matching-decompilation workbench.
The reviewed upstream is version 0.9.6, commit
`2b0321db4a505a04c4e9aa47ed914758dd7aaf6c` (2026-10-14). Its
[platform matrix](https://github.com/i12bp8/decomp.wtf/tree/2b0321db4a505a04c4e9aa47ed914758dd7aaf6c#platforms)
lists from-scratch PC support as forthcoming. Its existing-project route needs
a compatible object/build-report pipeline that this repository has not supplied.

Adopt these practices through the existing tools:

| Practice | Project implementation |
| --- | --- |
| Claim bounded work, inspect context, attempt, compare, hand off | Exact function IDs, owned files, original-byte evidence, `cmre` results, and the smallest next experiment |
| Recover shared types from multiple access sites | Offset/width/stride evidence, explicit unknown fields, packing assertions, and affected-function checks |
| Keep work and its evidence connected | A concise subsystem packet with raw local artifact paths and durable conclusions |
| Review the whole build after shared changes | Revalidate affected contracts under the tooling workflow; subsystem/runtime gates remain separate |

These adaptations follow the upstream
[task loop](https://github.com/i12bp8/decomp.wtf/blob/2b0321db4a505a04c4e9aa47ed914758dd7aaf6c/src/decompnet/app/guide/workflow.md),
[x86 guide](https://github.com/i12bp8/decomp.wtf/blob/2b0321db4a505a04c4e9aa47ed914758dd7aaf6c/src/decompnet/app/guide/x86.md),
and [structure guide](https://github.com/i12bp8/decomp.wtf/blob/2b0321db4a505a04c4e9aa47ed914758dd7aaf6c/src/decompnet/app/guide/structs.md).
Console ABI/alignment rules do not transfer to this x86 PE without instruction
evidence. Ghidra's `windows` compiler specification models an ABI; it does not
identify the original compiler version or optimization flags.

Keep matching scores separate from behavioral acceptance. Scratch matching
links at original addresses, while ADR 0001 deliberately tests relocated code.
The upstream [fork workflow](https://github.com/i12bp8/decomp.wtf/blob/2b0321db4a505a04c4e9aa47ed914758dd7aaf6c/src/decompnet/app/guide/fork.md)
also excludes inline assembly, which this project may need for faithful behavior.
An exact or normalized instruction match can help diagnose code generation;
it does not exercise RNG sequencing, state changes, API behavior, or gameplay.

## REA's role

[REA](https://github.com/morluto/rea) is the interactive evidence layer:
function dossiers, disassembly, pseudocode, references, and explicit unknowns.
Use the existing inventories for routine lookup and REA for a specific unresolved
question. Follow [REA.md](agent/REA.md) for target copies, installed schemas,
Evidence retention, snapshots, and connection recovery. REA's own verification
or hypothesis labels do not replace `cmre accept` or a roadmap gate.

The working stack here is REA 6.4.0, Ghidra 12.1.4, JDK 21.0.10 and Python 3.13.
The current upstream release is 6.5.0, but version freshness alone is not a reason
to invalidate this working setup or upgrade another workspace's global tool.
Review a needed fix/capability against the
[6.5.0 changelog](https://github.com/morluto/rea/blob/rea-agents-6.5.0/CHANGELOG.md)
before a scoped upgrade. Record the version and analysis-profile change, verify
the provider, and repeat a bounded native query. Old Evidence retains its
original provenance; snapshot compatibility must be checked, not assumed.

## Throughput review (2026-10-21)

Measured on the pinned binary. Accepted C/legacy covered 10.0% of inventory
bytes; lift-inclusive coverage was 48.6% and had been flat since 2026-10-14.
At the measured ~7.5 KB/hour, the remaining C bytes would take about 520 hours.

| Bottleneck | Evidence | Status |
| --- | --- | --- |
| Harness inputs that no original call receives | AE2000 error path reached from ~1,260 queued functions (~65% of C-queue bytes); 6/6 AE2000 and 5/5 DF WIP sources pass once corrected | Fixed for new C: [ADR 0004](adr/0004-versioned-c-input-policy.md) |
| Drafts rarely compile | 22/200 sampled drafts compiled; 1/48 passed the gate | `cmre draft` repairs type-level errors and forwarded receivers: 64/200 compile. Remaining failures are mostly dropped or reordered call arguments |
| Half the binary is unlifted, so C acceptance (`needs-lifted`) cannot start | 1,240 functions / 2.16 MB below 0.6 coverage. The original-only explorer already ran on almost every one. A 40x exploration budget changed 1 of 9 sampled functions | Open: needs realistic inputs, such as runtime-captured state for the M2 consumed-input replay, or branch solving through callee returns |

Next levers, in order: re-test and review held v2-passable WIP sources; select
cohorts from the error-path majority now open to plain C; then make game state
available as fixtures for the large unlifted functions. Concurrency and model
tier remain measured through [the farm status](agent/FARM.md) and do not
substitute for these.

Concurrent sessions from any agent client follow [PARALLEL.md](agent/PARALLEL.md).
There is one integrator, which accepts, and any number of translators, which
select with `cmre next --c --claim NAME`. Reviewers scale with the translators.
Acceptance is serial and fast; review is the scarce step. Throughput scales only
while review keeps pace, so add reviewers before translators.

## Installation decisions

| Component | Decision and trigger |
| --- | --- |
| Existing REA/Ghidra/JDK | Reuse the verified stack and project launcher. Repair only the actual client/provider gap; use no blanket setup. |
| Project agent skills | Install thin Claude and Codex adapters pointing to the same workflows; keep policy in `docs/agent/`. |
| decomp.wtf application/tool bundle | Defer: current from-scratch PC route is incomplete and fork mode lacks a compatible target/build pipeline here. Reassess on demonstrated PC support or an explicitly bounded fork experiment. |
| [objdiff](https://github.com/encounter/objdiff) | Defer until legitimate original relocatable objects and a compatible report producer exist. Linked PE code does not establish original COFF relocations. |
| [reccmp](https://github.com/isledecomp/reccmp) | Candidate for a later PE/PDB comparison experiment after compiler provenance and a suitable rebuilt Windows executable exist; it is not an acceptance gate. |
| [m2c](https://github.com/matt-kempster/m2c#target-architecture--compiler--language) | No x86 target in the reviewed support list, so it does not serve this executable. |
| Modern platform libraries | Choose after recovered API contracts and target requirements justify them; no renderer, language rewrite, or 64-bit ABI is selected by this review. |

Store acquired tools under ignored `tools/`, with source URL, version, checksum,
settings and smoke-test receipts under ignored analysis storage. Keep Python
commands on `py -3.13`. Tool installers must preserve the project's pinned
engines and other workspaces' configurations.

## Agent infrastructure

`AGENTS.md` is the common contract; `CLAUDE.md` adds runtime-specific wiring.
[PLAYBOOK.md](agent/PLAYBOOK.md) routes tasks to one procedure. Skills carry
triggers and pointers, not another set of gates. New routes are
[subsystem recovery](agent/SUBSYSTEM.md), [evidence review](agent/REVIEW.md), and
[native port work](agent/PORT.md). The current handoff selects concrete work;
historical notes and generated status do not override live commands.

When delegation is authorized, the parent owns shared layout decisions, jobs,
acceptance scheduling, runtime experiments, and the final gate. Workers receive
nonoverlapping scopes and return evidence. A reviewer checks claims separately
from the worker's success message; it cannot promote a recovery state.
