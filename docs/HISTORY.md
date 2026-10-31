# Workspace history (2026-09-27 to 2026-10-04)

This is the original README narrative, moved here unchanged apart from link paths on
2026-10-05. Current entry points: [README](../README.md), [AGENTS.md](../AGENTS.md),
[playbook](agent/PLAYBOOK.md). The "Next-session prompt" below is superseded by the playbook.

## Championship Manager 01/02 inspection workspace

Prepared on 2026-09-27 from the supplied game ISO and Sports Interactive 3.9.68
patch. **The analysis target is `CM0102/Game/cm0102.exe`, official version 3.9.68.**
This workspace contains the extracted game installation, its database,
graphics, sounds, tactics, histories, editor, tools, and PDF manual.

"Latest" here means the final official game version, 3.9.68. Community patchers
and current-season databases are separate modifications and have not been applied.
The official patch itself updates the player/team data to 2002/03 while retaining
the original season start date; see `CM0102/Game/changes.txt`.

## Locations

| Path | Purpose |
| --- | --- |
| `CM0102/Game/` | Complete official 3.9.68 working installation; primary analysis target |
| `CM0102/Original-3.9.60/` | Original disc version, including original quick starts, for comparisons |
| `CM0102/InstallMedia/CM0102.iso` | Unmodified supplied disc image; mounted as I: during setup |
| `CM0102/InstallMedia/SI-Games-Patch-v3.9.68.exe` | Supplied official patch package |
| `CM0102/InstallMedia/Patch/Extracted/` | Verified unpacked patch resources and update.dat |
| `CM0102/InstallMedia/BasePayload/` | Cabinet payload, extracted with original accented filenames |
| `CM0102/Inspection/` | Hash manifests, PE headers/imports, cabinet listings, runtime screenshots |
| `tools/unshield/` | Local Unshield 1.5.1 cabinet extractor |

The workspace had no Git repository at preparation time. A local repository was
initialized during the 2026-10-03 continuation and the workspace source and
documentation were committed locally. No remote is configured and nothing was
published. Supplied assets, runtime data, and generated analysis remain ignored.
`.gitignore` excludes game assets, installation media, tools, and machine settings
for a future repository. Do not publish the game binaries with analysis code.

## Launch and verify

From PowerShell in this folder:

```powershell
.\Launch-Game.ps1
python scripts/verify_game.py
```

The launcher mounts the original ISO if necessary and starts the supplied,
unmodified `cm0102_GDI.exe -windowed` with the correct working directory. The ISO
is retained for the game's CD check. No no-CD patch or compatibility DLL was used.
No persistent compatibility registry changes were made.

The standard `cm0102.exe -windowed` reported a DirectDraw/16-bit graphics error on
this desktop, including when tested with a process-local `16BITCOLOR` setting.
The GDI executable successfully rendered the settings screen, main menu, and
new-game league selection. The settings screen confirms **3.9.68, build Nov 21
2002 17:16:15**; screenshots are in `CM0102/Inspection/gdi-startup.png`,
`gdi-menu.png`, and `gdi-new-game.png`.

The initial new-game failure is now diagnosed: the extraction put the 40 supplied
formation `.pct` files in `Tactics/`, while the loader opens them from `Data/`.
The historical baseline remains untouched. A recorded additive repair on
`CM0102/Runtime/gdi-england` enabled a completed new game, save/load, two weeks of
progression and three simulated friendlies. Saves, screenshots and input hashes
are frozen in `CM0102/Runtime/reference-20260927-verified`; see [runtime evidence](RUNTIME.md).
Repeat-run determinism remains unvalidated. The unchanged primary executable now
completes new-game initialization, save/restart/load, progression to 7 August 2001
and a simulated friendly in the isolated VM; see [primary VM evidence](RUNTIME-VM.md).

The smoke test created `game.cfg` and ten root-level language files. The verifier
reports these as additional runtime files; all **774 patched baseline files and
784 original files** still match their recorded SHA-256 hashes. All **46 patch
payloads** also match `update.dat` byte-for-byte. Do not regenerate manifests to
hide future modifications: work on a copy of the target binary.

## Provenance and reconstruction

The ISO was already mounted when this session started. Windows read the mounted
filesystem successfully. 7-Zip's UDF reader reported a header error and trailing
data, so the base files were read from the Windows mount rather than from a
7-Zip ISO extraction. Unshield extracted all 779 available cabinet payload files.
The ISO additionally supplies the game executables, tools, and manual.

Unshield's default extraction sanitizes accented filenames. The retained
`BasePayload` was extracted with `-R`, preserving names such as `1.FC Köln.his`.
Group names still use Unshield's underscore convention. The reconstruction script
maps those installation groups to the game's Data, Pictures, History, Sounds,
Tactics, Editor, and qstart folders.

The patch EXE is a self-extracting ZIP. All 20 extracted files were checked against
the supplied EXE archive, including archive CRC validation. `Patch/242.exe` is an
identical duplicate of the supplied patch EXE. Source hashes are recorded in
`CM0102/Inspection/source-manifest.json`.

The official patch's `setup.exe` contains a 46-entry filename/directory table at
VA `0x410070`. Its unpacker at `0x403430` reads a 184-byte header of 46 little-endian
file lengths, followed by the corresponding complete files. The lengths exactly
cover `update.dat`. `scripts/prepare_game.py` reproduces that extraction, copies
the loose fonts/readmes, and excludes obsolete 3.9.60 quick starts from the patched
installation, matching the updater's quick-start removal behavior. Original quick
starts remain in the base copy. It does not run either legacy installer or register
an uninstall entry. The script is pinned to the inspected patch hashes and refuses
nonempty output folders.

For reconstruction in a separate workspace with the supplied media and tools:

```powershell
# Substitute the mounted ISO drive if different.
.\tools\unshield\unshield.exe -R -d CM0102/InstallMedia/BasePayload x I:/data1.cab
python scripts/prepare_game.py --disc I:/
python scripts/verify_game.py
```

The existing Game and Original-3.9.60 folders must be preserved; the preparation
script intentionally will not overwrite them. The script now maps the Tactics
cabinet group into Data for future empty reconstructions; the preserved manifests
describe the historical layout, not this correction. Patch/Extracted must already contain
the 20 files extracted from the supplied patch EXE with 7-Zip.

Temporary copies remain at `CM0102/Game-incomplete`,
`CM0102/Original-3.9.60-incomplete`, and `CM0102/InstallMedia/BaseExtracted`.
They are not analysis targets. These were created during setup; automatic approval
rejected their recursive cleanup with "blocked by policy". The first reconstruction
stopped at the ISO executable's read-only attribute; the successful reconstruction
makes its working copy writable before applying the patch.

## Starting reverse engineering

The primary executable is a native **32-bit x86 PE**, image base `0x00400000`,
entry-point RVA `0x00547A13` (VA `0x00947A13`). PE section and import listings are
saved in `CM0102/Inspection/cm0102-pe.txt` and `cm0102-gdi-pe.txt`.

Primary executable SHA-256:
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`

GDI executable SHA-256:
`0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b`

The decompilation roadmap is in [docs/ROADMAP.md](ROADMAP.md). It requires
instruction-derived recovery of the original engine, differential validation,
and a standalone faithful build before modernization. Unknown logic must remain
explicitly unresolved; it must not be replaced with synthetic game behavior.

Reproducible Ghidra tooling and local projects are established under
`CM0102/Analysis/`; see [analysis setup](ANALYSIS-SETUP.md),
[startup findings](STARTUP.md),
[community research intake](COMMUNITY-SOURCES.md), and
[community offset findings](OFFSETS-COMMUNITY.md). The generated projects,
binary copies, strings and pseudocode remain local and ignored. Decompiled text
is not compiled or validated recovered source. See the session results in the
analysis setup note for completed runs and remaining limitations.

The champman0102 "Offsets" thread has been captured in full and verified against
the official 3.9.68 binary. Three results matter for static recovery: the thread's
hex edits address `cm0102.exe` by raw file offset rather than by VA (97.3% of them
resolve in our clean build); 1454 of 1779 quoted instruction lines are
byte-identical to our build, with 39 posts matching exactly; and 789 of 791
original developer source paths listed in the thread are exact strings in our
executable, which recovers the original C++ module map. The material is
corroborated, not yet authoritative: no algorithm was accepted from it, and each
claim still needs confirmation from the original instructions.

The new [recovery map](RECOVERY-MAP.md) follows embedded source paths to
1,925 candidate functions, retaining 29 ambiguous cases. It exports decoded call
sites including unresolved indirect calls. The [full orchestration map](ORCHESTRATION.md)
now reaches the recurring three-phase/day update loop and identifies a bounded
date-family candidate for the next recovered slice. The subsequent
[date recovery](DATE.md) compiles six routines to 32-bit x86 and compares
them with the original instructions, including ABI, shared state, overflow and
the original negative-32768 recursion defect. Its recovery ledger is
`recovery/date/ledger.json`. No recovered code is integrated into the game.

The [date-family extension](DATE-FAMILY.md) now compiles sixteen routines
(ten added this session, covering 2,500 additional original routine bytes).
It recovers both YMD variants, diagnostic version support, weekly arithmetic,
month/day conversion, leap testing and both date-difference operations. The
function map documents 28 family functions plus two support routines and checks
7,051 decoded call records. O0/O2 differential runs preserve original defects
and explicit CRT/UI, SEH and return-register residuals; these are finite emulator
results, not a complete Date subsystem or game integration.

The [localization recovery](LANG-FORMAT.md) adds eight compiled routines:
the formatting wrapper, language getter, five date-name/order helpers and ordinal
numbers. Each final O0/O2 corpus passes 81,232 differential cases under explicit
lookup, morphology and CRT/UI hooks. The combined selections now contain 24
compiled routines; the updated date/language map checks 15,872 decoded calls.
The [lookup recovery](LANG-LOOKUP.md) now adds both memory/file translation
paths at `0x0066FBB0` and the key comparator: two application routines covering
1,662 original bytes, plus separately accounted CRT search support. Each O0/O2
run passes 69,491 comparisons, including one bounded long-loop observation.
Captured rows check lookup against the supplied 9,807-record language database
layout; separate synthetic cases exercise lookup through the recovered formatter. The combined selection
has 26 compiled application routines; the map checks 15,980 decoded call records.
The [numeric date recovery](DATE-NUMERIC.md) adds two compiled formatters
covering 1,242 original bytes. Each final O0/O2 run passes 531,967 comparisons,
including original prior-stack month-byte behavior, language-10 direct formatting
and chains through recovered lookup. The selected date/localization application
inventory reached 28 compiled routines at that stage.
The [date text recovery](DATE-TEXT.md) subsequently adds all seven remaining
formatters, covering 6,917 original body bytes and preserving byte/dword prior
stack storage, reverse-scan day behavior and helper-call ordering. All 28 entries
in the selected contiguous Date family now compile; the combined date/localization
selection reached 35 application routines and separate CRT search support.
Each accepted final-b O0/O2 run passes 1,860,817 comparisons, recorded in its
separate ledger and frozen source snapshot.
Allocation, I/O, morphology and diagnostics remain explicit boundaries.
The [language dependency map](LANG-DEPENDENCIES.md) verifies 18,558 original
instruction records for setup/loader/lifecycle and morphology candidates.
The [language lifecycle recovery](LANG-LIFECYCLE.md) now compiles five of
those routines: setup, membership, row construction/destruction and teardown.
Each O0/O2 run passes 131,931 comparisons, covering 829 instruction bytes
(816 Ghidra body bytes plus a manually identified 13-byte constructor).
That stage contains 40 application routines plus separate CRT search support.
The [language loader recovery](LANG-LOADER.md) adds database loading and
the byte-order probe, covering 2,513 original instruction bytes. The selected
inventory reached 42 compiled application routines plus CRT search support.
Both full O0/O2 runs pass 131,500 comparisons, including unmodified 9,807-row
database loads for IDs two and ten. The seven-ledger map checks 16,070 calls and
three direct tail transfers separately. Original array/sort support executes
in the emulator with recovered callbacks; file/heap/UI boundaries remain explicit.
Maintained [compiler probe](STACK-PROBE.md) and dependency-free [string search](CRT-SEARCH.md)
add three separately accounted CRT entries. Probe passes 3,020 comparisons;
two search entries/shared exit pass 44,423. [Maintained classification](CRT-CLASSIFY.md)
passes 11,788 per O0/O2, adds one separate CRT entry and retains its deeper
fixture. The then-current twelve-ledger selection contains 45 application entries
plus five CRT entries, 16,337 selected calls and three tails. Eight artifacts
match across fresh final-a/b maps, with seventeen pinned inputs.
No recovered routine is live integrated.

The [recovered-probe morphology relink](LANG-MORPH-RELINKED.md) is accepted at
323,545 comparisons per O0/O2 build, fifteen frozen inputs and one compiled probe
execution per case. Original application/probe instructions are prohibited on
the recovered side. Four CRT fixtures and original data remain explicit.
The interrupted O0 attempt is preserved and unaccepted; replacement O0 finished
with exit zero/PASS. Prior full ledgers, maps and tested sources remain unchanged.
The then-current twelve-ledger map selects this relinked parent once.
[Search composition](LANG-MORPH-SEARCH.md) is now accepted after full
O0/O2,323,545 comparisons per build and seventeen frozen inputs. Each build
executes 202,868 compiled search calls across 45,716 cases. Its reference find
fixture and six artificial cases/32 fixture calls remain explicit. Three steady
CRT fixtures and original data remain required. Maintained classification is
still unadopted by case conversion/morphology. That stage's map is
`scripts/map_morph_search_family.py`; prior maps remain unchanged.

[Case-conversion candidates](LANG-MORPH-CRT-DISCOVERY.md) recover both complete bodies and pass
23,304 comparisons per O0/O2 with explicit classification/mapping fixtures.
They remain isolated/unadopted. Morphology now executes compiled search on
unforced calls; reference find and six artificial cases remain explicit
boundaries. Formatting, locale support and original data closure
remain open. M2 replay, M4 integration and M6 standalone gates are unmet.

Current continuation: [bounded-length CRT support](CRT-LENGTH.md) is accepted
separately after a fresh 48,176-case maintained build. The current thirteen-ledger
map is `scripts/map_crt_length_family.py`: 45 application plus six CRT entries,
16,340 calls and three tails; nine artifacts reproduce across fresh maps. Prior
accepted records remain unchanged. [Deeper locale discovery](CRT-LOCALE-DISCOVERY.md)
compiles 965 bytes including omitted local SEH blocks; 5,757 corrected cases
execute all 393 compiled instructions including maintained helpers. Locale
candidates remain unaccepted, with imported APIs and shared exception dispatch
explicit. The initial locale apparent pass is invalidated and preserved. Use
`py -3.13` for these harnesses; `python` currently selects an unrelated venv.

[Runtime replay](RUNTIME-REPLAY.md) now restores the later save, maps four
complete RNG bodies/constants and a Date transition fragment, and freezes two
56-request sequences. Their initial bounded state agrees; displayed input/capture
outcomes and endpoints diverge, so M2 remains partial. The [isolated primary
reference VM](RUNTIME-VM.md) signs in without user credentials and runs the
unchanged primary on a private guest copy using Standard VGA at 16-bit colour.
New game, save/restart/load, progression and a simulated friendly are demonstrated.
Repeated-run determinism and consumed-input replay remain open. Its original OS
disk is unchanged; VirtualBox's source-configuration normalization was recorded
and restored.

Next-session prompt:

> Read AGENTS.md, README.md and docs/ROADMAP.md first. Verify preserved baselines
> and current length acceptance with fresh audit outputs. Read docs/CRT-LENGTH.md,
> docs/CRT-LOCALE-DISCOVERY.md, docs/RUNTIME-REPLAY.md and docs/RUNTIME-VM.md.
> Preserve the thirteen-ledger map (45 application/six CRT entries), corrected
> 5,757-case locale candidate and all failed/invalidated evidence. The 433-byte
> exception closure compiles and passes 1,964 synthetic cases over all 158
> instructions, but remains unaccepted; actual Windows unwind behavior is unknown.
> Sign-in is resolved in CM0102-reference-20261003-a. The unchanged primary now
> completes new game, save/restart/load, progression to 7 August, and one friendly.
> Continue repeated primary scenarios and instrument consumed-input acknowledgement
> before M2 acceptance. The two GDI posted sequences remain divergent observations,
> not an accepted deterministic reference. Recover shared dispatch, locale
> initialization and classification composition without assuming Windows API or
> exception behavior. Keep M2 as prerequisite to M4, then continue faithful
> subsystem and standalone recovery as gates allow. Do not rerun completed large
> controllers or alter prior acceptance records.

For the complete frozen next-session prompt, read
[docs/NEXT-SESSION-PROMPT.md](NEXT-SESSION-PROMPT.md).

Historical detailed handoff before this continuation:

> Read README.md, docs/ROADMAP.md, docs/RUNTIME.md, docs/RECOVERY-MAP.md,
> docs/ORCHESTRATION.md, docs/RNG-INIT.md and docs/DATE.md. Verify the preserved baseline.
> The runtime blocker was misplaced .pct assets; use the repaired disposable
> copy, never modify CM0102/Game. The frozen GDI corpus demonstrates new game,
> starting-save restore, July 14–28 progression and three simulated matches.
> Restore the July 28 save and build repeated fixed-interval comparisons; M2
> determinism and primary/GDI mapping are still open. Keep the RNG slice closed
> unless evidence fails. Date default construction, assignment and add/subtract
> are now compiled and corpus-validated in a 32-bit differential harness; preserve
> their shared globals and original 0x8000 recursion behavior. Read
> docs/DATE-FAMILY.md: sixteen routines are compiled and differentially tested,
> including constructor 0x00549F70 and initializer 0x0054A130 with modeled diagnostics.
> Preserve low-byte week counts, stored leap flags, shared guards, asymmetric
> day-distance and component-borrow defects, and the explicit AX/EAX and SEH
> exclusions. Read docs/LANG-FORMAT.md and recovery/lang/ledger.json: the wrapper
> at 0x0066F4E0, five name/order helpers, language getter and ordinal helper are
> compiled and tested. Final c-O2/d-O0 reports each pass 81,232 cases under hooks.
> Read docs/LANG-LOOKUP.md and recovery/lang/lookup-ledger.json: lookup and its
> comparator now compile, with separately accounted CRT search support. Reproduce
> the final-a O0/O2 reports in new output folders. Preserve file/query normalization
> asymmetry, ownership/free order, ignored I/O results and signed language loops.
> Read docs/DATE-NUMERIC.md and recovery/date/numeric-ledger.json: the two numeric
> entries compile and pass final-a O0/O2 corpora, including the prior-frame month
> bytes at incoming ESP-0x205/0x209. Read docs/DATE-TEXT.md and
> recovery/date/text-ledger.json: all seven remaining mapped date formatters now
> compile with original prior byte/dword contracts and retained helper arguments.
> Verify their final-b O0/O2 acceptance. Read docs/LANG-LIFECYCLE.md and
> recovery/lang/lifecycle-ledger.json: five setup/ownership routines compile and
> pass final-a O0/O2 corpora, with modeled OS/loader/array/CRT/UI boundaries.
> Preserve constructor order-array bytes, destructor later-field reads and EAX,
> cached teardown header and setup's loader EAX. Read docs/LANG-LOADER.md and
> recovery/lang/loader-ledger.json: loader 0x0066EB40 and marker 0x0090C140 now
> compile. Check the full final-b O0/O2 report acceptance before relying on them.
> Preserve positive +4 SEEK_END, incoming marker argument writes, prior header/ID
> slots, filtered file ordinals and key normalization asymmetry. Keep original
> array/sort support and file/heap/UI boundaries explicit, and use
> -fno-optimize-sibling-calls for this loader chain build. Read
> docs/LANG-MORPHOLOGY.md and recovery/lang/morph-tables-ledger.json: both helpers
> 0x006820A0/0x00682150 pass final-a O0/O2. Preserve first-match/sentinel behavior,
> full incidental EAX, low-byte properties and dword-before-byte copies.
> Preserve morphology 0x006701F0's measured P=entryESP-0x2818 frame and
> five incoming arguments. Do not adopt its generated two-parameter signature,
> zero all private buffers or add a null guard before the original scan.
> Read docs/LANG-MORPH-PARENT.md and recovery/lang/morph-ledger.json:
> the full 64,465-byte application body passes 323,545 comparisons per O0/O2.
> Verify scripts/verify_morph_parent.py and the new scripts/map_morphology_family.py
> using fresh output folders. Prior partial source/ledger remain unchanged.
> Read docs/LANG-MORPH-RELINKED.md and verify the accepted O0/O2 pair,
> terminal controller and scripts/verify_morph_relinked.py in a fresh folder.
> Current map: scripts/map_morph_search_family.py. Read
> docs/CRT-SEARCH.md: two dependency-free entries pass 44,423 comparisons;
> Read docs/LANG-MORPH-SEARCH.md and verify scripts/verify_morph_search.py:
> both full controllers completed with exit zero/PASS; do not restart them. Read
> docs/LANG-MORPH-CRT-DISCOVERY.md:
> case candidates pass 23,304 per build but classification/mapping fixtures
> remain. Read docs/CRT-CLASSIFY.md: maintained classification passes
> 11,788/build with a deeper fixture; validate case composition separately.
> Recover deeper locale, formatting and data closure for a standalone chain.
> Preserve preview
> aliasing, dynamic language reads and original uninitialized month-byte paths;
> do not invent safe defaults. Reproduce the harnesses and the combined map
> with fresh local output names. M2/M4 gates remain open.
> Use the instruction-backed module/call map; candidate ownership is not validation. Do not invent engine logic or begin M4 prematurely.

References: the supplied `changes.txt` and patch readmes;
[community patch installation guidance](https://www.champman0102.net/viewtopic.php?t=1943)
distinguishing official 3.9.68 from community modifications;
[Unshield project](https://github.com/twogood/unshield) and the
[Windows binary distribution](https://github.com/ScoopInstaller/Binary/tree/master/unshield).
