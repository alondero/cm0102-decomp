# Isolated primary-reference VM preparation

The primary target remains official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`,
preferred-image VA base `0x00400000`. The unchanged primary now renders its
settings, main menu and new-game league selection in the isolated guest.
No primary-runtime, M2, M4 or standalone acceptance follows from preparing it.

VirtualBox 7.2.8r173730 and an existing powered-off Windows 7 32-bit VM are
available locally. `prepare_reference_vm.py` made a full machine/disk clone named
`CM0102-reference-20261003-a`, UUID `ee5fd160-8940-4800-b732-8ecb225d3241`, beneath
`CM0102/Runtime/primary-vm-20261003-a`. This local ignored directory contains the
copied OS disk and must not be published. The original VM was never started.

The clone has one vCPU, all eight NICs disabled, clipboard/drag-and-drop/VRDE
disabled, audio disabled and EHCI disabled. A single read-only share `cmref`
exposes only `CM0102/Runtime/replay-20261003-a`. The supplied original ISO is
attached to the clone's DVD drive. No game installer or compatibility patch was
applied. The game will need a private guest-local working copy because the
shared input folder is read-only.

## Preserved failure and restoration

`preparation.json` records commands, times, VirtualBox/tool/script identities and
source/clone disk/configuration hashes. Its preservation check failed: VirtualBox
normalized the **source** VM's `<NAT/>` to
`<NAT localhost-reachable="true"/>` while cloning. The source OS disk remained
byte-identical. This failure is preserved, not relabelled as a pass.

The source primary configuration was restored byte-for-byte from its captured
copy. `source-config-change.diff`, `source-vm-config-after-clone.vbox` and
`source-config-restoration.json` retain the changed file, delta and restoration
check. A fresh `source-config-restoration-provenance.json` preserves the exact
executed restoration body/command and current interpreter/tool pins; it does not
rewrite the original restoration record. The actual preparation script used is frozen in `source-used` before its
subsequent revision. That revision reads the actual clone disk path and checks
source preservation in `finally`, including failed preparations; it has not
been rerun to produce another clone.

Fresh `primary-vm-audit-a/audit.json` independently passes **before boot**:
source configuration restored, source disk freshly hash-checked, clone disk
identity checked, every NIC disabled, exactly one read-only share and the correct
ISO. It does not erase the failed preparation or establish guest compatibility.

## Sign-in resolved and primary startup

`vbox_reference_action.py` targets only this audited clone, validates its UUID,
workspace path and disabled interfaces, and writes command records before acting.
It records partial failures. Headless boot succeeded. `boot-screen-b.png` /
`boot-screen-c.png` show Windows 7 Starter reporting:

> Your password has expired and must be changed.

The earlier request for user sign-in was unnecessary. On continuation, the
normal password-change form already contained the old credential for `vboxuser`.
Submitting blank new/confirmation fields succeeded in this isolated clone.
`login-form-a.png`, `login-result-a.png` and the corresponding action records
preserve that workflow without logging a credential. The desktop and a later
guest restart both succeeded. The existing source VM has VirtualBox unattended
installation files; who originally provisioned it remains unestablished.
The original VM remains powered off and its disk/configuration hashes still match.

Fresh `primary-input-20261003-a` is shared read-only as transient `cmprimary`.
Transient writable `cmout` exposes only `primary-vm-guest-results-a`, an initially
empty experiment-output directory. No virtual NIC, clipboard, drag-and-drop or
VRDE was enabled. Mapping these folders with `net use` worked after initial UNC
access failed. Each completed launch uses a private `C:\CM0102-*` copy; the
guest hashes all 830 recorded copy inputs, including the 814 baseline/formation
assets, the two frozen saves and configuration. Extra setup files are separately
hash-pinned. The first PowerShell 2/.NET 3.5 cleanup failure remains preserved;
the fresh helper uses `HashAlgorithm.Clear`.

The observed VirtualBox WDDM mode-list rows showed 32-bit modes; the screenshot
does not enumerate the complete list. The VirtualBox mode hint left the display
unchanged. Windowed launch
`verification-b` failed graphics initialization. Removing `-windowed` while
retaining the copied configuration (`primary-fullscreen-d`) still selected the
windowed path. Removing copied `game.cfg` after retaining it as evidence
(`primary-fullscreen-e`) reached the fullscreen path but also failed graphics.
The intervening `launch-c` failed input verification before any launch because
the earlier game startup had removed runtime-created root language files.
Fresh copies prevent reusing that changed directory.

Only in this disposable clone, Windows Device Manager installed the built-in
Standard VGA driver, version `6.1.7600.16385`; the guest was restarted. The fresh
display probe enumerated 23 modes, passed `CDS_TEST`, applied a dynamic display
change without a registry update, and read back **1024 by 768 by 16 bits**.
WMI's resolution fields were empty with this driver; the read-back comes from
`EnumDisplaySettingsW`, not those empty fields. Driver hashes, mode list and
support source are recorded under `display-probe-a` and its preparation.

`primary-windowed-f` verifies 830 copied inputs, retains/removes the copied
configuration, then launches `cm0102.exe -windowed -seed 1`, guest PID 1776 at
capture, actual module base **`0x00400000`**. The primary hash matches the pinned
target. With the clone's audio device disabled, startup reports sound warnings;
dismissing them permits rendering. The settings show **3.9.68, Nov 21 2002
13:10:31**, English, Minimum database, sound Off and Windowed screen mode.
The unchanged primary completed a new English/Minimum database game with Arsenal
as the controlled club. The first Reference Manager reached **14 July 2001 AM**;
see \`primary-new-game-complete-f.png\`. A starting save was made on the private
guest copy (\`primary-start-f.sav\`, 17,071,016 bytes, SHA-256
\`18a372b1c3cc629c8738325c7c2e748121d34ff544095316a9edc819dc6d35a1\`).

The game was restarted from its own options, then that save was restored in a
fresh fullscreen guest process, PID 160, module base \`0x00400000\`. The restored
Reference Manager displayed Arsenal and **14 July 2001 AM**. A sequential,
unsuspended six-field memory observation recorded the mapped primary RNG/date
globals. Date and phase matched the pre-save sample, while sampled LCG and mix
globals differed. This does not establish that those RNG fields are serialized,
or identify why the values differ. The saved and restored screens need not have
pixel identity because the news background differed.

From the restored game, the observed calendar advanced to **7 August 2001 EVE**.
Arsenal's fixtures showed friendlies Bradford (away) 1–0, Dundee United (away)
3–0 and Hearts (away) 1–2. Bradford's match page showed the 0–1 result, player
ratings, and statistics: shots 3–18, on target 0–9, off target 3–9, corners
0–6, free kicks 10–9, throw-ins 8–9, fouls 3–5, offsides 6–6, passes completed
70%–77%, tackles won 34%–70%, headers won 42%–58%, yellow cards 2–2, red cards
0–0. This is one simulated primary match, not a repeated-run comparison.

The end-of-progression save and captures are in the ignored local
\`primary-vm-guest-results-a/primary-progress-capture-g-b\` and
\`primary-vm-20261003-a\` directories. The end save
\`primary-start-fprimary-aug07-g.sav\` is 17,115,716 bytes, SHA-256
\`e02a7fed9dd3f07616d2eb20567b4d9d1f831ccceed6dde27e59636c173955d7\`.
The repeated \`primary-start-f\` prefix is the filename actually entered and
shown by the game; it preserves the UI record faithfully. An earlier attempt to
replace the default filename instead appended text and targeted a nonexistent
save. Its partial capture failure is retained; the later fresh capture succeeded.

These checks demonstrate primary new-game initialization, save, restart/load,
calendar progression and simulated-match paths in this VM. Mouse requests were
recorded but not instrumented at the game event queue, and no consumed-input
replay or repeated identical scenario was run. They do not close the M2
determinism/environment gate or authorize M4.

Extend repeated primary scenarios and measure semantic input acknowledgement
before any hybrid replacement. Keep the divergent GDI corpus from
[RUNTIME-REPLAY.md](RUNTIME-REPLAY.md) intact.

```powershell
py -3.13 scripts/vbox_reference_action.py capture --audit CM0102/Runtime/primary-vm-audit-a/audit.json --record CM0102/Runtime/NEW-screen.json --image CM0102/Runtime/NEW-screen.png
```

`vbox_reference_mouse.py` now records supported mouse control with a shared COM session and guest display
coordinates. Oracle's [VirtualBox 7.2.8 SDK reference](https://download.virtualbox.org/virtualbox/7.2.8/SDKRef.pdf)
documents the shared lock and absolute mouse API; these support-tool contracts
do not imply original game input consumption. Current read-only COM inspection
reported absolute mouse support and a 1278 by 960, 32-bit guest display before
the separate legacy-driver experiment. Mouse requests alone do not establish
consumed-input replay parity. The [documented desktop display APIs](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-changedisplaysettingsw)
used by the probe establish no recovered locale API or exception behavior.

## 2026-10-18 saved-state continuation

The new attempt records are under the ignored local
`CM0102/Runtime/primary-vm-observation-20261018-a/` directory. Every VM action
targeted the audited clone UUID; the source VM and supplied primary executable
were not modified.

The clone was found in `saved` state from 2026-10-05, not powered off. Before
resuming it, the 982,520,335-byte saved-state file was copied and hash-checked;
the retained copy is `saved-state-before-resume.sav`, SHA-256
`7d388f9ce440a19a6d3dd6e6671281925d3cb6bd01d66ca9e8356b9b4f990748`. The
resume action and its pre/post VM state are recorded in `resume-saved-state.json`.
The resumed guest restored an existing primary game at the previously observed
Bradford–Arsenal 0–1 match page. This is a resumed old session, not a repeated
scenario.

The first fresh launch attempt verified all 830 copied inputs but found the old
primary process still running and displayed “Championship Manager 2001/02 is
already running on this machine.” That failed attempt and its screenshots are
retained. The old process was exited through the game UI. A second uniquely
named copy, `primary-repeat-b-20261018-a`, passed the same 830-input check; its
primary hash matched the pinned SHA-256, it ran with `-windowed -seed 1` as guest
PID 1724, and its actual module base was `0x00400000`. The settings displayed
version 3.9.68, English, Minimum database, Sound Off, and Windowed mode. The
recorded UI sequence reached England selection, Test Manager / English
nationality, and Arsenal, then stopped at “Take control of Arsenal?” before a
new game completed.

The VirtualBox `setvideomodehint` request returned successfully but did not
change the 800×600×16 guest mode. A separate bounded guest display probe
enumerated the available modes and dynamically changed the guest desktop to
1024×768×16 (`CDS_TEST=0`, apply result 0) without a registry update. After that
change, screenshots showed a black game surface; no further game progression
was established. `save-experiment-state.json` records the VM being saved in this
incomplete state. The output of `display-probe-c` and the screenshots are
retained beside the action records.

This attempt demonstrates a controlled primary launch and screen-level response
to some UI requests, but it does not tie a particular message to a game queue
acknowledgment, complete a repeated scenario, establish determinism, or close
M2. The next runtime step is a fresh fullscreen launch on a clean clone state,
then repeat the same primary scenario with input and state observations at
stable semantic boundaries. Keep the earlier saved-state copy and all failed
attempts intact.
