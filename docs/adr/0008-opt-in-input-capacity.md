# ADR 0008: Opt-in input capacity and explicit import transcripts

Status: accepted by the project owner (2026-11-04). Release validation pending.

## Context

The coverage spike found objects larger than the 0x40000-byte arena, independent
HeapAlloc/HeapSize returns, and missing file/import output bytes. Capturing deeper
object graphs cannot remove these limits. Import stubs remain test scaffolding;
this decision makes no Windows, CRT or game-behaviour claim.

## Proposed decision

Keep the existing default arena, random templates, pointer generation and seeded
import returns. Add `arena_size` to function inputs (including `c_hints`), in
page multiples between 0x40000 and 0x260000 (2,490,368 bytes). Both machines use
the same capacity. Keep the first 256 KiB random template and its random draws;
explicitly fill the extension with zero test bytes. Large `struct_size` inputs
reserve their complete object within the selected capacity. Existing fixtures
can write aligned dwords throughout it. This fill is a chosen test input, not
inferred original initialization.

Add optional `import_transcript` to an individual fixture. A top-level transcript
is also supported when every generated case intentionally shares that boundary
contract. Prefer fixture-local scripts: random cases otherwise retain seeded
returns. A transcript is an ordered list of objects with:

- `name`: exact supported import name;
- `args`: exact unsigned argument dwords, including heap/handle identity;
- `result`: explicit EAX dword;
- optional `writes`: objects containing an argument index `arg` and hex `data`;
- `heap`: `alloc`, `size` or `free`, required on the corresponding Heap import.

The supported transcript subset is explicit: HeapAlloc, HeapSize, HeapFree,
CreateFileA, ReadFile, WriteFile, SetFilePointer, GetFileType, CloseHandle,
FlushFileBuffers, SetEndOfFile, GetLastError, GetStdHandle,
GetLogicalDriveStringsA and GetDriveTypeA (all KERNEL32.dll). The general stdcall
argument-count table grants no transcript support. HeapReAlloc, HeapDestroy,
callback-dependent APIs and every other import are refused at declaration.

All output bytes must lie inside the selected arena. Terminal imports cannot be
scripted. With an active transcript, terminal/unknown imports always reject,
including empty or already-consumed transcripts. With no transcript, historical
terminal/unknown import stops remain unchanged. Unknown calls, extra calls and
argument drift reject the case. A normal
return with unused steps rejects it. Rejection cannot compare equal and pass,
even if both sides reject identically. Host output writes are recorded as ordered
byte writes and participate in final dirty-memory comparison and reset.

Heap annotations check fixture claims: successful allocations must fit and not
overlap another live block; HeapSize must name the same heap and live pointer and
return exactly the declared allocation size; successful free retires that block.
HeapAlloc failure (zero) and HeapFree failure (zero on a known live block) are
explicit. HeapSize is restricted to the exact successful allocation size;
HeapSize failure and initially live blocks are unsupported. There is no inferred allocator, handle generator,
zeroing, filesystem access, clock, drive enumeration or fallback transcript value.

The original malloc and CRT file routines continue to execute original code.
Their HeapAlloc/HeapSize and CreateFileA/ReadFile/etc. import boundaries receive
explicit supplied values and bytes. Existing initial globals can seed evidenced
CRT tables. This does not replace malloc/fopen with invented C behaviour. Real
constructor/file-reader fixtures and appropriate initial CRT state still need
authoring and original-instruction provenance; this branch proves the mechanism,
not that the coverage-spike functions now pass. HeapReAlloc and callback-dependent
APIs need additional explicit contracts before use. Outputs into stack/image
memory and pointer arguments varying with compiler stack layout are unsupported.

## Lift correctness finding

For the pinned 3.9.68 executable SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`, preferred-image
VA 0x0055f1b0 is a callback thunk referenced by fn_0055efe0 and invoked by the
vector destructor at VA 0x009462fd. Its bytes `e9 4b fd 08 00` jump to VA
0x005eef00. The lifter discovers the callback as a literal data reference and
copies it into a raw DataRun. At recovered VA 0x6e5d11d0 those same bytes target
0x6e661f20, eventually causing the observed fetch at 0x6e657420 instead of the
original read fault at VA 0x005e42b9.

Add a conservative refusal when referenced raw data begins with a decoded direct
relative call/jump. This detects an unsafe ambiguity; it does not discover code
by guessing from data. The real function now refuses to lift. Recovering its
auxiliary thunk and reconciling its instruction extent with the inventory remains
separate work. Neither the original mismatch nor the guard is a passing tier.

## Initial x87 state correctness finding

Unicorn 2.1.4 exposes an initial control word of zero while its internal
arithmetic still uses extended precision. Reloading that same word applies
PC24. An original `__ftol` saves, changes and restores the word; a compiled
FISTTP cast does not. Authored FILD/FMUL probes reproduce an integer result of
117724151 without the reload and 117724152 after it, with the visible word
unchanged. This explains the first retained fn_00598800 case-3 discrepancy;
it is not proof that the complete recovered function is correct.

Before saving the pristine CPU context, reapply the existing visible FPCW
through Unicorn's public register setter. This makes its internal precision
agree with that word without selecting a new incoming word or game ABI.
Explicit per-case control words still override it. Regression tests compare
unchanged and same-word reload paths, precision/rounding controls and reset
isolation. No source arithmetic, constants or comparison rules are changed.

## Release and validation

Follow ADR 0007 at the next authorized batch boundary. Changed HARNESS_MODULES
are emu.py, imports.py, difftest.py and lift.py. Guest mappings, import execution,
comparison rejection and discovery change: impact is global/unknown, with no
bounded compatibility proof. A complete inventory revalidation and affected
composition checks are required before this release supports current evidence.
No inventory job or acceptance mutation was created on day 1.

The worktree `cmre revalidate --plan --json` is unavailable: it lacks the supplied
executable and analysis inventory. A retained isolated impact-only experiment
used the main checkout's assets and proposed harness digest with CLI
`revalidate --plan -n 1 --json`. It selected execution for both
sampled tiers (lifted and C) because global execution/harness identity changed. Its existing component hashes
remain main's: this is a bounded scope illustration, not a full candidate plan,
compatibility receipt or current evidence. Do not extrapolate numeric tier cost.

Focused tests cover positive and negative capacity/heap/transcript contracts,
synthetic x86 guest output writes, reset, normalization rejection, fixture scope,
and the relative callback guard. Full asset-dependent pytest, full inventory
plan, differential validation and integrity remain release gates, not claims here.
