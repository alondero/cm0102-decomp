# ADR 0009: Captured entry inputs from the running original game

Status: proposed and implemented 2026-11-11 at the project owner's request to
remove the recovery throughput limit. Release validation ([ADR 0007](0007-harness-releases.md)) finished 2026-11-12:
11,719 tiers passed, 0 failed, 0 inconclusive, 0 pending; closing gate exit 0. The owner
decisions this needs are listed at the end.

Addresses are preferred-image VAs (base `0x00400000`) in the pinned 3.9.68
executable, SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

## Context

On 2026-11-11, 1,139 functions (2.11 MB, 79% of all bytes without accepted C)
had no lifted tier because their test inputs never reach 0.6 instruction
coverage. A function cannot enter the C queue without a lifted tier, so this
set was closed to every translator regardless of how many ran. A four-hour
multi-provider session that day netted 207 bytes.

A stop census over the 1,068 of them in the lift queue
(`CM0102/Analysis/cmre/throughput-supply-20261111/census/`, 600 cases each):
the dominant non-return stop is a memory fault for 87% of the measured
functions, most often inside a callee. Random arena bytes are not game
objects.

Two cheaper ideas were measured first and do not work:

- Real memory with guessed arguments. One process image (41 MB, extracted from
  the reference VM) under the unchanged emulator, driving every inventory
  function with pointers harvested from that image: 61 functions (25 KB)
  reached 0.6.
- More random cases. The 2026-10-21 review already found a 40-times larger
  budget changed 1 of 9 sampled functions.

The pinned primary executable runs and renders on the development host at its
preferred base. Its `.text` in memory equals the file bytes.

## Decision

Add one opt-in input source: a **capture** is the input one function received
in the running, unmodified game. It holds the registers and x87 control word at
the function's first instruction, the next 64 stack words, and the 4 KiB pages
that one emulated call of the original function touched, read from the frozen
process at that moment. It holds no result and no behaviour.

1. **Recording** (`cmre capture record --pid PID`, tooling). A debugger
   attaches to the running game. The only bytes it writes are a one-byte
   breakpoint at a target entry and the original byte that restores it. At a
   hit it emulates that call in the unchanged emulator; absent pages are read
   from the frozen process on first touch. The call is then repeated from the
   stored pages alone and must give the same outcome and registers. Detaching
   restores every entry and leaves the game running.
2. **Store** (`cmre/capture.py`, harness module). A capture is named by the
   SHA-256 of its canonical bytes and lives under `local/captures/fn_<va>/`.
   It is bound to the pinned target hash and to one function. Captures contain
   original game memory: they are local-only and are never published.
3. **Replay** (`cmre/emu.py`, `cmre/difftest.py`, harness modules). A hint may
   list capture names under `captures`. Such a case uses the captured registers
   and control word, the first `arg_count` captured stack words on the test
   stack, a zero arena, and maps the captured pages at their original
   addresses on both machines for that case only. The function's own frame uses
   the test stack exactly as in every other case, so the caller's real frame is
   ordinary compared memory and every existing comparison rule applies
   unchanged. A captured page may replace a page of a writable original section
   (import slots keep their test stubs) or occupy unmapped memory. Code,
   read-only data and harness memory are refused.
4. **Mix.** When fixtures or captures exist, each case chooses one of them with
   probability one half, as fixtures already did. Random cases remain.
   Functions without captures draw the same random stream as before, so their
   result digests do not change.
5. **Expansion** (`cmre capture expand`, tooling). Coverage-guided mutation of
   stored captures on the original side only: change one to three registers,
   stack words or bytes the run read as input; keep the mutant only when the
   original executes an instruction of the function no stored input reached.
   A kept mutant is stored as a capture whose metadata names its parent and
   edits. It is a derived test input, like a random case, not an observation.
6. **Application** (`cmre capture apply`, tooling). Select a small covering set
   (observed captures first), list it in the function's hints, and run the
   unchanged lifted gate. Acceptance is the existing `cmre accept`.

Nothing is compared differently. No threshold, profile or policy changes.

## What a capture does and does not establish

- A passing tier still means: on these cases the recovered routine and the
  original agree under the tier's comparison profile. Captures make the cases
  ones the game produced.
- Import stubs are unchanged test scaffolding. A captured call that reaches an
  import continues with seeded stub results, as any case does. The emulated
  path after that point is not the path the game took; it is still an identical
  input to both sides. In the first session 139 of 266 reached functions
  (343 KB) called no import.
- Memory the emulated call did not touch is absent. A recovered routine that
  reads other memory faults where the original did not, which is a visible
  mismatch.
- A process page at an address the harness reserves (test stack `0x02000000`,
  arena `0x03000000`, link base and above `0x60000000`) is not captured. Reads
  there see harness memory on both sides. This lowers realism, not validity.
- The recorder is a debugger. It can perturb timing. It never supplies data to
  the game.

## Consequences

- A tier whose hints list captures cannot be re-run from a clean clone without
  those capture files. The hint and ledger pin each capture by content hash, so
  a wrong file cannot pass; a missing file makes the test unavailable, never
  passing ([BUILDING.md](../BUILDING.md) rule for game-dependent checks).
  `local/captures/` joins the retained local evidence and its backups.
- `emu.py`, `difftest.py`, `env.py` and `recover.py` change and `capture.py`
  joins `HARNESS_MODULES`: one harness release, one inventory revalidation.
  Existing tiers have no captures, so their inputs and digests are expected to
  be identical; the revalidation is the proof.
- Coverage from real calls is partial for large functions, because error
  branches do not run in normal play. Expansion and the retained random cases
  raise it. Functions never reached in a play session stay unlifted until a
  session reaches them.

## First measurements (2026-11-11, evidence under `CM0102/Analysis/cmre/throughput-supply-20261111/`)

- Recorder: 1,068 entry breakpoints armed; about 70 ms of frozen process per
  hit. One 15-minute session (database load, new game, one in-game day)
  reached 266 unlifted functions (607 KB) and kept 393 captures.
- Expansion added 1,423 derived inputs in 30 minutes on 16 processes.
- Unchanged lifted gate over the 258 functions with captures: 62 pass
  (79,849 bytes), 181 low-coverage (455,554 bytes; 43 of them at 0.45 or
  above), 6 fail, 7 lift-blocked, 2 could not be built (the host antivirus
  quarantines their linked test executable).
- All 62 were accepted with the unchanged `cmre accept`. The ready C queue went
  from 73 functions (4,202 bytes) to 125 functions (77,783 bytes).
- `fn_009058b0` (45 bytes) was then translated and accepted as semantic C at
  `-O0` and `-O2`, coverage 1.0: the first function through the whole path.
- `fn_008b2880` (829 bytes), one of the 2026-11-03 "deep object graph" examples,
  is among the 62.
- The six failures are lift mismatches that only real inputs reach (escapes
  through indirect transfers, one register difference at return). They are
  findings for triage, not capture defects.

## Decisions for the project owner

1. Accept captures as an input source for lifted and C tiers (this ADR).
2. Accept that capture-backed tiers are re-verifiable only where
   `local/captures/` is present, and keep that store in the local backup set.
3. Not proposed here: lowering `min_coverage` for capture-backed functions, and
   recording real import results for calls that reach `HeapAlloc`, `ReadFile`
   and similar. Both would unlock more; both change evidence policy and need
   their own decision with numbers.
