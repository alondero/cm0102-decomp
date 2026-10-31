# ADR 0004: Versioned C input policy

Status: accepted (2026-10-21, chosen by the project owner).

## Context

Two input-generation properties of `cmre/difftest.py` made correct plain C
unable to pass the [semantic gate](0002-semantic-c-profile.md):

- **Callee-internal globals were randomized.** Discovery randomizes every
  writable dword the original reads, including reads inside original callees.
  MSVC 6 keeps string literals and C runtime state in writable `.data`, so the
  error path's `sprintf` format (`0x009870e8`), `__FILE__` paths and CRT
  variables were randomized. The formatters then over-read stack bytes whose
  layout the semantic profile deliberately does not compare. That error path is
  reached from about 1,260 queued functions (about 65% of C-queue bytes, on
  2026-10-21). This was the "AE2000" WIP class (memory page `0x00ae2000`).
- **A quarter of cases entered with DF set** (EFLAGS `0x602`). The Win32 ABI
  clears DF at every call, so string instructions ran backwards in the original
  only. This was the "DF string op" WIP class.

Measured on 2026-10-21 (receipts under the session scratchpad, summarized here):
6/6 AE2000 WIP sources failed under the old inputs and passed with own-read
globals at unchanged coverage. 5/5 DF-blocked sources passed with DF cleared.
Applying the change to every record would have failed 4/80 sampled accepted C
records (coverage or budget changes), so the change is versioned instead.

## Decision

`difftest.Options.inputs` selects an input policy (`difftest.INPUT_POLICIES`):

| Policy | Used by | Cases |
| --- | --- | --- |
| `v1` | lifted records; C accepted before this ADR | unchanged |
| `v2` | new C (`recover.C_INPUTS`) | DF cleared from the same EFLAGS draw; only writable globals read by instructions inside the function are randomized |

`recover.c_inputs(va)` keeps an accepted C record on the policy recorded in
its validation (`inputs`, absent meaning `v1`), like `c_profile`. v2 draws the
identical random stream as v1, so the cases differ only in the DF bit and in
which globals discovery selects. Read attribution (`emu.Machine.attribute_reads`)
runs only during discovery cases. Hints cannot select a policy.

The entry rule applies after selecting either a generated case or a saved fixture.
Raw fixture decoding preserves captured flags; v2 clears DF on the selected case,
leaving fixture bytes, other inputs and RNG state unchanged. The 2026-10-23 replay
correction fixes an early-return bypass of this rule. It requires whole-inventory
revalidation and preserves previous reports, including accepted v2 fixture runs
whose digests may change under the corrected policy.

## Consequences

- Unchanged records reproduce their recorded digests (0/70 sampled mismatches
  before the whole-inventory revalidation). The harness identity still changed,
  so the full revalidation in [HARNESS.md](../agent/HARNESS.md) was required.
- Callee-internal state now keeps its image bytes in v2 cases. A function whose
  coverage depends on a callee's global has fewer input variants; it needs
  evidenced `globals` hints rather than a policy switch.
- Held AE2000 and DF sources must be re-tested and reviewed. A pass under v2
  is still `needs-review`, not acceptance.
- `cmre compose-test` still uses v1 cases.
- `tests/test_cmre_input_policy.py` holds the reproductions. It covers the
  backward `stosd` store under v1, its v2 pass, the unchanged random stream,
  read attribution and the recorded-policy rule.
