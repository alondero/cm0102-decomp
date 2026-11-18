# Offline recorder import-observation adapter proposal

`cmre/capture_import_observe.py` is opt-in tooling over `ImportObserver`. It
does not change the capture format, emulator, input policies, comparison,
acceptance or `HARNESS_MODULES`. It does not attach, launch, click, continue,
single-step or detach a process. It is **not wired into the game recorder**.

## Supported observation boundary

An identity-checked recorder owner supplies the pinned image's import-slot map
and text bounds, one serialized stopped debugger event stream, thread-generation
tokens stamped at event ingress, and explicitly bounded function-invocation
scopes. Only one capture scope per thread is supported; scopes cannot be reused.
Nested imports and threads can share a return breakpoint through the existing
protocol. Ending a scope makes outstanding calls incomplete, not successful.

Only `HeapAlloc` and `ReadFile` are admitted. Every entry retains and checks:

- raw register snapshot, return word and exact stack arguments;
- a six-byte `CALL [IAT]` immediately before the continuation in pinned text;
- the live IAT value matching the entry IP.

The selected Win32 stdcall endpoint SP expectation is retained explicitly; a
matching endpoint does not prove the callee's RET path. Other caller encodings,
tail-jump wrappers and incomplete entry reads are rejected, never guessed.
The owner must establish process/module identity; these checks alone do not
authenticate an OS module or make an arbitrary stream trustworthy.

Outputs are read only at a protocol-eligible top pending return endpoint.
Heap observations retain the returned pointer and requested allocation range;
NULL allocation is incomplete. ReadFile retains the count DWORD and declared
buffer range, including raw unwritten tails. Each actual output read is capped
at 64 bytes while its original requested size and partial-read error survive.
Invalid ranges do not trigger reads or discard other successfully read outputs.

Synchronous file-handle provenance must be supplied and retained by the owner,
for example an observed CreateFileA call with no overlapped flag. A NULL
`lpOverlapped` alone is insufficient. Non-NULL overlapped arguments, unproved
handle provenance, short reads, API errors/pending, missing/unreadable counts,
and capped reads remain incomplete. No async completion or GetLastError/TEB
tracking is implemented.

## Backend and cleanup contract

`read(address, size, purpose)` returns an actual `MemoryRead` from the stopped
identity-checked process, preserving partial bytes and errors.
`breakpoint(request)` performs the requested physical arm/remove and returns
readback evidence, or raises `OSError`. Success must mean exact write,
instruction-cache flush and verified readback; it is not merely scheduling a
future operation. The adapter forwards its acknowledgment to `ImportObserver`.
Requests concern return sites only. Entry-site instrumentation remains owned
by the surrounding recorder.

Before changing a context or unpatching for a single-step, the owner calls
`begin_step`. It must complete physical stepping/rearm and observe cleared TF
before `finish_step`. A pending step blocks endpoint processing. On interruption,
`close` calls `recover_step(lease)` for that exact thread generation; the callback
must restore safe control state and return the observed complete cleared-TF
snapshot. Failed recovery leaves cleanup incomplete. Stale generations cannot
receive recovery operations. Return-breakpoint removal is still attempted on
recovery failure; failed removals can be retried without overwriting evidence.

Peer suspension leases, stopped-event ownership, instruction progress, actual
byte restoration and safe continuation are **backend responsibilities**, not
facts established by mocked tests. There is no claim that the existing entry-only
`Recorder` already meets this contract.

## Evidence and tests

After close, `save` creates an exclusive, never-overwritten JSON file confined
to ignored `local/import-observations/`. Incomplete cleanup can and should be
retained. Public reports are detached snapshots. Every report is explicitly
`replayable: false`, `effects_complete: false`, and `recorder_wired: false`.
Raw bytes, addresses, contexts and observations stay local.

Offline tests (no game, native child or desktop):

```powershell
py -3.13 -m pytest tests/test_capture_import_observe.py tests/test_cmre_import_observe.py -q -o addopts=""
```

These test adapter behavior with debugger doubles, not Windows API semantics or
live restoration. The existing native fixtures remain an independent next
validation layer. A live recorder integration needs separately reviewed identity,
authorization, resource budgets, entry/return instrumentation and cleanup;
observation-to-replay conversion needs a separate completeness/address design
and harness release. This proposal releases **zero recovery bytes** by itself.

## Wiring investigation: why the flag is not enabled yet

The bounded follow-up inspected the actual `Recorder.run` and `_hit` path, not
just the observation facade. At base `915194a`:

- `capture_record.py:270-295` disarms a function entry, rewinds EIP, emulates
  while stopped, then returns. It does not follow the real invocation to its
  return. A function scope cannot safely be closed using emulator completion.
- `capture_record.py:329-335` recognizes only its inventory entry breakpoints;
  external system-DLL breakpoint exceptions are swallowed as loader events.
  It has no single-step handler or distinction between initial attach and
  subsequently owned external traps.
- `_disarm` restores from `img.read`, which cannot supply original bytes for
  external DLL targets. `_write` does not validate byte count, instruction-cache
  flush success or readback. Neither is suitable for acknowledging the new
  adapter's external/return breakpoint operations.
- `run` does not register a pending stopped event through exception handling.
  Its deadline may expire while the process runs, before `finally` restores
  bytes and detaches. External breakpoint cleanup requires an owned stop,
  queued-trap IP normalization and peer suspension, not this assumption.
- The existing authored backend implements these mechanisms in
  `tests/native_import_observer.py:188-381,485-609,634-720`. Its unsafe cleanup
  can terminate **only its newly created child**. A game backend cannot inherit
  that fallback for an arbitrary user PID. Nor can production tooling import
  this nested test driver and silently inherit fixture manifest/marker writes.

A constructor/CLI flag with only event forwarding would therefore expose an
unsafe or nonfunctional recorder mode. No such flag or integration is claimed.
No additional API definitions were added merely to expand a mock-only surface.

### Concrete next implementation batches

1. **Extract the real backend, retaining the authored-child client.** Move the
   process-neutral stopped-event, physical breakpoint, generation, step/rearm
   and suspension-lease machinery into a tooling module. The existing native
   fixture driver becomes its first client. Keep fixture launch/marker changes,
   child-only kill fallback and game attachment policies outside the common
   backend. Do not change `ImportObserver` or any harness module.
   First RED cases must exercise its real dispatcher with a fake WinAPI, not
   direct calls to observer methods: queued peer traps, delayed exit events,
   stale generations, short writes/readback failure, interruption before/after
   a step, and cleanup without an owned stop.
2. **Wire one game-recorder invocation, with HeapAlloc only initially.** Add an
   opt-in constructor/CLI option only with the backend available. The actual
   `Recorder.run` event dispatcher creates a scope at a function-entry hit,
   validates the function's observed return word/endpoint contract, observes
   actual resolved import entry/return traps, and ends the scope only on the
   real function endpoint or an explicitly incomplete lifecycle event.
   Retain entry-target, external-target and return-site breakpoint ownership
   separately, including address collisions. Apply event/time/byte budgets.
   Default mode must execute the old path with identical captures and writes.
3. **Prove cleanup and retain failure evidence.** Save non-replayable evidence
   from a `finally` path even when capture/observation fails. Never use the
   child-kill fallback for a game PID. An unprovable running/stopped-state
   transition must not be reported as successful detach; implement and test
   explicit safe stopping/retained-stop ownership before enabling this client.
4. **Add ReadFile, then the four explicitly bounded API definitions.** The
   likely family is FindFirstFileA, FindNextFileA, FileTimeToLocalFileTime and
   FileTimeToSystemTime, but bind the exact requested set to original call/IAT
   evidence before implementation. Retain precise request/output ranges,
   before/after bytes, raw status and field-layout provenance. No allocation,
   directory contents, file times, success values or unwritten bytes may be
   synthesized. In particular, do not extend the current 64-byte cap and claim
   a complete WIN32_FIND_DATAA without an explicitly evidenced size/layout.

The smallest currently proven slice remains the 33 adapter tests plus 65
protocol tests. They establish scoped event-to-local-observation behavior,
**not** a live `Recorder.run` import path. The wiring follow-up stops at this
identified backend boundary rather than relabeling the facade as integration.
No desktop or native-child tests were executed in this follow-up.

## Physical backend extraction: implemented next batch

The extraction above is now implemented in `cmre/debug_observe.py`, a tooling
module outside `HARNESS_MODULES`. Its external interface is one
`run_session(client, ...)` call. It owns stopped-event receipt/continuation,
ingress thread generations, exact physical breakpoint operations, step/rearm,
peer suspension/reply-later leases, deadline checks and physical cleanup.
The established event loop was moved, not replaced by observer-method forwarding.

`tests/native_import_observer.py` now supplies only the authored-child client and
the original synthetic observation definition. The existing
`NativeResultsProfile` in `tests/native_import_results.py` remains a consumer
through the unchanged `observe_calls(..., _profile=...)` fixture interface.
Fixture process creation, marker suppression, stdout protocol, build identity,
raw evidence retention and owned-child termination remain in the fixture client.
The peer-exit regression's clock injection follows the extracted backend;
its Popen injection follows the fixture client.

This first backend interface admits **only owned authored-child clients**.
It rejects an arbitrary PID before process access. It does not expose user-game
attachment, and there is no process launch/kill implementation in the backend.
Unsafe cleanup requests termination through the owned-child client's policy,
then the backend drains its actual pending native events. The ownership label
is a trusted local Python contract, not a malicious-client security boundary.
Do not accept a user-PID client by merely changing that label.

Extraction TDD:

- `red-backend-extraction.log`: two actual failing interface regressions.
- `green-backend-extraction-attempt1.log`: two pass, including a newly created
  native child routed through the extracted engine.
- `native-extraction-regressions-attempt1.log`: **144 pass, zero skips**, including
  the old native HeapAlloc/ReadFile, ordinary/shared/recursive/threaded calls,
  in-flight detach, wrong-rights failure, multiwave peer exit and late-exit
  receipt tests, plus the existing adapter/protocol tests.

These logs remain under ignored `local/import-observation-tdd/`; raw protocol,
context, module and authored-child artifacts remain in the existing ignored
`local/native-*` and `local/debug-observe-extraction/` directories.

**Recorder wiring is still absent.** Report this extraction/native-evidence
batch for independent review before extending the backend to another lifecycle
client. The next vertical slice must create a real function-entry scope and
observe a real HeapAlloc invocation through the opt-in `Recorder.run` path,
with a fixture-owned client first. Simply forwarding `Recorder.run` to a
fixture profile with a hard-coded scope would not prove that integration.
The four enumeration/time definitions remain deferred rather than enlarging
the mock-only facade. No replay, capture-format, emulator or default recorder
policy changes are part of this extraction.

The subsequent [native function-scope tooling slice](ENTRY-IMPORT-SCOPES.md)
observes a real authored-child function entry, its enclosed OS imports and its
function endpoint through the same backend/protocol. It does not modify
`Recorder.run` or admit game-PID attachment; its scope and replay limitations are
explicitly separate from production recorder integration.
