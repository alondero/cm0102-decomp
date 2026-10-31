# Remaining morphology CRT boundaries

Primary target: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The hash-identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All addresses are preferred-image VA, image base `0x00400000`.

`CM0102/Analysis/morph-crt-static-a` is a fresh read-only Ghidra 12.1.4 export
of formatting `0x00944C9F`, upper `0x009471CD`, lower `0x009465FC` and substring
search `0x00945A60`. Its `run.json` pins the export, original bytes, tool/archive,
Java and script identities. These four entries remain fixtures in both the
preserved original-probe and probe-only morphology stages. The current
[search-composed stage](LANG-MORPH-SEARCH.md) executes native search except
six artificial find cases; its reference find fixture remains explicit.

## Full upper/lower candidate

The upper body `0x009471CD..0x00947298` is contiguous: 204 bytes/71 instructions.
Lower `0x009465FC..0x009466C6` is 203 bytes/78 instructions. Native C/assembly
in `crt-case-native-candidate-a` recovers all 407 bytes/149 instructions, with
explicit classification `0x0094C322` and locale-mapping `0x0094C0FE` fixtures.
It is **an isolated candidate, not adopted maintained CRT or morphology source**.

The zero-locale path compares the full input as a signed DWORD against ASCII
bounds. Nonzero locale at `0x00DE3048` has additional behavior:

- Signed input below 256 enters classification. Signed multibyte maximum
  `0x00AD91DC` above one calls classification; otherwise it reads a byte through
  the ctype table pointer at `0x00AD8FD0`, indexing with x86 DWORD wrap.
- The input cached in EBX survives classification callback mutations. A zero
  classification result returns that cached input. The table pointer is reread
  before testing bit 0x80 in the high-byte row.
- A lead byte clears incoming argument byte two, then writes its first two bytes
  in high/low order. A single byte clears argument byte one and writes its low
  byte. Other incoming high bytes are retained. These are caller-slot writes;
  an ordinary C local copy would lose observable state.
- Mapping rereads the locale handle, passes upper flag 0x200 or lower 0x100,
  the mutable argument pointer, input length one/two, output at saved ECX,
  capacity three, zero and one. A zero return preserves the cached input; one
  returns the output low byte; any other nonzero value reconstructs two bytes.
  Partial fixture output leaves original saved-ECX bytes observable.

`crt-case-candidate-focused-a-O0/O2` each pass **23,304 comparisons**: 1,536
ASCII/high-bit cases, 12,288 locale/all-byte cases, 7,200 signed/mapping boundary
cases, 1,280 callback-mutation/partial-output cases and 1,000 seeded word cases.
The driver compares EAX, cdecl ABI, incoming argument bytes, saved-ECX output,
globals/tables, ordered public global/table reads and argument writes, and
classification/mapping events. It rejects original body execution on the native
side. Volatile registers/flags, unrelated private worker-stack bytes and general
faults are outside this finite contract. The fixtures do not establish Windows
locale or classification implementation equivalence.

`crt-case-candidate-audit-a/audit.json` verifies both report/artifact/tool/source
chains, the complete original instruction partition against Ghidra and Capstone,
and five frozen inputs in `crt-case-candidate-source-used`. Independent review
also passed 144 mutation comparisons. No maintained full-routine count changes.

## Formatting and substring paths

Formatting's 82-byte wrapper initializes an original stream record with flag
0x42 and count 0x7FFFFFFF, then calls `0x00947C67` with that record, the format
pointer and incoming variadic slots. It decrements the updated count. A negative
result calls `0x00947B52` with zero and the record; otherwise it clears one byte
at the updated output pointer. The formatter result is preserved through either
termination path. Recovering just this wrapper would leave the general format
parser and output support explicit; it would not replace the formatting fixture.

Substring search's Ghidra body is **not one 133-byte contiguous extent**. It has
128 bytes at `0x00945A60..0x00945ADF` plus a five-byte exit block at
`0x00946530..0x00946534`. Empty needle returns the haystack without reading it.
A one-byte needle restores registers and tail-transfers to internal entry
`0x00946546`, with AL holding the character. That helper uses byte alignment and
DWORD zero/character detection, including reads beyond a terminating byte.
A replacement based solely on host string search would lose that access and
fault behavior. The subsequent [native search candidate](CRT-SEARCH-DISCOVERY.md)
maps the complete 321-byte closure, including the internal entry and shared
exit, and passes 44,423 comparisons. [Maintained search acceptance](CRT-SEARCH.md)
now separately verifies that source; the exported pseudocode alone is not a
completeness proof. Morphology now separately adopts compiled search under
[full composition acceptance](LANG-MORPH-SEARCH.md); reference find and six
artificial cases remain explicit.

Next adopt and validate the case-conversion bodies under their named dependency
scope, compose maintained classification and recover deeper classification/
mapping support and the format parser. Compiled string search is already composed. Keep original language/data pointers and CRT fixtures explicit
until the recovered implementations and data closure can run independently.

[Maintained classification](CRT-CLASSIFY.md) now separately accepts the complete
117-byte body at `0x0094C322` after 11,788 comparisons per O0/O2, with
`0x0094ED85` still an explicit fixture. The prior isolated candidate remains
historical; no case-conversion or [morphology adoption](LANG-MORPH-RELINKED.md)
is implied. [Search composition](LANG-MORPH-SEARCH.md) is accepted after full O0/O2
validation, with classification still unadopted.
