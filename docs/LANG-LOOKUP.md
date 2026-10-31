# Translation lookup and search recovery

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Its identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All executable addresses here are **preferred-image VAs**, image base
`0x00400000`; database positions are **file offsets**.

`recovery/lang/cm_lookup.c` recovers both normal-return translation-lookup
paths and their key comparator. It also provides instruction-derived CRT search
support, accounted separately from application recovery. Statuses, byte identities,
dependencies and accepted report hashes are in `recovery/lang/lookup-ledger.json`.
Existing date/formatter source and acceptance ledgers are unchanged.

| VA extent, inclusive | Contract | Original bytes | Accounting |
| --- | --- | ---: | --- |
| `0x0066FBB0..0x006701EE` | Translation lookup into a 52-byte record | 1,599 | Application |
| `0x00682060..0x0068209E` | Unsigned-byte key comparison | 63 | Application |
| `0x00946464..0x009464EF` | Bsearch-style callback search | 140 | CRT support |

All three compile at O0/O2 and pass their finite differential contracts;
all remain `integrated: false`. This adds **two application routines, 1,662
original bytes**, and one separate support routine. The date/localization
application selection now has 26 compiled routines. The existing wrapper is
tested in a longer recovered call chain but is not counted again. These counts
are selected-slice inventory, not whole-game coverage or a standalone build.

Later extension: [DATE-NUMERIC.md](DATE-NUMERIC.md) adds two numeric date
formatters while preserving their prior-frame month-byte behavior. Its separate
ledger raises the selected application count to 28 at that stage.
[DATE-TEXT.md](DATE-TEXT.md) subsequently compiles the other seven mapped date
formatters, bringing the selection to 35 application routines plus separate CRT
search support. [LANG-LIFECYCLE.md](LANG-LIFECYCLE.md) then adds five language
setup/ownership routines, bringing the selected application count to 40.
[LANG-LOADER.md](LANG-LOADER.md) adds loader and marker, bringing the current
application selection to 42 with the same separate CRT search support.
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) adds both table helpers, bringing the
current application selection to 44.
Independent ledgers record acceptance; the lookup results and earlier next-work
observations below remain unchanged historical evidence.

The comparator is absent from the accepted Ghidra function inventory. Its
boundary is established manually: callback push at `0x0066FCBF` selects
`0x00682060`, whose straight-line instructions end in RET at `0x0068209E`.
The ledger keeps its Ghidra count null. Search support is recognized by its
instruction contract and CRT-region placement; no original source symbol is claimed.

## Record and in-memory path

The record contains three owned string pointers at offsets 0/4/8, then ten
dwords at offsets 12..48 describing argument order. The header asserts its
original x86 size. Lookup is cdecl, receiving record, key string and optional
index pointer; full EAX is 0/1.

Query normalization removes literal spaces and braces, converts `\n` to LF
and `\"` to quote, and preserves square brackets and other bytes. Instructions
`0x0066FBEC..0x0066FC3D` establish these rules. When the resulting string is empty,
the original trim reads one private predecessor byte but its negative index
prevents a write. The C guards that dead read; private-stack effects are excluded.

Only mode dword `0x00B647B0 == 1` chooses memory search. Lookup allocates its
normalized query key, reads count `0x00B647B4` before table `0x00B679F0`, and
searches records at stride 52 (`0x0066FCD6..0x0066FCE5`). The comparator dereferences
record keys and returns full EAX -1/0/+1 with unsigned-byte ordering.
Search chooses the lower middle entry for even lengths; duplicate equal keys
therefore select index `(count - 1) / 2` when the entire table has that key.
Unsigned x86 address/count arithmetic remains explicit.

On a hit, key, format and text are replaced in that order. Each replacement
captures the source, frees the prior output pointer, allocates strlen+1,
publishes the new pointer, rescans the source length, then copies dwords before
the remaining bytes. This preserves visible alias/free ordering and allocation
callback mutation. Ten order dwords are copied sequentially. Optional index is
written only here, using the table pointer read again at that moment. The temporary
query key is freed on hit and miss. Misses leave output and index unchanged.
Whole-record aliases are tested, including hooks which poison freed storage;
this checks the original access sequence, not a guarantee about a real allocator.

## File path and preserved quirks

Every other mode opens the filename at `0x00B6586C` in `rb` mode. Open failure
uses the embedded Langlib.cpp path, diagnostic source line 3494, recovered version
getter, sprintf and error UI. After UI it clears `0x00B67A34` and writes the
original `Unable to open database file` bytes to `0x00B65004`; return is zero.
Existing output-owned strings and optional index are untouched on this path.

Successful open seeks to `12 + 4 * language_slots`, using `0x00B6616C`.
A signed nonpositive row count closes without reading. Otherwise each row reads
2,048 key bytes into shared scratch `0x00B6619C`, replaces the output key even
on a miss, and normalizes a private candidate. Candidate normalization removes
spaces and converts `\n`, but preserves braces and escaped quotes. It is
deliberately different from query normalization.

On a key match, lookup reads the common 30-byte format. It skips translation
blocks starting with ordinal 1 until reaching the signed selected ordinal at
`0x00B65868`, then reads/clones 2,048 text bytes and reads ten order dwords
individually into the record. Later translation blocks are consumed and discarded.
Misses seek relatively by `2088 * language_slots - 2058`, modulo 32 bits.
Language selection, slot count and row count are reread at their original
control-flow boundaries; controlled I/O callbacks test changes during reads.
The optional output index is never written by the file path.

Read, seek and close return values are ignored. Short reads retain prior scratch
or output bytes. Even a partial file can yield a successful lookup when enough
key bytes were received; the harness does not zero-fill or invent terminators.
Invalid selected ordinals are not clamped. `0x80000000` skips the leading-language
loop, then starts the trailing loop at signed -2,147,483,647. Validation compares
the first twelve I/O calls on this extremely long path, not completed return,
stack exhaustion or billion-iteration equivalence. The source retains the loop.

## Supplied database evidence

Read-only `CM0102/Game/Data/language.ldb`, SHA-256
`7f6fcec87709fc0072e05a44fa7f781978f2abd247630e1d977f74ecac449b59`, is
204,672,142 bytes. Its three little-endian header dwords are `(1, 9807, 10)`;
the next ten dwords are IDs `(0, 2, 3, 4, 5, 6, 7, 8, 9, 10)`.
Header size is 52. Each row contains:

| Relative file offset within row | Bytes | Field |
| ---: | ---: | --- |
| 0 | 2,048 | Key |
| 2,048 | 30 | Common format |
| `2,078 + 2,088 * (ordinal - 1)` | 2,048 | Translation text, ordinals 1..9 |
| Text offset + 2,048 | 40 | Ten little-endian order dwords |

Row stride is 20,870; `52 + 9807 * 20870` exactly equals the file size.
Rows 0, 1, 2, 3, 17, 101 and 9,806 are captured and individually hashed in
each local run. Their complete raw row bytes, including padding, are exercised
at selected ordinals 1/2/5/9 in reduced one-row files. Only the header row count
is changed; language-ID bytes and each captured row are preserved. These are
local test fixtures, not a full runtime database load. Slot ordinal, header ID
and current-language dword `0x009EB2DC` are distinct observations; their mapping
must be recovered from the loader rather than assumed. Runtime root-level `.lng`
files have a different header and are not identified as this format.

## Validation and reproduction

Accepted reports `CM0102/Analysis/lang-lookup-final-a-O2/report.json` and
`lang-lookup-final-a-O0/report.json` each pass **69,491 comparisons**, including
one bounded long-loop observation. Builds use freestanding 32-bit GCC with
warnings as errors. Reports pin source/tool/input/output hashes and commands;
535 exported lookup instructions are checked against primary executable bytes.
Exact source snapshots are in ignored `CM0102/Analysis/lang-lookup-source-used/`.

The corpus tests key normalization, string copy lengths through 2,047 bytes,
table hits/misses, duplicate selection, ownership/whole-record aliases,
allocation-time source mutation, file selection and scanning, open failures,
signed counts, ignored short reads/seeks/closes, callback changes to globals,
captured database rows and seeded random records. Comparator checks include all
65,536 first-byte pairs and long equal/differing prefixes; generic search tests
lengths 0..32 at strides 4/52/100. Independent expected results accompany parity
for search, comparison, index preservation and duplicate selection.

Original and recovered machines execute their own lookup, comparator and search
instructions. The recovered side executes no other original text except named
boundary hooks. Returning cases compare EAX, stack cleanup, callee-saved registers,
restored FS:[0], caller storage, watched shared state, table and heap contents,
ordered allocation/free/I/O inputs and state at those boundaries. Unexpected
application writes fail. Allocation, real CRT I/O, diagnostics, morphology and
the existing formatter's CRT remain explicit modeled boundaries. Transient SEH,
stack probing, exceptions, allocation failure, concurrency, invalid pointers,
unbounded strings, signed-word file-key counter wrap and arbitrary partial
overlaps remain excluded. These limitations are independent of normal-return parity.

The old formatter source still contains its fixed original lookup address.
For the new chain tests, the emulator redirects that entry to recovered lookup
on the recovered side only; the original side executes real lookup. This is a
test linkage mechanism, not a patched process or M4 integration.

```powershell
& py -3.13 `
  recovery/lang/test_cm_lookup.py --output CM0102/Analysis/lang-lookup-NEW-O2
& py -3.13 `
  recovery/lang/test_cm_lookup.py --output CM0102/Analysis/lang-lookup-NEW-O0 --optimization O0
& py -3.13 `
  scripts/map_date_family.py --output CM0102/Analysis/date-lookup-map-NEW
```

The combined map merges three ledgers, rejects duplicate addresses and checks
their original byte hashes. It covers 28 family entries and eight support entries
(including separately labeled CRT search), with 15,980 checked call records.
Missing Ghidra comparator inventory remains visible alongside manual boundary
evidence. Nine larger date-format routines remain uncompiled candidates.

Final map runs `CM0102/Analysis/date-lookup-map-a` and `date-lookup-map-b` have
byte-identical function, call, table and format-evidence exports. Lookup report
artifact/source hashes and all previous date/formatter accepted report hashes
and final source identities were rechecked.

## Review and preservation

Independent Standards and Spec reviews found no unresolved hard violations or
in-contract fidelity defects. Both caught the same README overstatement about
captured rows passing through the formatter; it was corrected to distinguish
direct captured-row tests from synthetic formatter-chain tests. Spec review also
checked 1,192 exported instruction-byte records and nine independent lookup cases
against the accepted O2 binary. Optional repeated map byte-hash checks were retained
to preserve the existing selected-entry guard alongside whole-ledger validation.

Both C builds passed with warnings as errors; changed Python files passed syntax
checks and all five existing runtime-formation tests passed. Final preservation
verification passed all 1,558 baseline files and 46 official patch payloads.
Generated executables, objects, raw database captures, maps, reports and snapshots
remain under ignored local `CM0102/Analysis/`. No baseline or manifest was changed.
There is no Git repository; no commit, remote, PR or publication was created.

## Next work

Extend numeric date formatting at `0x0054A470`/`0x0054A650` while preserving their
identified uninitialized month-byte paths. Translation loading/ownership and
morphology `0x006701F0` remain unrecovered application dependencies. Continue M2
repeated runtime replay and primary/GDI mapping before M4 replacement. No
recovered code is integrated and no runnable standalone source game is claimed.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/LANG-LOOKUP.md and the date/language ledgers.
> Verify preserved baselines and reproduce lookup at O0/O2 in fresh output folders.
> Retain file/query normalization asymmetry, owned-string ordering, ignored I/O
> results, signed language loops and explicit CRT/morphology boundaries. Extend
> the mapped numeric date formatters without inventing an initialized month for
> invalid dates; preserve earlier accepted source identities and M2/M4 gates.
