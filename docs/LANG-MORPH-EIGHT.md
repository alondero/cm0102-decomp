# Numeric language eight: articles, dictionary and suffix recovery

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This note records the historical eight-stage acceptance: 18,823 comparisons
and 6,154 parent bytes. The subsequent [language-six extension](LANG-MORPH-SIX.md)
retains that acceptance in history; six-stage source covers 7,287 bytes and passes
22,202 comparisons per build. The later [two extension](LANG-MORPH-TWO.md) raised
two-stage acceptance to 13,909 bytes and 67,707 comparisons. Transfers and current-stage descriptions below
refer to the eight-stage artifact.
The subsequent [four extension](LANG-MORPH-FOUR.md) retains all earlier
acceptance and original watch scopes in history; current [language-nine source](LANG-MORPH-NINE.md) passes 224,181 comparisons,
covers 49,588 parent bytes and freezes thirteen inputs.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
Addresses are preferred-image **VAs**, image base `0x00400000`.
Parent entry is `0x006701F0`; the comparison for numeric language eight is
`0x00670E2F`, and its body starts at `0x00670E38`.

The [partial morphology module](LANG-MORPH-PARTIAL.md) now compiles languages
zero, seven, ten and eight, initialization and shared composition. Eight adds
**2,738 original parent instruction bytes**: the 2,707-byte range
`0x00670E2F..0x006718C1`, the 26-byte shared copy block
`0x0067FD17..0x0067FD30`, and its five-byte continuation
`0x0067FD5C..0x0067FD60`. Current coverage is **6,154 bytes in eleven regions**.
The intervening `0x0067FD31..0x0067FD5B` appears in the static context listing but
is not claimed as recovered. The existing 107 manually decoded bytes omitted
from Ghidra are unchanged.

The parent remains identified but not fully decompiled, compiled, validated or
integrated. The eight full-routine ledgers still contain 44 application routines
and one separate CRT search routine. This extension contributes partial regions,
not a new fully recovered routine or a runnable standalone game.

## Prefix and property contracts

Frame base is `P=entryESP-0x2818`. Local word is at `P+0x1018`, prefix at
`P+0x818`, suffix at `P+0x18`, and zeroed scratch at `P+0x1818`.
Incoming argument slots and partial initialization follow
[the measured frame](LANG-MORPHOLOGY.md).

| Exact prefix | Recovered behavior |
| --- | --- |
| `upper` | Convert first local byte, then common composition; ignore suffix |
| `lower` | Convert all local bytes except exact `FIFA`, `UEFA`, `CONCACAF`; converted words go to common composition, exceptions proceed to suffix handling |
| `En`, `en` | Copy the selected article into the prefix buffer, then common composition |
| `En_lower`, `en_lower` | Select the article, lower every local byte including acronym exceptions, then common composition |
| `upper_the 1`, `upper_the 2`, `upper_the 3` | Select dictionary column one/two/three, convert first byte even after a miss or empty replacement, then common composition |
| `the 1`, `the 2`, `the 3` | Select dictionary column one/two/three, then common composition |
| Other prefixes | Process the suffix argument |

Articles depend only on the **low byte of the first property dword** at
`P+0x2828`: zero selects `En ` or `en `; any nonzero byte selects `Et ` or
`et `. High bytes do not affect selection. The second property dword at
`P+0x282C` is unused by this body. Dictionary columns instead come from the
exact prefix; they do not depend on these incoming property arguments.

Character dependencies receive a sign-extended byte as a full int at original
CRT addresses `0x009471CD` (upper) and `0x009465FC` (lower). Only return AL is
stored. The lower loop tests the following byte, so a conversion that writes
NUL does not itself stop conversion of subsequent bytes. These calls remain
fixtures, with no claim of actual CRT locale behavior.

## Compiled table-helper dependency

All six dictionary prefixes use the original four-pointer dictionary at
`0x009EB328`: 41 records of key plus three replacement pointers and one terminal
key pointer, **660 bytes**. Empty-key guard, first matching row, unsigned byte
comparison, replacement measurement including NUL and forward dword/byte copy
are preserved. The uppercase-prefix paths originally scan inline; the other
paths call `0x006820A0`, with actual CALL sites at `0x00671485` and
`0x006714D4`.

Both forms now use the already recovered `cm_lang_morph_table_a`. Its C/header
remain byte-identical to their separately accepted sources. The new harness
links that implementation and rejects original helper execution for recovered
languages. Its **171 original instruction bytes** are pinned separately as
support evidence; they are already included in full-routine acceptance and are
not added to the 6,154-byte partial-parent count. Original table/string data is
still required; closing this code dependency does not relocate those assets.

## Suffix behavior and return ordering

For an unhandled prefix, suffix `ens` checks local word length. Only lengths
**greater than four** can match. It compares the final four bytes with exact
` Cup`, then exact ` cup`; mixed or uppercase spellings do not match. A match
formats local word with `%sens` at `0x009F14FC` into scratch, measures scratch
again including NUL, and copies it back to the local word in dwords then bytes.
It then uses common composition. Thus the word ` Cup` itself is unchanged,
whereas `A Cup` becomes `A Cupens`. Other suffixes except `s`/`S` leave the
word unchanged and use common composition.

Suffix `s` or `S` uses the same instruction-derived ending algorithm as
[language ten](LANG-MORPH-TEN.md), with different original data. The list at
`0x009EB2F8` is six pointers (24 bytes): ` 'B'`,
` B Tropp Reservlag Reservtropp Tropp U21`, ` U21 Tropp`, ` Under 21`,
` Under 21 trupp`, then `*`. First selected ending wins; `*` terminates.
The stem selectors/targets at `0x0067FEAC`/`0x0067FEA4` and word
selectors/targets at `0x0067FEDC`/`0x0067FED4` are byte-identical to ten's
corresponding selector/target data. The ending lists are different.

Candidate matching uses **caller word + local word length - ending length**,
with 32-bit arithmetic. Only equal lengths skip comparison; short words retain
the original subtraction into preceding mapped bytes. A match records the
corresponding pointer into the local word. Repeated measurements, next-sentinel
comparison, stem-copy reads and first-match ordering remain explicit.
Signed last-byte subtraction by `0x53`, followed by an unsigned bound of 39,
selects original dispatch data. Depending on that data the direct formatter
uses whole word, word plus suffix, or stem plus suffix plus selected ending.
Empty word/stem can return full EAX=zero.

These ending formatters return directly, bypassing the common post-format
language reread. Article, dictionary, conversion and `ens` paths use common
composition, which rereads `0x009EB2DC` and applies language-two contraction if
the formatting fixture changed it. The `ens` scratch-format call can also
change language or shared output before the subsequent common formatter.

## Differential acceptance and history

`CM0102/Analysis/morph-eight-final-a-O0/report.json` and
`morph-eight-final-a-O2/report.json` each pass **18,823 comparisons**,
`quick=false`. GCC 15.2.0/binutils 2.45 build freestanding 32-bit x86 with warnings
as errors and the explicit flags recorded in the reports. Nine tested inputs
are frozen under `CM0102/Analysis/morph-partial-source-used-eight/`; these include
the unchanged table-helper C/header. Python 3.13.12, Unicorn 2.1.4, Capstone
5.0.7, commands, tool hashes, build outputs and static exports are pinned.

The preceding 8,949 inputs are retained, with the former language-eight neutral
case moved from residual transfers into its own recovered group. **9,874 new
eight-language comparisons** are added per build:

| Group | Comparisons |
| --- | ---: |
| Original dictionary keys, misses/empty/acronyms, 13 prefixes, five suffixes | 2,990 |
| All article property bytes, four prefixes, three high-byte patterns | 3,072 |
| All first bytes across conversion/article-lower paths | 1,024 |
| All last bytes with no ending and each five original endings | 1,536 |
| Exact-ending short words and varied stems | 60 |
| `ens` boundaries and spelling variants | 55 |
| `ens` lengths through 2,044 input bytes | 36 |
| Synthetic ending lists | 24 |
| Synthetic dictionary guards, duplicates and empty replacements | 24 |
| Full character-return bits | 20 |
| Post-format language changes | 21 |
| Language mutation during scratch/final `ens` formatting | 2 |
| Nonempty shared output created during `ens` formatting | 2 |
| Suffix-slot mutation during character conversion | 4 |
| Shared-output aliases | 4 |
| Seeded eight-language inputs | 1,000 |

Comparisons check full EAX, cdecl cleanup, ESP, callee-saved registers, restored
FS head, incoming argument bytes, watched frame/global state and all sixteen
table regions, plus ordered CRT events with arguments and boundary state.
Frame bytes `P+0x10..P+0x2813` include other uninitialized private-buffer bytes.
The relocated probe return-address dword at `P+0x2814` is explicitly excluded.
A complete ordered memory-read/write trace is not compared by this parent
harness. The helper's separate full-routine corpus retains its stronger access
trace contract.

For languages zero/seven/ten/eight, recovered execution permits only the original
stack probe and four explicit fixture addresses; it cannot reuse original
parent/helper instructions. Fourteen neutral cases exercise explicit remaining
language transfers at `0x006718C2`, with one neutral dispatch each for recovered
ten/eight. They do not validate general unfinished-language behavior.
Formatting fixtures support literals, one/two/three `%s` fields and `%sens`.
Acceptance requires valid terminated strings, mapped preceding memory for
short-word tests, bounded buffers, DF clear and normal return. Actual CRT errors,
faults, concurrency, all aliases, overflow and Windows guard-page/SEH behavior
remain outside the finite contract.

Revision three of `recovery/lang/morph-partial-ledger.json` pins this pair and
the nine-source snapshot. Before editing, revision two ledger/verifier/docs were
archived in `morph-eight-prior-state`. Its nested history retains revision one
in `morph-ten-prior-state`. The old 8,949- and 3,505-case report pairs and their
seven-source snapshots are unchanged. `scripts/verify_morph_partial.py` checks
both historical versions against frozen sources and current reports against
current/frozen sources, as well as primary bytes, artifacts, tools, regions,
tables, separate helper acceptance and the eight unchanged full-routine ledgers.
`CM0102/Analysis/morph-eight-acceptance-audit-a/audit.json` passes these checks.
The quick smoke report is diagnostic evidence only.

Static evidence is `CM0102/Analysis/morph-eight-static-a` and
`morph-eight-data-a`. Adjacent inspection scripts record commands, script/tool
versions and input/output hashes. No preserved executable, asset, old report or
frozen source snapshot was modified. Analysis artifacts remain local and ignored.
Fresh-output reproduction commands are in LANG-MORPH-PARTIAL.md.

Final Standards review found no hard violations or actionable smells. Final Spec
review independently verified the current/historical report identities, frozen
sources, regional/helper byte accounting and full-ledger preservation. Its one
documentation ambiguity about a historical failed audit was corrected. Python
syntax and both warnings-as-errors builds pass. Preservation verification passes
all 1,558 baseline files and 46 official patch payloads. Existing runtime files
are reported separately. `git status` confirms this workspace is not a Git
repository; no commit, push or PR was made.

Language nine has since been accepted; next recover language five
at `0x0067C2EE` using its isolated candidate. See
[its isolated native candidate](LANG-MORPH-FIVE-DISCOVERY.md) before extending
the maintained source and combined corpus. This note retains the earlier
artifact's dispatch, case counts and watch contract. M2/M4/M6 remain open.
