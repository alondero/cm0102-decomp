# Morphology language three: native control flow and two suffix passes

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

This note describes the frozen three-stage acceptance and its original watch
scope. Current [language-nine acceptance](LANG-MORPH-NINE.md) passes 224,181
comparisons, covers 49,588 parent bytes and freezes thirteen inputs. All
three-stage counts, transfer addresses and review results below are historical.


Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` is byte identical.
Addresses are preferred-image **VAs**, image base `0x00400000`.
Parent entry is `0x006701F0`; language-three comparison is `0x00674BA9`,
body `0x00674BB2`, and next comparison `0x0067B506` tests language nine.

The new native worker is `recovery/lang/cm_lang_morph_three_body.inc`, included
by the partial-parent C source and entered by its assembly dispatch. It recovers
**26,973 original instruction bytes / 8,731 instruction records** in
`0x00674BA9..0x0067B505`. The worker lowers 8,729 records; the other two are the
comparison and conditional jump replaced by the assembly dispatch. Joining
single-predecessor fallthroughs produces **626 native C basic blocks**, retaining
conditional, table-target and shared-tail labels at original VAs. The generated
source executes C expressions and compiled branches; it copies no original
instructions and contains no instruction interpreter.

Accepted regional total is **46,052 bytes in twenty
nonoverlapping regions**, including **8,987 manually decoded bytes / 2,079
records** missing from the existing Ghidra instruction body. Language three
adds **4,897** of those absent bytes. Helpers A and B are separately accepted
171-byte routines, outside the parent count. The parent remains incomplete;
integration and a standalone game remain unachieved. Eight unchanged
full-routine ledgers still contain 44 application entries and one separate CRT.

## Prefix capture and helper B

Use the measured frame `P=entryESP-0x2818` from
[LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md). Word, prefix, suffix, property1,
property2 are at `P+0x281C`, `P+0x2820`, `P+0x2824`, `P+0x2828`, `P+0x282C`.
Local word is `P+0x1018`, prefix `P+0x818`, suffix `P+0x18`; output is
`0x00B64804`. Existing initialization, widths and common composition are reused.

The body captures the prefix argument into EBP before comparison. Both `upper`
and `Upper` uppercase the signed first local-word byte through `0x009471CD`,
store AL and continue into suffix handling. `lower` uses `0x009465FC` for each
signed byte, without acronym exceptions. It preserves the original next-byte
read when a callback returns zero.

`upper_pos1`, `upper_pos2`, `upper_pos3` call the recovered **helper B** at
`0x00682150` with column 1, 2, 3, then uppercase the first local byte even after
a miss or empty result. `pos1`, `pos2`, `pos3` select those columns without
uppercasing. The original 41-row, four-pointer dictionary and terminal key occupy
660 bytes at `0x009EB7AC`. First match, empty-key guard and DWORD-before-byte
copying remain the independently accepted helper behavior.

The source preserves all **94 distinct prefix keys** and their comparison order,
including original German qualifier bytes. Property dispatch reads the signed
low byte of property1. Five-slot tables use unsigned `JA 4`; other paths use
signed negative and 3/4 comparisons. They cannot be replaced by a single generic
grammar rule. Some `Ein` branches copy an empty prefix and fall through to
uppercase; other selections leave without that conversion. Unknown nonempty
prefixes erase the first caller byte, then continue to suffix processing.

`0x00675A12` is an out-of-line suffix entry. Prefix mismatch branches jump around
it and continue comparing the captured prefix. Physical instruction order
alone therefore does not establish which argument a later comparison uses.

## Captured property2 and repeated suffix handling

Suffix entry captures its argument, compares the original literal
`default scope` at `0x009F058C`, then captures the **signed low byte of property2
once** into BL at `0x006776C7`. Later suffix rules use that captured byte, even
when a CRT fixture mutates the incoming property DWORD. The native worker keeps
an explicit signed `property2` value with the same lifetime.

`default scope` selects the ten-pointer table at `0x00680BD0` for unsigned
indices 0..9 and copies the selected literal into **shared output**, not the
local word or suffix. Unsupported indices skip the copy. It then continues at
`0x0067A683`. The first-pass `e`, `es`, `en`, `en2` rules also use ten-slot tables.
Other first-pass paths use the original signed comparisons and five-slot data.

Many successful first-pass suffix rules copy through `0x0067A666` and continue
into the **second suffix pass** at `0x0067A683`. Repeated qualifier comparisons
there are reachable and can overwrite an earlier suffix. The source retains
both passes and the trailing bytes from each copy. An unmatched first pass sets
the DWORD at `P+0x14` to zero at `0x0067A62F`; final unknown-suffix handling at
`0x0067B4EB` erases a nonempty caller suffix only when that flag is zero.
The complete fixture set contains **115 distinct suffix keys**, including
`default scope`; comparison occurrences are retained without deduplication.

The `s` path sign-extends cached BL and accepts unsigned indices 0..100 into the
101-byte selector at `0x00680CA8`, then reads one of four DWORD targets at
`0x00680C98`. Selector values 0/1/2/3 occur 3/2/1/95 times. Targets copy `s`,
copy an empty suffix, perform word-ending logic, or copy a space. The ending
target `0x0067843D` returns **EAX zero for an empty local word**. For a nonempty
word it formats an empty suffix after last `S`/`s`, otherwise `s`, and then
continues into the second pass. Other paths reach common composition and its
dynamic language reread/postprocessing; this early return bypasses both.

The harness watches **204 original table regions**: the previous 103, the
98 new EAX-indexed tables (93 five-pointer and five ten-pointer), the two special
selector regions, and helper B's dictionary. Data tables still contain original
code pointers. Native switches map their verified values to recovered C labels.
Arbitrary edits introducing unsupported original-code targets are outside this
immutable-table contract and trap explicitly. Original strings and table data
remain dependencies.

## Reproduction and acceptance boundaries

`CM0102/Analysis/morph-three-static-a` pins the full original region and literal
bytes. `morph-three-dependencies-a` and `morph-three-selector-a` record original
tables and helper evidence. `summarize-morph-three.py` produced
`morph-three-summary-a`, collapsing 237 string-comparison idioms while retaining
their original equal/miss destinations. `generate-morph-three.py` verifies and
pins that summary report and matcher artifact as semantic inputs, along with
the original binary, tool versions, command, static report and generated outputs.
`morph-three-generated-d` contains the final native source, frozen generator and
`lowering.json` crosswalk, including every joined fallthrough block.

The initial generator run `morph-three-generated-a` failed on a shared default
suffix target `0x0067FD0E`; no application differential mismatch occurred.
Generation B succeeded. C joined basic blocks and closed semantic-input
provenance. D renamed a local variable after reading the actual literal bytes.
All diagnostic generation artifacts remain local and retain their own hashes.

An early focused fixture called the qualifier `Genus`; that text is unknown to
this ladder and did not exercise `default scope`. Its native comparison always
used the correct original literal VA. The corrected full corpus uses
`default scope`; the independent Spec review additionally passed 674 actual
default-scope/property/alias/callback comparisons. Preserve this diagnostic
distinction when interpreting the initial focused-a 3,285-case report and the
reviewer's earlier 491 comparisons. The corrected focused-b 3,285-case report
exercises the actual `default scope` qualifier.

Full `morph-three-final-a-O0/O2` reports each pass **208,504 comparisons**,
including 117,361 additional three-language cases and all previous 91,143 inputs.
Twelve tested inputs are frozen under `morph-partial-source-used-three`; revision
seven and its passing `morph-three-acceptance-audit-a` preserve six earlier
acceptance pairs against their frozen sources. The eight full-routine ledgers remain
unchanged. The 48,128 prefix-property and 58,880 suffix-property cases cover all
256 bytes with two high-bit patterns. Selector, helper dictionary, independent
properties, aliases, conversion, slot/property mutation, dynamic language, output
replacement, long words and seeded cases cover the distinct control paths.

Comparisons check external CRT events, frame `P+0x10..P+0x2813`,
all 2KB shared output bytes, dynamic language state, every pinned table region,
the full 16KB caller arena and incoming argument bytes. The relocated probe
return-address DWORD is excluded. The fixture batches adjacent table reads and
resets while retaining the same per-table byte comparison contract.

The original `0x00945610` stack probe still executes. CRT print, upper, lower
and find remain explicit fixtures. Recovered languages must execute no original
parent body or original helper A/B instruction. Initial languages outside
0/7/10/8/6/2/4/3 transfer with original frame/register state to `0x0067B506`.
Ten neutral residual cases establish selected transfers, not recovery of every
remaining body. General faults, overflow, all aliases, concurrency, Windows
guard pages/SEH, real CRT behavior and live game integration remain excluded.

Language nine has since entered maintained source and passed the full combined
corpus; see [LANG-MORPH-NINE.md](LANG-MORPH-NINE.md). Continue with
[language five's isolated candidate](LANG-MORPH-FIVE-DISCOVERY.md) at
`0x0067C2EE`, and close shared/default paths before whole-parent acceptance.
M2 replay/GDI mapping, M4 live integration and M6 standalone remain open.

## Final review and preservation

Final independent Standards and Spec reviews found no hard violations,
actionable smells or material fidelity/acceptance/documentation findings.
Standards checked 130 current/historical source identities and 80 artifacts;
Spec independently recomputed all sixteen acceptance audit fields. Both verified
the 208,504-case O0/O2 pair, twelve current/frozen inputs, regional/manual/table
accounting, helpers A/B, six earlier acceptance pairs and eight unchanged
full-routine ledgers. The semantic matcher-input provenance finding is closed.
At this historical review, language nine was an isolated, unpromoted candidate.

Python syntax checks pass. Both accepted C/assembly builds
use warnings as errors. Preservation verification again passes all 1,558
baseline files and 46 official patch payloads; runtime-created additional files
are reported separately. The workspace is not a Git repository; no commit,
push or PR was made. Binaries, supplied data and diagnostic artifacts remain
local. No recovered code is integrated and the standalone game remains unfinished.
