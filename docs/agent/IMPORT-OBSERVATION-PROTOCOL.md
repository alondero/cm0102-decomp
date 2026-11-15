# Import endpoint observation protocol (isolated proposal)

Status: tooling proposal, not wired into `capture_record.py`, capture loading,
or replay. `cmre/import_observe.py` operates on a serialized debugger event
stream. Its unit tests use synthetic observations. A separate test-only adapter,
`tests/native_import_observer.py`, now exercises it against a newly created
authored i386 child using actual caller-continuation breakpoints. These native
observations are fixture evidence, not game captures or recovered API semantics.
No acceptance or inventory validation is claimed.

## Adapter contract

1. Construct `ImportObserver(enabled=True, source=...)`. Supply JSON provenance
   containing `kind` and a unique `session`; retain the process ID, image hashes,
   load addresses, debugger version, import-binding evidence and raw trace paths
   there when available. These are caller-supplied labels, not verified identity.
   One observer represents one process lifetime. Register each observed thread
   with `generation = thread_start(tid)`; thread ID reuse after exit returns a
   new positive generation token. Supply it as the required `generation=`
   keyword to **every** `entry`, `return_hit`, `exception`, `unwind` and
   `thread_exit` call. Stamp it when the raw event enters the adapter, before
   buffering or dispatch, and carry it with the event. Resolving an old event's
   generation later from its TID defeats this contract. The native adapter
   stamps each received Windows event before dispatch and retains the token in
   its raw event/deferred-trap records.
2. At an import entry, supply a `Snapshot` of raw unsigned 32-bit registers and
   explicit `MemoryRead` samples. Record the return word as an exact four-byte
   sample at entry ESP. Record stack argument bytes as separate samples, with
   the actual requested ranges, returned bytes and read errors. Supply an
   enclosing `scope_id`, observed import name, and raw process-VA `return_site`.
   Entry IP is the normalized address of the trapped instruction, not IP+1.
3. Supply exact expected endpoint `return_sp` and a retained `return_sp_basis`
   only when established independently of guessed API semantics. An authored
   fixture can reference its verified return instruction. For unknown cleanup,
   omit both: the observation remains pending and cannot pair. This protocol
   does not discover return instructions or validate the supplied basis.
4. Drain `breakpoint_requests()` while the process is stopped. A physical backend
   owns installation, original byte retention, full write-count checks, cache
   flushes, restoration and readback. Acknowledge each exact address/revision
   using `acknowledge_breakpoint(..., success=..., error=..., memory=...)`.
   Include original/installed/restored byte reads and errors in `memory`.
   Failure leaves physical state unknown; success means the adapter acknowledges
   the operation, not that this module independently proved it.
5. On a return-site breakpoint, obtain registers before the caller executes,
   normalize IP to the trapped instruction, read only explicitly selected
   argument/output ranges, then call
   `return_hit(tid, address, snapshot, generation=event_generation)`.
   The result is a unique import invocation ID or `None`. Preserve the report
   in either case; a rejected endpoint retains its raw snapshot and reason.
6. Dispatch non-protocol exceptions to `exception(...)`, including first-chance
   status and raw context/memory. This marks active frames incomplete without
   assuming unwind or swallowing the exception. The adapter retains exception
   flags/parameters and OS continuation decisions in its raw trace. Use
   `unwind(tid, through_invocation, generation=event_generation, reason=..., snapshot=...)` only for an
   independently observed unwind. It terminates that frame and its nested
   suffix. `thread_exit` and `process_exit` terminate their pending invocations.
7. In every cancellation/error path, call `detach(reason)` and drain restoration
   requests. Failed removal remains unknown; `retry_cleanup()` issues a fresh
   revision. Check `report()["cleanup_complete"]` before declaring acknowledged
   breakpoint cleanup complete. This flag covers this registry only, not OS
   detach, thread suspension/trap flags, handles, or other debugger breakpoints.
   A process exit cancels pending writes and marks sites `process-exited`, not
   restored. `process_exit` remains usable during cleanup.

The adapter must serialize operations and drain requests before continuing the
debuggee. A stale revision is an error: it cannot acknowledge a superseded arm
or removal. Multiple imports, recursion and different threads share a return
site's logical breakpoint. Removal is requested only after its last owner ends.
These are return-site leases only. A backend must coordinate collisions with
entry breakpoints or other users of the same physical byte.

## Pairing and evidence

Pairing requires one active candidate with the same thread **generation**, return site and
exact expected ESP, at the top of that thread's invocation stack, with an
acknowledged armed breakpoint and matching normalized IP. Duplicate candidates,
non-LIFO hits, unknown SP and unarmed hits consume nothing. Near misses add
explicit gaps to that thread's pending records. Foreign threads do not consume
or taint another thread's invocations. A return word contradicted by another
observed sample (including a partial/overlapping sample) cannot arm a site.

All thread-scoped event APIs reject a stale or inactive generation **before**
touching any current frame/lease. They retain a `rejected-thread-event` with the
supplied and active generations, event kind, raw snapshot/declared gaps or exit
metadata, and a rejection gap. Entry/return return `None`; exception, unwind and
exit return `False`. Missing generation is an API error, never a fallback to the
current TID; malformed tokens are rejected without mutation. Stale events
neither complete/abort nor add gaps to the replacement generation.
Report schema 2 identifies this required-generation contract; earlier schema-1
reports remain historical evidence, not silently upgraded identity proof.

Invocation IDs are session-unique; records retain thread generation, parent,
scope, entry/return snapshots and event sequence numbers. Register values stay
raw and unsigned. Missing registers remain absent. Memory samples preserve
requested size, exact returned bytes, purpose and error; short reads are never
zero-filled. User-declared gaps, exceptions and instrumentation failures remain
in the record even if matching endpoints are later observed. Reports and
request lists are defensive copies. Superseded requests remain in event history.

| State | Meaning |
| --- | --- |
| `pending` | Entry observed; no safely paired endpoint |
| `paired-endpoints` | Discriminators matched; no declared endpoint gaps |
| `returned-incomplete` | Endpoint matched, but evidence has explicit gaps |
| `incomplete` | Pending invocation ended by unwind, exit or detach |

All states, and the report itself, have `replayable: false`. An endpoint pair
does not prove an actual RET, complete memory effects, handle/heap identity,
last-error/TLS state, x87/SIMD state, async completion, or absence of unobserved
imports. An import name does not determine output ranges or output sizes.
Supply before/after samples only for ranges established by independent evidence.
The protocol neither computes an API result nor exports `import_transcript`.

Persist `report()` as JSON under retained ignored local evidence storage.
Process-memory bytes remain local-only under ADR 0009. Preserve the adapter's
raw native trace alongside it; self-reported `source.kind` does not turn
synthetic/authored data into a live recording. This module has no capture
loader, file-store integration or integrity/authenticity verifier.

## Authored native integration and production limits

The current recorder disarms entry breakpoints and emulates a call from the
frozen process. That call uses seeded import stubs; the recorder never observes
live import execution or a live return. This protocol intentionally leaves it
unchanged.

The native adapter builds the external-breakpoint variant of the separately
authored fixture. It validates the source/definition/executable hashes before
starting a child and accepts no PID. Fixture sources/helpers were copied
unchanged from `.worktrees/debug-fixture-sol-20261115`,
branch `tooling/debug-fixture-sol-20261115`; their provenance is below.
`tests/test_native_import_observe.py` covers:

- Real caller continuation pairing, observed EAX/EDX/output bytes, recursive and
  concurrent shared return sites, and repeated entry hit/step/rearm.
- Unreadable output, nonreturning thread exit, and first/second-chance fault.
- In-flight detach at entry, after setting TF but before continuing, and at the
  resulting single-step stop; the latter two include multithreaded children.
- Injected host failures at both step boundaries; byte readback, IP rewind,
  cleared debugger TF and restored suspension counts; normal child completion
  after detach.

**Identity precondition:** the executable, source, sibling build receipt and
producer are trusted local inputs. Comparing their hashes detects accidental
artifact drift; a self-consistent attacker-replaced executable/receipt can pass.
This is not an authenticity verifier or a security boundary. Generation tokens
likewise enforce consistency of an honestly stamped, serialized process event
stream; they cannot identify an event whose adapter has forged or restamped its
generation. OS lifecycle ordering and process ownership remain adapter
responsibilities. Native thread-ID reuse is not forced by this fixture;
stale-event rejection is covered at the public protocol interface.

At each software breakpoint the adapter restores the byte, rewinds IP, suspends
peers and sets only the owner's TF, then restores flags/rearms before releasing
peers. Already-queued peer traps are retained and deferred without overwriting
the current step. A shared continuation may also be reached via an ordinary
branch that bypasses the CALL (the fixture's depth-zero case). Those unmatched
hits are retained, consume no invocation, and conservatively mark that thread's
pending observations incomplete.

Cleanup holds all known threads suspended, restores debugger-owned bytes and
the interrupted context, continues/drains already-queued owned traps with their
IP rewinds, detaches, then releases only its own suspension counts. Independently
duplicated thread handles remain usable after detach. Failed suspension is
accepted as a dead peer only with a signaled thread handle or observed terminal
exit code; unknown state fails closed. The injected attach thread must exit
before the child's initial command gate is released.

For early detach only, the adapter explicitly changes the fixture-owned future
`done` marker from INT3 to NOP so the child can complete without a debugger.
This is recorded under `fixture_controls`, separate from the original-byte
readbacks of every debugger-inserted breakpoint. Thus these cases prove removal
of debugger-inserted traps and post-detach execution, NOT equality of the entire
process image with the original executable. Production code has no such patch.
Ordinary completion leaves the fixture's done marker unchanged.

Build commands, hashes, objects and raw JSON observations remain under ignored
`local/native-import-observe/<uuid>/`. Every exception retains raw WOW64 context
bytes with context flags, code, address, flags and exception parameters; paired
snapshots retain both normalized IP and `debugger_raw_eip`. Reports include
restored-byte readbacks and suspend/resume counts, including cleanup-drained
events. Preserve failed attempts alongside successful witnesses.

**Queued-trap regression gap:** native tests do not require a deferred-peer trap
or cleanup-drained trap to occur. The two-thread rendezvous does not force
kernel/debugger queue ordering. Naturally occurring successful raw witnesses
were retained. Tests check incidental deferred-peer trap fields, but do not
assert cleanup-drained trap fields. These are incidental witnesses, not
deterministic branch regression coverage. A dedicated fixture
handshake or event-buffer scheduling witness forcing both branches remains
required before production integration. No such deterministic native witness
is claimed in this proposal.

This adapter supports stopped-event cancellation only. If cleanup cannot obtain
an already outstanding stopped event, it records cleanup unavailable and
terminates only its own child; it does not claim safe asynchronous detach.
Arbitrary imports/callbacks, native thread-ID reuse, real unwind, native write
failure, live-game ABI/effect ranges and unsupported exception-during-step paths
remain unproven. The unit state machine tests cover corresponding logical
boundaries but are not native witnesses. A production adapter must establish
them before wiring the recorder. If instrumentation is interrupted or misses
observations, report `breakpoint_lost(...)`; a successful rearm does not remove
the gap. No observer can recover an event the adapter silently omits.

Capture schema/loading/replay changes remain a separate proposed harness
release requiring full regression and inventory gates at a stopped-farm boundary.
These tests establish no additional accepted C bytes or game live integration.

### Unchanged fixture-copy provenance (SHA-256)

| File under `tests/` | Hash |
| --- | --- |
| `fixtures/native_debuggee.c` | `38191cbf5630ac4301d10e43d96b4dfaed592fb955ea2a3e15d4f695348c7019` |
| `fixtures/native_debuggee.def` | `b27c9a4557c445fb01c802d8ad6911220140ef50ff13d2241adbf83e72aa9d6e` |
| `fixtures/native_debuggee.md` | `ab8b8e64d123c4b44dc45771ed74f91ddc154c2f359388c9b2ef6af2ed739529` |
| `native_debug_fixture.py` | `862c4dc7b482aeb14f1d69549b0b9d86ca494649679746aa770b7737161d6f0b` |
| `test_native_debug_fixture.py` | `bd5785ded49ee4406fc00c6de0d78deea99ea5cddf7bbcfeab1d19c078be9e81` |
