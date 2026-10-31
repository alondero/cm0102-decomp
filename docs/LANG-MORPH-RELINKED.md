> **Preserved probe-only acceptance.** The [current morphology stage](LANG-MORPH-SEARCH.md)
> separately adopts compiled search after full O0/O2. This fifteen-source
> acceptance remains unchanged; current inventory uses `scripts/map_morph_search_family.py`.

# Accepted morphology with recovered compiler support

Primary input: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Addresses are preferred-image VAs, base `0x00400000`. The unchanged complete
application body is `0x006701F0..0x0067FDC0`: 64,465 instruction bytes and
20,658 records. This stage changes its compiler-support dependency.

`recovery/lang/cm_lang_morph_relinked.S` replaces only the original
`call 0x00945610` with `call _cm_stack_probe` plus a provenance comment.
Shared application C, recovered tables and language bodies remain unchanged.
`recovery/lang/morph-relinked-ledger.json` preserves prior full acceptance and
partial-stage history; the prior original-probe ledger is not rewritten.

Full O0 and O2 each pass **323,545 comparisons**, with the exact prior
152-group corpus. Each recovered invocation executes the compiled 47-byte,
17-instruction [stack probe](STACK-PROBE.md) once. The harness prohibits original
application and probe instructions. Only four CRT entry hooks are permitted:
format `0x00944C9F`, upper `0x009471CD`, lower `0x009465FC` and substring
`0x00945A60`. Original data and these modeled CRT boundaries remain required.
Accepted [string search](CRT-SEARCH.md) is not yet linked into morphology.

Fifteen current/frozen source inputs are pinned in `morph-relinked-source-used`.
Per-build `probe-link.json` pins the actual call bytes/target, compiled helper
bytes and all seventeen relocated instruction semantics. The acceptance audit
also verifies PE bytes, prior accepted inputs, tools, artifacts and source identity.

The first O0 run ended without a report; its artifacts remain preserved and
unaccepted. The replacement hidden controller `morph-relinked-O0-control-b`
finished with exit zero and terminal PASS, matching full O0 report hash and logs.
No process remains pending for this accepted pair. Historical running snapshots
in support closeout records retain their original point-in-time meaning.

The preserved acceptance-stage map uses `scripts/map_morphology_relinked_search_family.py`:
eleven ledgers, sixteen pinned inputs, **45 application plus four CRT entries**,
16,311 selected calls and three separate tail transfers. Seven final-a/b artifacts
are byte-identical. The parent VA is selected once; support acceptance does not
increase application coverage. Prior maps are retained unchanged.

```powershell
python scripts/verify_morph_relinked.py --output CM0102/Analysis/morph-relinked-recheck-audit
python scripts/map_morphology_relinked_search_family.py --output CM0102/Analysis/morph-relinked-search-recheck
```

Use fresh folders. The prepared ten-ledger `map_morphology_relinked_family.py`
omits maintained string support; the twelve-ledger `scripts/map_crt_classify_family.py` is now the current
combined inventory, adding maintained classification as separate CRT support. Full tests can be repeated with
`recovery/lang/test_cm_lang_morph_relinked.py --optimization O0` or `O2` and a
fresh `--output` folder, preserving inputs during each run.

Next validate a separate link to maintained substring/character search. The
[locale candidates](LANG-MORPH-CRT-DISCOVERY.md) and
[maintained classification](CRT-CLASSIFY.md) remain unadopted by morphology; deeper
classification/mapping, formatting and original data closure remain open.
No live integration, Windows guard-page/SEH equivalence, M2 replay, M4 integration
or M6 standalone game build is established.

[Search composition](LANG-MORPH-SEARCH-PENDING.md) is now under detached full
O0/O2 validation after focused passes. [Classification](CRT-CLASSIFY.md) is
maintained and separately accepted, but case/morphology adoption remains pending.
