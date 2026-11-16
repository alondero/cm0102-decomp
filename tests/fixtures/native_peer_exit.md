# Native peer-exit transition: diagnosis and bounded recovery

Scope: the authored-child test backend only. No production protocol/HARNESS or
game-recorder changes. Real OS import output reads/statuses are unchanged and
remain non-replayable.

## Exact failure and matched control

The original failure was reproduced, not dismissed after successful retries:

```text
SuspendThread -> DWORD(-1), GetLastError=5
GetExitCodeThread -> success, value=259
WaitForSingleObject(thread, 10) -> WAIT_TIMEOUT (258)
```

The two-thread fixture was sufficient; a twelve-wave version makes the same
thread-exit/peer-breakpoint interleaving frequent without outside processes or
desktop activity. The new multi-wave regression failed before the fix in 1.18s
with that exact signature.

An adapter copied verbatim from `80f6412:tests/native_import_observer.py` also
failed with that signature. A matched control used **the same built fixture
executable and scenario** for old and updated adapters: old failed; updated
completed, including two confirmed exit deferrals. Independent child schedules
are not claimed identical. Source copy, build hashes, failures and raw traces
remain ignored local evidence; none were discarded or relabelled passing.

Ranked hypotheses before instrumentation were:

1. Termination/debug-event transition.
2. Wrong-rights, stale or wrong-identity duplicate handle.
3. ctypes signature/error-capture defect.
4. New private fixture-profile seam regression.

At an actual failure, read-only diagnostics established:

- duplicate's real TID and PID matched the registered child thread/generation;
- `NtQueryObject(ObjectBasicInformation)` succeeded and reported access mask
  `0x00121c7b`, including `THREAD_SUSPEND_RESUME` (`0x2`);
- the native last status, captured immediately after SuspendThread and before
  other native calls, was `0xc000004b`, **STATUS_THREAD_IS_TERMINATING**;
- that peer had returned from its observed calls; its actual exit event had not
  yet been dispatched.

The Win32 declarations were checked: HANDLE-width parameters/duplicates, DWORD
SuspendThread/Wait return values, BOOL plus DWORD-pointer exit query. Their
widths match these native APIs; no declaration change was justified as a fix.
The same unmodified baseline failure rules out the profile seam as necessary.

Thus the observed cause is a **real begun-termination transition**, not a
missing-rights handle in the retained failure. The old backend safely aborted,
but lacked a transport path to await that exit while holding another breakpoint.
It was not safe to reinterpret error 5 or 259 as proof of death.

## Safety mechanism

Microsoft [ContinueDebugEvent](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-continuedebugevent)
documents `DBG_REPLY_LATER` (Windows 10 version 1507+): suspend the breaking
thread, reply later, allow other debug events, then resume it to replay the
original breaking event.

The adapter now:

1. Acquires peer suspension leases **before** recording an endpoint, changing TF,
   or restoring the hit instruction. A replay therefore cannot duplicate an
   observer entry/return.
2. For the retained exact failure signature, validates handle identity, current
   generation and granted rights. Native status only selects a **candidate for
   bounded waiting**; it never proves termination or authorizes skipping a peer.
3. Holds the breaking thread with an exact owned suspend-count lease and uses
   `DBG_REPLY_LATER`. All existing code breakpoints remain armed, and the raw
   breaking context is retained. Other queued owned traps are held the same way.
4. Requires the actual `EXIT_THREAD_DEBUG_EVENT` for the expected TID,
   generation and owned duplicate handle within a one-second deadline. Only that
   lifecycle event retires the candidate. It is continued before releasing any
   acquired suspend counts.
5. Releases only its own counts, then validates the replayed trap's generation
   and address before normal observation/step/rearm. Raw hold, exit-confirmation
   and replay evidence is retained separately.

A signaled owned thread handle is also positive completion evidence; an exit
value different from 259 alone is no longer used as a shortcut. The multi-wave
fixture deliberately uses 259 as an actual worker exit value and joins handles,
proving the backend does not assume that number is an impossible exit result.

Unknown native status, missing rights, identity/generation mismatch, unsupported
reply-later, unowned exception, inability to hold a new peer, missing exit proof,
or unresolved replay is **fail-closed**. No step/unpatch occurs across such an
unknown peer. Unsafe cleanup terminates only the owned child and records its PID
and terminal return code; it does not claim a successful detach/restoration.

The candidate native status is diagnostic provenance, not a public Win32
guarantee that every error-5 failure has that cause. Native query availability
and this host's observed signature are deliberately narrow. General debugger
support on other Windows versions is not claimed.

## Regression loop

```powershell
py -3.13 -m pytest tests/test_native_peer_exit.py -o addopts="" -q --color=no --tb=short
py -3.13 -m pytest tests/test_native_peer_exit.py tests/test_cmre_import_observe.py tests/test_native_import_results.py tests/test_native_import_observe.py tests/test_native_debug_fixture.py -o addopts="" -q --color=no --tb=short
```

The live multi-wave case uses twelve newly-created owned children, each with
twelve waves, 24 worker lifetimes and 48 observed calls. Native interleavings
remain scheduling-dependent; exact deferral counts are reported as witnesses,
not required synthetic outcomes. The retained regression went red before the
fix; deterministic near-miss policy checks and a live wrong-rights negative
case prevent “ignore error 5” from passing.

The wrong-rights case creates a real duplicate without suspend/resume rights.
Actual SuspendThread produces error 5 / STATUS_ACCESS_DENIED (`0xc0000022`);
the test checks no exit deferral, no successful detach and owned-child
termination. It does not invent an API return.

A separate transport fault-injection test withholds an **actual native exit
event** from dispatch, keeping its exact bytes for later owned-child kill/drain.
It verifies the one-second proof deadline, no additional single steps, no exit
confirmation, no detach, and owned-child termination. This test caught a
misplaced deadline check in the initial fix (red in 4.79s); the check was moved
to the active event loop and the regression went green. This injected transport
delay is not an API outcome or an import result.

### Receipt-time boundary (independent review blocker)

Checking the deadline only before `WaitForDebugEvent` was insufficient: a real
matching exit could be returned after the deadline and dispatched as proof.
The earlier timeout-only test did not cover this. Independent review reproduced
it; regression-first validation also produced `LATE_EXIT_ACCEPTED` for a real
1.2-second delayed delivery and deterministic exactly-at/after receipt times.
The before-deadline control succeeded (3 red / 1 green before the fix).

The active event loop now registers the received **owned native event as
pending**, then checks receipt time **before any lifecycle/protocol dispatch**.
Receipt at or after the deadline is rejected (`>=`); no exit confirmation,
replay, step or detach is permitted. The raw event, stamped generation, actual
receipt time and deadline are retained. Termination cleanup kills only the owned
child **before continuing that pending native event**, then drains exit events.
The received stop is never forgotten just because its proof is too late.
The adjacent event-budget rejection explicitly marks the owned-but-unnormalized
stop unsafe rather than accidentally treating it as a detach-ready event.

`test_received_native_exit_checked_before_dispatch` uses four live controls:

- actual unmodified matching native exit event delayed by 1.2 real seconds;
- real matching exit with backend-local receipt time just before the deadline;
- the same native proof at exactly the deadline;
- the same native proof after the deadline.

The clock controls replace only the test backend's clock, not shared `time`,
stdlib watchdogs or native event bytes. Pre-receipt clock progress is bounded
and nondecreasing; a real delivery stall still expires instead of freezing the
controller. The before case confirms/replays normally. All other cases require
no confirmation/replay/additional step/detach, actual owned-child termination,
and continuation of the received event only after kill has begun. Exact native
event bytes and PID/TID/generation are compared at receipt. These tests do not
fabricate API results or claim replayability.

Raw contexts, module memory, executable, build receipts, every failure and
successful stress result stay in ignored `local/native-peer-exit/` and
`local/native-import-results/`. The local diagnostic/control runner is retained
as `local/native-import-results/repro_suspend.py`, not a production entry point.

## Primary references (consulted 2026-11-15)

- [SuspendThread](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-suspendthread):
  required rights, DWORD failure value and previous-count ownership.
- [GetExitCodeThread](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodethread):
  confirms that 259 can also be an application exit value; recommends waiting on
  the thread object to establish termination.
- [WaitForDebugEvent](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-waitfordebugevent)
  and [ContinueDebugEvent](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-continuedebugevent):
  debug-event lifecycle and reply-later transport.
- [MS-ERREF NTSTATUS values](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55):
  `0xc000004b` means an attempt to suspend a thread that has begun termination.
