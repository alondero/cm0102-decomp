# Proposal: live x87 state at the original `__ftol` boundary

Status: **unsupported; diagnostic prototype only** (2026-11-18).
Not a numbered/accepted ADR, registered adapter, harness release, acceptance
permission, or standalone implementation.
An independent diagnostic review now exists at the parent-local path
`CM0102/Analysis/cmre/session-20261118-validation-handoff/x87-prototype-review.md`.
Owner review and review/adoption as an accepted ADR remain pending. This note
records an unsupported investigation only: zero supply or recovery release.

The intended helper is preferred-image VA `0x00945640`, image base `0x00400000`,
CM0102 3.9.68 SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
This lane could not access the original executable; helper facts below are
requirements derived from ADR 0006, **not new original-instruction proof**.

## Decision for this lane

Keep `x87-live-stack` and related `__ftol` boundaries unsupported. Do not add a
recovery header, register a helper, broaden `x87_transport`, retry a function,
or claim any supply pool was released.
Preserve the experimental tests, including their two failing requirements.

ADR 0006's cast and ADR 0010's individual `FLD m80` operands do not establish
the actual deeper register stack or its status. Reconstructing a plausible
stack from C variables, resetting it, choosing a compiler frame, or changing
comparison to discard discrepancies is not an alternative proof.

## Proposed seam and provenance

The experiment examines a possible **opaque CPU-produced state packet**, not
an array of values constructed by translated C:

```c
/* Illustrative contract ONLY; not declared by any recovery header. */
unsigned long long ftol_packet(void *inout_fnsave108);
```

The 32-bit CPU must have produced the 108-byte legacy x87 save image by
executing `FNSAVE` at a justified boundary. Ownership must
tie that state to the caller's actual original instruction stream. The packet
contains raw register slots in logical ST(0)..ST(7) order, together with the
physical-register tag word and TOP needed to interpret their relationship,
CW, SW and saved pointers, rather than values guessed from the original result.
Neither this legacy image nor the sampled VM fields establish complete hardware
or exception state. The packet is not an `emu.Case` fixture or a replacement for
the missing producer in translated C.

The test-only assembly saves the compiler's ambient x87 state in its own
ordinary stack allocation, restores the packet, calls the helper address once,
saves its resulting state to the packet, attempts to restore ambient x87 state,
and returns EDX:EAX on the exercised normal-return path. This describes the
synthetic VM experiment, not a complete restoration guarantee; the failing FIP
requirement and unsupported fault/exception paths preclude that claim.
It performs no floating-point arithmetic or integer conversion.
Stack allocation uses `LEA`, not arithmetic that replaces helper EFLAGS.
There is no host FP-register write/normalization, original ESP repair, or
original frame forcing.

**Crucial missing producer:** ordinary C cannot extract its intended original
live register stack, tags, pending exceptions and instruction history from
`long double` locals. No producer is implemented or assumed. The synthetic
producer is test scaffolding only. The approved EIP-only `x87_transport`
executes selected original instructions without synthesizing numeric state;
it does not currently supply this packet/call protocol. Its exact opcode,
complete-ownership and overlap gates must not be bypassed to create one.

## Obligations before any registered adapter

| Boundary | Required proof |
| --- | --- |
| Operand provenance | Actual ST0 and every deeper live value from the justified CPU stream, with no narrowing. Bind each site to pinned original instructions and prove operand sensitivity through observed outputs. |
| Depth and precision | Actual tags/TOP, depth including a full eight-register stack, raw m80 bits, CW precision/rounding and status survive transport; no load-induced overflow or invented empty slots. Sparse tags, denormals, unsupported encodings and empty ST0 are not covered by this prototype. |
| Original helper | Execute the unchanged pinned body. Retain read-only listing/body identity, instruction/call order and a proof of the helper's GPR/memory readset. The scaffold is not that proof. |
| Returns/registers | Deliver full EDX:EAX without narrowing; preserve callee-saved GPRs and ambient state and do not accidentally replace helper flag residue. The prototype uses EBX as packet pointer during the call: original instruction evidence must prove that this does not alter helper behavior. |
| Post-call state | Continue from the actual helper's resulting x87 state, including deeper values, CW, SW, tags and observable pointer fields, not a guessed pop or a clean state. Merely restoring the compiler's ambient stack is insufficient; the packet consumer must be proven too. |
| Stack observation | Account for actual helper local-memory addresses, scratch residue, CALL return word, saved EBP, aliasing and faults. Existing semantic omission of stack memory is not evidence of unobservability. No frame-size luck, forced original ESP or pointer comparison exemption. |
| Exceptions/faults | Establish waiting/nonwaiting instruction order, pending unmasked exceptions, invalid conversion, overflow/underflow and memory faults through the complete transport. Do not turn an exception/fault into a return or claim native exception delivery from VM results. |
| Composition/lifecycle | Establish ownership of call, return and continuations alongside active return/jump/x87 transports, interruption/reset, budget exhaustion, import tracing and original call order. No acceptance escape whitelist without an owned protocol. |
| Compiler/build | Reviewed registered assembly adapter analogous to ADR 0010; no inline assembly in translated C, compiler-frame constraints or replacement math. Test normal C callers at both required optimizations. |

## Synthetic experiment and counterexamples

Locally retained files, not shipped or approved for publication:

- `tests/fixtures/ftol_packet_prototype.S`: unregistered test-only assembly.
- `tests/test_cmre_ftol_packet_prototype.py`: isolated Unicorn VM; no game reads.

The helper address is populated **only in the test VM** with an authored
scaffold for ADR 0006's temporary-chopping-CW/FISTP mechanism. It is not the
original body, and its behavior is never substituted in game validation.

Normal-return characterizations check actual m80 ST0/deeper values (including
a low bit lost by binary64), depth 1/2/4/8, multiple CW precision/rounding
inputs, actual VM sticky status, full 64-bit results, negative/zero/NaN/infinity/
overflow inputs, preserved GPRs and exactly one helper call. A read-only output
packet faults after helper entry without recovering ambient state; this is
explicitly unsupported, not a successful fault-equivalence test.

The experiments expose two unresolved, unnormalized requirements:

1. **Ambient FIP round trip:** `FNSAVE; FRSTOR` changes the current VM's sampled
   FIP from the actual `FLD1` PC to zero. The saved packet contains that actual
   PC; the sampled FIP after restore is zero. Other sampled fields match in this
   minimal case. This is an observed VM limitation; no native CPU conclusion
   or package/toolchain change is proposed here.
2. **Helper FDP/stack observation:** with otherwise matching synthetic inputs,
   the adapter's ordinary save area and extra CALL move the helper frame by
   116 bytes. Actual post-helper FDP and the saved packet's FDP differ from a
   direct call. The numeric-characterization tests explicitly assert this
   difference; two separate full-contract tests remain ordinary failing tests,
   not skipped/xfail tests. No production comparator was changed.

An initial red test also exposed adapter stack arithmetic changing EFLAGS
from 2 to 6; replacing `SUB/ADD ESP` with `LEA` fixes that narrow defect without
claiming the two state/observation requirements are resolved.

## Evidence availability and next decision

Fresh isolated tree: `.worktrees/x87-ftol-sol-20261118-1520`, based on
`7e37c2e97cfd91e0730b826f087da15c40cf9bdc`.
The author reports that the older `x87-ftol-go-20261117` tree has unfamiliar
uncommitted header/tests and a proposed ADR, and was not overwritten. The author
also reports external-directory access denial preventing inspection of those
contents or reading main's executable, with no workaround, binary copy, junction,
installation, process attachment or desktop action used. This access/preservation
history is author-reported, not independently certified by the diagnostic review.

Local retained logs are under
`CM0102/Analysis/cmre/ftol-packet-prototype-20261118/`.
The final synthetic run retains built test artifacts, observed packets/state
and identities under its explicitly named pytest base directory. These are
synthetic ABI evidence, not game differential receipts or original-instruction
proof. The prototype, assembly, tests and artifacts remain local and unchanged;
none is shipped with this investigation note. No broker/full-pytest/inventory/
composition/live-runtime gate was run. Normal C callers at O0/O2 remain future
work, not coverage established by the existing ADR 0010 tests.

The retained final log/XML confirm **40 passed, 2 failed** in 22.32 seconds,
as confirmed by the independent diagnostic review. **Exit 1 is author-reported,
not established by a genuine process-exit receipt.** The prototype accounts for
27 passes and both failures; the existing ADR 0010 build tests and authored x87
initial-state tests account for 13 passes. The two failures are:

- `test_current_vm_roundtrip_retains_actual_fip_required_by_contract`
- `test_helper_stack_observation_matches_direct_call_required_by_contract`

`final-v1.log`, `final-v1.xml` and `final-v1/` retain the result and artifacts.
Do not reuse that pytest `--basetemp` path: pytest clears it before running.
Choose a fresh suffix for any rerun. The focused command selected these files:

```powershell
py -3.13 -m pytest tests/test_cmre_ftol_packet_prototype.py tests/test_cmre_x87_bridge.py tests/test_cmre_x87_initial_state.py
```

Review may retain the diagnostic counterexamples or choose another transport
design; it must not merge this prototype as a working adapter. A future release
needs accessible read-only pinned inputs, the real state producer/consumer,
original helper/call-site evidence, pointer/exception/ownership resolutions,
O0/O2 synthetic and real differential tests and per-site operand mutations.
Workers/broker must use identical tooling and have their required local inputs.
Then freeze a reviewed release, inspect its dependency-aware inventory plan,
and run the required full gate for the changed ABI/execution scope, affected
compositions and parent session check at the owner's stopped-farm boundary.
No such release/integration/acceptance/publication is authorized for this lane.
