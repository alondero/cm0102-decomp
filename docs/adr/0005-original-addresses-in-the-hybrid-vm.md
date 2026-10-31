# ADR 0005: Preserve observable original addresses in the hybrid VM

Status: implemented locally (2026-10-23); whole-inventory revalidation pending.

Original CALL return words can become ordinary data. In preferred-image VA
`0x005230c0`, an original callee's unaligned array read crosses a saved value and
the return word from CALL VA `0x005230c3`. Relocating that CALL changes a later
HeapFree argument. The target is CM0102 3.9.68, image base `0x00400000`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

For explicitly declared lifted E8 sites, retain generated assembly and ordinary
round-trip validation, but execute the checked original CALL through an owned
EIP transfer. The CPU writes the original return word once. Only an observed
32-bit near RET from the owned slot, with its matching word and post-ESP, may
resume at the compiled continuation. Original bytes and data/register values
are not repaired. This avoids inventing byte-value equivalences for data that
depends on a relocated return word.

Unicorn counts hook arrivals that redirect EIP without executing an instruction.
Machines containing this lifted CALL binding therefore count executed
instructions, excluding proved redirect arrivals. Mixed native expansions retain
their separate costs: a C PUSH and JMP each count as an instruction. A finite
native-step guard fails closed on a zero-work cycle. Existing C-only transports
keep their native counting. Reports identify the binding and budget policy.

An added recovered PE also changes the test address space. An identical absolute
input address can fault as write-unmapped in the original and write-protected
in a recovered-only read-only page. Keep both native fault details and compare
data addresses by raw identity before their established relocation relation.
The different fault classes may match only for identical raw address and size,
the profile's existing instruction relation, and a complete nonwrapping page
witness: initial and current read-only recovered PE ownership, absent from every
original and common mapping. Record the witness and whether that relation was used.

An unconditional protection-to-unmapped conversion was rejected: a genuine code
pointer can move from original read-only code to recovered read-only code, and
both faults must remain protected. Addresses that differ, incomplete witnesses,
original protection faults, changed widths and changed observable effects still
fail. Successful reads or execution of extra recovered pages remain unresolved.

These are hybrid VM execution and comparison contracts. The selected original
CALL bytes, other original callees and import scaffolding remain explicit
dependencies; these tests do not establish native integration or a standalone
game. Any global implementation change still requires full pytest and a new
complete accepted-inventory revalidation, including existing C tiers.
