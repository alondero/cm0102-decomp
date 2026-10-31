# Numeric date formatting with original stack-dependent behavior

Later extension: [DATE-TEXT.md](DATE-TEXT.md) compiles the seven remaining mapped
formatters, including this note's `0x0054A950` starting contract. The selected
date/localization inventory reached 35 compiled application routines at that
stage. [LANG-LIFECYCLE.md](LANG-LIFECYCLE.md) adds five setup/ownership routines,
bringing it to 40 application routines plus separate CRT search support.
[LANG-LOADER.md](LANG-LOADER.md) adds loader and marker, bringing the current
application selection to 42 with the same separate CRT search support.
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) adds both table helpers, bringing the
current application selection to 44.
Text-format acceptance has its own ledger; the
two-routine results and next-work observations below are historical and unchanged.

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis reads its identical copy in `CM0102/Analysis/baseline-c/`.
All addresses below are **preferred-image VAs**, image base `0x00400000`.
Incoming ESP denotes the function's original x86 entry stack pointer.

`recovery/date/cm_date_numeric.c`, `.S` and `.h` recover the two numeric
date formatters. C implements instruction-derived normal-return logic; small
x86 entry shims retain original thiscall cleanup and the exact prior-frame byte
used by invalid-ordinal paths. No month default or host calendar is substituted.
Independent statuses and byte identities are in `recovery/date/numeric-ledger.json`.
Previous date, name, wrapper and lookup source/acceptance identities are unchanged.

| VA extent, inclusive | Contract | Original body bytes | Interface |
| --- | --- | ---: | --- |
| `0x0054A470..0x0054A64E` | Numeric date with full year word | 479 | thiscall, full EAX 0/1, RET 12 |
| `0x0054A650..0x0054A94A` | Numeric date with year modulo 100 | 763 | thiscall, full EAX 0/1, RET 12 |

These add two application routines covering 1,242 original bytes. The C worker,
diagnostic/time helpers and entry shims are implementation support, not additional
original routines. The separate previously recovered CRT search remains support.
The selected date/localization application inventory becomes 28 compiled routines.
All remain unintegrated. This advances source recovery toward a buildable game;
M2 runtime determinism, M4 replacement and M6 standalone parity remain open.

## Parameters, guards and diagnostics

ECX carries the original eight-byte Date pointer; stack arguments are output,
size and time. Only the signed low word of size and low byte of time participate.
Null output is checked first, before accessing Date. Size below 9 triggers the
second diagnostic, even if its unsigned representation is large. High argument
bits are ignored. The routines return zero on these errors and one on other
normal paths. Neither error clears the output, and size is not subsequently
used as a write limit. A passing capacity check does not guarantee enough space
for the formatted date, especially with time text or noncanonical inputs.

Diagnostics call the same original Date.cpp splitpath, version-pointer, sprintf
and UI boundaries as earlier date recovery. Null-output source lines are
248/306; bad-size lines are 255/313 for the full/modulo-year routines respectively.
The diagnostic flag at `0x00B67A34` is cleared after UI. Normal-return tests retain
the argument order and shared diagnostic buffer effects. Private diagnostic
buffer positions, real UI and exception behavior are outside validation.

## Month extraction and prior stack bytes

The stored leap dword selects the table: zero uses `0x009B8448`, any nonzero
value uses `0x009B8460`; leap is not recomputed from year. Tables are signed
words, beginning at -1. Signed ordinal zero therefore selects January and
prints day 1. Both routines scan all twelve offsets with a **strict less-than**
test against the cached signed ordinal; each successful comparison updates the
month byte. The last success wins. A large positive ordinal still uses the
last month and can produce a large day number.

On ordinal -1 or smaller, no offset is smaller. The original month byte comes
from uninitialized private storage, not a sentinel or canonical month. The
full-year routine loads BL at `0x0054A544`; its address is incoming ESP minus
`0x205`. The modulo-year routine loads BL at `0x0054A724`; its address is incoming
ESP minus `0x209`. These positions follow directly from the original frame
reservations and register pushes. Both sign-extend BL, access the selected table
at that signed index, and subtract the loaded word modulo 16 bits. The signed
result is the day argument; month argument is signed index plus one. Indices
outside 0..11 can read adjacent original-image words, including string bytes.

The assembly shims reserve the original entry frame sizes and capture these
bytes before calling the C worker. C receives defined byte bits and performs
explicit unsigned x86 address arithmetic and signed-word interpretation. A C
uninitialized local would permit compiler transformations unsupported by the
original behavior. The assembly does not claim equality of other private-frame
contents, flags, volatile registers, instruction timing or exception unwind.
Future callers must treat the prior-frame byte as part of the invalid-ordinal
contract; compiled caller changes can alter it.

## Localization and observable call order

Time bytes 0/1/2 format the original AM/PM/EVE annotated templates into shared
buffer `0x00AE24AC`. Other byte values take the date-only path. Captured time
bits are reused; later calls cannot change branch selection. Original internal
empty-string fallback blocks are unreachable from these captured values and
remain present in the static evidence, without invented reachable behavior.

The full-year routine uses original annotated templates at `0x009B8560`
(date only) and `0x009B84A0` (with time), passing signed day, signed month+1
and the zero-extended year word. The modulo-year routine reads current language
before optional time formatting and retains that branch choice. Language 10
prints year/month/day directly using original sprintf templates `0x009B866C`
and `0x009B8610`; it bypasses date localization. Other IDs use the annotated
templates at `0x009B8624`/`0x009B85AC`. The year word is zero-extended before
remainder by 100, matching the original signed IDIV's nonnegative input.

Year is read after time formatting on time paths. Tests change it at that
boundary and exercise Date/output aliases with shared time or diagnostic storage.
Day, selected table and month remain the earlier cached values. Dynamic language
reads inside the recovered wrapper remain observable; they do not retroactively
change the modulo-year routine's stored language decision.

## Differential evidence

The new `recovery/date/test_cm_date_numeric.py` builds freestanding 32-bit C and
assembly with warnings as errors at O0/O2. Original instructions run at their
preferred base; recovered code uses a separate text mapping. Reports pin all
source/tool/input/artifact hashes, build commands and the two prior-byte offsets.
All 400 Ghidra-exported instruction byte records are checked against the primary
executable, and contiguous decoding covers both full routine extents.

Scalar sweeps use an explicit formatter hook. That hook records the original
template address, argument bits/string bytes, destination and state, and supplies
a visible fixture response. It makes no claim of localization equivalence.
The corpus sweeps all ordinal words under both stored leap choices, all year
words, all size words with nonzero high bits, and all time bytes. It exercises
all 256 prior-month-byte values at both table choices with ordinal zero, -1 and
-32768. The zero cases verify normal month selection ignores the prior byte.
Year mutation at the time-format boundary checks later source reads.

Separate chain cases execute original versus recovered wrapper, lookup,
comparator and search instructions. These cover ordinary and negative ordinals,
selected prior-byte extremes, language 10's bypass, dynamic language sequences,
preview mode, guard failures and aliases. Synthetic translation tables and files
exercise the complete chain through actual lookup, rather than a returned-record
fixture. Those rows are test data, not captured game database translations.
Independent rendered expectations include January 1, 2001 as `01.01.2001`,
`1.1.01`, and language 10's `01-01-01` under modeled CRT formatting.

Returning cases check EAX, stack cleanup, callee-saved registers, restored FS:[0],
caller and date state, plus watched language/table/heap state and ordered boundary
events for chain cases. Unexpected native application writes fail. Each call's
named prior-byte position is explicitly seeded; the original and recovered entry
receive equal prior storage, not equal entire resulting private frames.

Allocation, file I/O, morphology, CRT formatting and diagnostics remain explicit
modeled boundaries. Lookup entry redirection is emulator-only on the recovered
side, preserving the earlier formatter source. No live original process is
patched. Invalid pointers on dereferenced paths, arbitrary partial overlaps,
concurrency, unsafe markup/overflow paths, allocation failure and exception
unwind remain excluded. Prior-stack dependence is recovered within the stated
entry contract; this does not establish universal stack-dependent game parity.

```powershell
& py -3.13 `
  recovery/date/test_cm_date_numeric.py --output CM0102/Analysis/date-numeric-NEW-O2
& py -3.13 `
  recovery/date/test_cm_date_numeric.py --output CM0102/Analysis/date-numeric-NEW-O0 --optimization O0
& py -3.13 `
  scripts/map_date_family.py --output CM0102/Analysis/date-numeric-map-NEW
```

The map now merges four ledgers with duplicate/address/hash checks. Its selected
inventory remains 28 family entries and eight support entries, with 15,980 checked
decoded call records: these two formatters were already mapped candidates.
Seven larger date formatters remain uncompiled. Current acceptance reports and
source snapshots are recorded below. The pending map run is intermediate evidence
with compiled but unvalidated status and is not the final acceptance map.

## Accepted results

Final reports `CM0102/Analysis/date-numeric-final-a-O2/report.json` and
`date-numeric-final-a-O0/report.json` each pass **531,967 differential cases**.
They include 262,144 ordinal-word comparisons, 131,072 year-word comparisons,
131,072 size-word comparisons, 3,072 explicit prior-byte cases, 224 selected
prior-byte chain cases and 192 synthetic translated-lookup chain cases. Counts
include three independent rendered expectations; the remaining groups cover
time/language, aliases, diagnostics, mutations, preview and random bit patterns.
The ledger pins accepted report/source hashes and marks both routines validated
within the stated contracts, with integration false. Exact source snapshots and
a byte-identity manifest are local in `CM0102/Analysis/date-numeric-source-used/`.

Final map runs `CM0102/Analysis/date-numeric-map-final-a` and
`date-numeric-map-final-b` have byte-identical function, call, table and
format-evidence exports. The intermediate `date-numeric-map-pending-a` predates
validation promotion. Final artifacts, source snapshots and all earlier
date/formatter/lookup acceptance identities were checked against their pinned hashes.

## Standards review

Independent review found no hard documented-standard violations or optional
smells requiring action. It checked original prologues/BL loads against both
assembly shims and confirmed the final acceptance-ledger and handoff counts.
The original bugs, explicit boundaries and separate recovery stages remain visible.

## Spec review

Independent review found no in-contract fidelity defects. It verified all 400
original instruction-byte records, checked shim argument forwarding and RET 12,
and passed fourteen additional O2 differential chain cases for negative ordinals,
prior-byte extremes, time branches and changing language sequences. Its final
acceptance check confirmed source/report hashes and the documented validation limits.

Review summary: zero unresolved Standards findings; zero Spec findings. Both GCC
builds passed with warnings as errors, changed Python files passed syntax checks,
and all five existing runtime-formation tests passed. Final preservation verification
passed all 1,558 baseline files and 46 official patch payloads. Generated binaries,
objects, reports, maps and snapshots remain local under ignored `CM0102/Analysis/`.
No supplied binary, asset or baseline manifest was changed. No Git repository
exists; no commit, remote, PR or publication was created.

## Next work

Extend the full-weekday numeric formatter at `0x0054A950`, then the ordinal/month
date formatters. Recover each original frame-dependent month contract and preserve
shared name/ordinal/time buffer ordering. Translation loading and morphology
`0x006701F0` remain unrecovered application dependencies. Continue repeated M2
runtime replay and primary/GDI mapping before M4 replacement. No runnable
standalone source game is claimed.

Static starting evidence for `0x0054A950`: its signed size guard compares against
18 at `0x0054A994`; diagnostics use source lines 385/392. The prologue reserves
`0x20C` bytes, and its four saved-register pushes place the month-byte load at
`0x0054AA1F` (ESP+0x13) at incoming ESP minus `0x209`. This is a verified next
entry contract, not compiled recovery of that formatter. Its remaining control
flow, template metadata and source reads still require instruction-derived recovery.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/DATE-NUMERIC.md and recovery/date/numeric-ledger.json.
> Verify preserved baselines. Reproduce both numeric entries at O0/O2 in fresh
> output directories, including equal incoming-stack seed bytes. Preserve strict
> month scans, stored leap choice, year-read timing, signed size/time widths and
> language-10 bypass. Extend 0x0054A950 and remaining ordinal/month formatters
> from instructions; retain prior source identities and M2/M4 gates.
