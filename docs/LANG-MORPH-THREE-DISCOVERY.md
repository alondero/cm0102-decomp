# Language-three preflight: instructions and dependencies only

Historical preflight, preserved as identified-only evidence. It is superseded
by [accepted native language-three recovery](LANG-MORPH-THREE.md); the status
and next-step wording below describe the earlier four-stage state.

Input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
`CM0102/Analysis/baseline-c/cm0102.exe` has the same hash. All addresses are
preferred-image **VAs**, image base `0x00400000`.

This is static discovery for the next branch of parent `0x006701F0`.
**Language three remains original code: no recovered implementation or regional
acceptance is claimed here.** The accepted partial slice is still the 19,079-byte
zero/seven/ten/eight/six/two/four source. Its residual transfer starts at the
language-three comparison `0x00674BA9`; body is `0x00674BB2`, JNE destination
`0x0067B506`. Eleven neutral residual cases cover selected transfers only.

Read-only `scripts/inspect_morphology_region.py` produced
`CM0102/Analysis/morph-three-static-a` for exclusive end `0x0067B506`. It checks
the primary and analysis-copy hashes, preceding Ghidra export and complete
contiguous Capstone decoding. The region contains **26,973 bytes / 8,731 decoded
records**, SHA-256
`1450179a72bec21cb37ac6d70e1d401c0bd28227c8978a2a8cdca8e663c863a2`.
The report pins tool/script/command identities, instruction text and literal-byte
artifacts. Those bytes do not become compiled coverage merely by being listed.

Local `CM0102/Analysis/inventory-morph-three.py` produced
`morph-three-dependencies-a`, pinning its script, command, tools, preceding static
report and generated artifacts. It identifies **98 EAX-indexed property-table
sites / 258 distinct targets**. Ninety-three bounds are `CMP EAX,4` with five
DWORD slots, and **five bounds are `CMP EAX,9` with ten slots**. The record for
each site includes the actual preceding instructions, original table bytes/hash
and pointer values. Property source, signedness, index transformations and
destinations must be traced per site; these are not generic article rules.
MOV ESI literal sites are candidates, not an accepted count of unique qualifiers.

An initial inventory assertion assumed every bound was four. It rejected the
ten-slot dispatch at `0x006776E2` before output creation. The retained script
instead records each actual bound; this is a corrected analysis assumption,
not an application differential failure. The EAX inventory does not include
the additional ECX-indexed selector below.

| Direct call site | Original target | Dependency |
| --- | --- | --- |
| `0x00674C83` | `0x009465FC` | Lowercase character CRT |
| `0x00674D54` | `0x00682150` | Table helper B |
| `0x00674D62` | `0x009471CD` | Uppercase character CRT |
| `0x00674DB1` | `0x00682150` | Table helper B |
| `0x00675A03` | `0x009471CD` | Uppercase character CRT |
| `0x00678480` | `0x00944C9F` | Literal `s` formatter |
| `0x00678497` | `0x00944C9F` | Empty-literal formatter |

Both table helpers are already separately compiled and differentially accepted.
Language three's helper-B dictionary starts at `0x009EB7AC`, with the previously
measured 660-byte four-pointer-row extent. The current partial harness links
helper source containing B but invokes only A in its recovered paths; language
three must explicitly add B's original-byte/dependency evidence and watched data
before calling it from new source. Preserve helper incidental EAX, first-match,
empty-key guard, selected column and DWORD-before-byte copying.

`CM0102/Analysis/inspect-morph-three-selector.py` produced
`morph-three-selector-a`, checking the extra selector instructions and pinning
their source/artifact identities plus accepted helper-B bytes/source/ledger.
At `0x00678406` the original MOVSX extends BL, then compares EAX with `0x64`
using unsigned JA. Supported indices 0..100 read a **101-byte selector** at
`0x00680CA8`, then a **four-DWORD target table** at `0x00680C98` through ECX.
Selector values 0/1/2/3 appear 3/2/1/95 times. Targets are `0x00678421`,
`0x0067842F`, `0x0067843D`, `0x0067A65D`. The third target tests local word
length and can return zero before the later suffix ladder. Do not assume all
language-three paths return shared output or reach common composition.

The suffix pointer is captured at `0x00675A12`; out-of-line prefix and suffix
blocks are interleaved in the region. At `0x0067A62F` the original writes zero
to frame DWORD `P+0x14`. The final unknown-suffix path at `0x0067B4EB` tests
caller suffix emptiness and this flag before entering the shared caller-erasure
copy. This changes the default-erasure contract from the earlier two/four
bodies. Preserve that flag, continuation order and exact branch destinations.

Start recovery with the already measured `P=entryESP-0x2818` frame, original
qualifier/literal bytes and both property-table sizes. Preserve callbacks,
selected aliases, early returns and uninitialized buffer effects. Extend the
finite differential corpus and freeze sources before updating the partial
ledger. Keep all prior acceptance and eight full-routine ledgers intact. M2
runtime replay/GDI mapping, M4 live integration and M6 standalone remain open.

Independent Standards and Spec reviews checked the static report chain,
instruction/table/selector counts, original helper-B/dictionary pins, suffix
capture, early return and erasure-flag claims. No material findings were found.
Python syntax checks pass for both local discovery scripts. These reviews do
not promote the branch to compiled or differentially validated source.
