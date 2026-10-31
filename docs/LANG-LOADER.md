# Language database loader and byte-order probe

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All executable addresses below are **preferred-image VAs**, base `0x00400000`.
Incoming ESP means the original function entry stack pointer.

`recovery/lang/cm_lang_loader.c`, `.h` and `.S` recover two application
routines from original instructions. Full O0/O2 runs pass **131,500 differential
cases each**; `recovery/lang/loader-ledger.json` records independent recovery
flags, byte identities and accepted report/source hashes. No recovered code is
integrated. This extended the selected inventory to 42 compiled application
routines plus the separately accounted CRT search routine. The original CRT
array/sort routines executed by this harness are not counted as recovered source.
The subsequent [table-helper extension](LANG-MORPHOLOGY.md) brings the current
application selection to 44; this loader acceptance remains unchanged.

| Entry VA | Recovered interface | Instruction/hashed bytes | ABI/result |
| --- | --- | ---: | --- |
| `0x0066EB40` | Select language and load the database | 2,422 | cdecl `(language, progress)`; full EAX 0/1 |
| `0x0090C140` | Probe a file's byte-order marker | 91 | cdecl `(file)`; full EAX marker, writes incoming argument slot |

Both extents are contiguous Ghidra bodies, totaling 2,513 bytes. C workers and
assembly entry wrappers implement these two original functions; private helpers
do not add application coverage. Static loader/CRT exports and Capstone decoding
check 1,075 instruction records against primary bytes.

Static evidence uses `lang-loader-static-b`, `lang-loader-support-static-a`
and `lang-loader-crt-static-a` under local `CM0102/Analysis/`. Their run reports
pin commands, Ghidra/Java identities and every output hash. A separate
`lang-loader-io-static-a` export establishes the CRT seek/read/tell contracts.
The loader eight-entry export is reproducible using the command in
[LANG-DEPENDENCIES.md](LANG-DEPENDENCIES.md#reproducible-evidence); fresh
support exports use:

```powershell
& py -3.13 scripts/map_recovery.py --output CM0102/Analysis/lang-loader-support-check --functions 0090c140 009462bb 009463af 00945333
& py -3.13 scripts/map_recovery.py --output CM0102/Analysis/lang-loader-crt-check --functions 00946419 00946323 00945487 009454d5
& py -3.13 scripts/map_recovery.py --output CM0102/Analysis/lang-loader-io-check --functions 00946a22 00946aae 00945bfe 00945b00 00945a09
```

## Loader state and ownership

| VA | Observed state |
| --- | --- |
| `0x009EB2DC` | Current language ID, `0xFFFFFFFF` before setup/after reset |
| `0x00B647B0` | Memory mode; only exactly one loads an in-memory table |
| `0x00B647B4` | Record count |
| `0x00B64764` | Available language ID array |
| `0x00B65868` | Selected file ordinal, initially `0xFFFFFFFF` |
| `0x00B6586C` | Database filename |
| `0x00B6616C` | File language count |
| `0x00B66170` | Available language count |
| `0x00B6619C` | Shared work buffer |
| `0x00B679F0` | Translation table pointer |

An unset current language diagnoses Langlib.cpp line 462 and returns zero.
Otherwise, a nonzero progress argument formats the original changing-language
label; zero initializes only its first byte. An existing table is destroyed
before opening the new file. The loader captures its pointer, the preceding
allocation header and its count, calls the original array destructor with
52-byte rows and the recovered row callback, frees the captured header, and
clears the global table. Callback changes do not substitute a later global
pointer for the captured header. Current language becomes `0xFFFFFFFF` before
opening the global filename with the original `rb` string.

An open failure diagnoses line 479 and returns zero without closing a file.
Invalid version diagnoses line 495, closes and returns zero. Diagnostics preserve
the original version/splitpath/sprintf/UI sequence, scan the selected error text
after UI, clear flag `0x00B67A34`, then copy the text including NUL to
`0x00B65004` in dwords followed by bytes.

Versions one and two are accepted. Header and ID reads are unchecked; the loader
does not reject short reads or substitute zero-filled data. Assembly captures
the prior version dword at incoming `ESP-0x4EC` and prior ID at `ESP-0x4E0`.
The worker begins with those bits. Partial reads preserve unwritten bytes; later
ID reads retain the previous, possibly swapped, word. Recursive partial reads
from unrelated prior private frames require additional evidence and are excluded.

Counts and IDs are swapped only when the probe returns `0xF1FFFFF1`. The initial
language count controls entry to the signed-positive ID loop; subsequent loop
bounds reread the global. Each ID is written into the current available slot
before filtering. Version two excludes IDs zero and one without incrementing the
available count or matching the request, so a later ID overwrites that slot.
Comparisons use all 32 bits. Duplicate matching IDs select their last **file
ordinal**, not their available-array position.

If the request is absent, the loader copies the original missing-language error
without showing diagnostic UI or clearing the diagnostic flag, then closes the
file. No available IDs returns zero. Otherwise it assigns the first available
ID to current language and recursively loads that ID with the same progress
argument, returning the recursive EAX unchanged.

A selected language becomes current. Memory mode other than exactly one, or
requested ID zero, closes and returns one after header/selection processing.
These paths do not load rows. In memory mode, allocation size is captured count
times 52 plus four with 32-bit wrap. A successful allocation writes that captured
count into its header and calls original array construction with the recovered
row constructor/destructor; the table is published after construction. The
constructor clears only three pointers, preserving prior order dwords. Table
allocation failure diagnoses line 565, destroys a table introduced by a UI
boundary if present, resets current language, closes and returns zero.

## Row reads, normalization and sort

Each row reads 2,048 key bytes and 30 format bytes into shared work, replaces the
owned key/format strings, removes literal braces in place, skips earlier
translations, reads the selected 2,048-byte text and ten separate order dwords,
and skips later translations. The first file language's block supplies key and
format, which have a different layout from subsequent translation blocks.
Order fields are swapped immediately after each read when required. Table
pointers and global counts are reread where the original does; private aliases
are retained where the original captures them.

String replacement frees the old pointer, measures work for allocation, publishes
the result, then measures work again for copying. The second measurement occurs
after the allocator. Original dword-then-byte copying including NUL is retained.
String allocation failures and malformed unbounded strings are outside the
accepted valid-heap contract; no new null checks or truncation are introduced.

A second key pass removes literal spaces and converts literal backslash followed
by `n` to LF. Its source index uses the signed low 16 bits of a 32-bit counter.
It does **not** unescape backslash-quote. This preserves the asymmetry with the
query normalization in [LANG-LOOKUP.md](LANG-LOOKUP.md). The harness includes
all nonzero byte values in finite valid keys.

Progress calls use thiscall ECX `0x00B74450`, method `0x00802F80`, and four stack
arguments `(label, position, count, 0xFFFFFFFF)` with RET 16. There is an initial
zero-position update, then an update after a row whose zero-based index is a
multiple of 250 or is the last row. Final original qsort uses 52-byte rows and
the recovered unsigned-byte key comparator at original VA `0x00682060`. The
loader closes the file and returns one, ignoring close results.

## Byte-order probe preserves a positive end seek

The marker routine saves ftell, seeks **positive four bytes relative to end**,
reads one four-byte item into its own incoming file-argument slot, then restores
the saved position before examining the result. Both seek results are ignored.
Read return must be exactly one. Only `0xF1FFFFF1` and `0xF2FFFFF2` are accepted;
all other results return `0xF3FFFFF3`. The cached file pointer is used after the
argument slot has been overwritten.

The positive offset is established by original PUSH 4 and inspected CRT fseek,
not inferred from marker intent. On an ordinary file a successful positive end
seek reaches beyond EOF, making the read return zero. This original behavior is
preserved. Explicit failed-seek fixtures exercise accepted native/swapped
markers, partial argument-slot writes and failed restores. No binary is patched.

## Executed support and explicit boundaries

Both emulator sides execute original normal-return support instructions:
array destructor `0x009462BB`, constructor `0x009463AF`, their normal-finally
helpers `0x00946323`/`0x00946419`, qsort `0x00945333`, shortsort `0x00945487`,
and byte swap `0x009454D5`. Original callbacks run on the original side; compiled
callbacks run on the recovered side. Their identities and stdcall cleanup are
checked. This is an explicit hybrid test, not recovered CRT source or live game
integration. Exception unwinding remains unrecovered and is not executed.

File APIs, allocation/free, diagnostic CRT/UI, label formatting and progress UI
are explicit fixture boundaries. The label formatter's source exists separately,
but this loader test hooks its entry. Setup chains execute recovered setup and
loader with emulated GetKeyboardLayout import resolution, then membership chains
exercise the resulting available IDs. These chains do not claim closure of all
localization logic. Morphology remains unrecovered; its table helpers are
subsequently recovered in [LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md), outside this
loader acceptance contract.

## Differential validation and reproduction

The harness compares full EAX, cdecl ESP, callee-saved registers, restored FS
head, incoming argument bytes, file cursor, ordered calls and boundary state.
Marker argument-slot writes are included. SHA-256 covers every byte of the
13,020-byte watched global region, the 1,280-byte date globals, and every
allocated heap page through the fixture allocation cursor, including page
padding. The separately watched current-language dword is compared directly.
The mapped 8 MiB heap beyond that allocated span is not included in these
boundary hashes. Dirty-page caching changes cost, not allocated-page coverage.

Corpus coverage includes every low-word version and requested ID; filtered,
duplicate and full-dword IDs; signed count boundaries; partial header/ID/row
reads; ignored read and close results; destruction before failed open;
fallback and failed recursive open; captured counts and boundary mutations;
real construction/destruction/sort callbacks; progress boundaries; setup and
membership chains; and captured database prefixes.

Supplied `CM0102/Game/Data/language.ldb`, SHA-256
`7f6fcec87709fc0072e05a44fa7f781978f2abd247630e1d977f74ecac449b59`,
is read only. It contains version one, 9,807 rows and ten language IDs. Prefix
fixtures replace only the record-count header and retain original IDs plus the
first one, seven or fourteen complete rows. Full runs load the unmodified file
for IDs two and ten. Both original/compiled state and an independently decoded
row multiset are compared, including key, format, text and all ten order values;
loaded keys must also be bytewise sorted. Reports pin fixture recipes/hashes.
These are emulator database loads, not a captured in-game load.

Scalar cases allow 200,000 instructions, larger fixtures two million, and complete
9,807-row loads one billion. Every case must actually return; reaching a limit
never passes. Private transient frames, SEH/exceptions, invalid pointers,
unbounded malformed strings, concurrency, arbitrary overlaps and recursive
partial private-header reads remain outside the contract.

```powershell
& py -3.13 recovery/lang/test_cm_lang_loader.py --optimization O2 --output CM0102/Analysis/lang-loader-check-O2
& py -3.13 recovery/lang/test_cm_lang_loader.py --optimization O0 --output CM0102/Analysis/lang-loader-check-O0
```

Output directories must be fresh. Build commands, GCC 15.2.0/binutils 2.45 tool
hashes, source hashes, static exports, compiled outputs and input identities are
recorded. Full database cases are expensive. `--quick` omits full database loads,
samples the two exhaustive word loops and reduces progress fixtures; a quick
PASS is not full acceptance.

The new build recipe requires `-fno-optimize-sibling-calls`. Diagnosis in
`CM0102/Analysis/lang-loader-tail-diagnosis/` reproduces an O2 tail jump from
setup that overwrites its incoming argument slots even though other observed
fields match. O0 and O2 with this flag preserve the original argument slots.
The flag belongs to this new chain build; existing accepted source and ledgers
are unchanged. This is an ABI requirement, not a recovered loader logic change.

Initial full-a runs were stopped when retaining raw global snapshots per event
grew to over 2 GiB per process. They are incomplete and are not accepted. Full-b
retains hashes of the same watched bytes, reducing trace storage. C/assembly
logic and original support execution are unchanged.
The bounded replay in `CM0102/Analysis/lang-loader-trace-diagnosis-b/report.json`
pins the reconstructed prior driver, confirms identical full-a/full-b compiled
objects, and compares every return/event/state field of a fourteen-row original
load after converting raw state bytes to their hashes. Independent one-byte
perturbations check all 13,020 watched loader bytes, 1,280 date bytes, 8,192
initially owned heap bytes and four current-language bytes. State payload
storage falls from 14,340 to 104 bytes per snapshot, excluding Python/event/input
overhead. These checks support trace coverage; they do not turn aborted runs
into accepted reports.

## Accepted results and handoff

| Accepted full report under `CM0102/Analysis/` | Build | Differential cases | Full database cases |
| --- | --- | ---: | ---: |
| `lang-loader-final-b-O2/report.json` | O2 | 131,500 | IDs 2 and 10, all 9,807 rows each |
| `lang-loader-final-b-O0/report.json` | O0 | 131,500 | IDs 2 and 10, all 9,807 rows each |

Both reports pin identical final sources and the same corpus. They include
65,536 version-word cases, 65,536 requested-ID cases and 428 other finite
cases. Fifteen tested source files are preserved under
`CM0102/Analysis/lang-loader-source-used/`, with identities in `manifest.json`.
The ledger accepts only these full-b reports. Earlier quick/failed/aborted
runs remain diagnostic evidence.

`CM0102/Analysis/lang-loader-map-final-a/` and `lang-loader-map-final-b/`
merge seven ledgers: 28 Date-family and fifteen support entries, 42 application
routines plus one separate CRT search routine. They check 16,070 actual calls
and the same three separately decoded direct tail transfers. Their functions,
calls, tails, format evidence and table artifacts are byte-identical. All 43
entries are validated within their individual finite contracts; none integrated.
Reproduce maps with fresh local names:

```powershell
& py -3.13 scripts/map_date_family.py --output CM0102/Analysis/lang-loader-map-check-a
& py -3.13 scripts/map_date_family.py --output CM0102/Analysis/lang-loader-map-check-b
```

The local `CM0102/Analysis/lang-loader-acceptance-audit.json` verifies eighteen
accepted/historical reports, fourteen reports pinning current sources, 168
source identities, 144 artifact identities, original function bytes, both maps
and the frozen loader sources. Previous six ledgers and accepted application
source are unchanged. Python syntax and warnings-as-errors builds pass.
Independent Standards and Spec reviews found no in-contract defects and
verified the final report, source snapshot, tool, input and map identities.
Preservation verification passes all 1,558 baseline files and 46 patch payloads.
Analysis outputs remain ignored and local. This workspace has no Git repository;
no commit, push or PR was made.

The subsequent [morphology extension](LANG-MORPHOLOGY.md) recovers table helpers
`0x006820A0`/`0x00682150` and establishes the larger routine's measured frame.
Next: recover morphology `0x006701F0`, then connect formatting/lookup tests to
loader-produced state.
M2 repeated runtime determinism, M4 integration and M6 standalone build remain
open. Recovering this loader does not establish a runnable source-built game.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/LANG-LOADER.md and
> docs/LANG-DEPENDENCIES.md. Verify the preserved baseline and loader final-b
> O0/O2 acceptance in recovery/lang/loader-ledger.json. Use fresh output folders.
> Preserve positive +4 SEEK_END, incoming marker argument writes, prior header/ID
> dwords, filtered file ordinals, dynamic globals, allocator/free/copy order and
> key/query normalization asymmetry. Keep original normal-return array/sort
> support explicit; this chain requires -fno-optimize-sibling-calls. Read
> docs/LANG-MORPHOLOGY.md and verify table-helper final-a O0/O2 acceptance.
> Recover morphology 0x006701F0 using its measured five-argument frame,
> then connect lookup/formatting to loader-produced state. Do not invent missing
> behavior. M2 repeated replay and primary/GDI mapping remain prerequisites for
> M4 integration; M6 standalone remains open.
