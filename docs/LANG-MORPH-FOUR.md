# Morphology language four: original article and suffix dispatch

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This records historical four-stage acceptance: 91,143 cases per build, 19,079
parent bytes and eleven inputs. Current [language-nine acceptance](LANG-MORPH-NINE.md)
has 224,181 comparisons, 49,588 parent bytes and thirteen inputs. The transfers
and watch contracts below belong to the preserved four-stage artifact.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
Addresses are preferred-image **VAs**, image base `0x00400000`.
Parent entry is `0x006701F0`; language-four comparison is `0x00673777`, body
`0x00673780`, and next comparison `0x00674BA9`.

`cm_lang_morph_partial.c/.S/.h` now compile language four, with generated
address descriptors in `cm_lang_morph_four_data.inc`. This adds **5,170 original
instruction bytes**, the inclusive region `0x00673777..0x00674BA8`.
All earlier shared blocks are reused without double counting. Current partial
coverage is **19,079 bytes in nineteen nonoverlapping regions**; the separately
accepted 171-byte table helper remains outside that parent count. The parent is
still **not fully decompiled, compiled, validated or integrated**. The eight
full-routine ledgers still contain 44 application entries plus one separate CRT
routine, with no integration. No preserved executable is patched.

## Frame and qualifier contracts

See [LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) for the measured probe and frame.
Frame base is `P=entryESP-0x2818`. Incoming word, prefix, suffix, property1 and
property2 remain at `P+0x281C`, `P+0x2820`, `P+0x2824`, `P+0x2828`, `P+0x282C`.
Local word is `P+0x1018`, prefix buffer `P+0x818`, suffix buffer `P+0x18`.
Initialization copies the word and clears only the previously identified bytes;
it does not zero every private buffer. Shared output remains `0x00B64804`.

The original captures the prefix argument before comparing qualifiers. Prefix
`upper` sign-extends the first local-word byte, calls CRT address `0x009471CD`,
stores only AL and continues into suffix handling. Prefix `lower` converts each
byte through `0x009465FC`, including sign extension, stores AL, and reads the
next byte even when the returned byte is zero. This body has **no acronym
exceptions**. Both paths read the suffix argument slot after conversion returns.

All article paths read only the **signed low byte of property1**. MOVSX followed
by unsigned `JA 8` selects a table only for values 0 through 8; all other bytes,
including `0x80..0xFF`, copy the original single-space literal into the local
prefix. High property bits do not affect selection. Each dispatch table contains
nine original code pointers, not literal pointers. The compiled descriptors map
those verified targets to their original literal VAs.

| Prefix qualifier, in original comparison order | Original nine-pointer table |
| --- | --- |
| `la` | `0x006804A8` |
| `La` | `0x006804CC` |
| `della` | `0x00680610` |
| `dellla` | `0x00680610` |
| `dalla` | `0x006804F0` |
| `nella` | `0x00680514` |
| `sulla` | `0x00680538` |
| `alla` | `0x006805EC` |
| `a` | `0x006805EC` |
| `Alla` | `0x0068055C` |
| `Qualificata` | `0x00680580` |
| `una` | `0x006805A4` |
| `Una` | `0x006805C8` |

`dellla` is the supplied literal at `0x009F0E64`; its spelling is preserved.
For example, `la` selects `""`, `"il "`, `"lo "`, `"la "`, `"l'"`, `"i "`,
`"gli "`, `"le "`, `"l'"` for property1 values 0..8. The `La` table uses the
corresponding original capitalized literals. `una`/`Una` select the empty literal
for values 5..8; this is an actual one-byte NUL copy into local storage.

Unknown **nonempty** prefixes copy the empty original literal to the **caller
prefix pointer**, erasing its first byte. Unknown empty prefixes avoid that
write. They do not copy a replacement prefix into the local buffer. The public
interface declares qualifiers mutable. No conventional Italian grammar is
substituted for these machine contracts.

## Ordered suffix selection

The suffix pointer is read after prefix processing. Exact lowercase `s` copies
the empty literal into local suffix storage; uppercase `S` is unrecognized and
can erase caller storage. The following qualifiers dispatch on the signed low
byte of **property2**, using the same unsigned range 0..8. Unsupported values
copy one space into local suffix storage.

| Ordered suffix qualifier | Original nine-pointer table |
| --- | --- |
| `eliminata` | `0x00680634` |
| `Eliminata` | `0x00680658` |
| `qualificata` | `0x0068067C` |
| `Qualificata` | `0x006806A0` |
| `retrocessa` | `0x006806C4` |
| `si \xE8 qualificata` | `0x006806E8` |
| `costretta` | `0x0068070C` |
| `\xE8 retrocessa` | `0x00680730` |
| `viene eliminata` | `0x00680754` |
| `partita` | `0x00680778` |
| `partita (ban)` | Signed comparison; no table |
| `Invitata` | `0x0068079C` |
| `promossa` | `0x006807C0` |
| `\xE8 stata invitata` | `0x006807E4` |
| `\xE8 stata scelta` | `0x00680808` |
| `contratta` | `0x0068082C` |
| `giorno` | `0x00680850` |

The escaped literals above use original Latin-1 byte `0xE8`. No Unicode
normalization or locale conversion is introduced. Original leading spaces,
capitalization and copy lengths including NUL are retained. Each copy measures
its original literal and writes DWORDs followed by remaining bytes; it does not
use a replacement general string-copy routine.

At `0x006747C6..0x006747DA`, `partita (ban)` reads AL directly and executes
`CMP AL,1 / JLE`. Signed byte values **-128..1** choose `" partita"`; 2..127
choose `" partite"`. This differs from the unsigned bounds used by table paths.
The compiled check occurs before `Invitata`, preserving the original order.
Ordinary `partita` instead uses its nine-pointer table: 0..3 and 8 singular,
4..7 plural, unsupported bytes space.

The original repeats the exact `partita` comparison at `0x00674B47` after
`giorno`. It cannot be reached as a successful comparison: the earlier matching
`partita` branch always leaves the suffix ladder. Its later property-zero test
is therefore not made into an alternative rule in recovered source.

Unknown nonempty suffixes transfer through `0x0067C2D6` to the earlier recovered
caller-erasure copy; unknown empty suffixes avoid the write. All language-four
paths continue through common composition and its post-format language reread.
If a formatting callback changes current language to two, the original ordered
`de le`/`de Le`/`\xE0 le`/`\xE0 Le` contraction follows. Language four introduces
no direct formatter return that bypasses it.

## Instruction and data provenance

Read-only `scripts/inspect_morphology_region.py` produced
`CM0102/Analysis/morph-four-static-a`, checking original/analysis-copy hashes,
the preceding Ghidra export, full contiguous Capstone decoding and original
literal bytes. Local `CM0102/Analysis/prepare-morph-four.py` first archived the
revision-five ledger/verifier/docs under `morph-four-prior-state`, then produced
`morph-four-data-a`. Its report pins command, Python/Capstone, script SHA and
each generated artifact. Nothing in the older accepted source snapshots changes.

The new data evidence records **27 tables of 36 bytes each**, contiguous from
`0x006804A8` through exclusive `0x00680874`. It independently decodes each
table-target LEA destination, MOV EDI literal and outgoing JMP. The generated
include contains thirteen ordered prefix keys, sixteen ordered suffix-table
keys and **151 target-to-literal mappings**. `partita (ban)` is a separately
recovered signed branch. Original qualifier, literal and table data remain in
the loaded image; the include is address metadata, not a recovered standalone
data library. Arbitrary changed code-pointer tables are outside acceptance.

Current parent coverage has **6,129 decoded instruction records**. Of these,
**846 records / 4,090 bytes** are absent from Ghidra's unchanged parent export.
Language four adds **2,305** such instruction bytes. These bytes are included
within 19,079, not added again. Original property tables reach those omitted
branches. Bytes `0x0067FD31..0x0067FD3E` remain outside the recovered selection.
The Ghidra export still has 17,394 records / 49,771 bytes; no inventory or
preserved manifest is regenerated to hide differences.

## Differential evidence and remaining gates

Focused `morph-four-focused-a-O2` passes **481 comparisons**, covering every
qualifier with 0..9 and signed-byte boundaries. This is diagnostic evidence,
not full acceptance. An independent Spec reviewer checked all thirteen prefix
rules and 151 mappings against instructions and passed **367 additional focused
comparisons**, including alias and callback mutation cases. Initial Standards
and Spec reviews found no hard violations, actionable smells or material
implementation findings.

Full `morph-four-final-a-O0/report.json` and `morph-four-final-a-O2/report.json`
both pass **91,143 comparisons per build**, `quick=false`. Revision six of the
partial ledger pins the pair and eleven-input `morph-partial-source-used-four`
snapshot. Read-only `scripts/verify_morph_partial.py` passes in
`morph-four-acceptance-audit-a`, checking reports, current/frozen inputs, original
regions, generated descriptors, inspection evidence and artifacts/tools. It also
verifies all five historical accepted pairs against their unchanged frozen
inputs (67,707 / 22,202 / 18,823 / 8,949 / 3,505 per build), the eight unchanged
full-routine ledgers and whole-parent false statuses. The corpus retains all 67,707 previous
inputs and adds 23,436; neutral language four is reclassified from residual to
recovered dispatch. New groups are:

| Case group | Count |
| --- | ---: |
| Prefix properties: thirteen keys, all 256 bytes, two high patterns | 6,656 |
| Suffix properties: sixteen keys, all bytes, two high patterns | 8,192 |
| Signed `partita (ban)`: all bytes and two high patterns | 512 |
| Upper/lower first bytes | 512 |
| Independently varied valid properties with thirteen prefix keys | 1,053 |
| Prefix/suffix/word/property combinations | 5,400 |
| Unknown qualifier erasure | 16 |
| Selected qualifier aliases | 24 |
| Full character-return bit patterns | 8 |
| Suffix-slot and property mutations during conversion | 4 |
| Post-format language changes and nonempty shared output | 22 |
| Empty/short/long word lengths | 32 |
| Shared-output aliases | 5 |
| Seeded language-four random cases | 1,000 |

Comparisons check full EAX, incoming arguments, cdecl stack/register/FS state,
ordered CRT events with arguments and boundary state, `P+0x10..P+0x2813`, shared
output/current language, all **103 table regions** and the complete **16KB caller
fixture arena**. The relocated probe-return DWORD `P+0x2814` remains explicitly
excluded. Earlier reports retain their historical watch scopes. A complete
ordered memory-access trace is not compared.

Known languages zero/seven/ten/eight/six/two/four cannot execute original parent
or table-helper instructions. The actual original stack probe and four CRT
fixture entries are explicit exceptions. Other bodies transfer at
`0x00674BA9` with the original frame/register state. Eleven neutral transfers
do not establish arbitrary unfinished-language behavior.

GCC 15.2.0/binutils 2.45 compile freestanding 32-bit x86 with warnings as errors.
Reports pin eleven build/harness inputs and tool/artifact identities; Python
3.13.12, Unicorn 2.1.4 and Capstone 5.0.7 are recorded. Valid terminated strings,
mutable mapped qualifier storage, bounded buffers, DF clear and normal returns
are required. General aliases, arbitrary modified tables, faults, overflow,
concurrency, Windows guard-page/SEH behavior and production CRT locale/error
behavior remain outside scope.

Fresh-output reproduction commands are in LANG-MORPH-PARTIAL.md. Python
syntax checks pass for the harness/verifier and local inspection/acceptance
scripts. Baseline verification again passes all 1,558 preserved files and 46
official patch payloads; expected runtime extras remain separate. The workspace
is not a Git repository; no commit, push or PR was made. Analysis builds,
snapshots and diagnostic artifacts remain local under the existing ignored tree.
Final independent Standards and Spec reviews found no hard violations,
actionable smells or material fidelity/documentation findings. They verified
both accepted reports, eleven current/frozen inputs, original regional/manual
accounting, six inspection reports, generated descriptors, creator/verifier
provenance, all five historical pairs and eight unchanged full-routine ledgers.
Spec independently recomputed all 15 acceptance audit fields. Both also checked
the language-three preflight against original instructions and its pinned
report chain; it remains static identification only.

The next body at this historical stage was numeric language three: comparison **`0x00674BA9`**, body
**`0x00674BB2`**, JNE to `0x0067B506`, confirmed in
`morph-four-next-ladder-a`. Preserve the measured frame and explicit unfinished
branches. M2 primary runtime replay/GDI mapping, M4 live integration and M6
standalone remain open.

[Language-three preflight](LANG-MORPH-THREE-DISCOVERY.md) records the next body's
static dependency inventory, distinct five/ten-slot bounds, extra ECX selector,
helper-B calls and caller-erasure flag. Those discoveries add no compiled or
validated parent coverage at that historical stage. Three and nine have since
entered maintained-source acceptance; continue with
[five's isolated candidate](LANG-MORPH-FIVE-DISCOVERY.md) at `0x0067C2EE`.
