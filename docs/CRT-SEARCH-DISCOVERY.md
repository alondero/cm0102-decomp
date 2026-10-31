# Dependency-free substring and character search candidate

> Historical candidate evidence. [Maintained acceptance](CRT-SEARCH.md) supersedes candidate status; frozen reports/inputs remain unchanged.

Target: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` is hash-identical.
Addresses are preferred-image VA, image base `0x00400000`.

The substring entry at `0x00945A60` and character-search entry `0x00946540`
share a **321-byte, 155-instruction closure**:

| Original region, inclusive | Bytes | Purpose |
| --- | ---: | --- |
| `0x00945A60..0x00945ADF` | 128 | Substring front body |
| `0x00946530..0x00946534` | 5 | Shared unaligned character-match exit |
| `0x00946540..0x009465FB` | 188 | Character search and aligned DWORD loop |

The Ghidra substring body includes the remote five-byte exit; character search
branches to that exit even though it is outside its exported body. The shared
region is counted once in this closure. Fresh read-only exports
`morph-crt-static-a` and `morph-crt-dependencies-a` pin both sets of instructions.

`crt-search-native-candidate-a/cm_crt_search.S` is generated native assembly
with one provenance label per original instruction and relocated branch labels.
The generator verifies every branch target lies in the closure. It contains no
calls, global accesses, tables, external CRT fixtures, binary instruction replay
or fallback to original code. Assembly retains the exact flags, access widths
and order of the original unsigned arithmetic and DWORD detection loop.
This is an **isolated candidate, not maintained-source acceptance or adoption**.

Empty needle returns the haystack without reading it. A one-byte needle passes
its character in AL and tail-transfers to the internal character-search entry
`0x00946546`, bypassing the normal argument loader at `0x00946540`.
The public character-search entry uses only the low argument byte. Its initial
unaligned scan reads bytes until alignment; aligned scanning reads DWORDs and
uses the original 0x7EFEFEFF/0x81010100 zero/character detection sequence.
Reads can include bytes after a terminating NUL. Multi-byte substring matching
retains original scan restart and two-byte comparison order.

`crt-search-candidate-test-a/report.json` passes **44,423 comparisons**:

- 33,792 character-byte/high-bit/alignment/length cases;
- 8,320 substring prefix/overlap cases and 284 needle/haystack aliases;
- eight incoming-flags cases, including set DF;
- three empty-needle guards with unmapped haystack pointers;
- sixteen first unmapped-read CPU faults;
- 2,000 deterministic random byte-string cases.

The driver checks all general registers, ESP and flags, the entire stack/arena,
merged ordered data reads/writes and the first fault. Fault instruction addresses
are normalized through original/compiled provenance labels. It independently
checks returned pointers against byte-search results on ordinary valid cases.
The candidate machine maps only the compiled closure plus synthetic stack/input
memory; it does not map original executable code or data.

All 155 compiled instruction semantics are compared to the original through the
relocated branch map. The native encoding is 318 bytes; original closure coverage
remains 321 bytes. Independent review passed 96 additional comparisons.
`crt-search-candidate-audit-a/audit.json` independently
rechecks primary bytes, export/report/tool/artifact chains, compiled PE bytes,
instruction semantics and three frozen inputs in `crt-search-candidate-source-used`.
The original shared extent remains separately recorded rather than treating
the Ghidra function size as a contiguous range.

These finite CPU comparisons do not establish arbitrary pointer wrap, Windows
SEH or live game integration. Next promote the reviewed assembly and driver to
maintained CRT support, rerun that source, and relink substring search into a
new morphology build while retaining the existing callback-fixture acceptance.
Formatting, locale support and data closure still require recovery before a
standalone localization subsystem or game can be claimed.
