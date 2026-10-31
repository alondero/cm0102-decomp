# Accepted morphology with recovered probe and string search

Primary input: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
All addresses are preferred-image VAs, base `0x00400000`. Application body
`0x006701F0..0x0067FDC0` remains **64,465 bytes / 20,658 instruction records**.
This stage replaces a dependency, adding no application entry.

`cm_lang_morph_search.c` changes only the search header and FIND binding from
the prior application C. The existing recovered-probe frame adapter, shared
language bodies, table helpers and data descriptors remain unchanged. The
accepted ledger is `recovery/lang/morph-search-ledger.json`, with seventeen
current/frozen inputs in `morph-search-source-used`. Prior application/probe
acceptance and its interrupted attempt remain preserved and unmodified.

Both full O0 and O2 pass **323,545 comparisons / 152 groups**. Each records
**202,868 compiled search calls and post-entry body starts** across **45,716
cases executing search**. Each application invocation executes the recovered
47-byte/17-instruction compiler probe once. Native original application, probe
and search instructions are prohibited. The linked search closure verifies
all 155 original instructions after relocation: 321 unique original bytes,
318 compiled bytes, shared five-byte exit counted once and internal AL entry
retained. Per-build links, PE bytes, tools, artifacts and source identity are
checked by the acceptance audit.

Six original artificial find-return/property-mutation cases are retained as
**32 boundary-fixture calls, zero compiled search executions**. The three steady
native CRT fixtures are format `0x00944C9F`, upper `0x009471CD` and lower
`0x009465FC`. Reference-side find retains the original harness's boundary model.
Independent [search acceptance](CRT-SEARCH.md) establishes original instruction,
register/flag/access and selected fault fidelity. This full composition compares
application result/state/callback order under the retained reference contract;
it does not claim a new whole-memory search trace, universal fault equivalence,
or Windows SEH validation. The six artificial cases are neither dropped nor
counted as compiled search executions.

The detached full controllers both completed with exit zero/PASS, matching
report hashes, source pins and logs. They are no longer live. Focused evidence
remains separate at 1,645 cases/build, 5,883 compiled calls and 32 artificial
calls; independent review added 54 composition comparisons. Historical pending
notes, O2-only audit and running snapshots retain their point-in-time meaning.

The current twelve-ledger inventory uses `scripts/map_morph_search_family.py`:
**45 application plus five CRT entries**, seventeen pinned inputs, 16,337
selected calls and three separate tails. Eight final-a/b artifacts match byte
for byte. The parent VA is selected once; [classification](CRT-CLASSIFY.md) is
separately accepted and remains unadopted by case conversion or morphology.
The immutable library search ledger's adoption-false field records its original
acceptance point; this caller ledger supplies the separate composition evidence.

```powershell
python scripts/verify_morph_search.py --output CM0102/Analysis/morph-search-recheck-audit
python scripts/map_morph_search_family.py --output CM0102/Analysis/morph-search-recheck-map
```

Use fresh output folders. Full tests can be repeated with
`recovery/lang/test_cm_lang_morph_search.py --optimization O0` or `O2` plus a
fresh `--output`, keeping inputs immutable. `--focused-search` is preliminary
evidence and does not replace the complete corpus.

Next adopt/relink the case-conversion bodies, compose maintained classification
under explicitly preserved dependency scope, and recover deeper text
classification `0x0094ED85`, mapping `0x0094C0FE`, formatting and data closure.
Fixed original data remains required. Primary runtime replay, live integration,
Windows guard-page/SEH behavior and standalone game acceptance remain open.
