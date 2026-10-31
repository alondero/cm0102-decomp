> **Historical pending-stage record.** The replacement O0 controller completed
> with exit zero/PASS; the separate relink is now accepted at 323,545 comparisons
> per O0/O2. See [current evidence](LANG-MORPH-RELINKED.md). Earlier pending
> observations below remain historical, not current instructions.

# Recovered-probe morphology relink: acceptance pending

Primary: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`;
hash-identical copy `CM0102/Analysis/baseline-c/cm0102.exe`.
Addresses are preferred-image VA, image base `0x00400000`.

The [existing full parent](LANG-MORPH-PARENT.md), ledger and fourteen inputs are
unchanged. `cm_lang_morph_relinked.S` differs only in its probe call/comment.
It links [maintained compiler support](STACK-PROBE.md); shared C/data/body inputs
are identical. Its driver pins fifteen inputs, checks the compiled 47-byte
probe/call target and requires one recovered-probe entry per comparison.
Original application/probe instructions are prohibited; four CRT hooks remain.

`morph-relinked-final-a-O2/report.json` passed full **323,545 comparisons**.
Quick O2 passed 316,751; most property sweeps remain in quick mode. Neither
result supplies the missing full O0 evidence.

The first O0 handle and matching process disappeared without a final report.
`morph-relinked-final-a-O0` artifacts are preserved and **not accepted**.
A detached hidden runner records their hashes and executes fresh full O0 in
`morph-relinked-final-b-O0`. Controller/log/exit evidence lives in
`morph-relinked-O0-control-b`. Inspect its child PID with `Get-Process` and the
matching command line; a running JSON marker alone is insufficient. Do not
restart after an observation timeout.

Only after controller exit zero and terminal full report PASS:

```powershell
python CM0102/Analysis/accept-morph-relinked.py
python scripts/verify_morph_relinked.py --output CM0102/Analysis/morph-relinked-acceptance-audit-a
python scripts/map_morphology_relinked_family.py --output CM0102/Analysis/morph-relinked-map-final-a
python scripts/map_morphology_relinked_family.py --output CM0102/Analysis/morph-relinked-map-final-b
```

Prepared acceptance preserves prior full/partial stages, fifteen inputs,
compiled probe and interrupted-run provenance. It counts the parent once.
The prepared ten-ledger relink map omits accepted string support; the current
eleven-ledger map records that support with the preserved original-probe parent.
Update final relink docs/maps only after acceptance and review. No executable
patch, guard-page/SEH integration, M2 replay, M4 integration or M6 standalone
build is established. Original data and CRT fixtures remain required.
