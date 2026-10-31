> **Historical isolated candidate.** Byte-identical maintained C/frame sources
> are now [accepted separately](CRT-CLASSIFY.md) at 11,788 comparisons per O0/O2,
> six frozen inputs and one deeper fixture. Case/morphology adoption remains
> pending; the isolated records below retain their original unaccepted status.

# Isolated CRT classification candidate

Input: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
All addresses are preferred-image VAs, base `0x00400000`. The contiguous body
`0x0094C322..0x0094C396` is **117 bytes / 46 instructions**, checked against
the primary bytes and hash-locked `morph-crt-dependencies-a` disassembly export.

The native C/frame adapter in `crt-classify-native-candidate-a` is an isolated
candidate, not an accepted CRT ledger or additional application entry. Full O0
and O2 each pass **11,788** comparisons: 9,252 unsigned fast-path cases,
1,536 callback/partial-output/mutation cases and 1,000 deterministic random
words. Five current/frozen inputs and build/tool/artifact hashes are recorded
by `crt-classify-candidate-audit-a` and `crt-classify-candidate-source-used`.

The actual interface takes two stack DWORDs: character value and mask. Incoming
ECX initializes a four-byte local. With incoming ESP `S`, original EBP is `S-4`,
the local is `S-8`, value is `S+4`, mask is `S+8`. A wrapper recreates those
original locations while a C worker receives EBP explicitly.

The unsigned `(value+1)<=256` branch includes `0xFFFFFFFF` and bytes 0..255.
It loads the DWORD table pointer at `0x00AD8FD0`, then a **WORD** at wrapped
`table+2*value`, and ANDs it with the caller mask. It writes no argument output.
Ghidra's high-word assignment in this fast branch is register-variable notation,
not a write to caller memory. The original instructions establish that distinction.

Other values inspect bit `0x80` in `table+2*((value>>8)&255)+1`. A lead byte
clears local byte two and writes high then low bytes; otherwise it clears local
byte one and writes the low byte. Untouched bytes retain incoming ECX. The
`AND byte,0` instructions are read/write operations, not zero initialization
of the full local. The signed SAR followed by MOVZX CL has the same selected
byte as the C unsigned shift and mask.

The remaining original helper `0x0094ED85` is called with seven stack arguments:
`(1, local, length, &caller_value_high_word, 0, 0, 1)`. Zero result returns zero;
any nonzero result reads that caller high WORD and reloads the caller mask before
ANDing. Partial or absent writes retain preexisting high argument bytes. Tests
also mutate the mask, temporary bytes and table pointer at this explicit fixture.
No platform classification algorithm has been invented.

Compared scope: EAX, cdecl return/callee-saved ABI, complete caller value/mask
slots, retained local bytes, table state and ordered reads, and dependency events.
The independent fast-path oracle checks the table WORD/mask result. Caller-saved
registers/flags, private C-worker stack bytes, arbitrary faults, concurrency and
Windows locale behavior remain outside this finite normal-return contract.

Next review/adopt the candidate under an explicit ledger, link it into the two
case-conversion candidates and separately validate that composition. Recover
`0x0094ED85` and mapping `0x0094C0FE` before claiming a dependency-free locale
chain. [Morphology's accepted probe relink](LANG-MORPH-RELINKED.md) continues to
use case-conversion fixtures; this isolated candidate does not change that build.
