# Date family: function map and extended recovery

Later extension: [LANG-FORMAT.md](LANG-FORMAT.md) recovers the localization
wrapper, language getter, ordinal helper and the five name/order candidates
listed below. Its separate ledger preserves the original sixteen-routine
source/report identities. The current map merges both ledgers and covers
28 family entries, five support entries and 15,872 checked decoded calls.
The subsequent [lookup extension](LANG-LOOKUP.md) adds memory/file lookup,
comparator and separately accounted CRT search support. The latest map merges
three ledgers, covering 28 family entries, eight support entries and 15,980
checked calls. The inventory and accepted results below describe the earlier extension.

[DATE-NUMERIC.md](DATE-NUMERIC.md) subsequently recovers `0x0054A470` and
`0x0054A650`, including their prior-frame month-byte behavior. Its separate ledger
brings the selected application recovery to 28 routines at that stage.
[DATE-TEXT.md](DATE-TEXT.md) adds the seven remaining formatters. All 28 selected
family entries now compile, with 35 application routines across the combined
selection and separate CRT search support. Text-format acceptance is recorded
independently in its ledger. [LANG-LIFECYCLE.md](LANG-LIFECYCLE.md) adds five
language setup/ownership routines, bringing that selection to 40 compiled
application routines plus CRT search support. Its map merges six
ledgers: 28 family entries, thirteen support entries, 16,010 checked calls and
three separately checked direct tail transfers. [LANG-LOADER.md](LANG-LOADER.md)
adds loader and marker: that seven-ledger map contains 42 compiled
application routines plus CRT search support, 28 family and fifteen support
entries, 16,070 checked calls and three tail transfers.
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) adds two table helpers: the current
eight-ledger map contains 44 application routines plus CRT search support,
28 family/seventeen support entries, 16,074 calls and three tail transfers. Tables below describe
the historical sixteen-routine extension, not current recovery status.

Input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis reads its identical copy at `CM0102/Analysis/baseline-c/cm0102.exe`.
Every address here is a **preferred-image VA**, image base `0x00400000`.
The differential harness maps the original at that base. No GDI mapping or
live-game integration is implied.

This session extends the six-function [date slice](DATE.md) to sixteen compiled
routines: fourteen within the contiguous Date family, assignment elsewhere in
the executable, and the diagnostic version getter. It also documents the
remaining family candidates and their decoded callers. Compilation and finite
emulator validation remain separate from integration and complete subsystem
recovery. The acceptance evidence and exact scopes live in
`recovery/date/ledger.json` and the generated reports.

## Inventory and ownership

The selected inventory covers 28 detected functions from `0x00549F70` through
`0x0054D3B0`, plus two support routines elsewhere. Direct references to the
embedded `si\code\Date.cpp` path at `0x009B8478` associate the two YMD routines
and nine formatting candidates with that module. Adjacent routines share the
eight-byte date layout, constants, shared objects and call dependencies; their
membership is an evidence-backed family grouping, not a recovered original
translation-unit boundary. No functions have been renamed in Ghidra.

Caller counts below come from the existing decoded `calls.tsv`, with every
selected call's bytes rechecked against the pinned binary. Counts include
self-calls and omit unrecognized code or unresolved indirect incoming calls.
They measure observed use, not source recovery coverage. Sizes are Ghidra body
byte counts; formatting functions contain jump tables, so these counts must not
be treated as contiguous instruction extents.

| VA | Observed contract or explicit candidate | Body bytes | Direct callers | Recovery |
| --- | --- | ---: | ---: | --- |
| `0x00549F70` | YMD constructor, returns this | 414 | 369 | compiled; returning contract tested |
| `0x0054A110` | Default constructor | 21 | 554 | previous slice |
| `0x0054A130` | YMD initializer, carries incidental EAX | 412 | 33 | compiled; side-effect contract tested |
| `0x0054A2D0` | Adjust to a weekday | 273 | 11 | previous side-effect slice |
| `0x0054A3F0` | Calculate weekday | 118 | 74 | previous slice |
| `0x0054A470` | Numeric date, four-digit year candidate | 479 | 1 | unrecovered |
| `0x0054A650` | Numeric date, two-digit year candidate | 763 | 60 | unrecovered |
| `0x0054A950` | Full weekday plus numeric date candidate | 1,157 | 2 | unrecovered |
| `0x0054AE10` | Short weekday/month, ordinal day and year candidate | 1,177 | 5 | unrecovered |
| `0x0054B2F0` | Ordinal day, short month and year candidate | 598 | 1 | unrecovered |
| `0x0054B550` | Short weekday/month and ordinal day, no year candidate | 1,163 | 12 | unrecovered |
| `0x0054BA20` | Full weekday/month, ordinal day and year candidate | 1,166 | 48 | unrecovered |
| `0x0054BEF0` | Full weekday/month and ordinal day, no year candidate | 1,152 | 22 | unrecovered |
| `0x0054C3B0` | Ordinal day, full month and year candidate | 504 | 3 | unrecovered |
| `0x0054C5B0` | Add signed-word days | 422 | 252 | previous slice |
| `0x0054C770` | Add unsigned-byte weeks | 341 | 58 | compiled; value/state/ABI tested |
| `0x0054C8E0` | Subtract signed-word days | 433 | 113 | previous slice |
| `0x0054CAB0` | Subtract unsigned-byte weeks | 356 | 41 | compiled; value/state/ABI tested |
| `0x0054CC30` | Absolute year/day component difference | 358 | 32 | compiled; normal-return contract tested |
| `0x0054CDB0` | Signed day distance | 426 | 215 | compiled; value/state/ABI tested |
| `0x0054CF70` | Day of month, AX result | 56 | 19 | compiled; AX/state/ABI tested |
| `0x0054CFB0` | Zero-based month, or -1 | 46 | 83 | compiled; value/state/ABI tested |
| `0x0054CFE0` | Leap-year test | 85 | 2 | compiled; value/state/ABI tested |
| `0x0054D040` | Time-of-day name formatting candidate | 239 | 5 | unrecovered |
| `0x0054D130` | Language-dependent format selector candidate | 64 | 9 | unrecovered |
| `0x0054D170` | Weekday name formatting candidate | 130 | 9 | unrecovered |
| `0x0054D210` | Short month name formatting candidate | 366 | 10 | unrecovered |
| `0x0054D3B0` | Full month name formatting candidate | 361 | 16 | unrecovered |
| `0x00417C10` | Assignment returning a value | 51 | 917 | previous support slice |
| `0x0090D130` | Diagnostic version pointer | 6 | 3,872 | compiled; value/state/ABI tested |

The new map exporter checks and preserves **7,051 decoded call records**, including
incoming support-function calls and outgoing family dependencies. This is a union
of call records, not the sum of the caller column. `functions.json` keeps independent
identified/decompiled/compiled/validated/integrated flags, ownership evidence and
byte hashes for recovered entries; it does not upgrade formatting candidates.

The formatter descriptions come from verified PUSH instructions referencing
embedded language templates. Each has templates with and without time of day:

| Formatter VA | No-time template VA | With-time template VA |
| --- | --- | --- |
| `0x0054A470` | `0x009B8560` | `0x009B84A0` |
| `0x0054A650` | `0x009B8624` | `0x009B85AC` |
| `0x0054A950` | `0x009B86F8` | `0x009B867C` |
| `0x0054AE10` | `0x009B881C` | `0x009B87A0` |
| `0x0054B2F0` | `0x009B8900` | `0x009B8898` |
| `0x0054B550` | `0x009B89B0` | `0x009B894C` |
| `0x0054BA20` | `0x009B8AA0` | `0x009B89F8` |
| `0x0054BEF0` | `0x009B8B84` | `0x009B8B0C` |
| `0x0054C3B0` | `0x009B8C4C` | `0x009B8BD8` |

The exporter saves all eighteen instruction sites, their exact bytes and full
strings in local `format-evidence.json`. These templates contain annotation
markup consumed by the formatting boundary, so their apparent English pattern
does not establish final rendered output in every language. `0x0054A650` also
has alternate hyphenated numeric templates at `0x009B866C`/`0x009B8610`; its
branch selection is still unrecovered. Candidate names describe this evidence
rather than asserting complete control flow or API contracts.

## YMD construction and initialization

Both routines receive `this` in ECX and four stack dwords `(day, month, year,
target_weekday)`, then use `RET 16`. The day is checked as a signed low 16-bit
word against 1..31. The month is checked as a signed low byte against 0..11.
Invalid day becomes 1; invalid month becomes 0, after separate diagnostics in
day-then-month order. Upper argument bits are ignored for these checks. The
year is stored as a word and zero-extended for leap calculations.

They write year, leap flag, then ordinal. There is no check against the actual
month length: 31 February 2001 constructs ordinal 61, which the reverse month
lookup identifies as March. The source preserves this behavior.

| Month | Common offset | Leap offset |
| --- | ---: | ---: |
| January | -1 | -1 |
| February | 30 | 30 |
| March | 58 | 59 |
| April | 89 | 90 |
| May | 119 | 120 |
| June | 150 | 151 |
| July | 180 | 181 |
| August | 211 | 212 |
| September | 242 | 243 |
| October | 272 | 273 |
| November | 303 | 304 |
| December | 333 | 334 |

These immutable values come from `0x009B8448` and `0x009B8460` and are now
embedded in the recovered C. Ordinal is `(offset[month] + low_word(day)) mod
65536`. A target whose low word is `0xFFFF` skips adjustment; other targets
call the recovered weekday adjustment on the date itself, retaining its
original shared-state and alias ordering.

The two routines differ in their observable diagnostics and return registers:

- `0x00549F70` uses source lines 52 and 59 and finishes with `MOV EAX,ESI`
  at `0x0054A101`, restoring `this`. Its full return ABI is tested.
- `0x0054A130` uses lines 114 and 121. On the sentinel path EAX is the full
  target argument; adjustment leaves either the target or an expired stack
  pointer. The C interface is deliberately `void`, with normal-return side
  effects tested. This is not a general binary replacement contract.

Diagnostics call original `_splitpath`-style code at `0x00944CFF`, format a
message through `0x00944C9F`, and call the error UI at `0x005E8290`. The format
at `0x009870E8` is `v%s %s.%s %d`; title at `0x009870E0` is `Error`.
The original reuses the three arguments left on the stack by `0x0090D130` as
the last three `sprintf` arguments. The recovered C supplies those arguments
explicitly. The six-byte version getter ignores its arguments and returns
the `3.9.68` string at `0x00AD4394`; that helper is recovered separately.

The message buffer is at `0x00AE24D0`. After each error call the code clears
the dword at `0x00B67A34`, regardless of the UI result. Its wider purpose remains
unresolved. The harness hooks the three CRT/UI calls identically in both
machines, records argument contents and ordered calls, writes a deterministic
message, snapshots caller/shared state at each error call, and injects a
nonzero flag to observe the subsequent reset. It does not validate Windows
dialogs, real CRT formatting, filesystem parsing, or reentrant UI behavior.

## Weekly arithmetic and shared state

Both weekly operations take hidden result buffer followed by a week-count dword,
use `RET 8`, and return the result-buffer pointer. Only the unsigned low byte of
the count contributes: `0xFFFFFFFF` means 255 weeks. They perform 16-bit addition
or subtraction of `7 * count` before normalization. This preserves overflow and
invalid ordinal behavior. They do not dispatch through the signed day operators.

| Operation | Result object | Guard byte | Exit callback |
| --- | --- | --- | --- |
| Add weeks | `0x00AE2410` | `0x00AE2430` | `0x0054C8D0` |
| Subtract weeks | `0x00AE24C8` | `0x00AE24AA` | `0x0054CC20` |
| Component difference | `0x00AE2428` | `0x00AE249D` | `0x0054CDA0` |
| Day distance | `0x00AE2470` | `0x00AE2418` | `0x0054CF60` |

Each object initializes lazily to `(40, 1972, 1)`. Only guard bit 0 changes;
other bits survive. First use registers the listed original one-byte RET callback
through `0x0094627A`. Initializing a scratch that aliases the input can overwrite
that input before arithmetic begins. The tests retain these cases.

## Reverse conversion and leap testing

`0x0054CF70` and `0x0054CFB0` use the stored leap flag to select a table,
without recomputing it from year. They scan months from 11 down to 0 and choose
the first offset **strictly less than the signed ordinal**. Valid ordinal 0
therefore maps to January/day 1. A signed ordinal at or below -1 has no match:
month returns full EAX -1; day returns AX `0xFFFF`.

For large invalid positive ordinals, December remains the chosen month and the
day can exceed 31. Day-of-month's upper EAX contains incidental register/address
bits; the recovered result contract covers AX only. Both routines use plain RET
with `this` in ECX. `0x0054CFE0` instead uses cdecl, takes a stack dword, tests its
zero-extended low word, and returns full EAX 0 or 1. Year 0 and wrapped years
are preserved; there is no host calendar dependency.

## Date differences and preserved defects

Both operations pass the second date **by value** as two stack dwords, not by
pointer. Component difference receives a hidden output buffer before that value
and uses `RET 12`; day distance receives just the value and uses `RET 8`.

Component difference `0x0054CC30` orders operands using unsigned year and signed
ordinal. If the other date is greater it recursively swaps them. Otherwise it
subtracts their signed ordinals; a negative difference adds the length of the
later date's year and subtracts one from the year difference. It borrows using
that later year, even when the intervening preceding year has a different length.
The scratch's leap flag is not recomputed or overwritten after initialization:
its prior flag is copied to the output. A result has date-shaped storage but is
not a canonical date. Normal return ABI/state is tested; its temporary MSVC SEH
frame and exception unwind remain unrecovered.

Day distance `0x0054CDB0` returns signed ordinal difference plus counted years.
For a later `self`, it adds each intervening year's proper length after
decrementing the year. For an earlier `self`, it subtracts the length of
`year - 1` and then increments year. This makes some reversed distances unequal
in magnitude: ordinal 0/year 2000 to ordinal 0/year 2001 yields -365; the reverse
yields +366. The C preserves this original defect. Different-year paths copy
`self` into the separate shared object and change only that scratch's year;
same-year paths leave it untouched after any lazy initialization.

## Reproduction and remaining work

Use Python 3.13 with the installed Unicorn and Capstone, and fresh output names:

```powershell
& py -3.13 `
  recovery/date/test_cm_date.py --output CM0102/Analysis/date-family-NEW-O2 --optimization O2
& py -3.13 `
  recovery/date/test_cm_date.py --output CM0102/Analysis/date-family-NEW-O0 --optimization O0
& py -3.13 `
  scripts/map_date_family.py --output CM0102/Analysis/date-map-NEW
```

The full differential suite retains the prior arithmetic/weekday corpus, tests
all year words, both table choices at all ordinal words, all initializer day
words, all month bytes, all initializer target words, every weekly count byte,
a 400-year set of month boundaries, unsigned year wrap, signed ordinal extremes,
whole-object aliases, cold/warm guards, diagnostics, and seeded random bit
patterns. Reports record the exact counts, compiler commands, source/tool/input
hashes and artifact hashes. Compilation uses warnings as errors at O0 and O2.
Unexpected writes or execution in original code outside the explicit hooks fail.

The watched shared-state range is now 1,280 bytes, including diagnostic text;
all 256 caller-object bytes and the diagnostic flag are also compared. Stack
cleanup and callee-saved registers are checked. Exact private stack contents,
exceptions, arbitrary partial overlaps, invalid pointers, concurrency and real
CRT/UI behavior remain outside the contract. The negative-32768 day-operator
recursion remains bounded to 32 entries as in the previous slice.

Next static work should recover the formatting boundary at `0x0066F4E0` and
document its translation/formatting contract before porting the nine date string
functions. `0x0066F4D0` is a six-byte getter of the dword at `0x009EB2DC`; the
format selector calls it repeatedly, so its dynamic read order needs preserving.
Weekday/month name helpers use localization format strings and shared output
buffers; copying English constants would invent behavior for other languages.
Some formatters infer a month into a private stack byte: invalid ordinal paths
must be investigated for uninitialized values rather than normalized away.
M2 replay determinism and primary runtime remain open; M4 integration is still
gated. No standalone game or complete Date subsystem is claimed.

## Accepted session results

Final runs `CM0102/Analysis/date-family-c/report.json` (O2) and
`date-family-d/report.json` (O0) each passed **1,136,886 differential cases**,
including 24 bounded recursion observations. All sixteen selected routines are
compiled and marked validated within their individual stated contracts; all
remain `integrated: false`. The ten added routines cover 2,500 original bytes,
bringing the recovered inventory here to 3,818 original routine bytes. These
counts describe selected routines, not overall game coverage.

Both reports' source hashes match the final C, header and runner. The ledger
records their report hashes, source hashes, counts and limitations. Exact source
snapshots and a byte-identity copy manifest are preserved in the local ignored
`CM0102/Analysis/date-family-source-used/`. The a/b runs are intermediate
fifteen-routine evidence; use c/d for the current implementation.

Final map runs are `CM0102/Analysis/date-map-b` and `date-map-c`. Their
`functions.json`, `calls.json`, `tables.json` and `format-evidence.json` match
byte-for-byte. They cover thirty selected functions, 7,051 checked call records
and eighteen checked format-template references. `date-map-a` is an intermediate
map made before final validation statuses and the template exporter.

Independent Standards and Spec reviews found no hard violations or fidelity
defects. Standards noted an optional duplication smell in lazy initialization;
the explicit sequences were retained to keep original write order visible.
GCC builds passed with warnings as errors, Python syntax checks passed, and the
five existing runtime-formation tests passed. Final preservation verification
passed all 1,558 baseline files and 46 patch payloads. Generated binaries, maps,
reports and snapshots stay local and ignored. No Git repository exists; no
commit, remote, PR or publication was created.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/DATE-FAMILY.md and recovery/date/ledger.json.
> Verify preserved baselines. Reproduce the sixteen-routine date harness at O0/O2
> with new local output folders; c/d are the accepted reference reports. Preserve
> low-width arguments, independent scratch objects, diagnostic ordering, stored
> leap flags, asymmetric day distance, component-borrow behavior and explicit
> return-register/SEH residuals. Recover the formatting/localization boundary at
> VA 0x0066F4E0 before extending the nine mapped date-format routines. Use the
> verified template and caller map; candidates are not compiled recovery. Keep
> M2 deterministic runtime replay and primary/GDI mapping open, and do not
> integrate recovered code until the roadmap's M4 gate is met.
