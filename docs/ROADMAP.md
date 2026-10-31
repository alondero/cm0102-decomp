# Faithful decompilation and native port roadmap

## Objective and fidelity contract

Recover a maintainable, buildable implementation of the official Championship
Manager 01/02 3.9.68 executable, with the original simulation logic and observable
behavior, then a native modern port. Modernization follows a working, validated
standalone faithful baseline. This is
not a recreation of football-management rules from descriptions or a substitute
simulation that produces plausible outcomes.

The primary input is `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Its preferred image base is `0x00400000`; entry point is RVA `0x00547A13`,
VA `0x00947A13`. A VA is an address in the preferred loaded image; an RVA is
relative to the image base; a file offset is an offset in the disk file. Every
address claim must state which convention it uses. Runtime addresses must also
record the actual module base.

The supplied `CM0102/Game/cm0102_GDI.exe`, SHA-256
`0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b`,
is a separate executable. Its ability to reach menus does not establish that the
primary executable runs, or that either binary completes a game. Cross-binary
function equivalence must be demonstrated before transferring conclusions.

Preserve supplied executables, assets, and manifests. Import, patch, and instrument
copies. Record tool versions, commands, settings, input/output hashes, and any
environment requirements. Keep supplied media and proprietary assets local.

For recovered application logic:

- Each function has an original address, byte identity, evidence, and a recovery
  status. Decompiled text alone is not validated source.
- Unknown behavior stays explicitly unknown or calls the original code during
  the hybrid phase. Do not fill gaps with invented formulas, heuristics, random
  distributions, stub simulation results, or a fresh implementation of the rules.
- Preserve integer widths, signedness, overflow, layout, aliasing, calling
  conventions, floating-point precision and rounding, RNG state and call order,
  event ordering, and observable original bugs until modernization is authorized.
- Compiler/runtime library replacements require identified provenance and checked
  behavioral contracts. Do not count a renamed library call as recovered game
  logic. Inline assembly may be retained where needed to establish fidelity.
- Tests, trace tools, and platform adapters may contain new support code; they
  must not silently introduce new application or engine behavior.

Track **identified**, **decompiled**, **compiled**, **validated**, and
**integrated** independently. A function can reach one state without reaching the
next. Report coverage against an explicit inventory and record splits, merges,
thunks, and uncertain function boundaries. Do not advertise a percentage based
only on the number of functions emitted by a decompiler.

## Recovery process (2026-10-05)

New work is accepted only through the automated gate described in
[ADR 0001](adr/0001-automated-acceptance-and-two-tier-recovery.md): tier-1 lifted assembly
(compiled + validated) and tier-2 C (decompiled), each differentially tested against
the original instructions. Counts are generated in [STATUS.md](STATUS.md). The gates
below are unchanged; tier-1 acceptance extends M3/M5 coverage, not M2, M4 or M6.

## Milestones and acceptance gates

| Milestone | Deliverable | Gate before advancing |
| --- | --- | --- |
| M1: Reproducible analysis foundation | Pinned Ghidra import of a copied primary executable; machine-readable inventories; evidence-backed startup map | Independent clean import reproduces stable inventories; baseline hashes remain intact; every startup claim points to actual instructions or decompiler output |
| M2: Runtime reference and observation | Controlled working original environment, replayable scenarios, trace and state comparison tools | New-game initialization, a match, calendar progression, and save/load are demonstrated; repeated runs characterize determinism and environmental differences |
| M3: First recovered executable slice | One bounded original routine compiled with a differential harness | Original and recovered routine agree on outputs, state effects, ABI, and representative edge cases; source is traceable to original instructions |
| M4: Hybrid executable | Recovered routines execute within a working original process | Controlled scenarios remain equivalent after replacement; every remaining original-code dependency is listed; integration is reproducible |
| M5: Subsystem recovery | Increasingly complete application source and shared data model | Each subsystem passes local differential checks and whole-game replay gates before replacing its original implementation |
| M6: Standalone faithful build | Source build with all application logic accounted for and no original executable required | Complete game and season scenarios, match behavior, save/load, and subsequent progression pass the defined parity suite; no unaccounted residual original code remains |
| M7: Modernization | Platform and usability improvements on top of a frozen faithful reference | Each change states its intended behavioral differences and passes unchanged-behavior checks; the faithful baseline remains reproducible |

### Working toward the gates

The executable deliverables are M4's live hybrid, M6's standalone faithful game,
and M7's native modern port. Routine counts support these gates but do not close
them. Report accepted lifts, readable C, subsystem dependency closure and live
integration separately; use live `cmre status` for the supported counters.

Use [subsystem recovery](agent/SUBSYSTEM.md) to select connected source slices
with observed layouts and explicit original-code/data/API dependencies. Apply
[evidence review](agent/REVIEW.md) before claiming a wider gate. M2's consumed-input
replay and runtime-state comparisons remain required alongside static recovery.
Current runtime evidence and unfinished work are in [RUNTIME-VM.md](RUNTIME-VM.md)
and [RUNTIME-REPLAY.md](RUNTIME-REPLAY.md); neither lift coverage nor a GDI scenario
can substitute for primary-binary equivalence.

Port planning can identify platform contracts before M6. Changing recovered game
logic for the modern port starts after the standalone gate, following
[PORT.md](agent/PORT.md). Tool choices and the decomp.wtf/REA review are recorded
in [DECOMPILATION-STRATEGY.md](DECOMPILATION-STRATEGY.md).

### M1: Reproducible analysis foundation — first session slice

Verify the preserved installations with `scripts/verify_game.py`. Create an
analysis copy of the pinned primary executable and a repeatable Ghidra headless
import with recorded release, Java version, processor/compiler specification, and
analysis options. Record copied-input and original hashes before and after work.

Export sections, imports, strings, detected functions, and analysis warnings in
stable formats. Preserve a useful local Ghidra project while keeping its generated
database separate from reviewable scripts and notes. Trace the entry point through
runtime initialization to the first demonstrable application-owned startup code.
Distinguish detected structure from confirmed meaning; compiler startup is not
the simulation engine. Capture relevant disassembly/decompilation and unresolved
branches or indirect calls with their addresses.

Perform a second import into an empty project. Compare normalized inventories
rather than volatile project database files or timestamps. Record any differences
instead of hiding them. End with a reproducible command sequence, results, a
short startup map, and a candidate for the first bounded recovery investigation.
Do not claim a working source port at this stage.

### M2: Runtime reference and observation

Establish a controlled environment in which the original game completes useful
work. The GDI reference now completes an England new game, save/load, July 14–28
progression and three simulated friendlies; see [RUNTIME.md](RUNTIME.md).
The blocker was misplaced supplied formation assets, corrected only on the
runtime copy. Primary DirectDraw initialization still fails on this desktop.
Record OS/runtime, graphics path,
working directory, media requirements, database hashes, configuration, language,
league selection, and inputs. Avoid persistent compatibility changes unless needed
and explicitly documented.

First capture a small new game, a save/load round trip, a played or simulated
match, and several days of progression. Then add transfer/contract events and a
season rollover. Record checksums and selected semantic state at stable boundaries.
Do not assume raw save bytes are deterministic: identify timestamps, compression,
padding, and environmental fields before normalizing anything. Keep raw captures
alongside normalized comparisons.

The [2026-10-03 replay](RUNTIME-REPLAY.md) now restores the later save and records
two guarded 56-request sequences. Their bounded initial state agrees, but displayed
input/capture outcomes differ before the displayed calendar advances and endpoints diverge.
This does not accept deterministic replay or establish engine nondeterminism
under identical consumed inputs. The [isolated Windows 7 clone](RUNTIME-VM.md)
now signs in and runs the unchanged primary with its built-in Standard VGA
driver at 16-bit colour. Primary new-game initialization, save/restart/load,
progression to 7 August 2001, and one simulated friendly are demonstrated.
Repeated identical primary scenarios and consumed-input replay remain unfinished;
no user credential is required.

Determine RNG initialization and state, time dependence, ordering, and relevant
thread behavior. Instrumentation must have a documented effect on the process.
Repeated identical scenarios should either reproduce or have a specific account
of each remaining nondeterministic input. Static recovery can proceed while this
environment work is blocked, but behavioral validation cannot be declared complete.

### M3: First recovered executable slice

Choose the slice from M1 evidence: a small, bounded application routine with
understood callers and state, modest dependencies, and observable outputs. A
format conversion, parser helper, or data transformation may qualify; avoid
choosing the match engine merely because it is the eventual goal. A CRT helper
alone does not meet the application-recovery gate.

M3 candidate selection now has a checked evidence source rather than M1 exports
alone. The first slice taken is the application RNG pair at `0x0090D0A0` (init)
and `0x0090CFC0` (bounded PRNG), documented in [RNG-INIT.md](RNG-INIT.md). It is
application-owned, has a closed state block, 3772 PRNG call sites, and a
command-line `-seed` path in `Setup.cpp`. The MSVC LCG helpers it uses are
included; CRT `time()` and sprintf remain stubs. Community PRNG notes in
`mined/01-engine-routines.md` were hypotheses only; the recovered source follows
the original instructions.

The community thread catalogued in `docs/OFFSETS-COMMUNITY.md` still supplies
20 subsystem-level descriptions whose quoted instruction bytes are exact in the
pinned executable, in
`CM0102/Analysis/Community-Offsets/mined/01-engine-routines.md`. That shortlist
is a starting point for later slices, not a recovery status. Those candidates
skew towards competition and draw logic, which remains the eventual goal.

A second bounded slice now recovers date default construction, assignment and
signed-word addition/subtraction; see [DATE.md](DATE.md) and
`recovery/date/ledger.json`. Both original and recovered code execute as 32-bit
x86 under Unicorn with ABI/state comparisons. The original negative-32768 mutual
recursion is preserved and observed to a bounded depth, not claimed as exact
stack-overflow parity. This does not close M2 or authorize M4 integration.

The date family now includes sixteen compiled, finite-corpus-validated routines;
see [DATE-FAMILY.md](DATE-FAMILY.md). This adds both YMD variants, weekly arithmetic,
month/day conversion, leap testing, two difference operators and diagnostic
version support. CRT/UI calls remain explicit modeled boundaries; transient
SEH/unwinding and specified incidental return bits remain excluded. The selected
28-function family map plus two support routines verifies 7,051 decoded call
records without promoting formatting candidates. These results extend M3; the
M2 and M4 gates remain unchanged.

The [localization extension](LANG-FORMAT.md) adds eight compiled routines,
including the wrapper, date-name/order helpers and ordinal-number helper.
Each O0/O2 corpus passes 81,232 cases under explicit lookup, morphology and
CRT/UI hooks. The combined date/formatting selection is 24 routines; its
28-family/five-support map checks 15,872 call records. Real localization,
exception paths and nine larger date formatters remain incomplete. This extends
M3 without closing M2, M4 or the standalone-build gate.

The [lookup extension](LANG-LOOKUP.md) adds two application routines (1,662
original bytes) and separately accounted CRT search support. O0/O2 runs each
pass 69,491 comparisons, including one bounded signed-language-loop observation.
Both table and file paths execute recovered source, including a formatter chain
under explicit emulator entry redirection. Supplied database layout and seven
captured rows are checked read-only. The selected date/localization application
inventory is now 26 routines; the map covers 28 family/eight support entries and
15,980 call records. Allocation, I/O, morphology and diagnostic boundaries remain
modeled; no live integration or additional milestone gate is established.

The [numeric date extension](DATE-NUMERIC.md) adds two application routines,
1,242 original bytes, using C and small x86 shims which retain original prior-frame
month bytes on negative ordinals. O0/O2 each pass 531,967 comparisons, covering
all ordinal/year/size words plus selected actual wrapper/lookup chains and
language-10 direct formatting. The selected date/localization application count
reached 28 routines at that stage. No safe month
default, host calendar or original-bug correction is substituted. M2/M4/M6 remain open.

The [date text extension](DATE-TEXT.md) adds the seven remaining mapped formatters,
6,917 original body bytes, retaining prior stack byte/dword storage, the separate
reverse day scan and arguments retained across helper calls. All 28 selected
contiguous Date-family entries now compile; the five-ledger selection totals 35
application routines plus separate CRT search support at that stage. Final-b
O0/O2 each pass 1,860,817 comparisons, with source/report identities pinned in
the separate ledger. This does not establish complete subsystem or runtime parity; CRT,
morphology and diagnostic boundaries and M2/M4/M6 gates remain explicit.

The [language lifecycle extension](LANG-LIFECYCLE.md) adds five compiled
application routines: setup, membership, row construction/destruction and
teardown. Both O0/O2 runs pass 131,931 comparisons. Recovery covers 816 Ghidra
body bytes plus a manually identified 13-byte constructor; 927 hashed extent
bytes also include setup tables/alignment. The selected application inventory
reached 40 routines plus separate CRT search support. The six-ledger map
contains 28 Date-family and thirteen support entries, 16,010 checked calls and
three separately checked direct tail jumps that Ghidra labeled calls. OS layout,
the loader, array iterator and CRT/UI remain explicit boundaries; callback tests
do not claim complete array iteration. Those are this stage's fixture contracts;
its accepted source and reports are unchanged.

The [language loader extension](LANG-LOADER.md) compiles database loading and
the byte-order probe, totaling 2,513 original bytes. The seven-ledger selection
reached 42 application routines plus one separate CRT search routine,
28 Date-family and fifteen support entries, 16,070 checked calls and three
separate tail transfers. Both full loader O0/O2 runs pass 131,500 comparisons,
including unmodified 9,807-row database loads for IDs two and ten. Its new chain
executes original normal-return array/sort support with compiled callbacks and
keeps file/heap/label/UI boundaries explicit. It does not count those original
CRT routines as recovered source. That loader acceptance and its source remain
unchanged.

Maintained [compiler probe](STACK-PROBE.md) and dependency-free [string search](CRT-SEARCH.md)
add three separately accounted CRT entries. Probe passes 3,020 comparisons;
two search entries/shared exit pass 44,423. [Maintained classification](CRT-CLASSIFY.md)
passes 11,788 per O0/O2, adds one separate CRT entry and retains its deeper
fixture. The current twelve-ledger selection contains 45 application entries
plus five CRT entries, 16,337 selected calls and three tails. Eight artifacts
match across fresh final-a/b maps, with seventeen pinned inputs.
No recovered routine is live integrated.

The [recovered-probe morphology relink](LANG-MORPH-RELINKED.md) is accepted at
323,545 comparisons per O0/O2 build, fifteen frozen inputs and one compiled probe
execution per case. Original application/probe instructions are prohibited on
the recovered side. Four CRT fixtures and original data remain explicit.
The interrupted O0 attempt is preserved and unaccepted; replacement O0 finished
with exit zero/PASS. Prior full ledgers, maps and tested sources remain unchanged.
The current twelve-ledger map selects this relinked parent once.
[Search composition](LANG-MORPH-SEARCH.md) is now accepted after full
O0/O2,323,545 comparisons per build and seventeen frozen inputs. Each build
executes 202,868 compiled search calls across 45,716 cases. Its reference find
fixture and six artificial cases/32 fixture calls remain explicit. Three steady
CRT fixtures and original data remain required. Maintained classification is
still unadopted by case conversion/morphology. The current map is
`scripts/map_morph_search_family.py`; prior maps remain unchanged.

[Case-conversion candidates](LANG-MORPH-CRT-DISCOVERY.md) recover both complete bodies and pass
23,304 comparisons per O0/O2 with explicit classification/mapping fixtures.
They remain isolated/unadopted. Morphology now executes compiled search on
unforced calls; reference find and six artificial cases remain explicit
boundaries. Formatting, locale support and original data closure
remain open. M2 replay, M4 integration and M6 standalone gates are unmet.

Recover the full routine and necessary types from instructions, verify its calling
convention and memory accesses, and build a 32-bit harness that can invoke the
original and recovered versions with equivalent state. Restore mutable state
between runs. Compare return values, modified memory, exceptions, external calls,
and any floating-point or RNG state effects. Cover boundary values and realistic
captured inputs, not only one successful example. Record unresolved assumptions.

Select the compiler and flags based on required semantics. A modern compiler may
optimize away signed-overflow assumptions or use different floating-point behavior;
encode required behavior explicitly. Byte-identical output is useful evidence but
is not required if behavior and state effects are established. Passing a finite
test corpus is evidence, not proof of universal equivalence.

### M4: Hybrid replacement

The latest continuation separately accepts [length support](CRT-LENGTH.md) at VA
`0x009519D3`, preserving its extra read even for a zero limit. Current thirteen-ledger
`scripts/map_crt_length_family.py` selects 45 application/six CRT entries, checks
16,340 calls/three tails, and reproduces nine artifacts with eighteen pins.
Prior accepted records remain unchanged. [Locale discovery](CRT-LOCALE-DISCOVERY.md)
compiles mapping/deeper classification, fill and local SEH blocks; 5,757 corrected
CPU cases execute all 393 instructions including maintained helpers. These
candidates remain unaccepted; scripted API and lexical recovery checks do not
establish Windows API/SEH parity. M2 remains prerequisite to live replacement.

Introduce a reproducible integration mechanism on a disposable executable copy.
Replace only validated routines, preserving ABI, global state access, relocations,
and exception behavior. Keep a manifest of redirected entry points, original bytes,
generated bytes, and input/output hashes. Add a failure-safe way to run the original
scenario for comparison.

Explicitly list calls and data references that still depend on the original
executable. Hybrid execution is an incremental validation tool, not standalone
decompilation. Run both local differential cases and M2 scenarios after each
replacement batch. Investigate the earliest divergence before expanding the batch.

### M5: Recover subsystems in dependency order

Refine this order using the discovered call graph and runtime evidence:

1. Shared types, identifiers, strings, allocation/ownership conventions, file I/O,
   and configuration boundaries. The original module names are now known from
   verified embedded build paths: 396 distinct C++ paths in the direct-binary
   scan ([RECOVERY-MAP.md](RECOVERY-MAP.md)), under a two-tree
   layout, with `Database.cpp`, `History.cpp`, `match_eng.cpp`, `match_pl.cpp`,
   `transfer_manager.cpp`, `player_regen.cpp` and `search_eng.cpp` in the engine
   root, and a separate `si\code` platform layer holding `os.cpp`, `network.CPP`,
   `tcpip.cpp`, `Langlib.cpp` and `zipdir.cpp`. Use these as authoritative module
   names for recovered units; the names are evidence, the module boundaries and
   their internals are not yet established. See
   `CM0102/Analysis/Community-Offsets/source-paths.json`.
2. Database records, loading, indexing, and save serialization, including version
   and compression behavior. Establish layout evidence before inventing classes.
   Partial layout evidence already exists - candidate record strides and a
   labelled-ID region are recorded in
   `CM0102/Analysis/Community-Offsets/mined/02-data-model.md` and must be checked
   field by field against the shipped `.dat` files before adoption. The engine
   also stores data constants inline inside `.text`, so a decompiler will emit
   plausible-looking instructions for constant pools; identify and label those
   before reading any decompiled function as code.
3. RNG, date/calendar, event queues, and persistent world state. Establish the
   initialization and ordering contracts needed by later simulation tests.
4. Players, clubs, staff, contracts, transfers, finances, competitions, and their
   state transitions.
5. Match preparation, tactical decisions, simulation, commentary, and post-match
   updates, recovering actual logic and all relevant RNG interactions.
6. Remaining orchestration, screens, input, rendering, audio, and platform glue.

This is a starting dependency hypothesis, not a claim about discovered modules.
Investigate tightly coupled routines together when an artificial boundary would
obscure state. UI and platform work may be studied earlier to enable the runtime
reference; they must not distract from recovering application logic. Every batch
adds a source-to-address map, type evidence, regression scenarios, and a residual
dependency update. Preserve meaningful disagreements in an uncertainty register.

### M6: Standalone faithful build

Eliminate original-code fallbacks and prove that no recovered module secretly
depends on executing original text, startup stubs, or a bundled binary image.
Account separately for game logic, compiler support, operating-system APIs, and
identified third-party dependencies. A shipped original executable used as a code
library does not satisfy this milestone.

Build from documented source and toolchain in a clean environment. Original assets
and database files may still be necessary and remain local. Check save compatibility
in both directions where the original format supports it, progression after loading,
multiple league configurations, match cases, transfers, and season rollover.
Expand beyond the initial short reference scenario. Require zero unexplained
divergences in the agreed suite and an honest statement of untested paths; one
successful season cannot establish equivalence for all possible careers.

### M7: Modernize from the faithful baseline

Freeze a reproducible faithful build and its reference corpus. First replace
platform boundaries such as display/input/audio/file access while retaining the
simulation and persistence contracts. Then consider packaging, resolution, Unicode,
accessibility, portability, and tooling. Move to 64-bit only after separating
serialized layouts and pointer assumptions from host representation.

Gameplay changes, original bug fixes, expanded limits, and changed save formats
must be explicit features with separate expected results. Keep compatibility modes
or migrations where necessary. A change that produces better football results is
still a behavioral change and cannot be quietly included in faithful recovery.

Use the [port workflow](agent/PORT.md) to retain the source/toolchain/corpus
identity and compare each boundary against the frozen reference. The order is:

| Port step | Evidence before advancing |
| --- | --- |
| Freeze the M6 reference | Clean source build, no executable-code fallback, reproducible whole-game parity receipts |
| Replace one platform interface | Recovered API contract, selected host, observed scenario/state/RNG agreement, declared platform-visible differences |
| Widen host architecture where requested | Stored layouts separated from host pointers; numeric, serialization and save compatibility checks pass |
| Modernize presentation and packaging | Intended differences stated; unchanged engine/persistence checks pass; original assets remain local |
| Change gameplay or fix original bugs when explicitly requested | Separate expected results and retained faithful reference |

A native executable must run the recovered game logic without using the original
executable as a library. The target OS, renderer and other modern libraries are
design choices to be justified by recovered requirements, not guessed during
decompilation.

## Evidence, community references, and project structure

The supplied [community research forum](https://champman0102.co.uk/viewforum.php?f=35)
is a discovery source. A forum label or patch offset is a hypothesis until checked
against the pinned executable. Record the exact post URL, author/date when available,
claimed version, address convention, expected original bytes, and patch behavior.
Translate offsets through the PE section map. Inspect surrounding instructions,
callers, and data references; do not assume the same offset applies to 3.9.60,
3.9.68, GDI, or community-patched binaries. Preserve useful citations in focused
notes. Do not apply community patches to the reference target.

Its "Offsets" thread (topic 1540) has been captured whole and checked
mechanically; results, reproduction commands and mined documents are in
`docs/OFFSETS-COMMUNITY.md`. Two measured cautions belong with any future use of
that material or of similar threads:

- **Address convention differs within one post.** Hex edits there address
  `cm0102.exe` by raw file offset, while disassembly listings and annotated
  instructions use virtual addresses. Reading a hex edit as a VA places 1458 of
  2008 lines outside any section; read as a file offset, 97.3% resolve in the
  pinned build. For `.text`, `.rdata` and `.data`, `VA = file offset + 0x400000`;
  `.rsrc` does not follow that rule. State the convention for every claim.
- **Agreement is not proof of behaviour.** Because so much of the thread does
  match our bytes, an offset that resolves is easy to over-trust. A matching
  byte establishes only that the claim was made against a build that agrees with
  ours at that address. It does not establish what the code does, which path is
  taken, or that the described behaviour is complete. The original instructions
  remain the only specification, and a patch is never the engine's design.

Maintain reviewable material separately from large generated and supplied files:

| Material | Purpose |
| --- | --- |
| `scripts/` | Reproducible import/export, verification, trace, and comparison commands |
| `docs/ROADMAP.md` | Milestone gates and sequencing |
| Focused analysis notes | Startup, subsystem hypotheses, source citations, uncertainty, and evidence |
| Machine-readable recovery ledger | Function address/bytes, types, status, dependencies, tests, and confidence |
| Local generated analysis directory | Copied inputs, Ghidra projects, inventories, disassembly, and decompiler output |
| Future recovered source and harness directories | Reviewed application recovery and behavioral validation support |
| Local runtime corpus | Original saves, traces, snapshots, and manifests; kept out of publication by default |

The community intake under `CM0102/Analysis/Community-Offsets/` is deliberately
split across that boundary. Reviewable findings, the reproduction commands and the
caveats live in `docs/OFFSETS-COMMUNITY.md`. The bulk evidence - the captured
forum transcript, per-claim verdicts and the mined subsystem documents - stays in
the local, ignored analysis directory alongside the other generated material,
because it reproduces and quotes third-party posts at length. Nothing in it is a
recovery status; it is input to one. Back up that directory if the workspace is
ever cleaned, since regenerating it depends on the forum still serving the
thread.

Choose concrete locations as each artifact is introduced rather than creating empty
directories that imply completion. The workspace currently has no Git history;
version-control setup is a separate housekeeping decision, not a prerequisite for
the first reproducible slice. Never mistake generated pseudocode for authored,
compiled, or validated recovery.

## Risks and decision gates

- **Runtime reference unavailable:** prioritize a controlled original environment;
  continue static work but withhold claims of parity.
- **Decompiler errors or ambiguous boundaries:** check instructions and callers,
  repair analysis metadata with evidence, and retain uncertainty where unresolved.
- **Opaque global state and pointer graphs:** capture layouts and ownership before
  isolating routines; expand a slice if a tiny harness would misrepresent behavior.
- **RNG or floating-point divergence:** compare the earliest state transition and
  call sequence; never compensate by fitting output distributions.
- **Version-mismatched community offsets:** reject mappings whose original bytes
  or surrounding structure fail validation. The dominant risk in this project is
  now the opposite one: most of the catalogued community offsets do resolve
  against the pinned build, so an unverified adoption is more likely to look
  plausible than to fail loudly. Require the original bytes, surrounding
  instructions and enclosing function to be recorded before a claim informs any
  recovered code.
- **Misreading decompiled constant pools:** data constants live inside `.text` in
  this binary, so decompiler output mixes real instructions with operands read
  as code. A confirmed example is a 365-day window constant at VA `0x00807BAA`.
  Label constant pools before interpreting a function body.
- **Long-tail game behavior:** extend scenarios around exceptions, promotions,
  competition boundaries, and long careers; report coverage limits explicitly.
- **Scope exhaustion:** complete one evidence-backed slice at a time. There is no
  credible calendar estimate before the function inventory and first recovery
  establish actual difficulty.

## Next sessions after the foundation

1. The first bounded slice is the RNG pair; see [RNG-INIT.md](RNG-INIT.md).
   Residual CRT/OS dependencies are `time()` and sprintf. Table data still comes
   from the original image.
2. Continue M2 with `CM0102/Runtime/reference-20260927-verified`: new-game completion,
   starting-save restore, two weeks of progression and simulated matches are
   demonstrated in GDI. The later-save restore and two posted sequences are now
   preserved in `replay-20261003-verified-b`; semantic input acknowledgement and
   determinism remain open. The four RNG bodies/constants and one Date fragment
   have bounded primary/GDI mapping. The isolated VM demonstrates primary
   new-game, save/restart/load, calendar progression and one simulated match.
   Repeat primary scenarios and characterize consumed-input replay before M4.
   M2 is not yet fully accepted.
3. M4: integrate the validated RNG slice on a copied executable only after M2
   can replay a scenario that uses it.
4. The full `0x005CCCD0` map identifies its three-phase/day loop and selected
   subsystem call order; see [ORCHESTRATION.md](ORCHESTRATION.md). The second
   compiled slice now covers date arithmetic and its negative-increment dependency;
   see [DATE.md](DATE.md). Weekday calculation `0x0054A3F0` and adjustment `0x0054A2D0` are now
   compiled and differentially checked at O0/O2 (446,623 cases each). Adjustment
   excludes exception unwinding and its path-dependent EAX; see DATE.md.
   The [family extension](DATE-FAMILY.md) now recovers constructor `0x00549F70`
   and initializer `0x0054A130`, including diagnostic call ordering under modeled
   CRT/UI hooks. Sixteen compiled routines preserve the weekly/difference defects
   and explicit return-register/SEH residuals. The [localization extension](LANG-FORMAT.md)
   now compiles wrapper `0x0066F4E0`, its language getter, five date-name/order
   helpers and ordinal numbers under modeled boundaries. The [lookup extension](LANG-LOOKUP.md)
   now recovers both lookup paths, comparator and separate CRT search support.
   The [numeric date extension](DATE-NUMERIC.md) compiles the two numeric routines
   with original prior-frame month-byte contracts. The [date text extension](DATE-TEXT.md)
   compiles the other seven mapped formatters with byte/dword stack contracts.
   Verify their separate O0/O2 acceptance, then recover localization loading and
   morphology `0x006701F0`. Its [complete application-body acceptance](LANG-MORPH-PARENT.md)
   now passes 323,545 comparisons per O0/O2 build. Verify its full ledger and
   audit and the [accepted recovered-probe relink](LANG-MORPH-RELINKED.md).
   Its replacement O0 controller finished with exit zero/PASS; verify the ledger
   and the preserved twelve-ledger `scripts/map_morph_search_family.py`. The
   current thirteen-ledger map is `scripts/map_crt_length_family.py`; separately
   accepted length support adds one CRT entry. [String support](CRT-SEARCH.md) is
   now [composed into morphology](LANG-MORPH-SEARCH.md) after full O0/O2
   acceptance; verify its separate seventeen-source ledger. Classification is maintained but not linked into case
   conversion; validate that composition separately. Recover locale,
   formatting and data dependencies for an end-to-end localization build.
   Keep M2 replay as a prerequisite to integration.
   Preserve original uninitialized month storage and accepted source identities.
5. Use the reproducible instruction-backed source-path map for 1,925 functions
   ([RECOVERY-MAP.md](RECOVERY-MAP.md)); 29 have multiple candidate paths.
   Ownership remains tentative and no automatic renaming was applied. Verified
   semantic names and `.text` data-pool annotations remain follow-up work.

At each handoff state exactly what is reproducible, what is inferred, what passed,
and which milestone gate remains unmet.

The full morphology application body is accepted under its finite differential
contract. The compiler probe is now recovered and linked. Original data,
CRT fixtures and the standalone build remain open; see
[the current handoff](LANG-MORPH-SEARCH.md).
