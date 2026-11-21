# ADR 0013: Input lane — model-authored entry inputs for unlifted functions

Status: accepted (2026-11-21) under
[ADR 0011](0011-engineering-decision-rights.md). Builds on
[ADR 0008](0008-opt-in-input-capacity.md) (fixtures are chosen test inputs) and
[ADR 0009](0009-captured-entry-inputs.md) (captured inputs).

## Context

The farm translates lifted functions to C. On 21 Nov its queue was empty for
most of the day (`queue 0 idle 16`) while 791 functions (1.66 MB) were
unlifted. 412 of them were captured but below the 60% coverage threshold, and
the rest had never been captured. After 14:26 the automatic lift lane reported
`processed 0` on every hourly pass, because it only re-runs a function when its
capture set or expander version changes.

The supply that did arrive came from the coordinator session. It hand-wrote
structured entry inputs, such as packed records behind the ECX receiver, table
pointers in writable globals, and selector and flag values the listing
compares. It gated them with a throwaway script and accepted the passing lifts.
The farm then translated those lifts to C within minutes. About 20 functions
moved this way in 3.5 hours, one at a time, by the most expensive model, while
16 cheaper farm workers were idle. Random mutation of captures had produced
zero lift passes in the same session.

## Decision

The farm runs an **input lane** (`scripts/cmre_input_lane.py`) beside its
translation workers, by default with 4 workers (`--input-lane-workers`; 0
disables it; `set` changes it live).

1. **Candidates:** inventory functions with no ledger record, no blocked record
   and no hard work hold (`harness-blocker`, `evidence-needed`), excluding
   compiler-generated code and lifts that are blocked, mismatching or erroring.
   They are ordered by coverage band, nearest the threshold first, then by size.
   A function that was never gated is gated once per canonical-hint state
   before any model time (the lane keeps the screened coverage). If that gate
   passes, the function is accepted with no model call; if that accept is
   refused, the function is not retried until its hints change.
2. **Worker:** one function per job, using the farm's model ladder (one rung per
   attempt, at most 3 attempts per canonical-hint state). The worker may edit
   only its spec file. Its shell is limited to read-only `cmre` commands, the
   exact `scripts/cmre_input_probe.py try|gate` commands for that spec, and
   `cat`/`head`/`tail`/`wc`/`Get-Content`. The farm's `rg` and `sed -n` are not
   allowed here, because `rg --pre` runs a program and `sed` can write files. A spec describes arena
   records, stack words, registers, writable globals and `vary` axes in compact
   form. The probe expands it into ordinary hint `fixtures` (the ADR 0008
   schema).
3. **The lane decides; the worker never does.** After the worker exits, the
   lane re-runs `cmre_input_probe.py gate` itself. That gate runs the spec's
   fixtures on the original side only and keeps those that reach instructions
   the current cases do not, plus one returning fixture when none returns (at most 48 in all). It
   **appends** them to the current hints in a private proposal and runs the
   unchanged lifted gate on that proposal. A worker's `FARM-RESULT` line is
   only recorded. The gate runs in a small thread pool beside dispatch, so a
   long gate does not stall other workers. A spec that expands to more than
   2,048 fixtures, including a single oversized `vary` range, is refused before
   anything is built.
4. **Apply:** on `pass`, the lane first checks that the hints its gate built
   the proposal on are the hints the job was dispatched with. A missing hint
   file is an explicit `absent` identity, never "no check". Under the writer
   lease it checks four things:
   - The canonical hints still have that identity.
   - The proposal bytes are the ones its gate wrote (SHA-256).
   - The proposal is append-only: every existing field, fixture, capture and
     provenance entry is unchanged, and the only additions are fixtures and
     `fixture_provenance` entries.
   - The proposal passes hint validation.

   It then writes the proposal and runs `cmre accept`, which joins the lease
   and re-gates from scratch. Anything other than an accept, including an
   exception, restores the previous hint bytes exactly. A busy lease keeps the
   pass, with its base identity and proposal hash, for a later tick.
5. A lift `fail` (mismatch) is recorded and that function is not retried until
   its hints change. Mismatches go to triage, and inputs are never reshaped to
   avoid them.

Provenance is recorded in the hints as `fixture_provenance["input-lane-<spec
sha>"]`. It holds the kind `authored-structured-entry-inputs`, the tool
version, the target SHA-256, the spec SHA-256 and the model. Specs, proposals
and worker logs stay local under `RUN/input-lane/`.

## Why this is safe for fidelity

- Fixtures are test inputs. The gate runs the original on each one and compares
  the lift with it as usual. No compared field, threshold or policy key can
  change: the probe refuses specs that name anything other than inputs, and the
  apply step refuses any non-append change.
- Fixtures do not only add cases. As with every ADR 0008 fixture and ADR 0009
  capture, each of the 600 cases draws a fixture or capture with probability
  1/2 (`difftest.make_case`), so about 300 random cases are replaced and the
  random stream changes. A lift that differed from the original only on a
  dropped random case could therefore pass. This is a property of the existing
  input design, not something new to the lane. The lane bounds it by adding at
  most 48 fixtures, chosen because they reach new instructions. The C
  translation of each such lift is gated against the original again, and it is
  audited after acceptance.
- Synthetic inputs may reach states the game never produces. That is the same
  standing as the random cases and ADR 0008 fixtures: the original's own
  behaviour on that input is the oracle. Coverage measures how much of the
  function the comparison exercised, not how realistic the inputs are.
- Import results stay seeded scaffolding (ADR 0008/0009). The lane adds no
  import transcripts.
- Accepted lifts carry the fixtures into the C gate, so C translations are
  compared on the same inputs that opened the lift. The exception is a function
  whose hints already set `c_hints.fixtures`: the C gate then uses those, and
  the new fixtures apply to the lift only.

## Consequences

- Supply no longer depends on an expert session or on new game captures. Idle
  farm capacity becomes lift supply.
- `cmre_farm.py status` shows `inputs: running N, accepted K fn / B`;
  `cmre_input_lane.py status RUN` and `plan RUN` give detail.
- The lane stops with the farm (`RUN/STOP`). It interrupts running workers,
  whose specs are kept, finishes its in-flight gates and applies, and exits.
  The farm waits for it before it reports `finished`.
- Measure the lane in bytes moved out of `unlifted` (`cmre pipeline survey`).
  If a model rung's attempts rarely pass, change the ladder with `set`, not the
  gate.
