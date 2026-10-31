# Maintained substring and character search

Primary: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`;
hash-identical copy `CM0102/Analysis/baseline-c/cm0102.exe`.
Addresses are preferred-image VA, image base `0x00400000`.

`recovery/crt/cm_crt_search.S` and `.h` expose cdecl `cm_crt_strstr(haystack,needle)`
and `cm_crt_strchr(haystack,character)`. Only the low character byte matters.
`recovery/crt/search-ledger.json` accepts two CRT support entries, zero game
application entries, and no live integration.

The complete closure contains 128 bytes at `0x00945A60..0x00945ADF`, 188 bytes
at `0x00946540..0x009465FB`, and a shared five-byte exit at
`0x00946530..0x00946534`: **321 unique original bytes / 155 instructions**.
Entry hashes cover their 316 contiguous bytes; the shared exit supplies five
separately pinned bytes, counted once. The internal AL entry `0x00946546`
serves one-byte substring needles. Compiled native encoding is 318 bytes.

Assembly retains original alignment, DWORD reads, zero/character arithmetic,
scan restart, flags and access order. Empty needle skips haystack reads.
The native test machine needs no original executable code/data, globals, tables,
imports or external fixtures. All jumps are relocated through provenance labels.

`crt-search-maintained-final-a/report.json` passes **44,423 comparisons**:
33,792 character/high-bit/alignment/length, 8,320 substring prefix/overlap,
284 aliases, eight flags including set DF, three unmapped-haystack empty guards,
sixteen first unmapped-read faults and 2,000 random strings. All general
registers/ESP/flags, full stack/arena, merged ordered accesses and first fault
are compared. Ordinary returned pointers also match a byte-search oracle.
Four inputs including the 32-bit header are frozen; the header passes GCC
warnings-as-errors syntax checking. The audit verifies original/compiled
semantics, branch labels, export/report/build/tool/artifact/source provenance
and shared-byte accounting.

```powershell
python recovery/crt/test_cm_crt_search.py --output CM0102/Analysis/crt-search-recheck
python scripts/verify_crt_search.py --output CM0102/Analysis/crt-search-recheck-audit
python scripts/map_crt_search_family.py --output CM0102/Analysis/crt-search-recheck-map
```

Use new folders. This assembly has no optimization-dependent C-lowering claim.
Finite CPU faults do not prove Windows SEH, arbitrary wrap or live integration.
[Candidate history](CRT-SEARCH-DISCOVERY.md) includes 96 independent comparisons.
The preserved eleven-ledger selection is **45 application entries plus four CRT entries**,
16,311 selected calls and three tails, with seven byte-identical final-a/b
artifacts. `crt-search-closure.json` separately preserves the shared extent.
[Morphology now adopts compiled search](LANG-MORPH-SEARCH.md) under the
full seventeen-source composition contract. Three steady CRT fixtures, six
artificial find cases, the reference find boundary and original data remain
explicit. Formatting, locale support and data closure remain open.

The preserved recovered-probe eleven-ledger map is `scripts/map_morphology_relinked_search_family.py`,
selecting the [accepted recovered-probe parent](LANG-MORPH-RELINKED.md) once.
The preserved pre-composition twelve-ledger `scripts/map_crt_classify_family.py` adds [maintained classification](CRT-CLASSIFY.md),
for 45 application plus five CRT entries and 16,337 calls/three tails.
The current map is `scripts/map_morph_search_family.py`, selecting the
[accepted search composition](LANG-MORPH-SEARCH.md) once. Prior maps remain preserved.
