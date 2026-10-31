> **Historical pending-stage record.** Both full controllers completed with
> exit zero/PASS. The [search composition is accepted](LANG-MORPH-SEARCH.md) at
> 323,545 comparisons per O0/O2 and seventeen frozen inputs. Earlier snapshots,
> PIDs and pending statements below are historical, not current instructions.

# Pending morphology composition with maintained string search

Primary: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
All addresses are preferred-image VAs, base `0x00400000`.
The application is still the complete `0x006701F0..0x0067FDC0` body.
Its [accepted recovered-probe stage](LANG-MORPH-RELINKED.md) remains unchanged.

The separate `cm_lang_morph_search.c` only adds the maintained search header and
binds FIND to `cm_crt_strstr`; it retains all application instructions/behavior.
The new harness builds the accepted probe and search assembly. Seventeen source
inputs are pinned; the previous fifteen current/frozen inputs remain unchanged.
`search-link.json` verifies all 155 original search semantics after relocation,
including shared exit/internal character entry: 321 unique original bytes,
318 compiled bytes. Per-build probe links are checked separately.

Focused O0 and O2 each pass **1,645** cases. Each records 5,883 compiled search
calls and post-entry body starts, across 1,221 cases executing search. Six
artificial forced-return/property-mutation cases instead record **32 fixture
calls and zero compiled search executions**. The full original corpus retains
those cases; they are not silently dropped or counted as executing real search.
Independent review added 54 composition cases (111 compiled starts and twelve
forced calls). Reference-side find still uses the existing boundary fixture;
the separate [search acceptance](CRT-SEARCH.md) supplies original instruction,
ordered-access and selected fault evidence. This composition compares full
application result/state/callback events under that explicitly retained reference
boundary contract, not a new whole-memory search-access or general-fault proof.

Full O2 now passes **323,545 cases / 152 groups**, with controller exit zero/PASS,
202,868 compiled search calls/body starts across 45,716 cases, and the same
32 artificial calls in six cases. Full O0 and joint acceptance remain pending.

Full O0/O2 run controllers are `morph-search-O0-control-a` and
`morph-search-O2-control-a`. At launch the controllers were PIDs 38696/80308,
children 16824/52640. Inspect actual PID, creation time and command line plus
controller/report state before inferring liveness. Never restart solely because
a poll times out or a running marker exists. Inputs remain immutable while live.

```powershell
Get-CimInstance Win32_Process -Filter 'ProcessId = 38696 OR ProcessId = 80308 OR ProcessId = 16824 OR ProcessId = 52640' | Select-Object ProcessId,CreationDate,CommandLine
Get-Content CM0102/Analysis/morph-search-O0-control-a/run.json
Get-Content CM0102/Analysis/morph-search-O2-control-a/run.json
```

Only after **both** controllers finish with exit zero/PASS and each full report
passes all **323,545 cases / 152 groups**, identical seventeen sources and matching
metrics, run:

```powershell
python CM0102/Analysis/accept-morph-search.py
python scripts/verify_morph_search.py --output CM0102/Analysis/morph-search-acceptance-audit-a
```

Prepared acceptance gates check full scopes, frozen prior acceptance, sources,
artifact/tool identities, actual compiled probe/search links, exact application
C transformation, case/metric partition and both terminal controller/log/report
identities before writing a new ledger. Preserve the accepted parent and count
its VA once. After acceptance, prepare a fresh twelve-ledger inventory replacing
the prior parent selection with the search-composed parent; retain prior maps.

No search-composed acceptance ledger exists yet. Three steady native CRT fixtures
(format/upper/lower), the six artificial find cases and original data remain
explicit. [Maintained classification](CRT-CLASSIFY.md) is accepted separately and
still unadopted. No original application/probe/search instructions are permitted
on the recovered side. Primary replay, live integration, Windows SEH and the
standalone game build remain unvalidated.
