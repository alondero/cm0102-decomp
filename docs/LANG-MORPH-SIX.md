# Numeric language six: conversion and ending dispatch

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This note records the historical six-stage acceptance: 22,202 comparisons,
7,287 parent bytes and nine frozen inputs. The subsequent [two extension](LANG-MORPH-TWO.md)
retains it in history; two-stage source passes 67,707 comparisons, covers 13,909
parent bytes and freezes ten inputs. Transfers/watch contracts below describe
the six-stage artifact.
The subsequent [four extension](LANG-MORPH-FOUR.md) retains all earlier
acceptance and original watch scopes in history; current [language-nine source](LANG-MORPH-NINE.md) passes 224,181 comparisons,
covers 49,588 parent bytes and freezes thirteen inputs.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
All addresses are preferred-image **VAs**, image base `0x00400000`.
The incomplete parent starts at `0x006701F0`. Its language-six comparison is
`0x006718C2`, body `0x006718CB`, and next comparison `0x00671DE8`.

Language six is now compiled in `recovery/lang/cm_lang_morph_partial.c/.S/.h`.
It adds **1,133 parent instruction bytes**, with no new manual decoding outside
the existing Ghidra instruction inventory:

| New inclusive region | Bytes | Behavior |
| --- | ---: | --- |
| `0x006718C2..0x00671909` | 72 | Dispatch and first prefix comparison |
| `0x00671927..0x00671D02` | 988 | Lowercase, suffix and ending selection/stem dispatch |
| `0x00671D43..0x00671D8B` | 73 | No-selected-ending length and dispatch |

The shared uppercase block `0x0067190A..0x00671926` and direct format/return
blocks `0x00671D03..0x00671D42`, `0x00671D8C..0x00671DE7` were already accepted
in earlier partial stages. They are not counted twice. Current partial coverage
is **7,287 bytes in fourteen regions**, including the unchanged 107 manually
decoded bytes omitted from the original export. The full parent remains
identified but not fully decompiled, compiled, validated or integrated. Full
selection remains 44 application routines plus one separate CRT search routine.

## Instruction-derived paths

The measured frame remains `P=entryESP-0x2818`, with local word at `P+0x1018`,
scratch at `P+0x1818`, suffix at `P+0x18` and prefix at `P+0x818`. Selective
initialization, original stack-probe allocation, five incoming argument slots
and shared-output clearing before the word scan are unchanged. See
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md).

Exact prefix `upper` converts the first local byte and uses common composition,
ignoring suffix. Exact `lower` converts every byte unless the local word equals
`FIFA`, `UEFA` or `CONCACAF`. Converted words use common composition; those three
exceptions fall through to suffix handling. Other prefixes also process suffix.
Conversion receives a sign-extended byte at original CRT fixture addresses
`0x009471CD`/`0x009465FC`; only AL is stored. Lower conversion tests the following
local byte, preserving later conversions even if a callback writes NUL.
Neither incoming property dword is read by the six-language worker.

Exact suffix `e` copies literal `0x0099C360` including NUL to the suffix buffer
in dwords then bytes, then uses common composition. The source argument itself
is not substituted for that literal. Other suffixes besides `s`/`S` use common
composition without modifying the suffix buffer. Empty local word with `e`
therefore still formats through common composition; `s`/`S` can return EAX=zero.

Suffix `s` or `S` uses the instruction-derived ending-selection loop shared with
[language ten](LANG-MORPH-TEN.md) and [eight](LANG-MORPH-EIGHT.md). Six's six-pointer
list at `0x009EB310` is byte-identical to eight's pointer list at `0x009EB2F8`:
` 'B'`, ` B Tropp Reservlag Reservtropp Tropp U21`, ` U21 Tropp`, ` Under 21`,
` Under 21 trupp`, then `*`. The 24-byte lists are pinned separately and remain
original-image data dependencies.

The loop preserves first selection, `*` termination and repeated string
measurements. Candidate comparison reads **caller word + local word length -
ending length** using 32-bit arithmetic, not the local transformed bytes.
Equal lengths skip comparison; shorter words retain subtraction into preceding
mapped bytes. A match stores the corresponding pointer into the local word at
frame dword `P+0x10`. Stem copying tests remaining local text against that
selected ending and copies bytes into the zeroed scratch. Empty word/stem
returns full EAX=zero.

Six has an essential dispatch difference that must not be generalized away:

| Path | Signed last byte minus | Unsigned upper bound | Selector extent | Two-target table |
| --- | --- | ---: | --- | --- |
| Selected-ending stem | `0x53` | 39 | `0x0067FF0C`, 40 bytes | `0x0067FF04`, 8 bytes |
| No selected ending | `0x43` | 55 | `0x0067FF3C`, 56 bytes | `0x0067FF34`, 8 bytes |

Languages ten/eight use `0x53` and 39 for both paths. Six's word path instead
covers the wider original selector span beginning at `0x43`. The shared source
loop now takes the verified word base/bound explicitly while keeping the stem
base/bound unchanged. Selector zero formats whole local word with `%s`; other
targets format word+suffix or scratch stem+suffix+selected ending. The fixed
original table bytes determine selection, not an inferred grammar rule.

All ending formatters return directly to the caller, bypassing common
composition's language reread and contraction. Prefix conversion and `e` paths
use common composition, which rereads current language after formatting.
Callback mutation of language, shared output and suffix slots remains tested;
general concurrent mutation is not established.

## Acceptance and reproducibility

`CM0102/Analysis/morph-six-final-a-O0/report.json` and
`morph-six-final-a-O2/report.json` each pass **22,202 comparisons**, `quick=false`.
Both retain the previous 18,823 inputs; the prior neutral six-language residual
case is now named as recovered dispatch. Thirteen neutral original transfers
remain, with recovered neutral groups for ten, eight and six.
The **3,379 added six-language cases** per build are:

| Group | Comparisons |
| --- | ---: |
| Empty/basic/acronym/ending words, four prefixes, five suffixes | 180 |
| All first bytes with upper/lower | 512 |
| All last bytes with no ending and each original ending | 1,536 |
| Exact-ending short words and varied stems, both suffix cases | 60 |
| Synthetic ending lists: duplicates, empty, whole-word, early `*` | 24 |
| Full character-return bits | 8 |
| Dynamic language changes on common/direct paths | 12 |
| Word lengths through 2,042 bytes | 32 |
| Ignored property dwords | 9 |
| Shared-output aliases | 3 |
| Suffix-slot mutation during conversion | 2 |
| Callback-created nonempty shared output | 1 |
| Seeded six-language inputs | 1,000 |

Full EAX, cdecl cleanup, ESP, callee-saved registers, restored FS head, argument
bytes, watched frame/global state, twenty-one table regions and ordered CRT
events/arguments/boundary state are compared. Frame bytes `P+0x10..P+0x2813`
include uninitialized storage; the probe's relocated return-address cell at
`P+0x2814` is excluded. This harness does not compare a complete ordered memory
access trace. Known languages cannot execute original parent/helper code; only
the original probe and four CRT fixture addresses are permitted. Remaining
languages transfer explicitly at `0x00671DE8` and may use original common blocks.

Nine tested inputs are frozen under `morph-partial-source-used-six`. GCC
15.2.0/binutils 2.45 builds are freestanding 32-bit x86 with warnings as errors;
Python 3.13.12, Unicorn 2.1.4, Capstone 5.0.7, source/tool/export/artifact hashes
and full commands are recorded. Revision four of the separate partial ledger
pins this pair. The read-only verifier's passing audit is
`CM0102/Analysis/morph-six-acceptance-audit-a/audit.json`.
It verifies primary bytes, all regions/tables, separate compiled helper-A
evidence, current/frozen source, and the eight unchanged full-routine ledgers.
Helper A remains unchanged support already accounted as a full routine; its
171 bytes do not increase the partial-parent byte count.

`morph-six-prior-state` archives the accepted revision-three ledger/verifier/docs
before modification. The verifier recursively checks the three earlier pairs
(18,823, 8,949 and 3,505 cases per build) against their frozen inputs, preserving
their manifests and reports. Earlier acceptance audits remain historical.
The quick smoke run passes 18,108 sampled comparisons but is diagnostic only.

Static evidence is `morph-six-static-a`, `morph-six-data-a` and
`morph-six-next-ladder-a`. `scripts/inspect_morphology_region.py` checks the pinned
primary/analysis-copy identities and prior export artifacts, decodes the whole
requested region and records instruction/literal-byte listings with commands,
versions, source and output hashes. Its literal listing is an address heuristic;
pointer-array addresses can appear as raw bytes and must not be interpreted as
text without inspecting their data type. `CM0102/Analysis/prepare-morph-six.py`
records the archive and separately verified table/ending data.
Fresh-output reproduction commands are in LANG-MORPH-PARTIAL.md.

Acceptance remains finite: valid terminated strings, mapped preceding bytes for
short-word cases, bounded buffers, DF clear and normal return. CRT locale/errors,
faults, concurrency, all aliases, overflow and Windows guard-page/SEH behavior
remain outside scope. No supplied game file or existing acceptance snapshot is
modified. Preservation verification passes all 1,558 baseline files and 46
official patch payloads. Analysis artifacts remain local and ignored.

Final Standards review found no hard violations or actionable smells. Final Spec
review independently recomputed both accepted reports, nine current/frozen input
pins, all three historical versions, byte accounting, separate helper evidence
and eight unchanged full-routine ledgers. No material fidelity or acceptance
mismatch remains. Review clarified the historical eight-stage count in the ten
note. Python syntax and both warnings-as-errors builds pass. `git status` confirms
this workspace has no Git repository; no commit, push or PR was made.

Language nine has since been accepted; next recover language five
at `0x0067C2EE` using its isolated candidate. See
[its isolated native candidate](LANG-MORPH-FIVE-DISCOVERY.md) before extending
the maintained source and combined corpus. This note retains the earlier
artifact's dispatch, case counts and watch contract. M2/M4/M6 remain open.
