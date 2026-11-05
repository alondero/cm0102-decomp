# Triage one blocked function

Input: one `fn_XXXXXXXX`. If unassigned, select one from
`py -3.13 -m cmre next --blocked -n 10`. A delegated worker owns only that ID.
Its hint/ledger writes (`accept`, `block`, `unblock`) use the parent's allotted
mutation slot; keep them idle during machine jobs or another writer's slot.
See [PLAYBOOK.md](PLAYBOOK.md#2-route-the-task) for scheduling.

1. `py -3.13 -m cmre show fn_XXXXXXXX`: read its facts and complete block reason.
2. `py -3.13 -m cmre explain fn_XXXXXXXX`: identify original stops.
3. Apply the matching row:

| Reason starts with | Action and completion criterion |
| --- | --- |
| `low-coverage` | For an in-function unmapped access, identify the input base from instructions. Edit that function's hints, remove `auto`, then `cmre unblock fn_XXXXXXXX` and `cmre accept fn_XXXXXXXX`. At most three evidence-based attempts; then `cmre block fn_XXXXXXXX --reason "low-coverage: <what you tried>"`. |
| `mismatch` | `cmre explain fn_XXXXXXXX --case N`, using N from the reason. Re-block with field, case, and diagnosis. A normalization defect routes to [HARNESS.md](HARNESS.md); the lifted source remains generated. |
| `code-as-data` | Leave blocked; record the instruction-read hazard for integration. |
| `lift-blocked` with body-size disagreement | Leave blocked; record the doubtful boundary for inventory review. |
| `lift-blocked` with branch outside region / call into own region | Leave blocked; record addresses of shared code or doubtful boundaries. |
| `harness-error` or `nondeterministic` | Stop this function and return the exact reason to the parent for [HARNESS.md](HARNESS.md). |

Hints shape test inputs; they do not change the comparison. Supported basic schema:

```json
{
  "args": ["ptr", "int", "small", "any"],
  "ecx": "ptr",
  "esi": "small",
  "globals": {"0x009b8560": [0, 1, 2]},
  "cases": 2000,
  "budget": 200000
}
```

`args[i]` is the zero-based stack argument. Register keys shape initial registers;
`ptr` points into the arena, `small` chooses small/edge integers, `int` any 32-bit
value. For other kinds, inspect `Options` and `make_case` in `cmre/difftest.py`
before using them. Inference may replace files with `"auto": true`; removing
that key gives the agent ownership. Edit only this function's hints.

### Declared jump-table domains (parent HARNESS lane)

`lift_jump_tables` declares a contiguous signed index range for an original
indexed near JMP whose table does not start at index zero. It changes discovery
and relocation, not differential comparison. Task workers cannot add or edit this
field. The parent reviews original path guards, each physical slot and the full CFG;
the schema validates pins and bounds, not the truth of the analyst's domain proof.
Remove `auto` before claiming the hint. Example shape (replace every pin from bytes):

```json
{
  "lift_jump_tables": {
    "0x0094f144": {
      "bytes": "ff248d48f29400",
      "index_range": [-4, -1],
      "table_sha256": "<SHA-256 of the four actual slot dwords in address order>",
      "evidence": "<original branch guards and retained instruction/slot evidence>"
    }
  }
}
```

Keys are preferred-image instruction VAs for the pinned executable in AGENTS.md.
The effective first slot is `(displacement + 4 * first_index) mod 2^32`.
Slots must be contiguous, inside the function region and point to code in that
region. Physical slots shared by several dispatches are emitted once. A logical
displacement may overlap code: relocation uses the first actual slot and signed
index offset, without adding dummy entries. Stale bytes/hash, unused declarations,
overlapping slots/code and out-of-region entries block the lift. Unsupported sparse
domains stay unknown. After a harness change, complete HARNESS.md gates before
accepting the function; a complete decode alone establishes no recovery acceptance.

For nested pointers or correlated fields, `fixtures` can replay complete input
cases selected by executing the pinned original. Each fixture specifies `regs`
(every name in `emu.REG_NAMES`), `stack` (exactly `arg_count` unsigned dwords),
`arena_template` (an index into `arena_templates()`), `arena_writes` (hexadecimal
aligned arena offsets to unsigned dwords), `globals` (hexadecimal writable image
addresses to unsigned dwords), and `import_seed`. Arena writes are relative to
`emu.ARENA`; globals use preferred-image VAs. Keep the binary hash and the
original instruction evidence with the exploration artifacts.

Fixtures contain inputs at entry to the function under test. Store observed
results separately in witness receipts; replay no callee-result transcript or
invented callback, and leave original callee code unchanged. The generator mixes
fixture replay with ordinary random cases; both differential sides receive identical inputs.
Keep the normal acceptance budget and coverage threshold. A short original-only
probe is input exploration, not acceptance evidence; run the full `cmre accept`
gate before reporting recovery.

## Input-family loop

Use this loop for a named input gap or several consumers of the same layout.

1. Run the cheapest original-only probe that answers the gap. Retain the exact
   initial case, native stop/data address, registers/call context, options,
   image/code/input hashes and observation. Distinguish sampled instruction hits
   from complete-corpus coverage. Use the actual case index, not a fixture index.
2. Derive field widths, pointer/table bounds and branch conditions from original
   instructions. Select bounded values, indices and strings as explicit test
   inputs, then observe the original on those inputs. Each literal need not appear
   in a historical caller. Label the supplied state as a selected test input.
   Claim a constructor output or startup state only with its original proof.
   Keep unknown helper/OS behavior, scratch initialization and callee outputs unknown.
3. Save reusable components in the family's ignored evidence directory: layout
   and producer/consumer pins, exact initial writes and prerequisites, fixture
   schema/hash, original observations and consuming function IDs. Record its
   stage as `aggregate-screen`, `static-proposal` or `original-observed`.
   Mark `original-observed` only when the exact initial case/options and raw
   witness pins from step 1 are present; an aggregate stop and an unrelated
   case are not that witness.
   Reuse unchanged evidence and
   combine compatible components instead of rediscovering each consumer's graph.
4. Preserve existing shared/C fixture prefixes, ordinary random cases and all
   execution/acceptance policies when proposing a canonical update. Claim edited
   hints by removing `auto`. Private original-only exploration stays within the
   assigned worker budget; it neither resets nor replaces the canonical retry
   limits above or the C workflow's lifetime failure ceiling.
5. Classify the next stop before continuing. A missing initial input remains
   owner work; an unsupported native/import result, calling convention or observer
   capability becomes an evidence-bound hold with a specific retry condition.
   Stop repeated probes of that same capability gap. Once inputs are ready, run
   the unchanged complete corpus and required C/control gates in [C.md](C.md).

Completion: a reusable original-observed input bundle with a precise next gate,
or a concrete blocker with retained partial components. Only the normal reviewed
acceptance workflow establishes recovery.

Done: accepted through `cmre accept`, re-blocked with improved evidence, or
returned as a harness issue. Report
`fn_XXXXXXXX accepted|blocked|harness-issue: <reason>`. The parent owns the
session green gate.
