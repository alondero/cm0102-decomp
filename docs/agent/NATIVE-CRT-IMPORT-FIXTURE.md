# Authored file/time import fixture contract

`tests/native_crt_import_results.py` emits a minimal freestanding i386 Windows
child and import definition into ignored `local/native-crt-import-results/`.
`tests/test_native_crt_import_results.py` builds it with the existing configured
GCC/binutils tools and uses the existing `native_import_observer.observe_calls`
private fixture profile seam and `cmre.import_observe` protocol. No recorder or
production import model is changed.

## Isolation and cases

Each newly created, hash-bound child receives a unique test-created working
directory under its retained local build directory. The fixture creates empty
`authored-one.bin` and, for success, `authored-two.bin`. Enumeration uses only
`authored-*.bin` or deliberately unmatched `missingx-*.bin`, never a parent,
absolute path, external PID, supplied executable, game asset or desktop/window.
No file contents are read. `SetErrorMode(3)` changes only the child's error mode,
not a host-global setting. Handles are closed by that child.

* `success`: real FindFirstFileA and FindNextFileA return the two authored names;
  order is not prescribed.
* `exhausted`: one matching file, then real FindNextFileA returns false.
* `error`: unmatched FindFirstFileA, then real FindNextFileA with the returned
  INVALID_HANDLE_VALUE. Both failure buffers remain observations, not outputs.
* Every case also calls real FileTimeToLocalFileTime and FileTimeToSystemTime.
  Successful enumeration supplies its observed last-write FILETIME to the local
  conversion. The next conversion receives those actual local bytes. The error
  case supplies all-one FILETIME bytes, and explicitly overwrites the next
  conversion's input with all-one bytes to exercise its documented invalid range.
  The local-conversion result is retained without assuming success or failure
  for that extreme input. A separate child snapshot preserves the local output
  before the deliberate input overwrite.

## Evidence and fail-closed limits

The observer verifies exactly two u32 pushes and CALL through the PE-declared
IAT for each selected caller; it binds the resolved target to an executable
mapped allowed Windows DLL. Endpoint pairing checks the actual continuation
and observed SP against the evidenced stdcall expectation. This is not callee
instruction tracing. Incidental OS callers are not selected.

At entry retain raw stack arguments, return word, IAT slot, fixed input bytes
(15-byte authored pattern or 8-byte FILETIME), and output-storage bytes. At
return retain actual EAX and output-storage bytes: 320-byte WIN32_FIND_DATAA,
8-byte FILETIME or 16-byte SYSTEMTIME. A read is capped at its declared fixed
bound (maximum 320 here); partial reads/errors stay partial and disable byte
classification. No native result pointer is followed or promoted into VM replay.

Classification preserves complete before/after hex, actual scalar, changed byte
offsets and unchanged count. Equality does **not** establish definedness. On API
failure no output bytes are labelled defined, whether changed or unchanged.
On successful file enumeration only `[0,36)` (attributes/times/size) and
`[44,NUL+1)` within the 260-byte primary filename are labelled defined. Reserved
fields, alternate-name storage, filename tail and trailing structure padding
remain raw storage, not defined API output. Unterminated names retain a gap.
Successful time conversions define their documented 8 or 16 bytes; tests compare
child and observer bytes, never a hardcoded host-timezone conversion.

All records explicitly remain observational/incomplete and `replayable: false`.
No import transcript, all-effects completeness, original CRT frame recovery,
game execution, standalone runtime, or native game parity is established.
GetLastError and external OS effects are not observed; failure reasons are not
inferred from the scalar alone.

## Running and retention

```powershell
py -3.13 -m pytest tests/test_native_crt_import_results.py -q
```

Unsupported hosts or missing build tools are **unavailable** (pytest skips), not
passing native checks. Build failures or debugger failures fail the native test;
they must be reported, not counted as native success. The existing observer
retains raw protocol JSON, cleanup/readback evidence and child output even on
failure. Builds include source/definition/executable/tool hashes and command
outputs in `build.json`. Keep these, run directories and test logs local and
excluded from publication. Parent-session preflight/closing gates and publication
review are separate from this bounded fixture check.
