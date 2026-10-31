# Bounded GDI replay observations

This extends [the original reference](RUNTIME.md) without accepting deterministic
replay or M4. Primary `CM0102/Game/cm0102.exe` SHA-256 is
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`;
GDI `CM0102/Game/cm0102_GDI.exe` SHA-256 is
`0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b`.
Static addresses below are preferred-image VAs, base `0x00400000`.
Each live observation also records its actual module base and runtime VAs.

## Cross-binary mapping

`scripts/map_rng_gdi.py` verifies four complete instruction envelopes and
204,031 constant bytes on hash-checked copies in `rng-gdi-map-b`. The first
attempt remains preserved: the ten-byte seed store had two masked matches;
the final selection uses the already decoded caller target. It is not selected
by an assumed fixed address displacement.

| Primary VA | GDI VA | Checked body bytes |
| --- | --- | ---: |
| Bounded RNG `0x0090CFC0` | `0x0090C860` | 221 |
| Initialization `0x0090D0A0` | `0x0090C940` | 134 |
| LCG store `0x009469FA` | `0x0094625A` | 10 |
| LCG step `0x00946A04` | `0x00946264` | 30 |

The table, scale and seed-format constants agree. External time, formatting and
floating-point conversion callees only have corresponding referenced addresses;
their bodies remain unvalidated. No whole-binary equivalence follows.

Local `CM0102/Analysis/map-phase-gdi.py` verifies the 56-byte / sixteen-instruction
transition fragment starting at primary `0x005CDC16`, producing
`phase-gdi-map-b`. It maps phase `0x00AE2C88` to GDI `0x00AE1BD8` and Date
`0x00AE2C90` to `0x00AE1BE0`. Date/orchestration callee bodies are not covered.
The failed first assertion expecting seventeen instructions is retained.

`scripts/verify_replay_mapping.py` provides a fresh byte/constants audit and
current imported-helper pins in `replay-mapping-audit-a`. Independent spec review
rechecks all 141 instruction records. This supplement does not reconstruct
historical support-tool provenance or attest live instruction bytes.

## Prepared scenario and observer

`CM0102/Runtime/replay-20261003-a` is an unpatched disposable copy with the forty
formation assets repaired additively. `prepare_reference_replay.py` verifies all
814 baseline/repair inputs and installs the two hash-locked reference saves and
configuration. The original ISO is mounted. The game UI changed its remembered
save directory to the disposable runtime. No supplied save was overwritten.

The July-28 save now restores successfully. Exploration advanced to August 4
and saved separately as `replay-learning.sav`. Exploratory UI steps and the
earlier `formal-a` attempt are not used as a parity run. Initial automation used
only a Runtime-path guard; exact-PID targeting was added after review. Actual
earlier tools are frozen in `replay-input-tools-before-guard-a` and the earlier
RNG reader is in `replay-rng-first-source-used`.

`capture_replay_rng.py` checks the on-disk GDI hash, maps the actual module base,
and reads the mapped LCG/mix/cursor/end, cursor word, Date and phase. It does not
pause or write the process. Readings are sequential, not atomic or complete world
state. On-disk hashing does not independently attest live executable text.

The posted-input utilities require the expected PID/runtime/GDI hash and record
PID, creation FILETIME, path/hash and HWND. A held process handle and checks before
each message/capture detect termination or changed window ownership. Partial
failures retain successful posts and the error. Restart creates a new process;
the resulting incomplete restart logs are expected observations, not discarded
failures. Capture records pin the helper, PowerShell script and version.

## Two observed sequences

Frozen output: `CM0102/Runtime/replay-20261003-verified-b/comparison.json`.
`record_replay_comparison.py` rechecks the 814 runtime inputs, original saves,
process joins, all message payloads and 112 screenshot hashes, then freezes the
two runs, raw saves, action logs and nine source files.

Both fresh processes restored `reference-start-july28.sav`. Each received 56
Continue requests at client `(45,119)` as the same ordered three-message payload.
Their actual intervals differ; posting success does not acknowledge consumption.
Restart-inherited command lines were not independently captured. The frozen
`game.cfg` is post-run configuration, not an initial-state snapshot.

| Observation | Run b, PID 79440 | Run c, PID 81356 |
| --- | --- | --- |
| Actual module base | `0x00400000` | `0x00400000` |
| Initial LCG / mix | 3357800067 / 18467 | same |
| Initial cursor / end | 11333436 / 11350136 | same |
| Initial Date / phase bytes | `d000d10700000000` / `00` | same |
| Final displayed boundary | 4 August 2001 EVE, squad-number request | 30 July 2001 AM, Cygan bid |
| Final LCG / mix | 987692451 / 15070 | 1548577255 / 23629 |
| Final cursor | 11254808 | 11184092 |
| Final Date / phase bytes | `d700d10700000000` / `02` | `d200d10700000000` / `00` |
| Raw save bytes | 18,685,744 | 18,316,433 |

The first eight PNGs match exactly. PNG nine differs; by request ten there is a
visible semantic difference: run b remains on "Barcelona set to buy Henry"
while run c advances to "Barcelona seal Vieri deal". Run b's ninth and tenth
PNGs have identical hashes. Both runs remain at July 28 at that point. This
locates a visible input/capture discrepancy before the displayed calendar advances. It does
not identify which handler, input or timing condition caused it.

Raw saves differ first at file offset 1888 and have distinct sizes/hashes:

- `replay-clean-b.sav`: `209a83a705743c68bf04a6e576dbf3d173ad3d104228d6547b1efa2b15443902`.
- `replay-clean-c.sav`: `4a8d0ea50b50048c2332bc663b4a68f6a8fcfc6a5b38a9fe57a4036325744b91`.

No bytes were normalized. These are divergent observed sequences, not proof of
engine nondeterminism under identical consumed inputs. Ranked hypotheses are
busy-processing/input consumption, timer/worker effects from differing pauses,
and unobserved restored state. The next replay must acknowledge stable semantic
boundaries and retain every retry, then compare calendar/match/save outcomes.
M2 remains partial; no hybrid replacement or standalone game is accepted.

## Reproduction

```powershell
py -3.13 scripts/map_rng_gdi.py --output CM0102/Analysis/rng-gdi-map-NEW
py -3.13 scripts/verify_replay_mapping.py --output CM0102/Analysis/replay-map-audit-NEW
py -3.13 scripts/capture_replay_rng.py --pid ACTUAL_PID --phase --output CM0102/Runtime/NEW-state.json
py -3.13 CM0102/Analysis/replay-continue-batch.py --pid ACTUAL_PID --runtime CM0102/Runtime/DISPOSABLE --count 10 --output CM0102/Runtime/NEW-batch
```

The observer currently pins the reviewed `rng-gdi-map-b` / `phase-gdi-map-b`
reports and scripts. Keep them intact. Review source before changing those pins.
Use new output names; retain failed runs and their actual source inputs.
