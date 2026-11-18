# Native function-entry import scopes (bounded tooling proposal)

`cmre/capture_entry_observe.py` enrolls a stopped native function entry in the
existing `ImportObserver` endpoint/lease protocol. It does not change
`capture_record.Recorder`, its CLI, the physical debugger, `HARNESS_MODULES`,
capture loading, acceptance, or replay. It releases zero recovered bytes.

## Implemented interface

Construct `EntryImportScopes(observer, max_scopes=...)` with the **same observer**
used by the single physical debugger and import definitions.

At an identity-checked function entry, call `function_entry` with the ingress
thread generation, normalized complete register snapshot, exact return-word
sample at entry ESP, and independently established endpoint ESP and basis.
The module does not infer function stack cleanup. It refuses overlapping scopes,
other pending invocations on that generation, exhausted budgets, incomplete or
contradictory return samples, and pending debugger TF.

The root is an ordinary nested protocol invocation with an explicit
`function-entry!<raw-process-VA>` name. The metadata report identifies its kind
as `function-scope`; it is **not an API invocation**. Import definitions obtain
their scope ID through `scope_for(tid, generation)`, not a constant. Native
return hits, exceptions, exits and detach still go to the original observer.
Only its real paired endpoint closes a successful scope; emulator completion
does not. Stale generations cannot borrow or finish a replacement scope.

The physical owner drains and acknowledges every breakpoint request from that
one observer. Root and import return leases therefore use the same revision,
LIFO and shared-site rules. This module writes no process bytes, handles no
events, recovers no step, and makes no independent detach claim. Its
`cleanup_complete` reports only the protocol registry, not OS cleanup.

Reports remain `replayable: false`, `effects_complete: false`,
`game_recorder_wired: false`. Raw entry snapshots, paired endpoints, partial
reads and gaps remain in the existing protocol report. The metadata report is
not an independent capture or a serialized replay input.
Its per-scope `imports` index lists protocol invocation IDs in entry order,
checking ancestry, scope and thread generation rather than scope labels alone.

## Authored native evidence

`tests/native_entry_imports.py` is an owned-child client of the unchanged
physical backend. The existing authored i386 import fixture now exposes its
function entry and final C3 address in its manifest. Its build/source/executable
identity checks remain required. The client observes the actual function entry
return word, installs the protocol's function-endpoint lease, then observes
actual resolved HeapAlloc and ReadFile calls inside that scope.

Tests compare the observed heap pointer and bytes with the child's independent
post-detach output and check function/import parentage and endpoint ordering.
Cancellation at entry, step-prepared and step-stop retains an incomplete root.
Injected failures at both step boundaries retain the scope evidence from the
backend's finalization path, restore debugger bytes, clear TF and detach the
owned child. Child creation, fixture marker suppression and child-only
termination remain test policy, not arbitrary-PID capabilities.

Focused command (no game, desktop or supplied data):

```powershell
py -3.13 -m pytest tests/test_capture_entry_observe.py tests/test_native_entry_imports.py tests/test_native_import_results.py tests/test_native_import_observe.py tests/test_debug_observe_backend.py tests/test_capture_import_observe.py tests/test_cmre_import_observe.py tests/test_native_peer_exit.py -q -o addopts=""
```

At the development batch: **157 passed, zero skips**. Fail-first receipts and
native build/context/cleanup artifacts remain in ignored worktree `local/`.
The synthetic boundary RED receipt contains three actual failing assertions
(contradictory word, pending TF, unowned pending invocation); the initial scope
and native-client RED receipts are missing-module collection failures.
The subsequent native scope-index RED receipt contains an actual child run and
two failing missing-index checks (native and synthetic), before implementation.

## Exact remaining boundary

This is **not** the proposed opt-in `Recorder.run` integration. That recorder
only emulates from a frozen entry and does not follow the real invocation. The
backend still rejects arbitrary-PID clients. Safe asynchronous stopping,
retained-stop ownership, entry/external/return address collisions, deterministic
queued-peer cleanup witnesses and live-game endpoint ABI evidence remain
prerequisites; do not enable attachment by changing an ownership string.

Nor can this report faithfully populate `imports.Transcript` unchanged:

- Transcript arguments must exactly match the emulated call. Observed stack,
  heap, handle and output addresses are raw process values, not arena-relocated
  identities.
- Transcript output writes target a selected argument inside the test arena.
  A HeapAlloc buffer is addressed by its **result**, not an output argument.
  Heap allocation annotations require an arena allocation and coherent tracked
  heap identity. Copying an observed pointer or inventing a relocation fails
  that contract.
- Endpoint samples do not establish all memory effects, last-error/TLS,
  complete register/FPU effects, unseen imports, asynchronous completion or the
  original function's full captured input pages.
- Capture schema/loading does not bind these observations to an input case.
  Captured `import_seed` values still select seeded scaffolding; they are not
  native results.

Keep the raw protocol, scope metadata and native evidence local. A later
address/effect-completeness and capture-binding design is a separate reviewed
harness release, not an exporter that silently substitutes guessed outputs.

## Release classification

New recorder-only tooling module, test helper/tests, and an additive authored
fixture manifest/label change; no `HARNESS_MODULES` edits or production execution
path changes. Independent owner review and the parent's session gate are still
required. No full pytest, inventory revalidation, game integration, acceptance,
merge, commit or push is claimed by this isolated worker.
