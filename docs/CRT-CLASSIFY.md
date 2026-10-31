# Maintained CRT classification support

Primary input: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
All addresses are preferred-image VAs, base `0x00400000`. Body
`0x0094C322..0x0094C396`: **117 bytes / 46 instructions**.

`recovery/crt/cm_crt_classify.c` and `.S` are byte-identical to the reviewed
[candidate](CRT-CLASSIFY-DISCOVERY.md); the header provides a 32-bit x86 GCC
cdecl entry. O0 and O2 each pass **11,788 comparisons**. The maintained driver
only changes source paths, description and header pin; it retains the complete
candidate corpus. Six current/frozen inputs are pinned in
`crt-classify-maintained-source-used`, with build commands, tool identities,
artifacts and report hashes. `recovery/crt/classify-ledger.json` accepts one
**CRT/locale** entry, independently from application counts and adoption.

The unsigned `(value+1)<=256` fast path includes `0xFFFFFFFF` and bytes 0..255,
reads a table WORD and ANDs with the mask. Other words inspect the lead flag,
construct a one/two-byte local retaining original incoming-ECX bytes, and call
`0x0094ED85` with output pointing into the caller's value high WORD. Zero result
returns zero; nonzero result reads that WORD and reloads the mask. Partial/absent
callback output, mask mutations and retained temporary bytes are compared.
The detailed frame/branch evidence remains in the candidate note.

`0x0094ED85` remains an explicit boundary fixture. Table initialization/data
closure for mutable pointer `0x00AD8FD0` also remains unrecovered. This helper is
not dependency-free locale support. Caller-saved registers/flags, private worker
stack, arbitrary faults, concurrency and Windows locale/SEH behavior are outside
the finite normal-return contract. No original application instructions execute
on the recovered side; the deeper entry is intercepted before its instructions.

Independent review previously added 144 candidate comparisons. The maintained
acceptance preserves that candidate and its unaccepted historical audit; source
equivalence and new maintained runs establish this distinct acceptance.

```powershell
python scripts/verify_crt_classify.py --output CM0102/Analysis/crt-classify-recheck-audit
python recovery/crt/test_cm_crt_classify.py --optimization O0 --output CM0102/Analysis/crt-classify-recheck-O0
python recovery/crt/test_cm_crt_classify.py --optimization O2 --output CM0102/Analysis/crt-classify-recheck-O2
python scripts/map_morph_search_family.py --output CM0102/Analysis/crt-classify-recheck-map
```

Use fresh folders. The current twelve-ledger map contains **45 application plus
five CRT entries**, seventeen pinned inputs, 16,337 selected calls and three
separate tails. Eight artifacts match across fresh final-a/b runs; the additional
dependency artifact records the deeper fixture and unadopted state. Morphology
uses the [accepted probe/search-composed parent](LANG-MORPH-SEARCH.md) once via
`scripts/map_morph_search_family.py`; neither case conversion nor morphology
implicitly adopts this classification helper. Validate those links separately,
recover deeper locale/formatting support and relocate data. M2 replay, M4 live
integration and M6 standalone game gates remain open.
