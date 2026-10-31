# Language setup, membership and row lifecycle

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
Every executable address below is a **preferred-image VA**, base `0x00400000`.

`recovery/lang/cm_lang_lifecycle.c` and `.h` recover five application routines
from original instructions. Both final O0/O2 runs pass **131,931 differential
cases each**. Independent identified/decompiled/compiled/validated/integrated
flags, original byte identities, report hashes and tested source hashes live in
`recovery/lang/lifecycle-ledger.json`. Validation covers the stated finite
normal-return contracts; all entries remain `integrated: false`.

| Entry VA | Recovered interface | Instruction bytes | Hashed extent bytes | ABI/result |
| --- | --- | ---: | ---: | --- |
| `0x0066E790` | Database setup and keyboard-layout selection | 462 | 560 | cdecl; loader's full EAX |
| `0x0066E9C0` | Translation table teardown | 72 | 72 | cdecl; full incidental EAX |
| `0x0066EA10` | Row destructor | 52 | 52 | thiscall; full incidental EAX |
| `0x0066EA50` | Available-language membership | 230 | 230 | cdecl; full EAX 0/1 |
| `0x0066F4C0` | Row constructor | 13 | 13 | thiscall; returns ECX pointer in EAX |

These total **829 instruction bytes: 816 Ghidra body bytes plus a manually
identified 13-byte constructor**. The constructor is absent from the baseline
Ghidra function inventory. Loader PUSH at `0x0066EFDC`, bytes `68c0f46600`,
selects its address as the array-construction callback. Its six instructions
terminate at RET `0x0066F4CC`; three following NOPs are excluded. The harness
checks 264 exported instruction records plus these six manual instructions.
Hashed extents total 927 bytes, including setup's dispatch tables and alignment.
Private C helpers do not add original application coverage.

The combined map separates three original direct JMP transfers from calls:
`0x005D7BB4` to teardown and `0x0095C8D6`/`0x0095C8F6` to row destruction.
Ghidra's original call inventory labels these `UNCONDITIONAL_CALL` despite
recording JMP mnemonics and bytes. The exporter checks original bytes and
decoded targets, preserves those records in `tail-transfers.json`, and excludes
them from the 16,010 call records. Original inventories are unchanged; bounded
linear context following a transfer is not a reachability claim.

Together with [DATE-TEXT.md](DATE-TEXT.md), this brings the selected
date/localization application inventory to **40 compiled routines at that stage**, with one
separately accounted CRT search routine. All 28 entries in the selected
contiguous Date family compile. This is a selected inventory, not total game
coverage or a complete localization subsystem. [LANG-LOADER.md](LANG-LOADER.md)
subsequently compiles loader and marker, bringing the current application
selection to 42 at that stage. [LANG-MORPHOLOGY.md](LANG-MORPHOLOGY.md) adds
both table helpers, bringing the current application selection to 44.
The loader harness executes real original array/sort support with
recovered callbacks; the accepted lifecycle fixtures described below remain
unchanged and do not themselves claim that longer chain.

## Setup and membership

Setup receives `(filename, memory_mode)` as two stack dwords. It writes mode to
`0x00B647B0` before scanning filename, then copies through its NUL to
`0x00B6586C`. Original REP MOVSD followed by REP MOVSB grouping is retained.
Copying remains unbounded; the port does not substitute a length check.

It calls USER32 `GetKeyboardLayout(0)` through IAT slot `0x009671D8` with
stdcall argument cleanup. Only the low word matters. The low ten primary bits,
minus four with an unsigned range check, index 26 bytes at `0x0066E9A0`, which
select sixteen target dwords at `0x0066E960`. The source reads these immutable
original tables and expresses the checked target branches in C. Unsupported
values select ID zero. No locale names are inferred from those IDs.

Three branches additionally inspect secondary bits masked by `0xFC00`:
target `0x0066E7F0` selects 1 for `0x0400`, otherwise 0;
`0x0066E812` selects 18 for `0x0800`, otherwise 5;
`0x0066E8BA` selects 17 for `0x0400`, otherwise 9.
Other targets select 2, 3, 4, 6, 7, 8, 10, 11, 12, 13 or 14 as encoded.
Setup stores the selected ID to current language `0x009EB2DC` **before** calling
loader `0x0066EB40` with `(ID, 0)`, unrecovered in this lifecycle harness. The loader may alter state or
return arbitrary bits; setup returns its EAX unchanged.

Membership first checks current language for `0xFFFFFFFF`. That path diagnoses
Langlib.cpp line 422 through the recovered version getter and explicit
splitpath/sprintf/UI dependencies. It scans the original error string, clears
diagnostic flag `0x00B67A34` after UI, copies `Setup not yet called` through NUL
to `0x00B65004`, then returns zero. Otherwise it captures the signed count at
`0x00B66170`. Only a positive count permits a linear full-dword comparison of
the argument against IDs beginning at `0x00B64764`. Upper ID bits matter;
there is no low-word truncation. Match returns full EAX one, no match zero.

## Ownership, construction and destruction

The existing 52-byte `cm_lang_record` layout is reused: key, format and text
pointers at +0/+4/+8, then ten order dwords at +12 through +48. Construction
zeros the three pointer fields in that order and returns self. It leaves all
order values untouched, including prior uninitialized storage.

Destruction loads key and conditionally frees it, then loads format and
conditionally frees it, then loads text and conditionally frees it. The later
loads occur **after** earlier free calls; caching all pointers first would
change observable behavior. No row field is cleared. Free is an explicit
cdecl dependency at `0x00944C94`.

The original format-pointer load overwrites EAX from the key free. If format
is nonnull, its free result becomes EAX. Text is loaded through another
register; if null it leaves that EAX untouched, otherwise text's free result
becomes EAX. Thus a key-only row returns zero, a format-only row returns its
free result, and any nonnull text returns its free result. The C interface
exposes these full 32-bit incidental results for differential comparison.

Teardown captures table `0x00B679F0`, allocation header at table minus four and
count from that header before calling array destruction support `0x009462BB`
with `(table, 52, count, row_destroy)` and stdcall cleanup of sixteen bytes.
It then frees the **captured header**, clears table and sets current language
to `0xFFFFFFFF`; its return is the header free's EAX. With no table it still
resets current language and returns zero. Other lookup configuration remains
as the original leaves it. Callback identity is checked against the recovered
row destructor on the compiled side.

## Reproduction and acceptance

Use fresh output directories beneath ignored `CM0102/Analysis/`:

```powershell
& py -3.13 recovery/lang/test_cm_lang_lifecycle.py --optimization O2 --output CM0102/Analysis/lang-lifecycle-check-O2
& py -3.13 recovery/lang/test_cm_lang_lifecycle.py --optimization O0 --output CM0102/Analysis/lang-lifecycle-check-O0
```

Accepted reports are `lang-lifecycle-final-a-O2/report.json` and
`lang-lifecycle-final-a-O0/report.json` under Analysis. Both contain input,
ten source, static-export, tool and artifact hashes, full compiler/linker
commands and case counts. They build freestanding 32-bit C with GCC 15.2.0,
warnings as errors, and compare original and compiled instructions with
Unicorn 2.1.4 and Capstone 5.0.7. All source and artifact identities were checked
before recording acceptance. Exact sources and report hashes are preserved in
`CM0102/Analysis/lang-lifecycle-source-used/manifest.json`.

The corpus includes every one of 65,536 keyboard low words and every one of
65,536 membership low words, plus high-bit patterns, filename lengths 0..259,
selected filename/shared-global aliases, loader state/return variations,
signed count guards, diagnostics, all pointer-presence combinations and free
result patterns, constructor order preservation including unaligned rows,
later-field mutation after free, empty/nonempty table headers, cached-header
mutation and 500 seeded random setup cases. Each call requires completed
return, full EAX, callee-saved registers, stack cleanup, FS head, watched
caller/shared/heap state, ordered external events and language reads to match.

OS layout, the loader, array destruction, free and diagnostic CRT/UI are explicit
fixture boundaries. The harness resolves the keyboard import only in emulated
memory. The array hook checks callback identity and supplies recorded effects;
it **does not execute the real array iterator**. Row callbacks are differentially
tested independently. Huge header counts are observations at that array
boundary, not assertions that billions of rows were destroyed.

Private frame storage, exception unwind, invalid pointers, concurrency, arbitrary
partial overlaps, filename overflow and huge nonmatching membership searches
are outside the tested contract. No binary on disk is patched. These compiled
executables are comparison artifacts, not runnable standalone localization or
a source-built game. M2 determinism, M4 live replacement and M6 standalone
acceptance remain open.

Standards and Spec reviews found no in-contract defects in these five ports.
Combined final map runs `CM0102/Analysis/date-lang-map-final-a/` and
`date-lang-map-final-b/` produce identical functions, calls, tail transfers,
format evidence and table artifacts. They pin all six current ledgers and
report 28 family/thirteen support entries, 16,010 calls and three tail transfers.
The shared acceptance audit and final session checks are recorded in
[DATE-TEXT.md](DATE-TEXT.md#accepted-results-and-handoff).
The [dependency inspection](LANG-DEPENDENCIES.md) supplies checked instructions
for loader `0x0066EB40`, morphology `0x006701F0`, and table helpers
`0x006820A0`/`0x00682150`. The subsequent [loader recovery](LANG-LOADER.md)
checks I/O, ownership, selection, sorting and failure effects in a separate
chain harness; this lifecycle ledger retains its accepted fixture boundary.
Continue morphology recovery, repeated runtime replay and primary/GDI mapping before
integrating any recovered code.
