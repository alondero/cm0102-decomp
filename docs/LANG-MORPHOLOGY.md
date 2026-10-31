# Morphology table helpers and original frame contract

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All executable and table addresses below are **preferred-image VAs**, base
`0x00400000`. Incoming ESP is `E`; the morphology saved-register base is `P`.

`recovery/lang/cm_lang_morph_tables.c` and `.h` recover two application
routines, totaling **342 original instruction bytes**. Both full O0/O2 runs
pass **164,070 differential comparisons each**. Independent statuses, original
byte identities and accepted report/source hashes are in
`recovery/lang/morph-tables-ledger.json`. The preserved nine-ledger selection contains **45 compiled application routines
and one separate CRT search routine**. The preserved [eleven-ledger support map](CRT-SEARCH.md)
adds compiler and string support, for 45 application entries plus four CRT entries.
The current [twelve-ledger map](CRT-CLASSIFY.md) adds maintained classification,
for 45 application plus five CRT entries,16,337 calls and three tails.
No recovered code is integrated.

The [full morphology application body](LANG-MORPH-PARENT.md) at `0x006701F0`
now passes 323,545 comparisons per O0/O2 build, covering all 64,465 instruction
bytes. The preserved prior stage names the original stack probe. [Maintained probe support](STACK-PROBE.md)
is now separately linked and validated by [current relink acceptance](LANG-MORPH-RELINKED.md). The
[partial source note](LANG-MORPH-PARTIAL.md) remains historical evidence.
The frame contract below is retained by the full maintained source.

## Table replacement contracts

| Original extent, inclusive | Recovered interface | Instruction/hashed bytes | Original table |
| --- | --- | ---: | --- |
| `0x006820A0..0x0068214A` | `cm_lang_morph_table_a(word, property)` | 171 | `0x009EB328` |
| `0x00682150..0x006821FA` | `cm_lang_morph_table_b(word, property)` | 171 | `0x009EB7AC` |

Both use cdecl, preserve callee-saved registers and caller argument slots, and
return full incidental EAX. Each table contains **41 four-pointer records**:
key plus three replacement pointers. Only the next record's first pointer is
needed for termination; the pointed string must begin with NUL. Both original
sentinels point to `0x00AD9C64`. Hash-locked table extents are 660 bytes each:
41 records times sixteen bytes plus that terminal pointer. Original byte
strings, pointer lists and table hashes are recorded in each accepted run's
`original-tables.json`. These are local ignored evidence, not relocated source
data. The source still depends explicitly on the original tables/strings.

The first key's empty-string guard runs before reading the caller string. A
nonempty key is compared to the caller with unsigned bytes, two byte positions
per loop. A first-position mismatch stops before reading the second position;
a matching NUL stops without reading beyond it. Each subsequent record gets
the same empty-key guard. The **first matching record** wins. A later duplicate
is not consulted. An empty key is a terminator, never a matching dictionary key.

Only the low property byte matters. Values one, two and three select the
corresponding replacement pointer; other values leave the destination untouched.
The replacement is measured before any write, including its NUL, then copied
forward in dwords followed by remaining bytes. There is no allocation, Unicode
conversion, case folding, length bound or CRT call. Empty replacement writes
one NUL byte. Unaligned dwords and selected overlapping source/destination
ranges preserve the original access sizes and order; replacing this with
memmove would change some observed cases.

Return values differ by exit:

| Exit | Full EAX |
| --- | --- |
| Empty initial key or unsuccessful search | Exact pointer to the terminating empty string |
| Match, unsupported property | Low property byte, zero extended |
| Match, property one/two/three | Copied byte count including NUL |

The generated pseudocode's void signature loses this incidental contract. The
two original callers per helper do not justify normalizing these bits to a
boolean. Four verified direct call sites in morphology are `0x00671485` and
`0x006714D4` to helper A, and `0x00674D54` and `0x00674DB1` to helper B. Their
surrounding instructions pass property two/three and three/one respectively.
Language names and grammar rules are not inferred from translated table text.

## Differential corpus and accepted sources

| Accepted report under `CM0102/Analysis/` | Build | Comparisons |
| --- | --- | ---: |
| `morph-tables-final-a-O2/report.json` | O2 | 164,070 |
| `morph-tables-final-a-O0/report.json` | O0 | 164,070 |

Each pair compares full EAX, ESP, callee-saved registers, restored FS head,
caller argument bytes, destination arena, watched pointer tables, affected
original string regions, and **separate exact ordered read and write traces**
with address, width and value/bytes. The harness does not merge those two traces
into a single interleaving. Both helpers run without CRT/application
hooks; the recovered side rejects execution of any original text. Original
table/string destination aliases alter emulator memory only, and are restored
between cases. Baseline files and static exports remain unchanged.

The full corpus covers 20,992 original-record/property-byte combinations;
131,072 exhaustive property low-word cases; all 256 unsigned first bytes for
each helper; 1,400 proper key prefixes; 328 key variations; 328 original string
destination aliases; empty-table guards with an unmapped caller pointer;
duplicate-first-match and early-empty-key cases; 4,112 replacement length and
alignment cases; 4,290 partial source/destination overlaps; and 1,000 seeded
original-key cases. Replacement lengths cover zero through 513, all destination
alignments modulo four, and overlap offsets minus sixteen through sixteen.
The skipped-caller guard cases do not establish general invalid-pointer safety.

Six tested source files are frozen under
`CM0102/Analysis/morph-tables-source-used/`, with report/source pins in its
manifest. Reports record GCC 15.2.0/binutils 2.45 identities, Python, Unicorn
2.1.4, Capstone 5.0.7, build commands and artifact hashes. Builds are freestanding
32-bit x86 with warnings as errors and explicit aliasing/stack flags.

```powershell
& py -3.13 recovery/lang/test_cm_lang_morph_tables.py --optimization O2 --output CM0102/Analysis/morph-tables-check-O2
& py -3.13 recovery/lang/test_cm_lang_morph_tables.py --optimization O0 --output CM0102/Analysis/morph-tables-check-O0
```

Output directories must be new. `--quick` samples property low words, original
properties, lengths, overlaps and random cases; a quick PASS is not accepted
full-corpus evidence. Every call must return within 200,000 instructions.
Acceptance is finite, DF clear, normal return, valid terminated tables/selected
strings. Invalid selected pointers, concurrency, exceptions, unbounded scans,
address wrap and all possible overlapping table layouts remain outside scope.

## The larger routine's stack probe and five arguments

Read-only export `CM0102/Analysis/morph-probe-static-a/run.json` pins Ghidra,
Java, commands, input and output hashes for the 47-byte probe at `0x00945610`.
The original morphology export remains `lang-loader-static-b`. The frame
inspector checks all **17,394 morphology instruction records, 49,771 bytes**,
and the probe against primary bytes and single-instruction Capstone decoding.
Those counts do not imply a contiguous function extent: embedded dispatch data
and excluded ranges occupy part of its address span.

Morphology loads EAX=`0x2808`, then CALLs the probe at `0x006701F5`. The probe
pushes incoming ECX, calculates the caller's pre-CALL ESP, probes dwords at
`E-0x1000`, `E-0x2000` and `E-0x2808`, relocates the CALL return address below
the new frame and returns to `0x006701FA`. At that point ESP=`E-0x2808`, ECX is
restored and EAX contains the return VA. This is not an ordinary C function with
an unchanged stack pointer. Subsequent PUSH EBX/EBP/ESI/EDI gives
**`P = E-0x2818`**.

| Argument | Offset from P | Original incoming slot |
| --- | --- | --- |
| Word pointer | `+0x281C` | `E+4` |
| Prefix pointer | `+0x2820` | `E+8` |
| Suffix pointer | `+0x2824` | `E+12` |
| First property dword | `+0x2828` | `E+16` |
| Second property dword | `+0x282C` | `E+20` |

The generated two-parameter signature and synthetic `in_stack` names are
misleading. These positions follow actual probe/register-save execution and
the known wrapper's five-argument call, not an assumed host ABI.

Initialization zeroes 2,048 bytes at `P+0x1818`, clears only the first bytes at
`P+0x18` and `P+0x818`, clears shared output byte `0x00B64804`, and copies the
word including NUL to `P+0x1018` in dwords then bytes. Dwords `P+0x10`/`P+0x14`
become zero/one. Other prior frame bytes, including storage at `P+0x2018`, are
not blanket initialized. Clearing shared output precedes scanning the word;
when the word aliases shared output, only the new NUL is copied.

The word's SCASB and copy precede the null test at `0x0067025B`. An unmapped
null source faults at SCASB `0x00670235`; a later nominal empty-string return
does not make null safe. The frame inspector demonstrates that original order
without adding a guard or substituting source behavior.

`CM0102/Analysis/morph-frame-final-a/` and `morph-frame-final-b/` each reproduce
8,193 normal prefix observations: every input length zero through 2,047 at four
source alignments, plus the shared-output alias. They compare the entire
10,264-byte saved-register/frame region to the instruction-derived expected
layout, verify unchanged five argument slots and the exact three probe reads,
and separately reproduce the unmapped-null fault. Execution stops at
`0x00670271`, before the language read. This is original-prefix observation,
not a compiled morphology comparison, Windows guard-page/SEH proof or a
whole-routine acceptance result.

```powershell
& py -3.13 scripts/map_recovery.py --output CM0102/Analysis/morph-probe-check --functions 00945610
& py -3.13 scripts/inspect_morphology_frame.py --output CM0102/Analysis/morph-frame-check
```

The inspector uses the accepted static export names and checks their artifacts.
No supplied executable is patched. Shared final composition was original code
in this frame experiment; it is now separately compiled in the partial slice:
`0x0067FD61` tests whether shared output already has text, `0x00670974`
formats into it otherwise, and `0x0067099B` rereads current language for the
language-two postprocessing. Initial dispatch reads current language at
`0x00670271`. Preserve this reread across formatting dependencies when porting.

## Combined acceptance and handoff

`CM0102/Analysis/morph-tables-map-final-a/` and `morph-tables-map-final-b/`
merge eight ledgers: 28 Date-family and seventeen support entries, 44 application
routines plus one separate CRT search routine. The five map artifacts are
byte-identical. They check 16,074 actual calls and the same three separate direct
tail transfers. All 45 selected entries are validated within their individual
finite contracts; none integrated. Morphology/probe prefix evidence is outside
this compiled selection.

The local `CM0102/Analysis/morph-tables-acceptance-audit.json` checks twenty
accepted/historical reports, sixteen reports pinning current sources, 180 source
identities and 154 artifact identities, original function bytes, both maps and
six frozen helper sources. It also verifies both frame reports, all their
sources/exports/artifacts, and byte-identical instruction/observation outputs.
The previous seven ledgers and accepted application source remain unchanged.
Python syntax and warnings-as-errors C builds pass. Executable and analysis-copy
hashes remain pinned. Analysis evidence remains local and ignored; this workspace
has no Git repository, and no commit, push or PR was made.
Independent Standards and Spec reviews found no implementation defects and
verified final source/report/map/frame identities. Review clarified that the
read and write traces are separately ordered, without a merged interleaving claim.

```powershell
& py -3.13 scripts/map_date_family.py --output CM0102/Analysis/morph-tables-map-check-a
& py -3.13 scripts/map_date_family.py --output CM0102/Analysis/morph-tables-map-check-b
```

The [full parent application body](LANG-MORPH-PARENT.md) is now accepted
separately: 64,465 bytes, all language dispatches and unknown-language paths,
323,545 comparisons per O0/O2 build. The earlier eight-ledger map and partial
ledger remain unchanged historical artifacts. The preserved nine-ledger map uses
`scripts/map_morphology_family.py`. In that preserved stage, original probe/data
and explicit CRT fixtures remain dependencies. The current composition instead uses
recovered probe/search and `scripts/map_morph_search_family.py`.
M2 replay, primary/GDI mapping, M4 integration and M6
standalone remain open.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/LANG-MORPH-PARENT.md and
> recovery/lang/morph-ledger.json. Verify scripts/verify_morph_parent.py with a
> fresh output folder. Preserve the measured five-argument frame, partial buffer
> initialization, original null-read-before-guard behavior, dynamic language reads,
> qualifier aliases and grouped copy order. Read docs/LANG-MORPH-RELINKED.md:
> the recovered probe is accepted and linked after full O0/O2 validation.
> Verify scripts/verify_morph_search.py and scripts/map_morph_search_family.py
> in fresh folders. Search is composed; review/adopt locale candidates,
> recover deeper CRT dependencies and relocate data, then validate the end-to-end
> localization chain. Search composition is now accepted; read
> docs/LANG-MORPH-SEARCH.md and verify its seventeen-source ledger.
> No live integration or standalone build is established.
