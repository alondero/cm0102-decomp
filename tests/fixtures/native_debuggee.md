# Authored i386 debugger fixture

These files contain no game instructions, assets, captured inputs or recovered
Windows API semantics. They are debugger plumbing and deliberately invented
fixture behaviour. Do not feed their observations to the recovery capture store,
acceptance gate, or import semantics database.

Run from the repository root, using **64-bit Windows Python 3.13**:

```powershell
py -3.13 -m pytest tests/test_native_debug_fixture.py -q --tb=short --color=no
```

The test driver uses `cmre.capture_record`'s existing `_kernel32`, `_DebugEvent`
and `_Wow64Context` interfaces. It does not instantiate `Recorder`, weaken its
pinned-game identity check, or modify the production debugger protocol.
Only a child created by this invocation can be attached; there is no PID option.
The marker driver also requires a matching build receipt for the current authored
source/definition and executable hash, rejecting an external-marker build.

## Build and retention

`build_fixture(directory, external_breakpoints=False)` compiles freestanding
authored C/assembly with the configured `cmre.env.TOOLCHAIN` GCC `-m32`, generates
an i386 kernel32 import library from the authored `.def` using `dlltool` (including
`--as-flags --32`), and links with `ld -mi386pe`. This also works with the installed
x86-64 GCC/binutils, without a 32-bit CRT or SDK libraries. It verifies PE machine
`0x14c`. Required binaries missing/platform unavailable cause an explicit test
skip; an actual compile, attach or execution failure fails the tests.

Each module run retains objects, executable, generated import library, build
commands/output/hashes, and raw JSON observations in a unique ignored
`local/native-debug-fixture/<uuid>/` directory. Failed attempts remain there.
Authored source/definition, driver, tests and this contract are the reviewable
files. No generated binary or raw observation belongs in Git.

## Observation contract v1

1. Child prints one JSON line on stdout containing integer **live VAs**:
   `version: 1`, `entry`, `return`, `done`, `probe`. Pair addresses with that
   build receipt's executable SHA-256; none are original-game addresses.
   Child then blocks on one stdin byte. A driver must attach and finish its
   attach-breakpoint handshake before sending the scenario byte.
2. Scenario bytes: `o` ordinary; `n` recursion; `s` three calls from the same
   caller site; `t` two threads with overlapping recursive calls; `f` unhandled
   access violation; `u` output allocation freed before return; `x` thread exit
   from inside the observed call.
3. Entry and return markers are fixture-owned `INT3`s by default. The debugger
   must handle them and **not rewind EIP**. At both markers the cdecl stack is
   `[caller_return_va, mode, depth, out_pointer, token]`, five little-endian u32s.
   `ESP` is the same for a matching pair; `(thread_id, entry_esp)` distinguishes
   live frames. Record raw registers at the stopped event. At the return marker,
   EAX is `token + depth + 17`, EDX is `0x2468ace0`; readable output is the u32
   `token ^ 0x13579bdf`. In `u`, the read must be explicitly `unreadable`, not zero
   or an empty successful result. Other register values have no expected meaning.
4. Recursive calls share the wrapper and return-marker addresses. `s` also shares
   the **caller continuation** VA. The `t` rendezvous requires both outer entries
   before either thread returns. Thread exit/fault can leave an entry without
   a return; it must not be fabricated.
5. `done` is a fixture-owned `INT3` after normal scenario completion. Continue
   it, restore any debugger-owned bytes, detach, then send any second stdin byte.
   The child checks the `probe` byte is its original `NOP`, executes it without a
   debugger and prints `{"cleanup_ok":true}` before exiting zero. The driver
   tests this even when it injects its own exception at `done`.
6. The cleanup probe is a separate debugger-owned software breakpoint installed
   at the attach stop, read back as `CC`, restored/read back as `90` while frozen
   at `done`, and executed **after detach**. Thus this checks an **unhit**
   breakpoint's restoration and successful detach, not rearming after a hit.

The return marker is **before the callee's RET**, not a production observer's
caller-continuation breakpoint. This marker driver does not test production
breakpoint ownership, single stepping, TF handling, cross-thread suspension,
return-site refcounts, missed-call detection, or arbitrary-application cleanup.
It proves the native fixture mechanics and Windows debugger observations only.
Thread scheduling order is intentionally not fixed.

## Later production-protocol integration

`build_fixture(directory, external_breakpoints=True)` compiles the same entry and
pre-return markers as `NOP`s. The external build's startup manifest and marker
bytes are tested. A production adapter can install/restore its own breakpoints
at `entry`, read the **actual** caller return VA from `[ESP]`, and observe that
continuation using its own protocol. `return` remains only an optional pre-RET
checkpoint; `done` remains a fixture-owned end-of-scenario `INT3`.

Do not treat the external-build contract test as evidence that the production
observer has been integrated: no such integration is done here. In particular,
later work must test hit/step/rearm restoration and detach with calls in flight
against that separately owned protocol.

Startup wait is bounded to 5 seconds, debug event loop to 10 seconds (50 ms waits),
native rendezvous/join waits to 5 seconds, each build tool to 30 seconds, and
post-detach exit to 5 seconds. On failure the driver terminates only its own child
and drains exit debug events when still attached. No desktop control or game
attach is involved.
