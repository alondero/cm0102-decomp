# Compiler stack-probe candidate

> Historical candidate evidence. [Maintained acceptance](STACK-PROBE.md) supersedes candidate status; frozen reports/inputs remain unchanged.

Target: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The hash-identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
Addresses are preferred-image VA, image base `0x00400000`.

The compiler helper at VA `0x00945610` occupies 47 bytes and 17 instructions
through `0x0094563e`. Its bytes hash to
`4be32df90e8e3db3f5eed0d31857bddd88255c7ed09ae57e0e20e4510124a255`.
This is a register ABI: EAX supplies an unsigned allocation size. It is not a
normal cdecl function. For incoming ESP S, its cursor starts at S+4. It reads a
DWORD after each 4096-byte decrement, then reads the exact remaining tail,
including a zero tail. It relocates the original return address beneath the
allocation. On normal return ESP is S+4-allocation, EAX is the original return
address, ECX and the other general registers are preserved, and flags retain
the final TEST result. The final return-address push needs four bytes beneath
the cursor even when the tail read succeeds.

`CM0102/Analysis/stack-probe-native-candidate-a/cm_stack_probe.S` is an isolated
native assembly candidate. The compiled body also occupies 47 bytes/17
instructions, with equivalent alternate register opcode encodings. Its hash is
`21b0d1ae0fe61dfd80264ca22da64cab82b87e0967d52f1270ea4a35f2f59446`.
The generation and audit pin the original and compiled instruction sequences,
build tools and inputs. It is **not accepted into a maintained ledger or adopted
by morphology**; the full morphology builds still call the original helper.

`stack-probe-candidate-test-b/report.json` records 3,020 successful comparisons:
2,442 mapped-stack cases, 512 deterministic random mapped-stack cases, 30 first
unmapped-read cases and 36 first unmapped return-address-write cases. The driver
compares registers, flags, ordered memory reads and writes, touched stack words,
and the first CPU fault. The candidate machine maps only its compiled helper;
it does not execute original instructions. The first driver run failed a fixture
expectation: readable tail bytes did not guarantee space for the return-address
push. That diagnostic is preserved in `stack-probe-candidate-test-a`; driver B
corrected the case classification without changing the assembly candidate.

`stack-probe-candidate-audit-a/audit.json` verifies the byte provenance, 17
normalized instruction semantics, report/tool/artifact hashes and three frozen
inputs in `stack-probe-candidate-source-used`. These are finite Unicorn x86
comparisons. Windows guard-page growth, SEH dispatch, arbitrary address wrap and
live application integration remain unvalidated.

Next: review and accept this dependency under its explicit scope, relink the
morphology entry to the recovered helper, and validate the resulting build.
CRT recovery and data relocation are still required for a standalone module;
this candidate alone does not satisfy the standalone-game gates.
