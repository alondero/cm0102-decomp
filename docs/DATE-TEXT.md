# Seven date text formatters

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All executable addresses below are **preferred-image VAs**, image base
`0x00400000`. Incoming ESP is the original function entry stack pointer.

`recovery/date/cm_date_text.c`, `.h` and `.S` recover the seven remaining
formatters in the mapped contiguous Date family. The code derives from original
instructions, including helper arguments that Ghidra pseudocode drops because
the original keeps them on the stack across intermediate calls. The assembly
entries retain the original thiscall ABI and named prior-frame storage.
Independent statuses and original byte identities are in
`recovery/date/text-ledger.json`.

| Entry VA | Recovered interface | Ghidra body bytes | Hashed extent bytes | Prior month slot | Minimum size |
| --- | --- | ---: | ---: | --- | ---: |
| `0x0054A950` | Full weekday, numeric day/month/two-digit year | 1,157 | 1,216 | ESP-0x209 byte | 18 |
| `0x0054AE10` | Short weekday/month, ordinal day, year | 1,177 | 1,248 | ESP-0x208 dword | 18 |
| `0x0054B2F0` | Ordinal day, short month, year | 598 | 598 | ESP-0x204 dword | 18 |
| `0x0054B550` | Short weekday/month, ordinal day | 1,163 | 1,232 | ESP-0x208 dword | 18 |
| `0x0054BA20` | Full weekday/month, ordinal day, year | 1,166 | 1,232 | ESP-0x208 dword | 50 |
| `0x0054BEF0` | Full weekday/month, ordinal day | 1,152 | 1,216 | ESP-0x208 dword | 50 |
| `0x0054C3B0` | Ordinal day, full month, year | 504 | 504 | ESP-0x204 dword | 50 |

These are seven original application routines covering 6,917 Ghidra body bytes.
Their 7,246 hashed contiguous bytes additionally include embedded weekday jump
tables and alignment in the five weekday routines. The test exports both
seven-target tables per weekday routine using verified original JMP operands;
table bytes are not counted as decoded instructions. Month-only extents stop at
the last instruction, excluding following alignment. Private C helpers, worker
and entry shims do not increase original application coverage.

All 28 entries in this **selected Date-family inventory** now compile. Across
the five date/localization ledgers the selection reached 35 compiled application
routines and one separately counted CRT search routine at this stage.
[LANG-LIFECYCLE.md](LANG-LIFECYCLE.md) adds five setup/ownership routines, bringing
that application selection to 40. [LANG-LOADER.md](LANG-LOADER.md) subsequently
compiles loader and marker, bringing that application selection to 42.
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) adds both table helpers, bringing the
current application selection to 44.
This is bounded source
recovery, not complete Date-subsystem or game validation. No recovered routine
is integrated; M2 replay, M4 replacement and M6 standalone gates remain open.

## Parameters and diagnostics

ECX holds an eight-byte Date pointer. Stack arguments are output pointer, size
and time. All seven return full EAX zero or one and remove twelve argument bytes.
Null output is rejected before Date access. Otherwise only the signed low size
word is compared against the table's minimum. The check is not a write bound;
long output can exceed the advertised minimum. Time uses only its low byte.
No output clearing occurs on these diagnostic paths.

Null/size source-line pairs in original Date.cpp are respectively 385/392,
447/454, 507/514, 564/571, 624/631, 687/694 and 749/756 in entry order above.
Splitpath, version selection, diagnostic sprintf and error UI retain the previous
date recovery's ordering. Flag `0x00B67A34` clears after the UI call. Actual UI,
exception unwind and private diagnostic-buffer placement remain outside scope.

## Stored month and day behavior

The captured leap dword chooses common offsets `0x009B8448` when zero and leap
offsets `0x009B8460` otherwise. Both signed-word tables begin at -1. The forward
scan compares all twelve offsets strictly below the signed ordinal word and
assigns only the low month byte. Ordinal zero selects January, day one. Ordinal
-1 or lower leaves the prior month storage untouched.

`0x0054A950` reads a byte; six others later pass the entire month dword to a
month-name helper. Its three high bytes are never initialized by the scan.
The helpers mask the low byte, but the argument bits remain observable at the
call boundary. The assembly captures the byte or dword at its original position
before the C worker uses defined bits. It does not replace the original with a
synthetic month or an undefined C local. Caller changes can alter these prior
bytes; that remains an explicit integration constraint.

Six routines sign-extend the low month byte, read the selected table at that
index and subtract its word modulo 16 bits for a signed day argument. Indices
outside 0..11 access adjacent original image words, including literal bytes.
These reads and original bugs are preserved.

`0x0054B2F0` differs: its independent reverse scan from index 11 to zero computes
the day, returning -1 if no strict comparison succeeds. It never indexes the
day table using the prior month byte. Without time it uses the earlier ordinal
and table; after time formatting it reloads ordinal and leap before this scan.
Month still comes from the earlier forward scan. Negative day goes to the
original unsigned ordinal interface with its original bits. Day zero triggers
the ordinal helper's own diagnostic and empty shared-string behavior.

## Ordering and shared localization state

Time bytes 0/1/2 cause AM/PM/EVE formatting into `0x00AE24AC` for the four short
forms and Morning/Afternoon/Evening into `0x00AE2484` for the three full forms.
Other bytes omit time. The short time templates retain their extra empty-string
argument. Branch selection uses captured time bits.

Weekday arithmetic preserves signed divisions/remainder, stored leap and
negative indices. The date-only path uses the captured ordinal/leap; the time
path reloads them after its time formatter. Year is read at weekday calculation.
Full weekday names share `0x00AE2434`; short names share `0x00AE2478`. Invalid
weekday indices clear the appropriate name buffer. Month names and ordinal text
use the unchanged recovered helpers and their shared buffers, including aliasing.

The numeric weekday routine rereads year after the weekday-name formatter and
passes year modulo 100. The ordinal routines with year read it after the
weekday helper (or time helper when no weekday exists), before the month helper.
Original instructions push and retain this year on the argument stack across
month and ordinal calls; the C worker explicitly caches it at that point.
Other Date fields retain their documented earlier captures. Tests mutate Date
at named formatter boundaries to distinguish these reads.

The full weekday final templates retain an extra metadata argument of 100 before
the weekday string, consumed by the wrapper's brace syntax. Numeric weekday
output contains a newline between weekday and numeric date. Literal spacing,
templates, metadata, destinations and final argument order are preserved rather
than reconstructed from the incomplete pseudocode signatures.

## Differential harness and remaining limits

Run each compiler configuration into a fresh ignored local directory:

```powershell
& py -3.13 recovery/date/test_cm_date_text.py --optimization O2 --output CM0102/Analysis/date-text-check-O2
& py -3.13 recovery/date/test_cm_date_text.py --optimization O0 --output CM0102/Analysis/date-text-check-O0
```

The harness builds freestanding x86 C/assembly with warnings as errors and pins
input, source, static-export, compiler/assembler/linker and artifact hashes.
Original application instructions execute against the pinned PE. Each selected
Ghidra instruction record is checked byte-for-byte before its disassembly is
exported. Fresh final-b O0/O2 exhaustive runs each pass 1,860,817 comparisons;
the ledger marks all seven entries validated within their stated contracts.

Scalar calls retain a 200,000-instruction limit. Dependency chains use a finite
1,000,000-instruction limit, with completed return and all parity/ABI assertions
still required. A local diagnostic isolated 84 O0 translated-file cases that
exceeded the inherited 200,000 limit; all 224 translation cases returned and
matched at the larger limit. Recovered C/assembly were unchanged. The first O0
full run remains incomplete evidence, not acceptance; fresh final-b runs pin
the corrected harness. Replay and hashes are under ignored
`CM0102/Analysis/date-text-bound-diagnosis/`.

Scalar sweeps compare all 65,536 ordinal words under both stored leap choices,
all year words, all size words, all time bytes, all prior month bytes and selected
untouched high dword patterns. The explicit formatter hook records template,
arguments, destination and shared state and supplies a fixture response. These
sweeps validate date logic at that boundary; they do not validate localization.

Additional cases execute the original and recovered wrapper, name, ordinal,
lookup, comparator and CRT search instructions in chains: language/time combinations,
preview and dynamic language reads, shared-buffer Date/output aliases, guards
with null Date, and seven independently checked rendered strings. Memory/file
translation chains use synthetic rows, not captured in-game translation results.
Selected chain cases compare caller, date/name/diagnostic, ordinal, language,
heap/table state, external events, return values and ABI. Guards check full EAX,
ESP cleanup and preserved registers through the common harness.

CRT allocation, file I/O, vsprintf/sprintf, string search, morphology, splitpath
and diagnostic UI remain explicit modeled dependencies. Prior-frame slots alone
are part of the private-frame contract; other frame bytes, exception behavior,
invalid pointers, arbitrary partial overlap, concurrency and unsafe markup or
overflow paths are excluded. Fixed-address lookup redirection exists only in the
emulator. No live replacement or runnable standalone source game is claimed.

## Accepted results and handoff

Accepted reports are `CM0102/Analysis/date-text-final-b-O2/report.json` and
`date-text-final-b-O0/report.json`. Each passes **1,860,817 cases**, including
917,504 ordinal-word cases across both stored leap choices, 458,752 year-word
cases, 458,752 size-word cases, prior-month byte/dword patterns, time bytes,
mutation/alias/guard cases and 224 translated lookup chains. Seven rendered
outputs are independently checked. The harness pins 2,161 original instruction
records totaling 6,917 code bytes; embedded tables are exported separately.

Both reports pin the same final nineteen sources. Their report hashes and
source identities are recorded in `recovery/date/text-ledger.json`; exact copies
and accepted report hashes are retained in
`CM0102/Analysis/date-text-source-used-b/manifest.json`. The earlier final-a O2
report predates the instruction-limit correction and is not current harness
acceptance. The incomplete final-a O0 run is retained as diagnostic evidence.

Final maps `CM0102/Analysis/date-lang-map-final-a/` and `date-lang-map-final-b/`
merge six ledgers with 28 Date-family and thirteen support entries. They check
16,010 calls and three separately recorded tail transfers. Their functions,
calls, tail transfers, format evidence and table artifacts match byte-for-byte.
The selection contains 40 compiled application routines plus one separate CRT
search routine, all validated within individual contracts and none integrated.
Older accepted source/ledgers are unchanged.

The read-only local audit `CM0102/Analysis/date-lang-acceptance-audit.json`
checks sixteen accepted/historical report identities, twelve reports pinning
current sources, 138 source identities and 126 artifact identities across the
six ledgers. Original executable and analysis-copy hashes remain pinned.
Independent Standards/Spec reviews found no in-contract defects after correcting
the handoff's lifecycle status. Python syntax and warnings-as-errors C builds
pass. The existing five runtime-formation tests pass; preservation verification
passes all 1,558 baseline files and 46 patch payloads. Analysis artifacts remain
local and ignored. This workspace has no Git repository; no commit or PR was made.

Later extension: [LANG-LOADER.md](LANG-LOADER.md) recovers loader `0x0066EB40`
and marker `0x0090C140` in a separate ledger, retaining all acceptance identities
above. [LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) subsequently recovers table helpers
`0x006820A0`/`0x00682150` and measures the larger routine's frame. Next work:
recover morphology `0x006701F0` from instructions.
Verified starting boundaries and commands are in
[LANG-DEPENDENCIES.md](LANG-DEPENDENCIES.md).
Continue repeated runtime replay and primary/GDI equivalence mapping before
replacing original code in a running game.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/DATE-TEXT.md, docs/LANG-LIFECYCLE.md,
> docs/LANG-DEPENDENCIES.md, docs/LANG-LOADER.md and their recovery ledgers. Preserve the official
> baseline. Date text final-b and lifecycle final-a O0/O2 are accepted references;
> use fresh output folders when reproducing them. Keep named prior-frame storage,
> helper read order, row order-array bytes, later pointer loads after frees,
> incidental EAX and cached allocation headers. Check loader final-b O0/O2
> acceptance. Preserve the marker's positive end seek and incoming argument write,
> prior header/ID bits, original CRT dependencies and key normalization asymmetry.
> Verify table-helper final-a O0/O2 acceptance in docs/LANG-MORPHOLOGY.md and
> continue morphology using its measured five-argument frame. M2 runtime replay and
> primary/GDI mapping remain prerequisites for M4 live integration and M6 build.
