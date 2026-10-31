# Numeric language two: property dispatch and caller-buffer effects

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This note records historical two-stage acceptance: 67,707 comparisons per build,
13,909 parent bytes and ten frozen inputs. The subsequent [four extension](LANG-MORPH-FOUR.md)
retains it in history; current [language-nine acceptance](LANG-MORPH-NINE.md) is 224,181 comparisons,
49,588 bytes and thirteen inputs. Transfers and watch scopes below describe the two-stage artifact.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
Addresses are preferred-image **VAs**, image base `0x00400000`.
Parent entry is `0x006701F0`; language-two comparison is `0x00671DE8`, body
`0x00671DF1`, and next comparison `0x00673777`.

The source now compiles language two in `cm_lang_morph_partial.c/.S/.h`, with
address descriptors in `cm_lang_morph_two_data.inc`. It adds **6,622 original
parent instruction bytes**:

| New inclusive region | Bytes | Recovered behavior |
| --- | ---: | --- |
| `0x00671DE8..0x00673776` | 6,543 | Language dispatch, prefix and 32 suffix-property branches |
| `0x0067C2D6..0x0067C2E4` | 15 | Unknown caller suffix test and transfer |
| `0x0067FCF4..0x0067FD16` | 35 | Shared suffix copy/default space |
| `0x0067FD3F..0x0067FD5B` | 29 | Caller qualifier erasure continuation |

Current partial coverage is **13,909 bytes in eighteen nonoverlapping regions**.
Earlier shared blocks are reused without double counting. Bytes
`0x0067FD31..0x0067FD3E` remain outside the recovered selection. The parent remains
identified but not fully decompiled, compiled, validated or integrated. The
eight full-routine ledgers still select 44 application routines and one separate
CRT search routine; partial recovery adds no fully recovered function.

## Frame, conversion and mutable qualifiers

Frame base remains `P=entryESP-0x2818`, with five argument slots, original
stack-probe allocation, selective initialization, shared-output clearing before
the input scan, local word at `P+0x1018`, prefix at `P+0x818`, suffix at `P+0x18`,
and scratch at `P+0x1818`. See [the measured frame](LANG-MORPHOLOGY.md).

Prefix `upper` sign-extends the first local byte into the original uppercase
dependency, then stores only AL. Prefix `lower` converts every local byte,
**without the acronym exceptions used by languages ten/eight/six**. Each loop
tests the following byte, so a callback returning zero does not itself stop later
conversions. Both paths continue into the suffix ladder; they do not return
directly after prefix conversion. Suffix arguments are read after conversion.

An unrecognized **nonempty prefix is erased in the caller's prefix buffer** by
copying the original empty literal, including its NUL. Empty prefix avoids that
copy. An unrecognized nonempty suffix is similarly erased in the caller's
suffix buffer. These effects occur after the initial word copy and can matter
when qualifiers alias each other or the caller word. The public partial
prototype now declares mutable `char *prefix` and `char *suffix` to expose that
contract. No guard, allocation or substitute grammar rule is introduced.

The worker captures the original caller-word pointer for prefix vowel/search
paths. Those paths read caller bytes rather than the transformed local copy.
General invalid-pointer, immutable-buffer or arbitrary alias safety is not
established by this finite acceptance.

## Prefix ladder and property data

The source preserves the order of the original 23 recognized prefix strings.
Literal words below are original byte strings; `\xE0` and `\xE9` denote their
single-byte encoding, not UTF-8 input.

| Prefix group | Behavior |
| --- | --- |
| `La position`, `la position` | Signed first **local** byte minus `0x61`, unsigned bound 20, original selector/target tables choose the article; formatting fixture writes it to prefix buffer |
| `de month` | Lowercase the first local byte, then use that return low byte with its own `0x61`/20 dispatch; format selected article |
| `la`, `le`, `La` | Signed low byte of first property chooses six-entry original dispatch data |
| `match de` | Ordered search for `Amical`, then `amical`, in the caller word; only a return equal to the caller pointer chooses `match ` instead of `match de ` |
| `STAD`, `\xE0 Stadium` | Ordered caller-word search and selected prefix/shared-output formatting, detailed below |
| `de la`, `\xE0 la`, `au`, `de nation`, `denation`, `en`, `en Nation`, `en nation`, `FIXla`, `FIXen` | Signed low byte of first property selects the corresponding original six-entry pointer table |
| `\xE9quipe de`, `Equipe de` | Signed low byte of **second** property selects the team-prefix table |
| Other nonempty prefix | Erase caller prefix; leave local prefix's initial NUL |

Every property dispatch accepts only values zero through five: MOVSX loads a
signed byte, followed by an **unsigned** comparison with five. Other values,
including negative sign-extended bytes, copy a single-space literal into the
local prefix/suffix buffer. High property bytes do not affect selection.
First-property prefix tables and second-property suffix tables are independent.

Some prefix property-zero paths additionally select on the **caller's first
byte**, using signed subtraction by `0x41` and unsigned bound 56, with original
57-byte selector data. Team-prefix property zero first copies the selected
vowel/consonant literal, then falls through and copies the ordinary team prefix
over it. Original `en` property two likewise copies `au `, then overwrites with
`aux `. These operations are retained, including bytes beyond the new NUL; the
source does not reduce them to just their visible final string.

For `STAD`/`\xE0 Stadium`, search order is `Stade`, `Stadium`, `Parc`, `les `,
`Les `, `le `, `Le `. A match must equal the caller pointer, not merely occur
later. The first three choose `au ` in the local prefix. The four article
matches format `aux %s`, `Aux %s`, `au %s`, `Au %s` into shared output using
caller pointer plus four/four/three/three bytes. No match chooses `\xE0 ` in
the prefix. Each path then processes suffix and reaches common composition.
The shared-output formatter therefore can run before the final formatter;
callback-created output/language changes are observed in that order.

Twenty-three prefix table extents are pinned: three `0x61`/20 selector/target
pairs, eleven six-entry property tables, and three `0x41`/56 selector/target
pairs. The table bytes and target instructions are original-image evidence.
No human language identity or property enum meaning is required to implement
them faithfully.

## Thirty-two suffix branches

Exact suffix `s` copies the original empty literal into the local suffix buffer,
including NUL. Uppercase `S` is unrecognized here and can erase the caller
suffix; language-zero/seven/ten/eight/six handling is not generalized to two.
Then the original ladder compares 32 exact qualifiers, in order. Each recognized
qualifier selects its own six-entry table using the signed low byte of the
**second** property. Unsupported values copy one space. An unrecognized suffix
uses the caller-buffer erasure path described above.

The 32 table extents are contiguous, 24 bytes each, at `0x006801A8..0x006804A7`.
Qualifiers include `est qualifi\xE9`, `a gagn\xE9`, `a`, `veut`, `y est parvenu`,
`revient`, `est`, `va`, `avait`, `a perdu`, `l'emportera`, `a d\xE9cid\xE9`,
`d\xE9cide`, `annonce`, `donne`, `souhaiterait`, `a propos\xE9`, `a invit\xE9`,
`b\xE9n\xE9ficie`, `jouera`, `est rel\xE9gu\xE9`, `remporte`, `peut`, `termine`,
`\xE9vite`, `n'a`, `est indisponible`, `essaie`, `part`, `attendu`, `se qualifie`,
`s'assure`. The source's descriptor array contains only original qualifier/table
VAs. Original strings remain data dependencies.

`CM0102/Analysis/prepare-morph-two.py` reads all table slots and verifies each
target's LEA destination and MOV EDI literal instructions. It records **67
distinct target-to-literal mappings** in the generated include. The harness
checks that include against the pinned generation artifact. Dispatch still
reads the original pointer tables; it selects compiled literal-copy behavior
and never executes original target instructions. Changing arbitrary table
targets is outside the immutable-data contract of this slice.

All two-language exits use common composition and its post-format current-
language reread. The already recovered `de le ` / `de Le ` / `\xE0 le ` /
`\xE0 Le ` contraction applies when that reread equals two. This initial body
is separate from that shared postprocessing; recovering only the contraction
would leave the article/property/suffix behavior incomplete.

## Omitted Ghidra blocks and static provenance

The original parent export remains 17,394 instruction records and 49,771 bytes.
The new selection contains **4,681 decoded records**, of which **395 records /
1,785 bytes** are absent from that export. Two adds **1,678 absent bytes** to
the previous 107. Reachable prefix and suffix dispatch actions account for many
of these omissions. The existing export is preserved; each selected instruction
is checked against primary bytes and single-instruction Capstone decoding.
`region-instructions.json` records export membership for every selected record.
Do not derive uncovered-byte totals by subtracting these different byte sets.

Local evidence is `morph-two-static-a`, `morph-two-data-a`, `morph-two-actions-a`,
`morph-two-shared-tail-a`, `morph-two-unknown-suffix-a` and
`morph-two-next-ladder-a`, all beneath `CM0102/Analysis/`. Inspection reports pin
commands, Python/Capstone versions, source/input/output identities and referenced
export artifacts. The first four are pinned in differential reports and the
partial ledger. The shared-tail context listing contains unclaimed
`0x0067FD31..0x0067FD3E`; only the eighteen regions are counted as recovered.

## Differential corpus and acceptance limits

`CM0102/Analysis/morph-two-final-a-O0/report.json` and
`morph-two-final-a-O2/report.json` both pass, `quick=false`. The full corpus contains
**67,707 comparisons per build**, retaining all previous 22,202 inputs
and adding **45,505** for two. The former neutral two residual is now a recovered
neutral dispatch; twelve original neutral transfers remain. New groups are:

| Group | Comparisons per full build |
| --- | ---: |
| All 256 property bytes, 23 prefixes, two high-byte patterns | 11,776 |
| All first bytes across 23 prefixes | 5,888 |
| All property bytes, 32 suffixes, two high-byte patterns | 16,384 |
| Independent property zero..five combinations across prefixes | 828 |
| Prefix/suffix combinations, six properties and two words | 9,384 |
| Caller-word prefix searches, matches and interior misses | 117 |
| Unrecognized qualifier erasure | 16 |
| Selected qualifier/word aliases | 24 |
| Full character-return bits | 12 |
| Suffix-slot mutation during conversion | 3 |
| Property mutation during conversion | 3 |
| Search-return/property mutation | 6 |
| Dynamic language changes | 21 |
| Stadium first-format language mutation | 3 |
| Stadium nonempty output mutation | 3 |
| Word lengths through 2,000 bytes | 32 |
| Shared-output aliases | 5 |
| Seeded two-language cases | 1,000 |

The harness now compares the **entire 16KB caller/qualifier fixture arena** at
external events and return, in addition to full EAX, cdecl cleanup, ESP,
callee-saved registers, restored FS head, incoming argument slots, watched
frame/global bytes and **76 table regions**. Older report contracts are not
retroactively advertised as having this extra arena comparison. Frame bytes
`P+0x10..P+0x2813` retain uninitialized-storage comparisons; the relocated probe
return-address cell at `P+0x2814` is excluded. A complete ordered memory-access
trace is not compared.

Known languages zero/seven/ten/eight/six/two cannot execute original parent or
table-helper instructions. The original stack probe and four CRT fixture
addresses are explicit exceptions. Remaining bodies transfer at `0x00673777`
with original frame/register state. Formatting fixtures add the four stadium
single-string formats to earlier literal/one-two-three-string/`%sens` handling;
production CRT locale/error/allocation behavior is not established.

Build inputs now include the generated include, for ten source pins. GCC
15.2.0/binutils 2.45 build freestanding 32-bit x86 with warnings as errors;
Python 3.13.12, Unicorn 2.1.4 and Capstone 5.0.7 identities and commands are
recorded. Quick smoke passes 62,713 comparisons but samples earlier length/random
groups and the two-language random group; it is diagnostic evidence only.

Revision five of `recovery/lang/morph-partial-ledger.json` pins the accepted pair
and ten-input snapshot `CM0102/Analysis/morph-partial-source-used-two/`.
`scripts/verify_morph_partial.py` checks reports, source/tools/build artifacts,
original regions, all tables, generated descriptors, inspection reports and
separate helper-A evidence. Its passing audit is
`CM0102/Analysis/morph-two-acceptance-audit-a/audit.json`.
Before editing, revision-four ledger/verifier/docs were archived under
`morph-two-prior-state`. The verifier follows all four historical versions and
checks their report pairs (22,202, 18,823, 8,949 and 3,505 comparisons per build)
against their unchanged frozen inputs. It preserves the eight full-routine
ledgers and whole-parent false statuses. Earlier watch scopes stay historical.

Fresh-output reproduction commands are in LANG-MORPH-PARTIAL.md. The
unknown-suffix inspection ends at exclusive VA `0x0067C2E5`; an earlier end
inside an instruction failed the complete-decoding assertion before creating
output. The corrected range is pinned in `morph-two-unknown-suffix-a`.

Acceptance requires valid terminated strings, mutable mapped qualifier storage,
bounded buffers, DF clear, normal return and the declared original data.
Selected qualifier aliases are compared; arbitrary layouts, faults, concurrency,
overflow and Windows guard-page/SEH behavior remain outside scope. No preserved
game file or old report/source snapshot is changed. Baseline preservation again
passes all 1,558 files and 46 official patch payloads.

Final independent Standards and Spec reviews found no hard violations,
actionable smells or material fidelity/documentation findings. They verified
both accepted reports, ten current/frozen inputs, regional/manual accounting,
generated descriptors and inspection provenance, all four historical acceptance
pairs and eight unchanged full-routine ledgers. Spec independently recomputed
all 15 acceptance audit fields. Python syntax checks pass for the harness,
verifier and local inspection/archive/acceptance scripts. The workspace remains
outside Git; no commit, push or PR was made.

Language nine has since been accepted; next recover language five
at `0x0067C2EE` using its isolated candidate. See
[its isolated native candidate](LANG-MORPH-FIVE-DISCOVERY.md) before extending
the maintained source and combined corpus. This note retains the earlier
artifact's dispatch, case counts and watch contract. M2/M4/M6 remain open.
