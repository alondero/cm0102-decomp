# Partial morphology source: languages zero, seven, ten, eight, six, two, four, three and nine

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

Input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`, with the same hash.
Addresses are preferred-image **VAs**, image base `0x00400000`.
Parent entry is `0x006701F0`. See [the measured frame](LANG-MORPHOLOGY.md)
before interpreting its generated pseudocode or stack variables.

`recovery/lang/cm_lang_morph_partial.c`, `.h`, `.S` and generated
`cm_lang_morph_two_data.inc` / `cm_lang_morph_four_data.inc` /
`cm_lang_morph_three_body.inc` / `cm_lang_morph_nine_body.inc` compile initialization,
the language-zero/seven/ten/eight/six/two/four/three/nine bodies, and shared composition/postprocessing. This is
an emulator hybrid: all other language bodies transfer into the original parent
at `0x0067C2E5`. The original stack probe and fixed-address strings/tables remain
dependencies. CRT formatting, character conversion and substring search are
explicit fixtures in the differential harness. No game executable is patched.

The separate `recovery/lang/morph-partial-ledger.json` records regional source
and acceptance independently from the whole parent. The parent is identified,
but **not fully decompiled, compiled, validated or integrated**. The existing
eight full-routine ledgers and combined map remain unchanged: 44 application
routines plus one separate CRT search routine. Partial regions add no fully
recovered function to that count. The stack probe also remains original code.

## Original byte coverage and missing export blocks

| Inclusive region | Hashed instruction bytes | Recovered behavior |
| --- | ---: | --- |
| `0x006701F0..0x006705A7` | 952 | Probe call, initialization, zero/seven dispatch and bodies |
| `0x00670974..0x00670A38` | 197 | Empty-output composition, language reread, contraction and return |
| `0x0067FD61..0x0067FDC0` | 96 | Shared-output test and temporary-buffer composition/copy |
| `0x006705A8..0x00670973` | 972 | Language-ten dispatch, case conversion and dictionary selection |
| `0x00670A39..0x00670E2E` | 1,014 | Language-ten suffix handling and caller-tail matching |
| `0x0067190A..0x00671926` | 29 | Shared out-of-line first-byte uppercase block |
| `0x00671D03..0x00671D42` | 64 | Selected-ending formatting and return |
| `0x00671D8C..0x00671DE7` | 92 | Whole-word direct formatting and returns |
| `0x00670E2F..0x006718C1` | 2,707 | Language-eight dispatch, articles, dictionary and suffix handling |
| `0x0067FD17..0x0067FD30` | 26 | Scratch-to-word grouped copy |
| `0x0067FD5C..0x0067FD60` | 5 | Copy continuation to common composition |
| `0x006718C2..0x00671909` | 72 | Language-six dispatch and prefix test |
| `0x00671927..0x00671D02` | 988 | Language-six conversion, e suffix and ending selection |
| `0x00671D43..0x00671D8B` | 73 | Language-six no-ending dispatch |
| `0x00671DE8..0x00673776` | 6,543 | Language-two prefixes and suffix-property ladder |
| `0x0067C2D6..0x0067C2E4` | 15 | Unknown caller suffix test |
| `0x0067FCF4..0x0067FD16` | 35 | Shared suffix copy/default space |
| `0x0067FD3F..0x0067FD5B` | 29 | Caller qualifier erasure continuation |
| `0x00673777..0x00674BA8` | 5,170 | Language-four property prefixes and ordered suffix selection |
| `0x00674BA9..0x0067B505` | 26,973 | Language-three native qualifier flow, helper B, two suffix passes and selector |
| `0x0067B506..0x0067C2D5` | 3,536 | Language-nine ordered qualifiers, selector and caller erasure |

These **49,588 bytes** are twenty-one regions of one incomplete function. Language ten
added **2,171 bytes** to the prior 1,245-byte slice; language eight added **2,738**;
language six added **1,133**. Two adds **6,622**, four **5,170**, three **26,973**, and nine **3,536**; shared blocks are reused without double counting.
The copy-context bytes at `0x0067FD31..0x0067FD3E` remain outside maintained-source
acceptance; the isolated five candidate implements their conditional guard.
The separately accepted 171-byte helpers A/B are support, outside this parent count.
The regional selection does not imply recovery of the remaining parent.
Ghidra's existing parent export has 17,394 instruction records totaling 49,771
bytes. That inventory omits two reachable vowel blocks:
`0x006703FA..0x00670413` and `0x00670489..0x006704A2`, 26 bytes each. Verified
original dispatch tables point to them. The harness decodes these **52 additional
bytes** directly from the primary executable with Capstone. Language ten adds
another 55 absent bytes at `0x00671D03..0x00671D0F` and
`0x00671D8C..0x00671DB5`, reached through verified suffix dispatch tables.
Two adds another **1,678 absent bytes** reached through verified dispatch data.
Four adds **2,305 absent bytes**, three adds **4,897**, and nine adds **1,372**.
The current slice records **10,359 manually decoded bytes** across 2,342 records
outside the Ghidra instruction body. It preserves the existing export.

Each report pins the original region hashes, the four zero-language tables at
`0x0067FDC4`, `0x0067FDCC`, `0x0067FE04`, `0x0067FE0C`, and both static exports
`lang-loader-static-b` and `morph-probe-static-a`. `region-instructions.json`
records every instruction in the twenty-one regions and its export-membership flag.
It also pins ten-language dispatch tables, the six-pointer ending list and
the 496-byte three-pointer dictionary extent. See [LANG-MORPH-TEN.md](LANG-MORPH-TEN.md).
The 225 pinned table regions also include eight's ending/dispatch data and
660-byte four-pointer dictionary. See [LANG-MORPH-EIGHT.md](LANG-MORPH-EIGHT.md).
Six adds its ending list and wider word-selector table. See [LANG-MORPH-SIX.md](LANG-MORPH-SIX.md).
Two adds twenty-three prefix and thirty-two suffix dispatch table extents;
its address descriptors match the pinned generation artifact. See [LANG-MORPH-TWO.md](LANG-MORPH-TWO.md).
Four adds twenty-seven nine-pointer tables and 151 target/literal descriptors;
see [LANG-MORPH-FOUR.md](LANG-MORPH-FOUR.md).
All exported parent/probe instruction bytes are checked against primary bytes
and single-instruction decoding before comparison.

## Recovered contracts

Incoming ESP is `E`; saved-register/frame base is `P=E-0x2818`. The assembly
entry calls the actual probe at `0x00945610` with EAX=`0x2808`, saves the four
original callee-saved registers, and invokes the C initializer with `P`.
The initializer preserves the original selective clearing and dword-then-byte
copy, including clearing shared output before scanning a possibly aliased word.
It does not move the original null test ahead of that scan.

Before unfinished-language transfer, assembly reconstructs the original register
state: EBX=caller word, EDX=`P+0x1018`, ESI=source after the copied NUL,
EDI=destination after the copied NUL, ECX=zero, EAX=current language. ESP stays
at `P`; original argument slots and saved registers remain available. Languages
zero, seven, ten, eight, six, two, four, three and nine enter recovered C workers; other values jump to the original
comparison ladder without repeating initialization or reading language again.
The two property dwords remain in their original incoming slots. Zero/seven/ten/six
do not use them. Eight's article paths read only the first property's low byte;
its other paths use neither property, and the second is unused throughout. No other language behavior
is generalized. Two uses signed low bytes of both properties, according to
the selected prefix/suffix; unknown qualifiers can erase caller prefix/suffix
buffers. Four uses signed low bytes 0..8 for article and suffix tables;
its `partita (ban)` branch instead compares signed AL <= 1. It preserves
unknown caller qualifier erasure. The public interface declares qualifiers mutable. Eight links the separately accepted table-helper C/header;
recovered languages cannot execute the original helper.

Language zero captures the prefix pointer, compares it with `upper`, and calls
original-CRT-address character conversion on the **sign-extended first byte**.
Only the return low byte is stored. It reads the suffix argument after that
dependency returns. Suffix `s` or `S` with an empty word returns EAX=zero;
otherwise last-byte `s`/`S` chooses the literal apostrophe, and other last bytes
choose apostrophe-plus-s. Prefix `an`, then `An`, selects a byte table indexed
by the signed first byte minus `0x41`, with an unsigned upper bound of 52.
Original table values and literals choose `an `/`a ` or `An `/`A `.

Language seven ignores prefix grammar. For suffix `s` or `S`, an empty word
returns zero; a last-byte `s`/`S` formats the original empty literal, otherwise
it formats `s`. Other suffixes leave the suffix buffer's first byte zero.
These are numeric language contracts from instructions, without inferred names
or substitute grammar rules.

Shared composition formats `%s%s%s` using frame prefix, word and suffix buffers
when output is empty. If shared output already contains text, it formats prefix,
shared output and suffix into `P+0x2018`, then measures and copies it back in
dwords followed by bytes. It rereads current language **after formatting**.
For value two, substring dependencies are tried in original order: `de le `,
`de Le `, byte string `\xE0 le `, then `\xE0 Le `. Only the first match is
contracted, preserving the original forward copy and its initial read/do-loop
order. Normal return is the fixed shared output pointer `0x00B64804`.
Direct suffix-formatting returns in languages ten/eight/six bypass this postprocessing;
their other paths use common composition. Language three also returns zero
for an empty local word on one `s` selector path. These distinct returns are retained.

Language three links compiled helper B and watches its original 660-byte
dictionary, 98 property tables and the 101-byte/four-pointer special selector.
That language-three stage contains 204 table regions; nine expands the contract to 225. It retains 94 distinct prefix
and 115 suffix keys, two suffix passes and the cached signed property2 byte.

Language nine preserves seventeen prefix and nine suffix comparison positions,
signed low property bounds 0..4, and its do/di 101-byte/seven-pointer selector.
Property 100 directly clears the first local-prefix byte. Unknown nonempty
qualifiers erase caller bytes; suffix capture follows character callbacks.
See [LANG-MORPH-NINE.md](LANG-MORPH-NINE.md).

## Differential acceptance and limits

Full accepted `morph-nine-final-a-O2/report.json` and
`morph-nine-final-a-O0/report.json`, beneath `CM0102/Analysis/`, each pass
**224,181 comparisons** against execution of the whole original parent. Both
builds use GCC 15.2.0/binutils 2.45, freestanding 32-bit x86, warnings as errors,
and the explicit compiler flags recorded in the reports. Source, tool, build
artifact and static-export hashes are pinned. Thirteen tested inputs are frozen
under `CM0102/Analysis/morph-partial-source-used-nine/`, including the unchanged
table-helper C/header, both address includes, and native language-three/nine workers.

The corpus covers 400 prefix/suffix/word combinations; 1,536 first-byte cases
(all 256 values with `an`, `An`, `upper` in both languages); 512 lengths zero
through 255; sixteen long-word cases including 511/512, 1023/1024 and 2039..2042;
six property cases; two shared-output aliases; ten post-format language changes;
two dependency-created nonempty-output cases; four character-return bit patterns;
one suffix-argument mutation during character conversion; nine neutral-word
original residual-language transfers plus one recovered neutral dispatch each
for ten, eight, six, two, four, three and nine; and 1,000 seeded zero/seven cases. These retain the prior 3,505 inputs.
The 5,444 additional ten-language cases are detailed in LANG-MORPH-TEN.md.
The next 9,874 eight-language cases are detailed in LANG-MORPH-EIGHT.md. The
previous 8,949 inputs are retained, with the neutral eight case reclassified.
Six adds 3,379 comparisons, retaining all 18,823 previous inputs and reclassifying
its neutral dispatch. See LANG-MORPH-SIX.md for its distinct word-selector bounds.
Two adds 45,505 comparisons, retains all prior 22,202 inputs and reclassifies its
neutral dispatch. See LANG-MORPH-TWO.md for caller mutation and property paths.
Four adds 23,436 comparisons and retains all prior 67,707 inputs, reclassifying
its neutral dispatch. See LANG-MORPH-FOUR.md for signed table/ban distinctions.
Three adds 117,361 comparisons, retaining all previous 91,143 inputs and
reclassifying its neutral dispatch. See [LANG-MORPH-THREE.md](LANG-MORPH-THREE.md)
for both suffix passes, actual default-scope literal, helper B and selector early return.
Nine adds 15,677 comparisons, retaining every previous 208,504 input and
reclassifying its neutral dispatch. Its property corpus covers every low byte
with two high-bit patterns; callback mutation, aliases and original direct-zero
behavior are compared.

Each comparison checks full EAX and watched frame/global state, original caller
argument bytes, and ordered CRT events with arguments and boundary state.
Current reports additionally compare the full 16KB caller/qualifier fixture arena
at events and return, plus 225 table regions. Earlier reports retain
their original narrower watch contract.
The inherited caller checks cdecl cleanup, ESP, callee-saved registers, FS head
and normal return within one million instructions. Frame comparison covers
`P+0x10..P+0x2813`; the probe's relocated return-address dword at `P+0x2814`
differs between the original and compiled entry and is explicitly excluded.
Other uninitialized private-buffer bytes in this range remain compared.
This corpus does not compare a complete ordered memory-read/write trace.

The recovered side rejects original instruction execution except the stack
probe, fixture entry addresses, and declared residual language bodies. For
languages zero/seven/ten/eight/six/two/four/three/nine it cannot silently reuse original parent instructions.
The nine residual cases establish selected transfers with neutral empty
qualifiers, not arbitrary behavior of unfinished languages or their dependencies.
Formatting supports literals, one/two/three `%s` fields, `%sens` and the four
stadium single-string formats. Selected qualifier aliases and caller erasure
are compared; general aliases remain outside acceptance.
Character conversion and formatting fixtures do not establish actual CRT locale,
error or allocation behavior. Inputs are valid terminated byte strings, DF clear,
bounded buffers and normal returns. General aliases, faults, concurrency,
overflow and Windows guard-page/SEH behavior are outside acceptance.

The earlier `morph-partial` full-a reports (3,489 cases per build) precede long-word cases and the
corrected return-address-cell wording. Quick smoke reports are diagnostic only.
The prior accepted `morph-partial-final-b` pair (3,505 per build), its seven-source
snapshot `morph-partial-source-used`, and the archived prior ledger remain intact.
The 8,949-case ten-final-a pair and its seven-source snapshot also remain intact.
The 18,823-case eight-final-a pair and its nine-source snapshot remain intact.
The 22,202-case six-final-a pair and its nine-source snapshot remain intact.
The 67,707-case two-final-a pair and its ten-source snapshot remain intact.
The 91,143-case four-final-a pair and its eleven-source snapshot remain intact.
The 208,504-case three-final-a pair and its twelve-source snapshot remain intact.
Before this extension, revision seven ledger/verifier/docs were archived under
`morph-nine-prior-state`, retaining six nested earlier revisions. The current
verifier checks all seven prior versions against frozen sources, and the new pair
against current and newly frozen sources. The eight
full-routine ledgers and their accepted application C/assembly remain unchanged.

```powershell
& py -3.13 recovery/lang/test_cm_lang_morph_partial.py --optimization O2 --output CM0102/Analysis/morph-partial-check-O2
& py -3.13 recovery/lang/test_cm_lang_morph_partial.py --optimization O0 --output CM0102/Analysis/morph-partial-check-O0
& py -3.13 scripts/verify_morph_partial.py --output CM0102/Analysis/morph-partial-check-audit
```

Use fresh output names. The verifier checks the recorded accepted pair and frozen
sources; newly generated report pairs need separate acceptance. `--quick` is a
sample, not full-corpus acceptance. The local partial audit verifies current and
frozen source identities, artifact/tool/export/region hashes, whole-parent false
statuses, and unchanged hashes of the eight accepted full-routine ledgers.

`CM0102/Analysis/morph-nine-acceptance-audit-a/audit.json` is the current passing
audit. The prior three, four, two, six, eight, ten and zero/seven audits are historical.
`morph-partial-acceptance-audit-a` records a verifier-only `KeyError('category')`: older
ledgers omit that field. The verifier now uses the existing application default
and separate CRT classification, preserving those ledger bytes. That diagnostic
is retained; it is not an application differential failure.

Earlier independent Standards and Spec reviews verified all prior acceptance.
The two-stage reviews checked all 67 mappings, current/frozen sources and its
67,707-case reports. Initial four-stage Standards review found no hard violations
or actionable smells; Spec independently verified thirteen prefix rules, 151
target mappings and 367 focused comparisons without a material finding. Final four-stage
independent Standards and Spec reviews found no hard violations, actionable
smells or material fidelity/documentation findings. They verified both 91,143-case
reports, eleven current/frozen inputs, regional/manual accounting, provenance,
five historical acceptance pairs and eight unchanged full-routine ledgers. Spec
independently recomputed all 15 acceptance audit fields. Both checked the
language-three discovery against original evidence and confirmed its
identified-only status, incomplete parent and zero integration.
Python syntax checks pass for the harness,
verifier and local inspection/archive/acceptance scripts.
`scripts/verify_game.py` again checks all 1,558 preserved
baseline files and 46 official patch payloads: PASS. Existing additional runtime
files are reported separately. The workspace is not a Git repository; no commit,
push or PR was made. Analysis builds, exports, snapshots and diagnostic evidence
remain local under the existing ignored `CM0102/Analysis/` tree.

The prior language-three review checked the 208,504-case pair, twelve frozen
inputs and all sixteen audit fields. Its historical evidence remains intact.
For nine, initial independent reviews confirmed token identity with the frozen
candidate and byte/table accounting; Spec ran sixty additional comparisons.
The current acceptance audit also verifies the sixteen frozen candidate inputs,
its earlier report/artifact/tool chains and adoption input/output hashes.

Both accepted nine C/assembly builds use warnings as errors. Analysis builds,
exports, snapshots and diagnostic evidence remain local. No recovered code is
integrated and the standalone game remains unfinished.

Language nine is now regionally accepted; [its focused note](LANG-MORPH-NINE.md)
records ordered qualifier behavior and the direct-zero selector. Next complete
language five at `0x0067C2EE` and shared guard `0x0067FD31..0x0067FD3E`.
[Five's isolated candidate](LANG-MORPH-FIVE-DISCOVERY.md) has focused evidence,
but still needs broad maintained-source comparison and acceptance. Preserve
unknown-language composition and audit the complete instruction extent before
considering whole-parent status. The original stack probe/data/CRT dependencies
also remain. Connect loader-produced records through lookup and morphology as
dependencies close. M2 runtime replay and primary/GDI mapping, M4 integration,
and M6 standalone remain open.

Final independent Standards and Spec reviews found no remaining hard violations,
actionable smells or material fidelity/acceptance findings. Standards checked
156 source identities and 92 artifacts. Spec independently recomputed all
seventeen current acceptance-audit fields, including the candidate adoption
chain, and verified five's separate focused pair and frozen inputs. Its 180
additional five comparisons passed. The stale candidate links and minor prose
findings were corrected before the final documentation hashes were recorded.

Python syntax checks pass for current recovery/scripts and local analysis tools.
Both accepted nine builds and both focused five builds use warnings as errors.
Preservation verification passes all 1,558 baseline files and 46 official patch
payloads; runtime-created additional files are reported separately. `git status`
confirms the workspace is not a Git repository; no commit, push or PR was made.
Supplied media, assets, binaries and diagnostic evidence remain local.

Next-session prompt:

> Read README.md, docs/ROADMAP.md, docs/LANG-MORPHOLOGY.md,
> docs/LANG-MORPH-PARTIAL.md, docs/LANG-MORPH-NINE.md and
> docs/LANG-MORPH-FIVE-DISCOVERY.md. Verify the separate partial ledger,
> nine-final-a O0/O2 reports and thirteen current/frozen source pins. Keep
> all seven prior stages and eight full ledgers intact. Continue from language
> five at 0x0067C2EE using the isolated candidate, preserving DL's captured
> property2, both suffix passes, del/de la selector and conditional caller erasure.
> Preserve P=entryESP-0x2818, five arguments, selective initialization, signed
> byte conversion, dword-before-byte copies and dynamic language rereads.
> Close remaining shared/default paths and explicit dependencies before
> promoting whole-parent status. Do not invent engine logic or begin M4 early.
