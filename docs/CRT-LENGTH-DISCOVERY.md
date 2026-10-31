# Bounded-length candidate and deeper locale export

This historical stage is an isolated, compiled and differentially tested candidate, **not an
accepted recovery ledger entry**. It adds no application entry to the inventory,
is not adopted by case conversion or morphology, and is not integrated into the
game. At this stage the accepted inventory remained the 12-ledger map described in
[search composition](LANG-MORPH-SEARCH.md): 45 application and five CRT entries.

The analysis target is `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Addresses below are preferred-image VAs, image base `0x00400000`. Its preserved
analysis copy has the same hash. Neither binary was patched.

## Original behavior

The contiguous body `0x009519D3..0x009519FD` contains 43 bytes and 20
instructions. It accepts a byte pointer and an unsigned DWORD limit as two cdecl
arguments. It preserves ESI and returns the bounded byte length for the ordinary
mapped strings in the corpus. Its memory contract has an unusual extra access:

- A zero limit still reads one byte at the input pointer.
- If no NUL occurs before the limit, it reads the byte at `pointer + limit`.
- A NUL reached by the loop is read again by the final comparison.

The final comparison is at `0x009519F0`. It affects flags and can fault even
when the returned length would already be determined. Replacing this routine
with a host bounded-length function would discard that behavior. The candidate
retains the original byte reads, argument reloads, register changes, flags and
unsigned loop arithmetic; it has no calls, globals or data dependencies.

## Recovery and evidence

The fresh, read-only Ghidra 12.1.4 export is
`CM0102/Analysis/crt-locale-deep-static-a`. Its `run.json` pins the binary, tools,
scripts and exported files. The instruction listing was checked against actual
original bytes with Capstone 5.0.7, rather than treating pseudocode as source.

`CM0102/Analysis/prepare-crt-length-candidate.py` produced labeled Intel GAS
source and an original instruction manifest in `crt-length-native-candidate-a`.
`prepare-crt-length-driver.py` pins its reuse of the earlier standalone search
observer and generates `test-crt-length-candidate.py`. GCC 15.2.0 and GNU
binutils 2.45 assemble and link the candidate to a separate 32-bit PE. Every
compiled instruction and relocated branch target is checked against the
original semantics: 43 compiled bytes and 20 records. This is native assembly
recovery; no C optimization-level equivalence is claimed.

`crt-length-candidate-test-a/report.json` records **48,176 passing cases**:

| Group | Cases |
| --- | ---: |
| All byte values, selected lengths, limits and alignments | 46,080 |
| Incoming flags, including DF | 24 |
| Zero-limit reads | 4 |
| Unmapped zero-limit pointers | 3 |
| Unmapped reads at or beyond the boundary | 15 |
| Terminated strings at mapped boundaries | 25 |
| Input aliases of stack arguments and nearby cells | 25 |
| Seeded random byte strings and limits | 2,000 |

The reference machine maps only the original helper. The candidate machine maps
only compiled helper instructions, a synthetic stack, synthetic byte arena and
return stop. It maps no original executable code or data and uses no boundary
fixtures. Comparisons cover EAX/ECX/EDX/EBX/EBP/ESI/EDI/ESP/EFLAGS, the complete
64 KiB stack and 8 KiB arena, ordered reads and writes, and first unmapped-read
CPU faults with normalized instruction addresses. An independent bounded-length
oracle checks ordinary nonaliasing mapped inputs. Zero-limit cases explicitly
require the extra read; unmapped boundary cases explicitly require a read fault.

`audit-crt-length-candidate.py` rechecks export and generation provenance,
original bytes, compiled PE instructions, all test artifacts and tool hashes.
`crt-length-candidate-audit-a/audit.json` records PASS and freezes the three
tested inputs under `crt-length-candidate-source-used`. Both audit and test
reports explicitly retain `accepted_into_current_ledger: false`.

Reproduce in fresh output folders; existing evidence folders refuse replacement:

```powershell
python scripts/map_recovery.py --output CM0102/Analysis/crt-locale-deep-static-next --functions 0094ed85 009519d3
python CM0102/Analysis/test-crt-length-candidate.py --output CM0102/Analysis/crt-length-candidate-test-next
```

The checked-in local preparation/audit scripts deliberately name their original
evidence folders; inspect them and choose new paths before regenerating those
stages. The test command above rebuilds the preserved candidate with fresh
artifacts. The local Analysis tree is ignored and must remain available with
the supplied binary to reproduce this evidence.

## Deeper classification remains unrecovered

The same export also selects `0x0094ED85`, the explicit dependency in
[maintained classification](CRT-CLASSIFY.md). Ghidra selects 117 instructions
covering 318 bytes across a 329-byte address envelope ending at `0x0094EECD`;
this is **not one contiguous 318-byte region**. Its pseudocode is unvalidated.
The original instruction listing shows FS exception-chain setup, calls to the
recovered stack probe at `0x00945610`, a helper at `0x0094A640`, and indirect
import calls. Those Windows locale and exception paths have not been compiled
or validated by this candidate. The bounded-length helper instead supports the
mapping side of the locale chain identified in
[CRT dependency discovery](LANG-MORPH-CRT-DISCOVERY.md).

Finite Unicorn CPU comparisons do not establish Windows SEH, live game behavior,
universal pointer-wrap equivalence or a standalone engine. Next, review and
promote the bounded-length helper into maintained CRT support, then recover the
locale mapping/classification closure with explicit imported API boundaries.
The [roadmap](ROADMAP.md) build and runtime acceptance gates remain open.

## Subsequent maintained acceptance

The reviewed candidate evidence still verifies. [Maintained length support](CRT-LENGTH.md)
separately accepts identical assembly after a fresh 48,176-case build. Prior
unaccepted audit/finish records remain intact. The current thirteen-ledger map
adds one CRT entry, with no application count or integration change.
[Deeper locale discovery](CRT-LOCALE-DISCOVERY.md) compiles mapping/classification
and local SEH blocks under explicit API/dispatcher limits; those candidates are
unaccepted.
