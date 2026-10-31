# Language loader and morphology: static starting boundaries

Input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The identical analysis copy is `CM0102/Analysis/baseline-c/cm0102.exe`.
All executable addresses below are **preferred-image VAs**, base `0x00400000`.

This records verified static starting evidence. The later
[lifecycle recovery](LANG-LIFECYCLE.md) compiles and differentially validates
setup, teardown, row construction/destruction and membership in its separate
ledger. Static inspection records retain their historical candidate flags;
inspection alone does not upgrade recovery status. The subsequent
[loader recovery](LANG-LOADER.md) compiles loader and byte-order probe in a
seventh ledger and connects table state to original ownership and selection
behavior. Full O0/O2 runs each pass 131,500 comparisons. The subsequent
[morphology table recovery](LANG-MORPHOLOGY.md) compiles both table helpers and
passes 164,070 comparisons per O0/O2 build. The larger morphology routine remains
uncompiled; existing lookup/format tests retain their declared fixtures.
This static note's candidate flags and earlier next-work observations remain
historical; current recovery flags are in the separate ledgers.
No recovered code is integrated.

## Reproducible evidence

Read-only Ghidra 12.1.4 export `CM0102/Analysis/lang-loader-static-b/run.json`
records the accepted baseline project, verified distribution, Java, command,
source and artifact hashes. It succeeded without changing the program. Its
eight requested entries include setup `0x0066E790`, which precedes table teardown.
The initial static-a export also requested adjacent object constructor
`0x00682200`; that entry is excluded from the language selection.

```powershell
& py -3.13 scripts/map_recovery.py --output CM0102/Analysis/lang-loader-static-check --functions 0066e790 0066e9c0 0066ea10 0066ea50 0066eb40 006701f0 006820a0 00682150
& py -3.13 scripts/inspect_language_dependencies.py --export CM0102/Analysis/lang-loader-static-check --output CM0102/Analysis/lang-loader-inspect-check
```

Both output names must be fresh. The inspection verifies every recorded export
artifact hash and checks **18,558 instruction records** for eight language
candidates against primary bytes and single-instruction Capstone decoding.
`CM0102/Analysis/lang-loader-inspect-c/` and `lang-loader-inspect-d/` reproduce
identical function identities over ordered address/length/byte records, decoded
calls, setup dispatch tables and a manual callback. Each report pins its inputs.
Ghidra body counts and instruction identities do not imply contiguous extents.
Generated pseudocode remains explicitly unvalidated.

| Entry VA | Static candidate contract | Ghidra body bytes | Checked instructions |
| --- | --- | ---: | ---: |
| `0x0066E790` | Filename/memory-mode setup and keyboard-layout language selection | 462 | 150 |
| `0x0066E9C0` | Translation table teardown | 72 | 20 |
| `0x0066EA10` | Row destructor, three owned pointers | 52 | 22 |
| `0x0066EA50` | Available-language membership check | 230 | 72 |
| `0x0066EB40` | Select language and load translation table | 2,422 | 756 |
| `0x006701F0` | Language-dependent morphology | 49,771 | 17,394 |
| `0x006820A0` | Table-based morphology replacement A | 171 | 72 |
| `0x00682150` | Table-based morphology replacement B | 171 | 72 |

The 13-byte row constructor at `0x0066F4C0..0x0066F4CC` is absent from the
accepted Ghidra function inventory. Manual decoded bytes are
`8bc133c98908894804894808c3`. It copies ECX to EAX, clears ECX, writes zero only to
row dwords +0/+4/+8 and returns; three following NOPs are excluded. The loader
passes this callback to original array construction support. Its ten order
dwords at +12..+48 are not initialized by that callback. This is a boundary
observation, not an assumption that every later order value is defined.

## Setup selects from the keyboard layout

`0x0066E790` takes database filename and memory-mode dword as cdecl arguments.
It stores mode to `0x00B647B0`, copies the filename including its terminator into
`0x00B6586C`, then calls `GetKeyboardLayout(0)` through IAT slot `0x009671D8` at
`0x0066E7C0`. The primary PE import inventory identifies that slot as USER32's
keyboard-layout API. This is not a read of the current application-language
getter, and no locale name is inferred from the returned bits.

The result is masked to its low word, then low ten bits. An unsigned range check
on primary bits minus four selects 26 bytes at `0x0066E9A0`, indexing sixteen
targets at `0x0066E960`; two entries target the same default branch. The inspection
exports both tables and verifies all target addresses are checked instructions.
Secondary bits `0xFC00` distinguish three branches; unsupported primary values
select language zero. Before each loader call the chosen ID is stored into
current language `0x009EB2DC`; progress argument is zero. Each branch calls
`0x0066EB40`, cleans its two arguments and returns the loader's EAX unchanged.
Filename copying is unbounded, and private alias/overflow behavior must not be
silently replaced with a safer setup policy during faithful recovery.

## Ownership and available-language state

At `0x0066E9C0`, a nonzero table pointer from `0x00B679F0` leads to the array
destruction helper `0x009462BB`, stride 52, count read from table minus four,
callback `0x0066EA10`. The header allocation is then freed through `0x00944C94`,
the table pointer cleared and current language `0x009EB2DC` set to -1. With a
null table it still sets current language to -1. This code does not clear all
other lookup configuration. The later port preserves the full incidental EAX:
zero for a null table, otherwise the captured header's free result.

`0x0066EA10` takes ECX as row pointer and conditionally frees +0, +4 and +8 in
that order. Loads of later fields occur after earlier free calls. It does not
clear those fields or the order array. The later C port retains this ordering
and exposes the original incidental return-register bits; details and tested
scope are in LANG-LIFECYCLE.md.

`0x0066EA50` checks current language against -1. The uninitialized-setup path
diagnoses Langlib.cpp line 422, clears diagnostic flag `0x00B67A34` after UI,
copies `Setup not yet called` into error buffer `0x00B65004`, and returns zero.
Otherwise it captures signed count `0x00B66170` and compares its stack language
argument against dwords beginning at `0x00B64764`. A positive count permits a
linear search; zero/negative counts return zero. Match returns full EAX one.
Original diagnostic sprintf retains earlier stack arguments; its pseudocode
alone does not provide the full actual argument sequence.

## Loader questions with concrete instruction anchors

`0x0066EB40` has a normal-return SEH frame and two stack arguments, selected
language and progress control. The export identifies these dependencies:
translation formatting `0x0066F4E0`, file open/read/close, byte-order marker
helper `0x0090C140`, allocation/free, array construction/destruction, qsort-like
CRT `0x00945333`, recursive selection and progress UI `0x00802F80`. Library
identities and exception/failure contracts require separate verification before
using them as recovered application behavior.

The first file dword is conditionally byte-swapped at
`0x0066ED2D..0x0066ED54`; only values one or two pass
`0x0066ED5E..0x0066ED6A`. Subsequent values populate record count
`0x00B647B4` and total-language count `0x00B6616C`. Initialization sets selected
ordinal `0x00B65868` to -1 at `0x0066EE8A` and available-language count
`0x00B66170` to zero at `0x0066EE94`.

The loop writes each read ID to the current available slot at `0x0066EEF4`.
Version two then excludes IDs zero and one from both matching and incrementing
the available count (`0x0066EEFF..0x0066EF0B`); the next read can overwrite that
same slot. Other IDs compare against the requested language at `0x0066EF0D`.
Match stores the **file ordinal** EDI at `0x0066EF11`; duplicate IDs would select
the last matching ordinal because the loop continues. The available count
increments separately at `0x0066EF17`. Thus available-list indices and file
ordinals can differ for version two.

The supplied database identified in [LANG-LOOKUP.md](LANG-LOOKUP.md) has version
one and ID sequence `(0, 2, 3, 4, 5, 6, 7, 8, 9, 10)`. Under complete successful
reads, these instructions select file ordinal one for requested ID two, through
ordinal nine for ID ten. These are static selection facts, not live loader
validation or inferred locale names. An unsuccessful request with a nonempty
available list sets current language to its first ID and recursively retries at
`0x0066EF6D..0x0066EF80`; loader error and failure parity remain unvalidated.

For memory mode, the loader allocates a header plus 52 bytes per row, constructs
only the three pointer fields, reads source/printf/translated fields and order
dwords, and normalizes source keys before sorting. The qsort-like call is at
`0x0066F487`, using the previously recovered unsigned-byte comparator
`0x00682060`. The read-only pseudocode shows brace deletion, space deletion and
literal backslash-n conversion; verify exact scan widths, ownership/free order,
unchecked I/O results, allocation failures and selected-language skipping when
porting. These transformations cannot be replaced by the query normalizer: the
accepted lookup already preserves their asymmetry.

The loader's source path at `0x009ED840` is Langlib.cpp. Its diagnostics and error
buffer allow failures to be observed independently of game UI. A useful next
slice now is loader parity against small synthetic files and supplied database
captures, building on the accepted setup, membership and row lifecycle ports. Preserve the
original array header, cursor positions and side effects on every tested exit.

## Morphology remains a substantial dependency

`0x006701F0` begins with EAX=0x2808 and calls stack-probe support
`0x00945610` before saving four registers. Ghidra emits a misleading two-parameter
signature and large synthetic `in_stack` offsets. The known wrapper calls pass
five cdecl arguments: word, prefix, suffix and two low-byte properties. Later
[original-prefix observation](LANG-MORPHOLOGY.md#the-larger-routines-stack-probe-and-five-arguments)
establishes probe allocation `0x2808`, saved-register base `P=entryESP-0x2818`,
all five argument positions and buffer initialization. The probe/morphology
remain uncompiled. Use that measured frame when translating local accesses;
do not copy the generated signature into source.

The word is loaded at `0x006701FE`; a strlen-like REPNE SCASB at `0x00670235`
and copy begin before its null test at `0x0067025B`. A later nominal empty-string
return cannot be treated as proof that null input is safe. Output first byte at
`0x00B64804` clears early at `0x0067021A`.

The export finds calls to sprintf, string search, two other CRT candidates and
the two table helpers, alongside many inlined string operations. The helpers
walk four-pointer records from `0x009EB328` and `0x009EB7AC`, compare the source
string and select replacement slot one/two/three using a low-byte property;
unmatched strings or other property values return without replacement. Exact
termination, byte handling, selected aliasing and table identities are now
checked in the [separate helper acceptance](LANG-MORPHOLOGY.md). This does not
make the 49,771-byte morphology routine recovered.

Continue M2 repeated replay and primary/GDI mapping in parallel with static
recovery. These exports add no runtime equivalence, live integration or standalone
build claim and do not relax M4/M6 gates.
