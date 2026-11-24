# ADR 0010: x87-register argument bridges and FISTP conversions

Status: accepted (2026-11-16). Adam delegated the choice of supply work
("do whatever you think is best for us to open up the supply"); this release
applies at a recovery batch boundary with one full revalidation (ADR 0007).

Addresses are preferred-image VAs (base `0x00400000`) in the primary binary,
SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

## Context

MSVC lowers `pow(x, y)` to `fld x; fld y; call 0x00945ff0` (`_CIpow`, 25
bytes). The helper takes the base in ST1 and the exponent in ST0, spills both
as doubles, loads the exponent's high dword into EAX and calls the shared math
core `0x00946012`, which returns the result in ST0.

[ADR 0006](0006-compiler-inserted-helpers-in-plain-c.md) lets plain C write
`__ftol` as the cast it lowers. That does not work for `_CIpow`: GCC lowers
`pow` to a cdecl call into its own library, which would replace the original
core and its `_matherr`/errno handling. Plain C cannot put a value in ST0 or
ST1 for a call. In the 15 Nov farm run, workers either wrote `#error` or
declared `cm_fn_00945ff0(long double, long double)` as a cdecl call, which
passes the operands on the CPU stack where the helper never reads them. On
2026-11-16, 18 held functions (97 KB) call `_CIpow`.

A prototype for `fn_007164b0` also exposed a second defect. GCC's default
`-m32` target on the pinned toolchain includes SSE3, so `(long long)x` compiles
to `FISTTP`. For an out-of-range value (here `C / 0.0`) the CPU and the
original `__ftol` (`FISTP` under a chopping control word) store the indefinite
integer `0x8000000000000000`; Unicorn stores `0x7fffffffffffffff`, so case 62
compared a low dword of `0xffffffff` against `0`. Every ADR 0006 conversion
that can see an infinity, NaN or overflow is affected. A sweep of the 22
retained parked candidates with such conversions found no other failure caused
by it.

## Decision

1. `recovery/include/cmre_x87.h` provides one reviewed bridge per registered
   x87-register helper. The first is

   ```c
   long double cm_x87_CIpow(long double x, long double y); /* pow via 0x00945ff0 */
   ```

   a four-instruction thunk that loads `x` then `y` from its stack arguments
   at full 80-bit width (`FLD m80`), calls `_cm_fn_00945ff0` (bound to the
   original helper like every other callee) and returns ST0. It computes
   nothing, rounds nothing and leaves the control word alone. Sources that call
   `_CIpow` include the header and name each original call site in a comment.
2. C builds pass `-mno-sse3`, so float-to-integer casts compile to `FISTP`
   under a chopping control word, the same mechanism as the original `__ftol`.
3. ADR 0006 condition 4 applies to bridge operands: a mutation of each
   operand's computation must fail the gate, or evidenced fixtures are added.
   A mutant fails the gate only with a concrete, non-budget mismatch at both
   ordinary builds. Budget-inconclusive cases are allowed up to the number the
   passing baseline already has at that build (2026-11-24). Those cases exhaust
   the budget on both sides and cannot yield a witness.
4. Inline assembly stays forbidden in `recovery/c/` sources. A new bridge needs
   an entry here with the helper's listing contract and a build test.

## Evidence

`CM0102/Analysis/cmre/diagnostics/x87-cipow-bridge-20261116/`: the
`fn_007164b0` bridge source passes the unchanged gate at O0 and O2 (coverage
0.9714, no inconclusive cases) once `FISTTP` is avoided, and fails on three
cases with it; `diag/` holds the intermediate-value trace; `sse3sweep/` holds
the parked-candidate sweep. `tests/test_cmre_x87_bridge.py` pins the emitted
bridge sequence and the absence of `FISTTP` at both optimization levels.

## Limits

- GCC spills its own x87 values before any call, so values deeper than ST1 in
  the original are not on the register stack when the helper runs. `_CIpow`
  and its core consume only ST0 and ST1; an x87 stack overflow at depth seven
  or more in the original is not reproduced. Review records any call site
  where the original stack is deeper than two.
- The helper clobbers EAX, ECX and EDX. A caller that reads their residue fails
  the gate; the source cannot model it.
- The M6 standalone build maps `cm_x87_CIpow` to the recovered `_CIpow` path.

## Consequences

- A new include file and a changed C build flag invalidate all C coverage, so
  this ships as one harness release with a full revalidation.
- Functions held for `_CIpow` transport (`x87-power-call-boundary`,
  `original-live-x87-argument`, `x87-live-stack` and related blockers whose
  hold names `0x00945ff0`) are retried with this ADR as the release reason.
