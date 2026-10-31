# Numeric language ten: dictionary and suffix recovery

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This note records the historical ten-stage acceptance (8,949 comparisons,
3,416 parent bytes, seven frozen sources). The subsequent
[language-eight extension](LANG-MORPH-EIGHT.md) retains these inputs and sources
in history; eight-stage acceptance was 18,823 comparisons, 6,154 parent bytes and
nine frozen inputs at the eight stage. The subsequent [six extension](LANG-MORPH-SIX.md)
raised six-stage acceptance to 22,202 comparisons and 7,287 parent bytes.
The subsequent [two extension](LANG-MORPH-TWO.md) raised two-stage acceptance to
67,707 comparisons and 13,909 parent bytes. Earlier original transfers mentioned below describe the
ten-stage artifact, not the current recovered entry.
The subsequent [four extension](LANG-MORPH-FOUR.md) retains all earlier
acceptance and original watch scopes in history; current [language-nine source](LANG-MORPH-NINE.md) passes 224,181 comparisons,
covers 49,588 parent bytes and freezes thirteen inputs.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
All addresses are preferred-image **VAs**, image base `0x00400000`.
This extends [the partial morphology source](LANG-MORPH-PARTIAL.md), parent
`0x006701F0`; numeric language ten starts at `0x006705B1`, following its comparison
at `0x006705A8`. No human language identity is required to recover these branches.

The same C/assembly module now compiles all reachable ten-language application
paths, adding **2,171 original instruction bytes** in five regions. The partial
selection totals **3,416 bytes** in eight regions. Parent statuses remain
identified=true, decompiled/compiled/validated/integrated=false: the other language
bodies remain original code. These regional flags do not add a fully recovered
function to the existing 44-application/one-CRT selection.

## Prefix paths and data contracts

The frame remains `P=entryESP-0x2818`, with word copy at `P+0x1018`, zeroed scratch
at `P+0x1818`, suffix buffer at `P+0x18` and prefix buffer at `P+0x818`.
Prefix is captured from incoming argument slot `P+0x2820`.

| Exact prefix | Original behavior |
| --- | --- |
| `upper` | Convert the first local word byte, then common composition; suffix is ignored |
| `lower` | Convert every byte unless word equals `FIFA`, `UEFA` or `CONCACAF`; converted/empty words go to common composition |
| `upper_the 1` / `upper_the 2` | Select dictionary column one/two, convert first byte even on dictionary miss, then common composition |
| `the 1` / `the 2` | Select dictionary column one/two, then common composition |
| Other prefixes, including the three `lower` exceptions | Read and process suffix argument |

Conversion dependencies are original CRT-address `0x009471CD` (upper) and
`0x009465FC` (lower). Both receive a **sign-extended byte as a full int**; only
return AL is stored. A lower conversion that returns zero does not stop the
loop by itself: original code next reads the following local byte to decide
whether to continue. The fixtures test zero, positive full-bit and negative
full-bit returns without claiming production locale behavior.

The dictionary at `0x009EB5BC` has **41 three-pointer records**, key and two
replacements, followed by a terminating key pointer. Its pinned extent is
`41*12+4 = 496` bytes. The first empty key terminates the search before comparing
the word. First matching row wins. Replacement is measured including its NUL,
then copied to the local word in dwords followed by bytes. No match leaves the
word unchanged. This is distinct from the previously recovered four-pointer
helpers at `0x006820A0` and `0x00682150`.

## Suffix paths and caller-byte comparison

On an unhandled prefix, suffix `en` or `a` is copied from the corresponding
original literal into `P+0x18`, including NUL, then common composition runs.
Unrecognized suffix likewise goes to common composition without a suffix copy.
Suffix `s` or `S` enters ending selection instead.

Ending pointers begin at `0x009EB2E0`. The original six-pointer list is five
byte strings plus `*`: ` B`, ` B Trupp Reservlag Reservtrupp Trupp U21`,
` U-21 trupp`, ` Under 21`, ` Under 21 trupp`, `*`. Comparison to `*`, rather
than an empty-key test, terminates this list. The accepted reports pin its
24 pointer bytes; literal data remains an explicit original-image dependency.

Selection preserves several less obvious instruction contracts:

- If local word length equals ending length, that candidate is skipped.
- Otherwise the comparison address is **caller-word pointer + local-word length
  - ending length**, using 32-bit unsigned arithmetic. It reads the caller's
  original bytes, not the transformed local copy. There is no greater-than-length
  guard: a short word can address preceding mapped bytes. The tested cases retain
  this subtraction and require mapped preceding memory, rather than adding a
  safety rule absent from the executable.
- A match stores **local-word pointer + local-word length - ending length** in
  frame dword `P+0x10`. Repeated measurements and the next sentinel comparison
  retain original ordering; the first selected candidate stops later selection.
- With that pointer set, original code copies the stem byte by byte to the
  zeroed scratch buffer until the remaining local word equals the selected
  ending, or NUL is reached. A zero-length stem returns full EAX=zero.

For a nonempty stem, the signed last stem byte minus `0x53`, with unsigned upper
bound 39, indexes the selector at `0x0067FE4C`. Its two-target table is at
`0x0067FE44`. Selector zero formats the **whole local word** with `%s` at
`0x00991944`, leaving it unchanged. The other target formats `%s%s%s` at
`0x009F1584` using scratch stem, current suffix argument and selected local ending.
The fixed table, rather than a newly invented character rule, determines the path.

With no selected ending, an empty local word returns zero. Otherwise an analogous
signed last-word byte index uses selectors `0x0067FE7C` and targets `0x0067FE74`.
Selector zero again formats the word with `%s`; the other target formats word and
current suffix argument using `%s%s` at `0x0099EC04`.

All these suffix formatters return `0x00B64804` **directly**. They bypass common
composition's current-language reread and contraction. For example, a formatting
fixture that changes language to two triggers contraction on common paths but
does not trigger it on these direct returns. The two property dwords are not
used by this language body.

## Reachable blocks missing from the export

The verified selector targets reveal two additional ranges absent from the
original Ghidra instruction body: `0x00671D03..0x00671D0F` (13 bytes) and
`0x00671D8C..0x00671DB5` (42 bytes). Both lead to the direct `%s` formatter.
The harness decodes all 55 bytes from primary instructions and records export
membership individually. Combined with the earlier 52 vowel-block bytes, the
partial slice now has **107 manually decoded instruction bytes** absent from
the unchanged 17,394-record/49,771-byte Ghidra parent inventory. Do not subtract
regional totals blindly from that inventory; their byte sets differ.

Local static evidence is `CM0102/Analysis/morph-ten-static-a`, `morph-ten-tails-a`
and `morph-ten-data-a`. Their adjacent inspector scripts record commands,
Python/Capstone where used, script hashes and input/output identities. The tails
listing contains surrounding unrecovered code for context; only the eight
regions in the accepted harness/ledger are claimed as compiled. The accepted
reports recheck all parent/probe exported instruction bytes and pinned exports.

## Accepted corpus and preserved history

`CM0102/Analysis/morph-ten-final-a-O2/report.json` and `morph-ten-final-a-O0/report.json`
each pass **8,949 comparisons**, `quick=false`. The original whole parent executes
against compiled partial entry. Languages zero/seven/ten cannot execute original
parent instructions on the recovered side; the probe and four CRT fixture
addresses are the explicit exceptions. Other languages transfer at `0x00670E2F`
and may execute earlier original common labels. This is an emulator hybrid, with
no game integration or standalone-build acceptance.

The previous 3,505 inputs remain covered. The former sixteen-case residual group
is now fifteen original transfers and one separately named recovered-ten neutral
dispatch. The extension adds 5,444 cases:

| Group | Comparisons per build |
| --- | ---: |
| 41 original keys plus miss/empty/acronyms, eight prefixes, six suffixes | 2,208 |
| All first bytes with upper/lower | 512 |
| All last bytes, no ending and each five original endings | 1,536 |
| Exact-ending short words and varied stems, both suffix cases | 60 |
| Synthetic ending lists: duplicates, empty ending, early `*`, whole-word candidate | 24 |
| Synthetic dictionary: first duplicate, early empty guard, empty replacement | 16 |
| Full character-return bits | 16 |
| Post-format language changes on common/direct returns | 20 |
| Word lengths including 0, 1, 2, 511, 1023, 2042 | 32 |
| Property dwords | 12 |
| Shared-output aliases | 4 |
| Suffix-slot mutation during character conversion | 4 |
| Seeded ten-language inputs | 1,000 |

The harness compares full EAX, caller ABI and argument bytes, watched frame/global
state and ten table regions, and ordered external events with arguments and
boundary state. It excludes the relocated probe return-address dword and does
not compare a complete ordered memory-access trace. Fixture formatting handles
only literals or one/two/three `%s` fields. Real CRT locale/errors, arbitrary
remaining-language inputs, faults, concurrency, all aliases, overflow and Windows
guard-page/SEH behavior remain outside the finite contract.

Seven current sources are frozen in `morph-partial-source-used-ten`. Prior accepted
3,505-case reports and their seven-source snapshot remain unchanged. The original
partial ledger, verifier and documentation were archived in `morph-ten-prior-state`
before extension, with a manifest and preceding successful audit. The revised
ledger pins that history; `scripts/verify_morph_partial.py` validates current
reports against current/frozen sources and historic reports against frozen source.
Its ten-stage passing audit is `morph-ten-acceptance-audit-a/audit.json`.

Diagnostic quick-a stopped because the harness initially prohibited original
common composition at `0x00670974` for unfinished languages. Quick-b passed after
allowing that explicit residual execution, but is superseded by the final pair.
Review separated the recovered ten neutral case from residual transfers before
final source pins. The eight full-routine ledgers remain byte-identical, with
44 application routines and one separate CRT search; no code integrated.
Final independent Standards and Spec reviews found no remaining findings and
recomputed current/historical acceptance identities. Python syntax checks pass;
warnings-as-errors O0/O2 builds pass. Preservation verification again passes
all 1,558 baseline files and 46 official patch payloads. No Git repository or
remote exists, and no commit, push or PR was made. All binary/data evidence stays
local in the existing ignored Analysis tree.

Language nine has since been accepted; next recover language five
at `0x0067C2EE` using its isolated candidate. See
[its isolated native candidate](LANG-MORPH-FIVE-DISCOVERY.md) before extending
the maintained source and combined corpus. This note retains the earlier
artifact's dispatch, case counts and watch contract. M2/M4/M6 remain open.
