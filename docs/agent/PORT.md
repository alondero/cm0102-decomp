# Work toward a native modern port

Use for port planning or implementation. Follow [PLAYBOOK.md](PLAYBOOK.md)
preflight and closure, then the M6/M7 gates in [ROADMAP.md](../ROADMAP.md).
Native means compiled recovered game logic running on the selected host without
executing the original executable as a library. Packaging the original process
or emulator does not meet the standalone or modern-port deliverable.
Users may supply original assets and databases and run documented local
extraction. This requirement is compatible with M6/M7; executing extracted
original code as the engine is not. Setup requirements and current availability
are documented in [BUILDING.md](../BUILDING.md).

## Before M6 is demonstrated: planning

1. Read the existing runtime and recovery evidence relevant to the requested
   boundary. State which M6 obligations remain and preserve them in the plan.
   Completion: no function count or platform prototype is mistaken for M6.
2. Inventory the observed platform contracts: display/input/message loop, clock
   and RNG initialization, file/save/database layout, allocation, locale/text,
   registry/settings, audio, and startup/shutdown where relevant. Associate each
   with instruction or runtime evidence; absent evidence remains unknown.
   Completion: the selected boundary's actual requirements and unresolved facts
   are listed, including original dependencies and 32-bit assumptions.
3. Propose one bounded adapter experiment, its target host, expected unchanged
   observations and acceptance scenario. Identify candidate libraries only when
   those requirements justify them. Record the plan in a focused `docs/` note;
   support experiments stay in ignored analysis/runtime areas. Completion: a
   reviewable proposal with an explicit entry gate, not changes to game logic.

Planning does not authorize rewriting recovered logic, correcting original bugs,
or declaring native portability before standalone parity. Explicit user tasks
still take priority over the generic recovery queue.

## After M6: implementation

1. **Freeze the faithful reference.** Retain a source revision plus any local
   worktree patch, compiler/tool settings, executable and asset hashes, scenario
   inputs, outputs, and clean-build instructions. Assets stay local. Completion:
   a fresh build reproduces the agreed M6 suite with no original-code fallback.
   Commit only when requested; otherwise capture the revision/patch in local
   analysis storage.
2. **Choose one platform boundary.** State the host, recovered contract, ownership
   and intentional differences. Start with one adapter rather than changing
   platform, data model and presentation together. Completion: reference/port
   observations and success criteria are specified before implementation.
3. **Implement and compare.** Keep the faithful build available. Compare saves,
   semantic state, RNG calls and event order at stable boundaries using the
   established runtime suite. Platform-visible differences must match the stated
   scope; unexplained game-state drift is a failure. Completion: the adapter
   meets its contract and the unchanged-behavior checks pass on the selected host.
4. **Separate stored layout from host representation before 64-bit work.** Trace
   pointer-sized fields, fixed-width integers, packed records, endianness,
   serialization, x87 dependencies and aliasing. Preserve original formats
   explicitly instead of allowing host `sizeof` to change them. Completion:
   layout checks, save compatibility and numeric scenarios pass before replacing
   the 32-bit reference as the runnable port.
5. **Modernize presentation or behavior in separate changes.** Resolution,
   scaling, input, text, usability and bug fixes each state their intended
   differences and comparison scope. Engine-rule changes require explicit user
   authorization; the native-port goal alone does not authorize them.
   Completion: each difference is documented, tested and reversible against
   the retained faithful reference.

## Exit evidence

Report the completed boundary and host, source/build identities, scenario
receipts, intentional differences, remaining original/platform dependencies and
untested paths. Apply [REVIEW.md](REVIEW.md) to the claimed M7 scope. A runnable
window, successful compiler invocation, or one successful match alone does not
establish a faithful native port.
