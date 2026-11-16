# Real OS import result prototype (authored child only)

Run in an isolated tooling worktree on 64-bit Windows:

```powershell
py -3.13 -m pytest tests/test_native_import_results.py -o addopts="" -q --color=no --tb=short
py -3.13 -m pytest tests/test_cmre_import_observe.py tests/test_native_import_results.py tests/test_native_import_observe.py tests/test_native_debug_fixture.py -o addopts="" -q --color=no --tb=short
```

This advances **native import endpoint recording**, not game capture wiring,
import replay, or API modelling. The newly-created, hash-bound authored i386
fixture makes two selected calls to actual Windows imports. No surrogate
implementation writes their results, and no `import_transcript` is generated.
`ImportObserver` continues to label every record `replayable: false`.

## Primary ABI/output evidence

Microsoft documentation consulted 2026-11-15:

- [HeapAlloc](https://learn.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapalloc):
  three arguments `(HANDLE, DWORD, SIZE_T)`, pointer result; `dwBytes` supplies
  the explicitly observed allocation range. Fixture requests 32 bytes with
  `HEAP_ZERO_MEMORY`, then preserves/prints the **real** pointer and raw contents.
- [ReadFile](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile):
  five arguments; `lpBuffer` receives bytes and `lpNumberOfBytesRead` points to a
  DWORD byte count. Fixture opens a disk file without `FILE_FLAG_OVERLAPPED` and
  passes NULL `lpOverlapped`. The docs state this synchronous form does not
  return until completion. Nonzero EAX is success, not necessarily a full read.
- [CreateFileA](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea):
  synchronous/asynchronous handle behaviour is determined by the
  `FILE_FLAG_OVERLAPPED` creation flag. The hash-bound fixture's setup source uses
  flags=0; synchronous provenance is not inferred merely from an import name or
  a NULL overlapped argument.
- [__stdcall](https://learn.microsoft.com/en-us/cpp/cpp/stdcall?view=msvc-170):
  Win32 API convention, right-to-left arguments and callee stack cleanup.
- [x86 argument passing](https://learn.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=msvc-170):
  32-bit argument slots and scalar return values in EAX.

The import name alone is **not** ABI evidence. At the initial attach stop:

1. Parse the built fixture's PE import directory and verify each manifest IAT
   slot actually names that kernel32 import.
2. Decode the live authored caller bytes. Require exactly three/five **u32**
   PUSH instructions followed by `FF 15 <verified IAT slot>`. Require the manifest
   continuation to be exactly the instruction after that six-byte CALL.
   Unit tests reject a missing argument, a 16-bit PUSH and a wrong import binding.
3. Read the resolved live IAT word. Require an executable `MEM_IMAGE` mapping in
   kernel32/kernelbase/ntdll and retain its mapped module path, allocation base,
   protection and bounded pre-instrumentation code bytes. Forwarding to another
   module is retained, not silently renamed to an authored callee.
4. Break at that **real resolved target**, read `[ESP]` and raw arguments, and
   accept only the explicitly evidenced authored caller continuation. Incidental
   OS callers hitting the same target are stepped/rearmed but not labelled as
   the selected calls.
5. Use the documented/evidenced stdcall endpoint SP expectation (entry ESP + 16
   for HeapAlloc, +24 for ReadFile). Pair only an actual breakpoint at that
   caller continuation with that observed SP. Live tests verify the delta.
   This does **not** claim to instruction-trace the callee's RET path.

There is no authored pre-RET marker at either API endpoint. The first authored
instruction at each continuation stores EAX without replacing it; post-detach
child JSON provides an independent fixture-side check of that value. Raw buffers
are compared both to the child observation and, for successful ReadFile, to
the authored on-disk input bytes.

## Bounded observation and incomplete results

Pre/post snapshots retain raw integer/control registers, debugger raw EIP,
stack arguments, verified return word and resolved IAT bytes. Raw WOW64 contexts
are retained by the shared backend. Explicit output ranges:

| Import | Return observation |
| --- | --- |
| HeapAlloc | Returned EAX pointer, then requested `dwBytes` raw bytes if non-NULL |
| ReadFile | Four bytes at `lpNumberOfBytesRead`, and the declared buffer range |

The ReadFile buffer observation deliberately includes the declared range, not a
guessed byte count when the count cannot be read. Unwritten trailing bytes are
**not** labelled file data. Only the byte count determines how many bytes the
API reports as read.

Each actual output read is capped at **64 bytes**. `MemoryRead` retains original
requested size, available partial bytes, and underlying ReadProcessMemory error;
a cap adds `observation-limit:64` rather than silently reducing the request.
Invalid/wrapping u32 ranges fail closed before a read. Partial/unreadable byte
counts are not filled with zero. Unit partial-read doubles test observation
plumbing only, not substitute API outcomes.

Live scenarios exercise:

- exact 24-byte authored file read plus 32-byte real heap allocation;
- real short read (5 of 24 bytes), explicitly `short-read`/incomplete;
- real 128-byte read with only 64 observed, explicitly limited/incomplete;
- real ReadFile error on an invalid handle, explicitly
  `api-error-or-pending`/incomplete, with actual EAX/count/buffer retained.

Overlapped operation, errors/pending, oversized/unreadable byte counts and short
reads are incomplete. No asynchronous completion tracking or remote
GetLastError/TEB capture is implemented. Other heap metadata, kernel/file state,
and memory effects outside declared ranges remain unobserved even for paired
endpoints. These are **never** claimed replayable or all-effects-complete.

## Isolation, lifecycle and evidence

`tests/native_import_observer.py` has a private trusted-fixture seam for source
binding, IAT/caller validation, output reads and additional entry targets.
The reviewed backend still owns hit/restore/single-step/rearm, raw generations,
peer suspension leases, breakpoint acknowledgments and stopped-event detach.
No production or HARNESS module is changed.
The peer-termination/debug-event transition is handled by a separately tested
bounded reply-later path; see [native_peer_exit.md](native_peer_exit.md).

The adapter has no arbitrary PID option. Only its own just-created fixture child
is attached. Build receipts bind source, import definition, tools and executable
hashes (local trusted-input drift checks, not a malicious-replacement boundary).
The child reads only a unique `authored-input.bin` created under the build's
ignored local run directory. There is no game or desktop access.

Software breakpoints at resolved OS code addresses modify **only this child's
live mapped pages** through its process handle. No system DLL file is opened
for writing, no DLL on disk is patched, and no other process is modified.
Live tests check restored bytes at the OS entries and caller sites, cleared TF,
successful detach and child exit. Injected errors before/after an OS-entry
single step also verify restoration/detach and mark the open call incomplete.

Limits inherited from the backend: 5-second startup, 12-second debug loop with
50-ms waits, 3-second cleanup event drain, 7-second post-detach wait, then bounded
owned-child termination/drain if needed. New profile adds a 1024-event budget.
Each build tool has a 30-second deadline. Unsafe/failed cleanup terminates
**only the owned authored child**, never resumes an unknown stop as if safe.

Executables, import libraries, authored temporary data, raw memory/module code,
contexts, receipts and protocol JSON remain in ignored
`local/native-import-results/<uuid>/`. The `.c`, `.def`, Python tests/helpers and
this authored contract are the only reviewable additions. Do not publish raw
observations or copy them into capture stores.

Remaining gate: integrate with an independently reviewed game recorder only
after its identity, authorization, scoping and resource policies are established.
Replay needs a separate evidence and completeness design; this prototype does
not establish it.
