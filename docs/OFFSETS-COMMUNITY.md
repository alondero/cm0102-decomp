# Community offset research: champman0102 topic 1540

Captured and verified 2026-09-27. Source: <https://champman0102.co.uk/viewtopic.php?t=1540>
("Offsets", 31 pages, 771 posts, 2020-11-28 to 2026-09-17).

This is intake, not specification. Everything below is a **hypothesis about the
engine** until it is confirmed against the original instructions of our own
binary. What changed in this pass is that the material no longer has to be taken
on trust: it can be checked mechanically, and when checked it overwhelmingly
agrees with our bytes.

## Headline findings

1. **Hex-edit addresses in this thread are raw file offsets into `cm0102.exe`,
   not virtual addresses.** Proved from the thread itself and measured against
   all four binaries we hold. 97.3% of hex-edit lines then resolve in our clean
   3.9.68 `cm0102.exe`. See "The address-space question" below.
2. **The community's disassembly is aimed at our exact build.** 1454 of 1779
   quoted instruction lines (81.7%) are byte-identical to our official 3.9.68
   `cm0102.exe`, and 39 posts with at least five listing lines are 100%
   byte-exact. Our own decode of those bytes agrees with the poster's mnemonic in
   1462 cases.
3. **The original developer's source-file names are recoverable.** Post 18779
   lists 791 MSVC build paths from the original tree. 789 of them are exact
   strings at the quoted addresses in our executable. We therefore have the
   original module map of a C++ codebase - see "Recovered module map".
4. **Record strides and the competition/staff/club layout are partly recovered**,
   with several confirmed independently by two or more posters and, in places, by
   the shipped `.dat` files.
5. **The community 8 KB "code cave" claim is wrong in detail but right in
   principle**: the usable slack after `.text` is 3,867 bytes in 3.9.68 and 395
   bytes in 3.9.60, measured from the PE section table and confirmed on disk.

## How this was captured, and how to reproduce it

Everything lives under `CM0102/Analysis/Community-Offsets/` (ignored local
analysis area). Nothing in `CM0102/Game/` was modified, patched or executed.

```powershell
cd CM0102\Analysis\Community-Offsets

# 1. capture all 31 pages, caching raw HTML under raw/
python fetch_thread.py --pages 31 --delay 1.5

# 2. parse into listing / patch / mention claims and labelled globals
python extract_offsets.py

# 3. capstone is required from here on; use the interpreter that has it
py313 = "py -3.13"
& $py313 address_space_test.py     # decides VA vs file offset
& $py313 verify_offsets.py         # per-claim verdicts vs 4 binaries + Ghidra
& $py313 verify_source_paths.py    # the 791 original source paths
& $py313 summarize.py               # the aggregate tables quoted here
& $py313 spot_check.py              # named offsets, decoded in place
```

`fetch_thread.py` records the SHA-256 of every captured page in `posts.json`, so
a later session can prove it read the same bytes.

## The address-space question

The thread mixes two conventions, sometimes inside one post. Both are recorded
separately by the parser, and the distinction is essential:

| Claim kind | Address means | Evidence |
| --- | --- | --- |
| Disassembly listing (`006B5B41 8B75 19 MOV ESI,...`) | **virtual address** | 100% byte agreement against our build as VAs |
| Annotated instruction without bytes (`005F9C47 - MOV ECX,[9CF338]`) | **virtual address** | 96.1% mnemonic agreement against our own decode |
| Hex edit (`004CDC56: 01 00`) | **raw file offset** | 97.3% resolve at the quoted offset in our file; 1.4% as VAs |

The proof is internal to the thread. Post **131884** (hodgy) quotes the same
byte twice, in two notations:

```
in a disassembly listing:  008CEBB5  A1 24F49C00  MOV EAX,DWORD PTR DS:[9CF424]
in a patch line:           004CEBB5: A1 B8
0x008CEBB5 - 0x00400000 == 0x004CEBB5
```

and our byte at file offset `0x004CEBB5` is `A1 24 F4 9C 00`, i.e. exactly the
`MOV EAX, [0x009CF424]` the listing describes.

`address_space_test.py` scores both readings over all 2008 hex-edit lines:

| Reading against our 3.9.68 `cm0102.exe` | Lines agreeing |
| --- | --- |
| as a virtual address | 57 (1.4%), with 1458 unmapped |
| as a raw file offset | 1897 (94.5%); 1954 (97.3%) resolve to a mapped offset |

**Consequence for our workflow:** every hex edit in this thread can be used
directly against our binary as `file offset`, and `VA = file offset + 0x400000`
for `.text`, `.rdata` and `.data`. Section table of our 3.9.68 target:

| Section | VAddr | VSize | RawPtr | RawSize |
| --- | --- | --- | --- | --- |
| `.text` | `0x00401000` | `0x005650E5` | `0x00001000` | `0x00566000` |
| `.rdata` | `0x00967000` | `0x0001F1CE` | `0x00567000` | `0x00020000` |
| `.data` | `0x00987000` | `0x00153000` | `0x00587000` | `0x00153000` |
| `.rsrc` | `0x00DE5000` | `0x00001238` | `0x006DA000` | `0x00002000` |

`.text`, `.rdata` and `.data` all satisfy `VA - file offset == 0x00400000`.
`.rsrc` does not (`0x0070B000`) and is not a target of any claim we found.

## Reading a quoted byte pair

A hex edit quotes `ADDRESS: ORIGINAL REPLACEMENT`. The replacement is by
definition a value the poster wants our build to *stop* holding, so "our bytes
differ" is the expected result for a well-formed patch spec and is **not**
evidence the claim is wrong. The informative test is whether our byte equals the
*first* quoted value:

| Outcome in our clean 3.9.68 file | Lines | Reading |
| --- | --- | --- |
| holds the **first** quoted value | 1627 | post is `original -> replacement`; the poster's build agrees with ours here |
| holds the quoted pair exactly | 246 | the post is quoting a current value (often a bulk table) |
| holds the **last** quoted value | 24 | post describes a state we do not have (patched build, or a revert) |
| no byte pair quoted | 111 | address only |

38 posts (1902 lines) resolve at 100% against our file, including the three
largest tables in the thread: post 150813 (1011 lines, a nation rename table),
post 53402 (294 lines) and post 7557 (65 lines, transfer windows plus "prevent
the board cancelling deals"). Post 139399 resolves at 55.3% and is aimed at a
patched build.

## What the human analysis contains, and where to read it

The thread is not a list of offsets. A large part of it is annotated
reverse-engineering: routines explained, record layouts deduced, data tables
labelled. Four documents in `CM0102/Analysis/Community-Offsets/mined/` hold the
mined result, each with per-claim provenance:

| File | Contents |
| --- | --- |
| `01-engine-routines.md` | 20 documented routines/subsystems: host and participant tables, Champions League group draw, next-match resolution, PRNG, league table row and sorter, competition object and phase machine, European distribution, cup round descriptors, knockout draw, transfer windows, fixture registration, calendar, person registry, work permits, call-up eligibility |
| `02-data-model.md` | 12 record families, 398 distinct labelled `.data` addresses, record strides, nation/competition ID evidence, global pointer tables, open questions |
| `03-patch-offsets.md` | 1504 distinct addresses, 12 subsystem groups, our bytes and file offset per row, bulk-table rules |
| `04-builds-and-tooling.md` | version attribution per post, the posts that do not match our build, measured code-cave facts, community tooling, 89 distinct linked forum URLs |

Machine-readable outputs, all regenerable: `posts.json` (771 parsed posts),
`claim_instances.json` (5964 claim lines), `offsets.json` / `offsets.csv`
(condensed catalogue), `verified-offsets.json` / `verified-offsets.csv` (per-claim
verdicts, our bytes, our disassembly, enclosing Ghidra function),
`globals.json` (714 labelled data addresses), `source-paths.json` (789 verified
original source paths).

## Recovered module map

Post 18779 (Xeno, 2021-02-26) lists the original build paths embedded in the
executable. 789 of the 791 quoted lines are exact strings at their quoted
addresses in our binary; the 2 exceptions are one line duplicated in the post
with a typo (`wc_south_american_league .cpp`).

Original tree, verbatim from the strings:

```
E:\dev\CM3\cm3 00-01\cm3\code\        <- the game
E:\dev\CM3\cm3 00-01\si\code\         <- the Sports Interactive layer
```

| Directory | Files | What it is |
| --- | --- | --- |
| `comp\leagues` | 172 | per-nation league definitions (`eng_prm.cpp`, `bra_reg_sp.cpp`, `jap_j1.cpp`, ...) |
| `code` (root) | 154 | the engine proper: `Database.cpp`, `History.cpp`, `match_eng.cpp`, `match_pl.cpp`, `transfer_manager.cpp`, `player_regen.cpp`, `search_eng.cpp`, `human_manager.cpp`, `tactics.cpp`, `injury.cpp`, `finance.CPP`, `scouts`... |
| `comp\cups` | 120 | per-nation cups (`eng_fa_cup.cpp`, `wel_cup.cpp`, `cup_stage.cpp`) |
| `ruling_body` | 88 | the disciplinary system, per nation (`ruling_body.cpp`, `rb_northern_ireland.cpp`) |
| `award` | 84 | awards and ratings per nation, plus `year_award.cpp`, `year_ratings.cpp` |
| `transfer` | 50 | per-nation transfer rules (`england_rules.cpp`, `italy_rules.cpp`) |
| `si\code` | 34 | platform layer: `main.cpp`, `os.cpp`, `network.CPP`, `tcpip.cpp`, `zipdir.cpp`, `Langlib.cpp`, `display.cpp`, `Date.cpp`, `Area.cpp` |
| `comp\eurocomp` | 30 | European and continental club competitions (`uefa_cup.cpp`, `intertoto_cup.cpp`, `conmebol_liber.cpp`) |
| `comp` | 26 | competition core (`comp.cpp`, `Cup.cpp`, `league.cpp`, `fix_man.cpp`, `fifa_rankings.cpp`, `uefa_seeding.cpp`, `stadium.cpp`) |
| `comp\intercomp` | 15 | international national-team competitions (`euro_champ.cpp`, `olympics.cpp`, `fifa_confed.cpp`) |
| `comp\worldcup` | 12 | World Cup and regional equivalents |
| `comp\friendly` | 4 | `mini_cup.cpp`, `mini_league.cpp` |

395 distinct modules, 389 `.cpp` plus 1 `.h` (`si\code\network.h`) and 5
uppercase `.CPP`. **The engine is C++**, which is the single most useful fact for
planning the port: expect classes, vtables, `this` pointers and STL containers
rather than the flat C that a Ghidra x86:LE:32 decompile will first suggest.

## Worked examples, checked against our bytes

Each row was decoded from our own `cm0102.exe` by `spot_check.py`.

| File offset | VA | Section | Claim (post) | Our bytes and decode |
| --- | --- | --- | --- | --- |
| `00011CD5` | `00411CD5` | .text | competition object field, `[ESI+3C]` stage index (p43678, Xeno) | `90` = `NOP`; our build holds the *original*, the post writes `66` |
| `00011D82` | `00411D82` | .text | same post, function tail | `5E C3` = `POP ESI; RET`; post turns `C3` into `C7`, i.e. converts an early return into a store |
| `000136A1D` | `00536A1D` | .text | `CMP AX,0FFFE` dispatch, wonderkid rate (p49418, Nick+Co) | `66 3D FE FF 0F 85 ...` = `CMP AX,0xFFFE; JNE 0x536AAC`, then `PUSH 0xA; CALL 0x90CFC0`. Exactly as described |
| `0003AFC4B` | `007AFC4B` | .text | find-all-players guard (p48796) | `0F 8D 9D 00 00 00` = `JGE 0x7AFCEE` |
| `0004CDC56` | `008CDC56` | .text | prevent the board cancelling deals (p7557) | `01 00 00 00` = a DWORD flag `1` at the tail of `FUN_008CDB80`; post sets it to `00` |
| `000407BAA` | `00807BAA` | .text | "expiring" search window (p48643) | `6D 01` = the constant `0x016D` = **365** days, stored as data inside `.text`. Post writes `B6` |
| `000566F00` | - | .text slack | English transfer window (p7557) | zero-filled cave; the post injects code there |

Note the last two: the engine keeps **data constants inside `.text`**, and
Ghidra will disassemble them as nonsense instructions. Any decompilation pass
should expect to find and label inline constant pools.

## The code cave, measured

Post 159201 claims `0x009660E5` is a region of `cm0102.exe` where code can be
written freely "for 8 kilobytes straight". Measured from the PE section table and
confirmed byte-for-byte on disk:

| Build | `.text` ends (VA) | next section | usable bytes | present in file? |
| --- | --- | --- | --- | --- |
| 3.9.68 `cm0102.exe` | `0x009660E5` | `.rdata` `0x00967000` | **3,867** | yes, all zero |
| 3.9.68 `cm0102_GDI.exe` | `0x00965925` | `.rdata` `0x00966000` | 1,755 | yes, all zero |
| 3.9.60 `cm0102.exe` | `0x00954E75` | `.rdata` `0x00955000` | **395** | yes, all zero |

In all three builds the virtual gap equals the run of zero bytes physically
present after the last non-zero `.text` byte, so the figures are real file slack
and not merely unmapped address space. The region is exactly what the community
uses for injected code, but "8 KB" is wrong: for our baseline the figure is
3,867 bytes.

## Version attribution, and what must not be applied to our binary

223 posts carry a version keyword, and 200 of those name something other than a
clean 3.9.68 - the Saturn patch (48 posts), the Tapani patch (10), Nick's patcher
(101 mentions), CM0102Patcher (7), or an unnamed modified executable. Only 23
name clean 3.9.68 exclusively. This attribution is a keyword scan reported in
`04-builds-and-tooling.md` rather than a script output of this directory, so treat
the per-post column as a lead and the per-post byte agreement figures as the
authority.

Six posts with substantial listings do not match our build at all and must not be
applied: **28023** (0/31), **28035** (0/31), **139425** (0/28), **140006** (0/28),
**162260** (0/20), **167134** (0/10). In several cases the reason is benign - the
poster is showing *their own injected code* (post 159201 scores 1.2% because it is
an ELO system the author wrote and injected, not original engine code).

The only off-thread reference made inside the thread, `viewtopic.php?f=35&t=1395`,
was fetched and is a single sticky post, "Introduction: Patches", not a second
offset database. The thread links to 89 distinct forum URLs in total (reported in
`04-builds-and-tooling.md`); none of the others were fetched.

## Rules for using this material

These follow from the evidence above and from the project rules in `AGENTS.md`.

1. A byte match proves the claim is aimed at a build that agrees with ours at
   that address. It says nothing about whether the described behaviour is
   correct or complete.
2. Never use a patch as the engine specification. Recover all paths from the
   original instructions; the patch is only an assertion about where a behaviour
   lives.
3. Keep the two address conventions apart. Convert hex edits by file offset;
   treat listings and annotated instructions as VAs.
4. Do not carry a GDI-runtime conclusion to the primary executable. The GDI
   binary is a different build with its own layout.
5. Record, for every adopted claim: post URL, author, date, claimed target,
   address space, our bytes, surrounding instructions, enclosing Ghidra function,
   and a verdict of *mismatch*, *unresolved*, *statically supported* or
   *dynamically validated*.
6. Hybrid calls into the original code stay explicit until the standalone-build
   gate in `docs/ROADMAP.md` is met.

## Concrete next steps this enables

1. **Name the Ghidra functions by module.** The 789 verified `__FILE__` strings
   sit in one initialised `.data` blob from `0x009870F8` upward. An
   assert-filename string is almost always referenced by code belonging to that
   translation unit, so each string gives a candidate owning module for nearby
   functions. This is a naming aid, not a proof of ownership; a future Ghidra
   script should confirm each by checking the referencing instruction.
2. **Harvest the inline constant pools in `.text`** (starting with the search
   constants such as `0x016D` at `0x00807BAA`) and annotate them, so the
   decompiler stops treating them as instructions.
3. **Adopt the record strides as struct hypotheses** and check each field against
   the shipped `.dat` files before adopting it as a layout.
4. **Resolve the recorded internal conflict**: posts 16076 and 163324 both claim
   a word at cup round descriptor `+0x07` means different things (round name vs
   leg-2 date). Only the instructions at that address can settle it.
5. **Use the 20 documented routines as an ordered work list** for the first
   faithful decompilation pass, since each already has a human explanation to
   check our output against.

## Files added by this pass

Under `CM0102/Analysis/Community-Offsets/` (ignored analysis area, not for
publication):

- `fetch_thread.py`, `extract_offsets.py`, `verify_offsets.py`,
  `address_space_test.py`, `verify_source_paths.py`, `spot_check.py`,
  `summarize.py`, `final_figures.py`, `show_modules.py`, `probe_linked.py`
- `posts.json`, `thread.md`, `claim_instances.json`, `offsets.json`,
  `offsets.csv`, `offsets.md`, `verified-offsets.json`,
  `verified-offsets.csv`, `globals.json`, `source-paths.json`
- `raw/` - the 31 cached HTML pages, each hashed in `posts.json`
- `mined/01-engine-routines.md`, `mined/02-data-model.md`,
  `mined/03-patch-offsets.md`, `mined/04-builds-and-tooling.md`

No file under `CM0102/Game/` or `CM0102/Original-3.9.60/` was modified, and no
game file was executed.
