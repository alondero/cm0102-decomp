# Application orchestration (VA 0x005CCCD0)

Input: official 3.9.68 `cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Preferred-image VAs. This is a static map of the function the application
wrapper calls after RNG init. It is **not** a recovered, compiled, or validated
module.

Ghidra body: 4361 bytes (`FUN_005cccd0`). Dump:
`CM0102/Analysis/rng-slice/orchestration-prefix.txt` and
`orchestration-calls.json`.

## Sequence established from instructions

1. SEH frame (`push 0x0095A443`), stack storage with an eight-byte Date value
   constructed by `0x0054A110` (21 bytes). The 16-byte reservation is not
   evidence of a 16-byte Date type; see [DATE.md](DATE.md).
2. Store `0x005D61B0` at `0x00DD6956` (unaligned dword; meaning unresolved).
3. `0x0068D520` message helper (Peek/Get/Translate/DispatchMessageA).
4. `0x0090BDE0("SI_DATA", "logo.rgn")` then `0x005E40D0` to load the picture.
   `0x0090BDE0` builds `SI_DATA` + `'\\'` (`0x009C3730`) + filename into
   `0x00DE279C`.
5. `0x0090CFC0(0x14)` — bounded PRNG, bound 20. Non-zero loads `eidos.rgn`;
   zero loads `savechip.rgn`. Same path helper and `0x005E40D0`.
6. Load `kio.rgn`.
7. If `0x00DD7EEC` is set, resolve `CM3_SOUNDS\intro1.wav` / `intro2.wav` and
   wait on `0x0084CC80` while pumping messages.
8. Direct call to `0x0081B890` (assertion string
   `E:\dev\CM3\cm3 00-01\cm3\code\Setup.cpp`). That function looks up env
   `CM3_QSTART`, parses command line `-seed` / `-nonetwork`, and calls
   `0x0090D0A0` again (see [RNG-INIT.md](RNG-INIT.md)).

A naive scan of `E8` bytes inside the 4361-byte body reports 153 hits / 95
targets, including obvious false positives (`0x3B6591BA`, `0x0DE7FFB7`). The
list is a lead, not a call graph. Confirmed repeats include `0x0090BDE0`,
`0x005E40D0`, `0x0090CFC0` (five times), `0x0068D520` (six times).

## Full-function export and correction

The original prefix dump and E8 scan remain historical evidence. The new
instruction-based export in `CM0102/Analysis/recovery-map-b/` supersedes that
scan: **152 call sites, 150 direct calls, two unresolved indirect calls, 92
distinct direct target VAs**. Both indirect sites are retained, not discarded:
`0x005CD31F` calls `[EAX+0x14]`; `0x005CD725` calls `[EAX+0x98]`.
Reproduce with [RECOVERY-MAP.md](RECOVERY-MAP.md). Instruction bytes, references
and complete unvalidated pseudocode are exported.

The rest of the function is not merely startup/UI work. It contains a recurring
date/phase update loop, with simulation-related callees. The previous note's
UI-heavy characterization described only the prefix.

## After Setup: established sequence

All addresses below remain primary-binary preferred-image VAs, not GDI addresses.

1. `0x005CD0BD` calls Setup `0x0081B890`. Its zero result takes cleanup via
   `0x0082FF40(1)` and returns. Setup itself sets phase byte `0x00AE2C88 = 0`,
   parses `-seed`/`-nonetwork`, and dispatches screen callbacks before returning
   one. The UI callbacks contain the actual new/load-game choices; simply calling
   Setup is not proof of successful initialization.
2. `0x005CD126` invokes `0x00417C10` with the current-date object at
   `0x00AE2C90`. Conditional work under `0x00DD7124` iterates the final 16
   entries of a `0x6E`-stride array. Meanings of these fields remain unresolved.
3. Loop begins at `0x005CD246`. A changed year word at `0x00AE2C92` triggers
   `0x005AA2D0`; that callee directly references `fix_man.cpp`.
4. Pending work under `0x00B63C98` dispatches `0x0081FEA0` for up to 16 slots
   and calls `0x007FC7E0`. A 0x38-byte object is allocated and constructed by
   `0x006B3C10` (direct `match_day.cpp` path evidence), stored at `0x00B67A50`.
   It calls `0x006B42A0`, `0x006B4360`, visits non-null entries of
   `0x00ADADFC` through virtual slot +0x14 with date/zero arguments, calls
   `0x0086DDA0`, then destroys/frees the object and clears the global.
5. Date/phase comparisons guard another update block. It walks a 0x6E-stride
   array via `0x0053AB90` (`Database.cpp` candidate), then calls `0x008C5880`
   and `0x004C7AC0` with date and phase.
6. `CMP byte [0x00AE2C88],2` at `0x005CD40B` gates the large phase-2 block.
   It contains 0x245-stride record walks, a 50-pointer loop and a signed
   day-word modulo-7 condition. These are instruction-derived layout/ordering
   observations; field names and formulas must not be inferred from them alone.
7. After this block, `0x005CDE40` is called repeatedly until AL is zero
   (`0x005CDBF9`, `0x005CDC0A`). Its event/UI semantics remain to be recovered.
8. `0x005CDC16` loads the **byte** phase, increments AL, compares unsigned to 2,
   and stores it. Values above 2 reset to zero. At `0x005CDC39`, ECX is the date
   object `0x00AE2C90`; the call to `0x0054C5B0` receives output storage and
   increment 1. `0x005CDC49` calls `0x00417C10` to assign the result back.
   Thus normal phases cycle **0 -> 1 -> 2 -> 0 with a one-day advance**.
9. When `0x009CD1F8` is nonzero and phase is zero, further record/RNG work runs,
   including bounds 2, 70 and 5, followed by `0x00754A30` and `0x00757C70`.
10. `0x005CDDB5` calls date formatter `0x0054BA20` (direct `Date.cpp` reference)
    into `0x00DE1F64` with argument 2000 and phase. `0x00802F80` follows, then
    the message helper at `0x005CDDCF`; `0x005CDDD4` jumps back to the loop.

## Selected phase-2 call order

These source labels come from direct path references **inside the callee**, not
from guessed names or proximity. They identify candidate ownership, not a full
behavioral contract. Calls without a path candidate remain unnamed in the export.

| Call site | Target | Candidate source |
| --- | --- | --- |
| `0x005CD9D2` | `0x009035A0` | uefa_seeding.cpp |
| `0x005CD9F1` | `0x005CE2D0` | game.cpp |
| `0x005CDA10` | `0x005D5F20` | game.cpp |
| `0x005CDA46` | `0x00839E60` | shortlist_manager.cpp; gated by signed day % 30 == 0 |
| `0x005CDA7A` | `0x007A65D0` | player_regen.cpp |
| `0x005CDAC9` | `0x0084BE20` | simulated_stats.cpp |
| `0x005CDAD4` | `0x0068F9C0` | manager_manager.cpp |

## Date evidence and next bounded slice

`0x0054A130` references `si\code\Date.cpp`, checks day 1..31/month 0..11,
stores the year at +2, chooses cumulative month tables and stores a dword leap
flag at +4. `0x0054C5B0` reads a signed 16-bit day and a 16-bit year, adds a short
increment, and rolls through 365/366-day years. It has shared result storage,
lazy initialization and a negative-increment dependency on `0x0054C8E0`.
Ghidra also reports removed unreachable blocks. Do not compile its pseudocode
without checking the instructions, hidden return-buffer ABI and edge cases.

This gives an evidence-backed next slice: recover the small date family and its
data layout with a differential harness. The large orchestration function remains
uncompiled/unvalidated, and none of its callees gains validation merely by being
named in this map. No engine behavior was substituted or integrated this session.

Follow-up: [DATE.md](DATE.md) now records compiled x86 recovery and differential
validation of default construction, assignment, addition and subtraction. The
negative-increment dependency and hidden return-buffer ABI are resolved for that
slice, including the original `0x8000` recursion defect. The complete
`0x0054A130` constructor remains uncompiled; orchestration remains static evidence.
