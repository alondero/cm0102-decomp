# M2 runtime reference

## Demonstrated on 2026-09-27

The unpatched GDI executable now completes a new game, save/load, calendar
progression and simulated matches on a disposable copy. **M2 remains partial:**
repeat-run determinism, replay tooling and primary-executable runtime are open.

The [2026-10-03 replay extension](RUNTIME-REPLAY.md) now restores the July-28 save,
maps the selected RNG/Date globals between binaries, and freezes two guarded
56-request sequences. Their endpoints differ; visible captures
already differ while both displayed dates remain July 28. Input consumption
remains unacknowledged, and M2 remains partial.

Input executables (preferred image base `0x00400000`):

- Primary `CM0102/Game/cm0102.exe`, SHA-256
  `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
- Runtime `CM0102/Runtime/gdi-england/cm0102_GDI.exe`, identical to preserved
  `CM0102/Game/cm0102_GDI.exe`, SHA-256
  `0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b`.

Host: Windows 11 Pro Insider Preview, version 10.0.28120, build 28120.
Working directory: `CM0102/Runtime/gdi-england`. Command:
`cm0102_GDI.exe -windowed -seed 1`. The restored process (PID 34084 at capture)
had actual module base `0x00400000`. The supplied ISO remained mounted; no CD or
community patch was applied. Build string: 3.9.68, Nov 21 2002 17:16:15.

## Root cause of the earlier failure

The cabinet group named `Tactics` was incorrectly reconstructed into `Tactics/`.
The formation loader requests the supplied `.pct` files from `Data/`.

On the unrepaired copy, Next produced `v3.9.68 formation..cpp 2272`
(`after-next.png`). Read-only process memory inspection found
`load_from_disk() : unable to open the file (data\352_v1.pct)` at absolute
runtime address `0x001ADE14`, and the path at `0x00DE16E4` (PID 34220).
That process's module base was not separately captured. These observations are
not a reusable primary-binary address mapping. Acknowledging the error produced
`tactics..cpp 146`; that partially initialized process was stopped.

The corresponding **preferred-image VA** branches were inspected separately:

| Evidence | Primary | GDI |
| --- | --- | --- |
| Formation-loader entry | `0x005B5D10` | `0x005B5E40` |
| Push line 2272 (`68 e0 08 00 00`) | `0x005B5E17` | `0x005B5F47` |
| File-open error format | `0x009CC76C` | `0x009CB76C` |
| Embedded `formation.cpp` path | `0x009CC3DC` | `0x009CB3DC` |

The 275-byte prefixes have matching mnemonic sequences and diagnostic strings,
including mode `rb`, a null-result check after the open call, and the error
branch. Absolute addresses and calls differ. This is a local diagnostic
correspondence, **not whole-function or cross-binary equivalence**.
`scripts/inspect_formation_failure.py` reproduces disassemblies and hashes under
`CM0102/Analysis/formation-failure/` using Capstone on copied inputs.

`scripts/repair_runtime_formations.py` copied all 40 preserved `.pct` assets into
the disposable copy's `Data/`, checking each against the existing baseline
manifest and recording `formation-layout-repair.json`. A fresh process then
loaded the database without that error. No executable bytes, preserved files or
baseline manifests changed. The repair also accepts already-correct Data layouts.

**Preservation is not installation correctness.** The 774-file historical Game
manifest still passes, but its directory layout contains this defect.
`scripts/prepare_game.py` now maps the cabinet group to `Data` for future empty
reconstructions. Do not rerun over the historical baseline or regenerate its
manifests. The pre-correction script SHA-256 was
`18830233dd2124380ab09fe7bb15539ec976c20af19e0572afe4a69b5336c6cc`.
The corrected full reconstruction has not been rerun; the additive runtime repair
is the exercised path.

## Reference scenario

Frozen corpus: `CM0102/Runtime/reference-20260927-verified/` (the earlier capture
`reference-20260927/` is also retained). `reference.json` records
process/command/platform/tool identity, 814 runtime input hashes (774 historical
baseline files plus 40 formation copies), and 14 artifacts. Configuration is
retained as `game.cfg`. English UI; database Minimum was observed in the earlier
settings setup. England through Conference; other countries inactive; real
players and attribute masking enabled.

1. Create **Reference Manager**, English nationality, take control of **Arsenal**,
   Finish. `new-game-complete.png`: 14 July 2001 AM, appointment news, board
   expectations and fixture-list notification.
2. Game Options > Save Game as `reference-start.sav`.
3. Restart Game, restore `reference-start`, acknowledge the CD notice.
   `restored-start.png` shows the same manager, club, date and news. Selected
   news row/background can differ; this is not screenshot pixel parity.
4. Reference Manager > Go on Holiday with default options. Click during holiday
   to request return; confirm Yes. This run stopped on 28 July AM. Stopping time
   was manual, not deterministic input replay.
5. Arsenal > Fixtures shows friendlies, from Arsenal's perspective:
   19 July Trabzonspor away **0:1**, 21 July C. Rizespor away **0:0**,
   23 July A. Hopaspor away **1:0**. The first result shows Trabzonspor **1–0**
   Arsenal, shots **9–7**, shots on target **2–5**, player ratings/substitutions.
   Evidence: `fixtures-july28.png`, `match-result.png`, `match-statistics.png`.
6. Save separately as `reference-start-july28.sav`; retain the starting save.

| Save | Bytes | SHA-256 |
| --- | ---: | --- |
| `reference-start.sav` | 17,089,661 | `efbd92e5b32d86ea9031aba1832bcf4a7f2296ccea9e01134d126ed869341cc8` |
| `reference-start-july28.sav` | 18,178,152 | `96dae5260c9bceb46a79c341e26c0bf1a27afed93da123236cb1f0e2df5f89e5` |

The second save was written/hashed, not yet restored. Holiday simulation
establishes completed matches, not an interactively managed match. Repeated-run
comparison and normalized save semantics remain open. `-seed 1` still permits
the initial time-seeded splash described in [RNG-INIT.md](RNG-INIT.md).

## Reproduction and interaction

```powershell
python scripts/prepare_runtime.py --output CM0102/Runtime/new-reference
python scripts/repair_runtime_formations.py --runtime CM0102/Runtime/new-reference
# Launch from that directory with -windowed -seed 1; keep the parent shell alive.
python scripts/gdi_interact.py info
python scripts/gdi_interact.py capture --out CM0102/Runtime/new-reference/screen.png
python scripts/gdi_interact.py click --x 275 --y 177
python scripts/gdi_interact.py text --text Reference
```

Inspect each screen before proceeding. Here the whole-window capture includes
a 26-pixel title bar and 3-pixel left border; clicks use **client pixels**.
The client is 800x626, with a 800x600 game region. The utility now sends each
mouse click once, supports ASCII text/key input and refuses game processes
outside `CM0102/Runtime`. The old helper's duplicate native/posted clicks were
a separate automation defect, not the established cause of the formation error.

Grey SELECTED/BACKGROUND labels are inactive choices; yellow with a border is
active. Earlier notes saying every league was selected were incorrect.
De-Select All then England SELECTED produces the small scenario.

Keep a long-lived shell; short-lived agent jobs can terminate launched processes.
If launched hidden, show the window (`ShowWindow(hwnd, 5)`) before interacting;
Restart Game inherited that hidden setting here. Capture with `PrintWindow`
flag 2; flag 0 can yield black output.

`scripts/record_runtime_reference.py` freezes the specifically named artifacts
with a current PID and new output directory. It is pinned to the two known save
hashes to prevent attaching these observations to a different scenario. It does
not infer semantic state from save bytes. All derived artifacts stay local.

Next: restore the July 28 save, replay a fixed interval twice from the start
save, map observable state/RNG between GDI and primary, and characterize
differences before M4 replacement.

## Session closeout

The game was exited after both saves and the final corpus capture. Five
filesystem regression tests cover historical and corrected layouts, idempotence,
tampered source, conflicting destination and refusal to target the preserved
installation (`python -m unittest discover -s scripts/tests -v`). Python scripts
compile; Ghidra compiled/executed its exporter twice. Final baseline verification
passed all 1,558 preserved files and 46 patch payloads.

Separate standards/spec reviews found two tooling issues, both corrected:
pinning scenario observations to known saves, and accepting already-correct
formation layouts. The spec review found no unsupported recovery/milestone claim.
No repository/remote exists; nothing was committed or published.
