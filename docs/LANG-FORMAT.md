# Localization wrapper, date names and ordinal numbers

Later extension: [LANG-LOOKUP.md](LANG-LOOKUP.md) recovers the actual memory/file
lookup, comparator and separately accounted CRT search support. Its new harness
tests lookup through this unchanged formatter source. The eight-routine corpus
and ledger below retain their original lookup fixtures and accepted identities;
the next-target observations below describe the state before that extension.

[DATE-NUMERIC.md](DATE-NUMERIC.md) also recovers both numeric date routines,
including the incoming-stack month bytes and language-10 sprintf bypass.
The numeric next-target observations below are retained as historical evidence;
[DATE-TEXT.md](DATE-TEXT.md) now compiles those seven remaining mapped formatters
under explicit prior byte/dword contracts. Its independent ledger records
acceptance status. [LANG-LIFECYCLE.md](LANG-LIFECYCLE.md) adds five language
setup/ownership routines, bringing that application count to 40.
[LANG-LOADER.md](LANG-LOADER.md) adds loader and marker, bringing the current
selection to 42 with separate CRT search support. [LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md)
adds both table helpers, bringing the current application selection to 44;
earlier counts and next-target observations below
are historical.

Input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis reads its identical copy in `CM0102/Analysis/baseline-c/`.
All addresses below are **preferred-image VAs**, image base `0x00400000`.
The differential harness maps the original at that base. No GDI mapping or
live-game replacement is established.

Eight additional routines compile as 32-bit x86 and pass a finite differential
corpus. This extends M3; M2 replay determinism, M4 integration and the standalone
build remain open. Source is in `recovery/lang/` and
`recovery/date/cm_date_names.c`. Independent recovery flags, original byte hashes
and accepted reports are in `recovery/lang/ledger.json`. The existing sixteen
date routines and their accepted source/reports were preserved.

| VA | Recovered contract | Inventory body bytes | Interface |
| --- | --- | ---: | --- |
| `0x0066F4D0` | Current language dword | 6 | cdecl, full EAX |
| `0x0066F4E0` | Localization/formatting wrapper | 1,743 | variadic cdecl, adjusted output pointer |
| `0x0054D040` | Short/full time-of-day name | 239 | cdecl, shared-buffer pointer |
| `0x0054D130` | Language-dependent date-format order | 64 | cdecl, AL only |
| `0x0054D170` | Full weekday name | 130 | cdecl, shared-buffer pointer |
| `0x0054D210` | Short month name | 366 | cdecl, shared-buffer pointer |
| `0x0054D3B0` | Full month name | 361 | cdecl, shared-buffer pointer |
| `0x0090C310` | Ordinal number with language/form branches | 512 | cdecl, shared-buffer pointer |

These add 3,421 selected original routine-body bytes according to the current
Ghidra inventory, bringing the date/formatting selections to 24 compiled routines
and 7,239 inventory body bytes. These are selected-slice counts, not game
coverage. Name-helper ledger hashes cover contiguous extents including verified
jump tables and padding; those extents exceed the body counts above. The evidence
exporter separates code from tables.

## Localization wrapper

`0x0066F4E0` copies leading literal spaces into the output, then operates on both
pointers after those spaces. It returns the **adjusted output pointer**, including
in preview mode. The caller cleans all variadic arguments. Source retains original
fixed data addresses and explicit unresolved calls, so it is not a standalone
library.

The dword at `0x00B679F4` controls a preview-like path. This name describes observed
behavior rather than recovering an original identifier. Language 0 and
`0xFFFFFFFF` bypass lookup; other values call `0x0066FBB0` with a 52-byte output
record, the format after leading spaces and a null third argument. The record has
three owned string pointers followed by ten dword argument-order entries. Failed
lookup or empty translated text selects fallback. Every nonnull key/format/text
allocation is freed in that order on normal return, including failure paths.

Successful translation scans the original format before formatting values.
Each `<` advances a field counter; `{` and `[` consume a low-byte property from
a separate leading part of the variadic arguments. A marker immediately after
`>` supplies a suffix property for the preceding field; other markers supply
a prefix property for the following field. Twenty property bytes start at
`0xFF`. Preview substitutes zero properties without consuming arguments.

The lookup's printf format goes to original vsprintf at `0x009470CE`, with the
argument cursor after the properties. Its result at `0x00B6619C` is split in
place on `~` into the pointer array at `0x00B66174`. Translated text uses byte
`0x08` for an insertion; the corresponding order entry selects a split string.
If both property bytes are `0xFF`, that string is copied directly. Otherwise
prefix/suffix text and the two byte properties go to unrecovered morphology at
`0x006701F0`. The translated walk consumes `{...}` annotations, resets prefix
state at spaces and collapses paired literal percent characters. It does not
call printf on the translated text itself.

Preview creates ten `" ~"` pairs. Splitting the tenth separator writes pointer
slot 10 at `0x00B6619C`, overlapping the start of the work buffer. Recovery retains
this aliasing; ten-slot preview tests compare its state and dependency inputs.
No extra pointer capacity or safer buffer layout was substituted.

Fallback removes empty property markers and explanatory text from annotated
printf fields while preserving printf specifiers. It consumes property arguments
before vsprintf. Preview builds the shared format but leaves output untouched
after the leading spaces already copied. Ordinary fallback then searches the
rendered string for `{`, followed if absent by `[`, through original strstr at
`0x00945A60`. Only strings shorter than 1,000 bytes enter a private-buffer
morphology pass. That pass supplies literal prefix, word and suffix strings and
property bytes `(100, 100)` to `0x006701F0`.

The backward word scan at `0x0066F9EF..0x0066FA36` starts its copy one byte before
the located boundary. It can include a preceding character; reaching index zero
reads before its private buffer. Source preserves this scan. Tests cover returning
cases with defined private-buffer reads. Underflow, uninitialized-stack cases,
malformed markers, missing terminators and oversized fields remain unvalidated.

| Original state/dependency | Role |
| --- | --- |
| `0x009EB2DC` | Current language dword; every read remains dynamic |
| `0x00B679F4` | Preview-like branch control |
| `0x00B66174` | Split-string pointer array |
| `0x00B6619C` | Shared format/formatted argument storage |
| `0x00B669C4`, `0x00B671EC` | Translated prefix/suffix buffers |
| `0x0066FBB0` | Translation lookup/parser; unrecovered |
| `0x006701F0` | Morphology; unrecovered |
| `0x009470CE`, `0x00945A60`, `0x00944C94` | vsprintf, strstr, free; modeled CRT boundaries |

Lookup references the embedded `si\code\Langlib.cpp` path. Wrapper ownership
follows adjacency/dependency; it has no direct source-path reference in the
accepted map. Morphology has 49,771 detected body bytes and substantial language
branches. It remains a dependency, not recovery credited to this session.

## Date-name helpers and format order

Name helpers call the recovered wrapper with original templates. Short
March/December and full May have `%s` comment annotations and pass the original
empty string at `0x00AD9C64`.

| Helper | Argument interpretation | Shared output |
| --- | --- | --- |
| Time-of-day name | unsigned low byte 0/1/2; full-name flag uses entire dword | `0x00AE2484` |
| Weekday name | signed low word 0..6; other words select empty text | `0x00AE2434` |
| Short month name | signed low byte 0..11; others select empty text | `0x00AE24A0` |
| Full month name | signed low byte 0..11; others select empty text | `0x00AE2450` |

Invalid values write the empty string without localization. Jump tables at
`0x0054D1F4`, `0x0054D380` and `0x0054D51C` determine name order; exact targets,
PUSH instructions, template VAs and strings are exported in `template-tables.json`.

Format order reads language separately for comparisons with 9, 3, 5 and 4,
in that order. IDs 9/3 return AL 1; 5 returns AL 2. For ID 4, variant low byte
0/1 returns 1 and 2 returns 3. Other paths return 0. Tests change the language
between reads and compare count/results. Original upper EAX is incidental;
the recovered interface and validation cover AL only.

## Ordinal-number helper

`0x0090C310` references embedded `si\code\utils.cpp` and writes shared output
at `0x00DE2734`. Zero triggers the existing diagnostic pattern with source line
966, resets `0x00B67A34` after the UI and clears only the first output byte.
It links the previously recovered version getter. CRT/UI remain hooks.

For language ID 2, low-byte form controls singular `1er`/`1\xE8re` branches.
Form 1 with nonsingular numbers prints a plain number; other nonsingular forms
use `%d\xE8me`. Bytes come from the original image without Unicode conversion.
Other paths read language a second time. IDs 4/9/7/11 with form 1, and 10/11
with form 3, print plain numbers. Remaining cases call the recovered wrapper
with the original annotated `st`, `nd`, `rd` or `th` template. Unsigned full-dword
division classifies residues 10..20 as `th`; outside that interval, last digits
1/2/3 select `st`/`nd`/`rd`. Formatting passes the same bits to signed `%d`, so
large unsigned inputs can render as negative numbers. These quirks are tested.

## Reproduction and accepted results

Use Python 3.13 with the installed Unicorn/Capstone and fresh output names:

```powershell
& py -3.13 `
  recovery/lang/test_cm_lang.py --output CM0102/Analysis/lang-format-NEW-O2
& py -3.13 `
  recovery/lang/test_cm_lang.py --output CM0102/Analysis/lang-format-NEW-O0 --optimization O0
& py -3.13 `
  scripts/map_date_family.py --output CM0102/Analysis/date-lang-map-NEW
```

GCC/ld builds freestanding x86 with warnings as errors. Recovered code runs in
a separate text mapping; execution in original text fails except at explicit
hooks. Checks compare returns, stack cleanup, callee-saved registers, restored
FS:[0], all 16 KiB of caller storage, watched Date/localization/ordinal buffers,
allocation contents, ordered dependency inputs, shared state at boundaries and
language reads. Unexpected application writes fail.

Lookup fixtures describe records returned by the hook, not game translations.
Morphology returns a visible fixture response to expose wrapper arguments/copying.
The CRT hook interprets only corpus specifiers. Results establish agreement
under these responses, not real Langlib/database, morphology, Windows dialog
or complete CRT equivalence. Exceptions/unwinding, arbitrary overlaps,
concurrency and identified unsafe paths remain excluded.

Accepted reports `CM0102/Analysis/lang-format-final-c-O2/report.json` and
`lang-format-final-d-O0/report.json` each pass **81,232 differential cases**
for the final source. Earlier smoke/first/final-a/final-b runs are intermediate.
The ledger pins accepted report/source hashes; all eight entries remain
`integrated: false`. The old date source is linked for its version getter;
unused linked routines are not counted as new recovery.

Read-only export `CM0102/Analysis/lang-date-static-a` holds checked instructions,
references and unvalidated pseudocode for wrapper, lookup, ordinal and all nine
larger date formatters. Reproduce with:

```powershell
& py -3.13 `
  scripts/map_recovery.py --output CM0102/Analysis/lang-date-static-NEW `
  --functions 0066f4e0 0066fbb0 0054a470 0054a650 0054a950 0054ae10 `
              0054b2f0 0054b550 0054ba20 0054bef0 0054c3b0 0090c310
```

The date/language map merges both ledgers, rejects duplicates and verifies their
byte identities. Larger date formatters remain uncompiled candidates; generated
pseudocode does not establish authored recovery or parity.

Final map runs `CM0102/Analysis/date-lang-map-a` and `date-lang-map-b` contain
33 selected functions (28 family, five support) and 15,872 checked call records.
Their functions, calls, tables and format-evidence exports match byte-for-byte.
Exact source snapshots are preserved in ignored
`CM0102/Analysis/lang-format-source-used/`, with a byte-identity manifest.

## Next targets

Numeric date routines `0x0054A470`/`0x0054A650` are close to a bounded port now
that their wrapper dependency is recovered. Diagnostics check null output and
a signed low-word size below 9. Both scan month offsets into a private stack
byte initially uninitialized when no offset is less than the signed ordinal.
Do not give it a synthetic default. The two-digit-year routine also branches
on language ID 10 and calls sprintf directly with year/month/day hyphenated
templates, bypassing localization. These are observations, not compiled recovery.

Lookup `0x0066FBB0` has both in-memory table search and file-backed paths,
allocation ownership, key normalization and ten argument-order dwords. Its
1,599 detected body bytes remain unrecovered despite available pseudocode.
The checked instructions at `0x0066FC6B` choose the memory path only when
`0x00B647B0 == 1`. The search call at `0x0066FCE5` supplies table base from
`0x00B679F0`, count from `0x00B647B4`, stride `0x34` and comparator
`0x00682060` to the bsearch-style helper `0x00946464`. The file path instead
opens the filename at `0x00B6586C` and reads through `0x00945BFE`.
Normalization at `0x0066FBEC..0x0066FC3D` removes braces and spaces and translates
literal backslash-n/backslash-quote sequences; square brackets remain. These
parameters narrow the next investigation without asserting parser recovery.
Recover this boundary next, then extend date formatting. Continue M2 deterministic
replay and primary/GDI mapping; the formatter corpus does not satisfy those
gates or establish a runnable source game.

## Review and preservation

Independent Standards and Spec reviews found no hard violations or in-contract
fidelity defects. Standards noted optional duplicate byte-hash checks in the
combined map exporter; the existing selected-entry check was retained alongside
the new whole-ledger check. Python syntax checks and all five existing formation
repair tests passed. Final preservation verification passed all 1,558 baseline
files and 46 patch payloads. Accepted report/source hashes, source snapshots and
the previous date slice's final acceptance identities were rechecked.

Generated binaries, reports, maps and source snapshots stay local and ignored.
There is no Git repository; no commit, remote, PR or publication was created.
