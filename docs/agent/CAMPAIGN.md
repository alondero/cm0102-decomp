# Run a bounded recovery campaign

Use when coordinating several workers toward one subsystem or comparing recorded
primary-runtime observations. Parent preflight and closure remain in
[PLAYBOOK.md](PLAYBOOK.md). The source campaign's result is a connected accepted
C slice with an observable contract and a named next executable checkpoint.
Lift acceptance is a prerequisite where needed; it has a separate progress measure.

1. Select exact function IDs for one observable slice. Create the manifest under
   ignored analysis storage, then inspect its live dependency report:

   ```powershell
   py -3.13 -m cmre campaign create getter-slice fn_004671b0 fn_004671c0 --milestone M5 --output CM0102/Analysis/cmre/campaigns/getter-slice.json
   py -3.13 -m cmre campaign status CM0102/Analysis/cmre/campaigns/getter-slice.json
   ```

   These example IDs demonstrate syntax; select the actual campaign explicitly.
   Creation refuses an existing output. Completion: finite scope, known direct
   dependencies, unknown data/import/indirect boundaries and the separate M2 lane
   are visible. Available C does not establish a recovered execution binding.
2. For shell-capable agents, use `cmre next --c --module NAME`, `cmre family`
   and `cmre work list` within the selected manifest. Give each worker exact
   nonoverlapping IDs and the direct [C loop](C.md). Review held sources before
   reassigning them. Use [THROUGHPUT.md](THROUGHPUT.md) for measured cohorts.

   For tool-less workers only, inspect the read-only packet dispatch plan:

   ```powershell
   py -3.13 -m cmre campaign plan CM0102/Analysis/cmre/campaigns/getter-slice.json --workers 2
   ```

   Set the worker limit within user authorization and available runtime capacity;
   selected live leases consume that limit. The plan separates `dispatch` (current
   reusable packets), `prepare` (ABI file present but still unverified), `active`,
   `review`, `blocked` and `stale_tasks`. Resolve interrupted publication and review
   submitted work before translating the same function again. Investigate a live
   lease with changed inputs; never assign around it. Packet preparation checks
   evidence bytes; an analyst still reviews ABI meaning. The plan never claims,
   prepares or accepts work. Record baseline `source_coverage` and global
   `cmre status --json`; unmapped functions are excluded from inventory-byte totals.

   Dispatch tool-less workers through [task packets](TASKS.md). Each worker owns
   one source/hint scope; the parent owns shared layouts and acceptance scheduling.
   Use the task CLI help for packet/claim commands. Completion: each assignment
   has an owner, fixed inputs, allowed files and a falsifiable result.
3. Apply [C.md](C.md) for individual source acceptance. Prioritize dependency
   closure within this slice. Return unknown ABI/layout/indirect-call/SEH/x87
   questions to the parent with the first mismatch. Completion: accepted records
   or preserved bounded discrepancies, with no unreviewed shared changes.
4. When every selected member has a source in `recovery/c/`, test the composition:

   ```powershell
   py -3.13 -m cmre compose-test CM0102/Analysis/cmre/campaigns/getter-slice.json
   ```

   The example getters demonstrate syntax, not an established dependency chain.
   Use the actual connected manifest for this step.
   The tool compiles selected definitions together at O0/O2, runs each entry twice,
   and traps execution into every selected original extent. Inspect outcomes,
   coverage and `observed_other_selected_entries`: separate successful entry tests
   do not prove that a recovered-to-recovered call was exercised. Selected
   sources compile separately and link into one image; the report retains object
   identities and rejects ambiguous bindings.
   A reviewed legacy routine may have an explicitly supported original-derived
   lift companion via `--lift-companion fn_00417c10`. The report identifies that
   execution as a fresh lift, not accepted C or new recovery credit; without the
   explicit option the tool does not substitute legacy source or assembly.
   Embedded jump tables are original data. Composition classifies the entire
   observed read/write interval against discovered instruction bytes: a table-only
   read is data, while a read crossing into an instruction remains code-as-data.
   Retain both cases when changing classification; exempting an access by its
   starting address alone can hide a straddling instruction read.
   Completion: saved source-identified report with observed composed calls, or the
   first reproducible failure. This experimental report never accepts a ledger or
   milestone. Original outside callees/data and modeled imports remain boundaries.
   The CLI saves the full report under `CM0102/Analysis/cmre/compositions/` and
   prints its path; use `--json` only when full output is needed. Compiler-selected
   includes are restricted and fingerprinted as in individual C validation.
   Composition requires zero budget-inconclusive cases. An individually accepted
   C function can retain inconclusives and still fail this stricter gate. Preserve
   original loops and the failed composition report; finite loop-prefix parity
   is separate supplemental evidence. Selecting an accepted leaf can also expose
   an unowned original caller outside the selection. Retain that escape boundary
   until its caller or execution binding has been recovered and reviewed.
5. For an M2/M4 or subsystem-parity claim, run the independent primary-runtime
   lane below. Local C recovery records the outstanding runtime experiment without
   promoting that gate. Completion for a runtime claim: repeated observed
   consumed-input checkpoints with a bounded comparison or an earliest divergence;
   retain the raw observer evidence for review before claiming M2/M4/M5.
6. Run the parent session check. Handoff the manifest path, outstanding function
   IDs, report paths and exact next command. Completion: both required exit codes
   observed; preserve any failed gate explicitly.

## Campaign throughput

Use the same finite manifest for before/after comparisons: selected accepted
C/legacy body bytes, newly exercised recovered-to-recovered calls, and remaining
original boundaries. Also record global accepted source bytes and source hashes.
Drafts and C-file presence earn no acceptance credit. C-tier may retain assembly
or VM adapters; report portability boundaries separately from source accounting.

Retain measured worker, coordinator and reviewer usage plus campaign wall time;
unknown values stay null. Compare useful accepted bytes and connected behavior
per total cost, rather than worker tokens alone or attempted function count.
Close the first connected composition before expanding the selection. If work
stalls, preserve the smallest discrepancy and move an independent ready packet
forward while the parent resolves it. Expand concurrency after a measured cohort
shows the review/publication lane can keep up, respecting the user's worker cap.

## Primary-runtime checkpoint comparison

The comparator consumes observations; it does not inject or acknowledge inputs.
An observer must establish consumption at a semantic game-event boundary. Posting
keyboard/mouse messages alone cannot populate `consumption: "observed"`.

Store each run as JSON with `schema: "cmre-replay-observations/1"`, the pinned
primary `input_sha256`, integer `actual_module_base`, and lowercase SHA-256 values
for `scenario_sha256`, `environment_sha256`, `initial_state_sha256` and
`observer_evidence_sha256`. Hash the actual scenario, environment manifest and
raw observer evidence; record how selected state and RNG bytes were captured.

`checkpoints` contains at least an initial checkpoint before input and a later
checkpoint after consumed input. Every checkpoint has:

| Field | Observed content |
| --- | --- |
| `id` | Unique semantic boundary name, in observed order |
| `consumed_input_count` | Monotonic count; initial zero, final positive |
| `consumption` | `observed`, supported by the raw acknowledgement evidence |
| `consumed_inputs_sha256` | Hash of the cumulative consumed-input sequence |
| `state_sha256` | Hash of the selected state; initial equals `initial_state_sha256` |
| `rng_sha256` | Hash of the selected RNG state |
| `ack_evidence_sha256` | Hash of the raw evidence for this checkpoint |

```powershell
py -3.13 -m cmre replay-compare CM0102/Analysis/cmre/replays/run-a.json CM0102/Analysis/cmre/replays/run-b.json
```

Different scenario/environment/initial-state/base identities are incomparable.
Otherwise the tool reports the first differing checkpoint field or
`matching-observations`. It rejects GDI identity, empty scenarios and posted-only
receipts. Hash equality alone cannot establish observer correctness, complete
state coverage or a roadmap gate. Keep original raw captures alongside receipts;
review their provenance and coverage independently.
