# Date arithmetic recovery

Current extension: [DATE-FAMILY.md](DATE-FAMILY.md) documents sixteen compiled
routines, the complete selected family inventory and constructor/diagnostic,
weekly arithmetic, conversion and difference recovery. Historical validation
runs below remain evidence for their recorded source versions.

Primary input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Analysis reads the hash-identical `CM0102/Analysis/baseline-c/cm0102.exe`.
All original addresses below are **preferred-image VAs**, base `0x00400000`;
the harness maps the original at that base. No GDI equivalence is claimed.

## Delivered slice

`recovery/date/cm_date.c` contains instruction-derived C, compiled to **32-bit
x86** and compared against the original x86 with Unicorn. The ledger is
`recovery/date/ledger.json`. These routines are not integrated into a game.

| Routine | VA | Bytes | Status |
| --- | --- | ---: | --- |
| Default constructor | `0x0054A110` | 21 | identified, recovered, compiled, corpus-validated |
| Assignment returning a value | `0x00417C10` | 51 | same |
| Add signed-word days | `0x0054C5B0` | 422 | same, exceptional-path qualification below |
| Subtract signed-word days | `0x0054C8E0` | 433 | same, exceptional-path qualification below |
| Weekday calculation | `0x0054A3F0` | 118 | recovered, compiled, corpus-validated |
| Weekday adjustment | `0x0054A2D0` | 273 | recovered, compiled, normal-return corpus-validated; SEH residual |
| YMD initializer (previous constructor candidate) | `0x0054A130` | 412 | compiled; normal-return side effects validated in the family extension |
| YMD constructor returning this | `0x00549F70` | 414 | compiled; normal-return ABI/state validated in the family extension |
| Add unsigned-byte weeks | `0x0054C770` | 341 | same |
| Subtract unsigned-byte weeks | `0x0054CAB0` | 356 | same |
| Absolute year/day components | `0x0054CC30` | 358 | same, excluding transient SEH/unwinding |
| Signed day distance | `0x0054CDB0` | 426 | same |
| Day of month | `0x0054CF70` | 56 | AX/state validated; upper EAX excluded |
| Zero-based month | `0x0054CFB0` | 46 | normal-return ABI/state validated |
| Leap-year test | `0x0054CFE0` | 85 | same |
| Diagnostic version pointer | `0x0090D130` | 6 | same; support outside contiguous family |

The family is associated with `si\code\Date.cpp` by the constructor's direct
path reference at `0x009B8478` and its calls into these routines through
`0x0054A2D0`. Ownership of every original translation unit is not established.
The two YMD routines include diagnostics and optional weekday adjustment.
Their application logic and version getter are now recovered; the CRT splitpath,
sprintf and error-UI calls remain explicit, hooked dependencies. Their distinct
EAX contracts and residuals are documented in the family extension.

## Layout and ABI

The Date **value** is eight bytes, not the 16-byte stack allocation reserved by
the orchestration function. Larger stack allocation alone is not type evidence.

| Offset | Width | Observed interpretation |
| --- | --- | --- |
| +0 | 16 bits | zero-based day of year; signed comparisons in arithmetic |
| +2 | 16 bits | unsigned year, zero-extended before leap calculations |
| +4 | 32 bits | leap-year flag; arithmetic recomputes it, assignment copies it |

Constructor month tables at `0x009B8448` / `0x009B8460` start with -1,
so day 1/month 0 becomes ordinal 0. Month is zero-based. The default constructor
stores **40, 1972, 1**, regardless of current time. These values are preserved.

The original four routines receive `this` in ECX. Default construction returns `this` in
EAX and uses plain RET. The other three take two stack dwords and use `RET 8`:
hidden result-buffer pointer first, then source pointer or increment. EAX returns
the buffer pointer. In particular, assignment modifies `this` **and** writes an
eight-byte copy to the hidden result buffer; treating it as a normal C++ assignment
returning a reference would be wrong. Caller `0x005CDC39` and assignment at
`0x005CDC49` demonstrate this return-buffer convention in the daily loop.

## Instruction-derived arithmetic and state

Addition and subtraction test the signed **low 16 bits** of the argument. For a
negative argument they assign the input to their own shared result, negate the
full 32-bit argument, call the opposite operation and assign its result back.
The upper argument bits do not change the ordinary low-word result.

The nonnegative path adds/subtracts in **16 bits before normalizing**. Addition
only rolls forward while the resulting signed day is at least the current year's
365/366 days. Subtraction only rolls backward while the signed day is negative,
using the preceding year's length. Overflow can therefore leave an apparently
invalid date: this is original machine behavior, not corrected calendar logic.
Year increment/decrement wraps at 16 bits. Leap logic uses divisibility by 4,
100 and 400 on the zero-extended year, including year 0 and years above 32767.

| State | Add | Subtract |
| --- | --- | --- |
| Shared eight-byte result | `0x00AE2420` | `0x00AE24C0` |
| Lazy initialization guard byte | `0x00AE2469` | `0x00AE249E` |
| Registered exit callback (single RET) | `0x0054C760` | `0x0054CAA0` |

Only guard bit 0 is tested/set; other bits survive. First use writes the default
date and calls CRT registration `0x0094627A`. Shared state is intentionally retained:
this is not a reentrant replacement library. Initialization can overwrite an input
that aliases its own scratch object. Tests include this ordering.

**Original recursion defect:** an argument whose low word is `0x8000` remains
negative after negation. Addition and subtraction alternate indefinitely. Both
original and recovered code reach the same first 32 alternating entries with
matching non-stack state and exit registrations. The harness stops there; it does
not claim equivalent eventual stack-overflow address, timing or OS exception.
The C retains recursion instead of returning an invented date or an error code.

## Reproduction and validation

Use the installed Python containing Unicorn 2.1.4 and Capstone 5.0.7:

```powershell
& py -3.13 `
  recovery/date/test_cm_date.py --output CM0102/Analysis/date-NEW-O2 --optimization O2
& py -3.13 `
  recovery/date/test_cm_date.py --output CM0102/Analysis/date-NEW-O0 --optimization O0
```

Outputs must be new directories under local ignored `CM0102/Analysis/`. The runner
checks both baseline/copy hashes before and after, records source/tool/artifact
hashes, commands, versions and outcome in `report.json`, and exports original
instruction bytes/disassembly plus layout evidence. GCC 15.2.0 uses `-m32`, C11,
`-Wall -Wextra -Werror`, freestanding/no-builtin mode and no strict aliasing.
Binutils 2.45 links an import-free test PE at `0x01000000`; Unicorn calls its
symbols directly. It is not a Windows game executable or an M4 integration.

The final O2/O0 runs are `date-slice-d` / `date-slice-e`. Each compares **78,110
cases**, including:

- all 65,536 year values at a year-end boundary;
- 6,864 combinations of signed day/argument extremes, century boundaries, year
  wrap and nonzero upper argument bits;
- a 400-year cycle in both directions, with cold initialization;
- 2,000 fixed-seed full-width randomized cases;
- whole-object assignment/output aliases and 480 shared-result alias cases,
  with guard bytes 0, 1, 0xA4 and 0xA5;
- 2,400 persistent calls: 1,200 daily steps forward and back across multiple
  years, returning to the starting value, with one registration per scratch;
- 24 bounded observations of the mutually recursive exceptional path.

For returning cases the harness compares EAX, all 256 bytes surrounding shared
state, all 256 bytes surrounding caller objects, and ordered exit registrations.
It checks ESP cleanup and EBX/ESI/EDI/EBP preservation for both implementations.
Memory writes outside these regions, the private stack and the modeled FS:[0]
chain head fail the run. Stack
temporaries, flags and caller-saved scratch registers are not parity outputs.
The earlier `date-slice-a` is retained as an intermediate 75,710-case run;
`date-slice-b` / `date-slice-c` are the first complete-corpus runs. The final
harness rejects Python `-O` / `PYTHONOPTIMIZE`, because those modes disable
assertions. That refusal was exercised and created no output directory.

## Residuals and gates

- CRT exit registration is hooked to record the callback and return success in
  both machines. Actual CRT allocation, failure and shutdown are not validated.
  The callback addresses still refer to original one-byte RET routines.
- Shared globals and the CRT call use fixed original addresses in the C.
  This explicit hybrid dependency is unsuitable for a standalone build.
- Whole-object aliases are covered; arbitrary partially overlapping or invalid
  pointers, concurrent/reentrant calls and exact stack-exhaustion parity are not.
- No live-game input trace is claimed for this corpus; the one-day case follows
  the instruction-backed daily-loop caller. No operating-system runtime or GDI
  equivalence follows from an emulator comparison.
- Constructor diagnostics are tested with modeled splitpath/sprintf/error hooks;
  actual CRT/UI behavior and reentrant calls remain unvalidated. Original
  `0x0054A130` EAX is excluded from the recovered void interface.
- Weekday adjustment is validated for normal returns only: its temporary MSVC
  SEH registration and exception-unwind handler are not reproduced. The harness
  checks restoration of the original FS:[0] chain head on return. Transient
  exception-chain state, exceptions and the path-dependent EAX result are excluded.

This advances bounded M3 recovery. M2 repeat-run determinism and primary runtime
are still open, and the M4 replacement gate remains closed. Next prioritize M2
fixed-interval replay; the next static date extension is
the formatting/localization boundary, with the weekday adjustment and component
difference SEH residuals retained explicitly. See the family extension.

## Session review

Separate standards/spec reviews found no recovered arithmetic or ABI defect.
The standards review identified the optimized-Python assertion bypass; the
runner now refuses it. The spec review required refreshing report/source hashes
after that change; the final runs and ledger use the updated runner. C compilation
with warnings as errors and Python compilation passed, as did the five existing
runtime-formation tests. Final preservation verification passed all 1,558 baseline
files and 46 patch payloads. No repository/remote exists, so nothing was committed
or published. Generated x86 binaries and evidence stay in the ignored analysis
directory; the reviewed C, harness, ledger and notes remain workspace source.

## Weekday extension (2026-09-29)

The two additional routines use the same pinned primary input and VA convention
above. The C follows the instructions, not a host calendar library. For unsigned
16-bit year `Y`, signed 16-bit ordinal `D`, and stored 32-bit leap flag `L`:

```
y = Y - 1600
v = 365*y + trunc(y/400) - trunc(y/100) + trunc(y/4) + D - (L != 0)
r = v - trunc(v/7)*7
weekday = r > 0 ? r-1 : r == 0 ? 6 : r
```

Division truncates toward zero. Negative remainder values survive unchanged;
this is not a normalized Gregorian weekday API. The stored leap flag is tested
for nonzero, not recalculated. All signed intermediates fit in 32 bits over the
entire input layout. `0x0054A3F0` receives ECX, returns the full signed result in
EAX and uses plain RET.

Adjustment `0x0054A2D0` receives ECX, then stack dwords `(target, destination)`,
and uses RET 8. It computes the weekday before copying the input to destination.
It compares the low words of current/target, subtracts their full 32-bit patterns,
and tests the signed low-word difference. Differences below -3 have 7 added;
those above 3 have 7 subtracted, once only. Both branches then call recovered
subtraction and assign its value to destination. A target outside 0..6 is not
clamped. The equal case still copies the entire input, including its leap flag.
Whole-object aliases of input/destination and both shared arithmetic objects are
covered, including cold guard initialization.

On the equal-weekday path, adjustment leaves the full target argument in EAX
(`0x0054A36D`, branch at `0x0054A374`). On changed-date paths, EAX points to an
expired local assignment buffer. The recovered interface is `void`; these
path-dependent EAX values and private stack layouts are not parity outputs.
The harness checks both original return patterns explicitly. Eleven decoded direct callers are exported with post-call
instructions in `adjustment-callers.json`: ten replace EAX/AX before consuming
it or call another function; the YMD constructor at `0x0054A2BC` carries it out
through its epilogue. The family extension distinguishes this side-effect
initializer from the returning constructor at `0x00549F70`.
This is a bounded side-effect contract, not proof of arbitrary binary drop-in
compatibility. No integration gate is advanced by this choice.

Final extension runs are `CM0102/Analysis/date-weekday-d` (O2) and
`date-weekday-e` (O0), using the reproduction command above with fresh names.
Each compares **446,623 cases**, retaining all 78,110 arithmetic cases and adding:

- every 16-bit year and every 16-bit ordinal (the latter in year 1599 with a
  deliberately noncanonical leap flag);
- every day in a complete 400-year cycle (146,097 dates);
- adjustment to all seven weekdays at six boundaries in each of those years;
- all 65,536 target words with nonzero upper bits;
- 8,000 seeded random calculation/adjustment cases and 1,008 alias cases.

The recovered machine rejects execution anywhere in original text except the
explicit CRT exit-registration hook. It therefore exercises recovered add/sub
and assignment dependencies, not hidden original-code fallbacks. Output/state,
callee-saved registers, stack cleanup, write boundaries and returning SEH chain
restoration are checked. Both optimization levels use warnings as errors.
Tool/source/input/artifact hashes and exact build commands are recorded in each
report; the decoded caller map is also hashed as an evidence input.

`date-weekday-a` is an intermediate run started before harness hardening; b/c
precede the review correction and original-EAX checks. Use d/e for final source
hashes and acceptance evidence. The previous d/e reports
remain historical evidence for the four-function arithmetic slice.

The extension's independent standards/spec review found one evidence-description
error: equality returns the target in EAX, rather than a temporary pointer. That
was corrected and both original return patterns are now checked. No side-effect
arithmetic or alias-order defect was found. The ledger records all six routines
and retains the older reports separately. No Git repository exists; no commit,
remote or publication was created.
