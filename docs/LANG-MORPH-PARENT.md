# Full morphology application-body recovery

> Preserved full-body acceptance with original probe. [Recovered-probe relink](LANG-MORPH-RELINKED.md) is separately accepted after full O0/O2 validation. Current maintained support: [probe](STACK-PROBE.md) and [string search](CRT-SEARCH.md); dependency adoption needs separate evidence.

Target: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`; hash-identical copy
`CM0102/Analysis/baseline-c/cm0102.exe`. All addresses are preferred-image VA,
image base `0x00400000`.

`cm_lang_morph` now recovers the complete application body at
`0x006701F0..0x0067FDC0`, inclusive: **64,465 bytes and 20,658 instructions**.
The ledger is `recovery/lang/morph-ledger.json`; maintained source is
`recovery/lang/cm_lang_morph.c`, `.h`, `.S` and its body/data includes. Both full
`morph-parent-final-a-O0` and `-O2` reports pass **323,545 comparisons each**.
Fourteen identical tested inputs are frozen in `morph-parent-source-used`.
The two table helpers retain their separate acceptance. The nine full ledgers
now select **45 application routines plus one separate CRT search routine**;
zero recovered routines are integrated into the game.

## Recovered extent and behavior

The 23 hash-locked regions form one contiguous partition. All 49,771 bytes /
17,394 instructions in the Ghidra body are covered; an additional 14,694 bytes /
3,264 instructions were recovered from original bytes and independently decoded.
The evidence pins 364 pointer/selector/table regions and 25 static inspections.
Ghidra body membership is not used as a contiguous extent or completeness claim.

The entry retains the five-argument ABI and P=entryESP-0x2818 frame from
[the frame contract](LANG-MORPHOLOGY.md). It preserves the 0x2808 allocation,
register saves, original word scan before the null guard, partial buffer
initialization, shared-output return pointer and incidental frame bytes.
All dispatches 0/7/10/8/6/2/4/3/9/5 execute maintained native C/assembly.
No residual original application-body jump remains. The separately accepted
table helpers also execute recovered code.

[Language five](LANG-MORPH-FIVE.md) completes the last dispatch and the 14-byte
shared qualifier guard. Unmatched language DWORD values jump directly to the empty
composition path corresponding to `0x00670974`: they do not read incoming
qualifier pointers or test whether a callback has populated the shared output.
This distinction matters for unmapped qualifiers and callback mutations.
Common composition and the post-format current-language-two contraction pass
retain original call order, dynamic language reads and forward grouped copies.
Earlier regional notes retain their historical report scopes and ledger pins.

## Acceptance and reproduction

The corpus retains all 224,181 prior regional inputs and all 5,126 isolated
language-five candidate inputs. Another 94,238 cases cover full property-byte
ranges, independent properties, qualifier aliases, long bounded buffers,
callback mutations and unknown-language behavior. There are 152 counted groups.
The maintained focused O2 run additionally passes 5,136 comparisons.

Comparisons watch full EAX, ESP, callee-saved registers, caller arguments,
caller qualifier arena, frame P+0x10..P+0x2813, writable globals/output, data
tables and ordered CRT boundary events. The relocated probe return-address
DWORD P+0x2814 is excluded. This is finite state/event evidence, not an exact
trace of every application memory access or universal equivalence.
Valid terminated byte strings, bounded buffers, clear DF and normal returns
define the accepted scope. General faults, overflow, concurrency and arbitrary
aliases remain outside it.

From the workspace root, use new, empty output directories:

```powershell
python recovery/lang/test_cm_lang_morph.py --optimization O2 --output CM0102/Analysis/morph-parent-recheck-O2
python recovery/lang/test_cm_lang_morph.py --optimization O0 --output CM0102/Analysis/morph-parent-recheck-O0
python scripts/verify_morph_parent.py --output CM0102/Analysis/morph-parent-recheck-audit
python scripts/map_morphology_family.py --output CM0102/Analysis/morph-parent-recheck-map
```

`morph-parent-acceptance-audit-a/audit.json` checks source/report/tool/artifact
hashes, complete instruction partition, manual membership, tables, generation
and preparation provenance, candidate evidence, prior regional acceptance and
all eight unchanged full ledgers. The initial harness generation had three
invalid non-ASCII byte literals; `morph-parent-prepared-b` preserves that input
and the exact spelling correction before any maintained application execution.
The previous partial ledger revision eight and its thirteen tested inputs are
unchanged and remain independently auditable.

The new combined maps have byte-identical six-artifact output in final-a/b.
They select 28 Date-family and 18 support entries and retain the raw Ghidra
selected-call inventory. A separate `morphology-call-inventory.json` decodes
every direct call in the full parent and marks calls missing from that selected
inventory rather than inventing Ghidra ownership. The selected map checks
16,135 calls and three tail transfers; the full-parent inventory has 60 direct
calls, six absent from the selected Ghidra inventory.

## Dependencies and open gates

The build still calls the **original 47-byte stack probe at `0x00945610`**.
[Its isolated native candidate](STACK-PROBE-DISCOVERY.md) passes 3,020 tests but
is not adopted by this build. Formatting `0x00944C9F`, upper `0x009471CD`, lower
`0x009465FC` and substring search `0x00945A60` are explicit CRT fixtures.
Fixed-address byte strings, tables and writable language/output globals still
come from the original image. Code-pointer table contents are treated as
immutable data; unsupported modifications trap.

No original executable was patched. This acceptance covers the application
body with named dependencies; it is not a standalone localization engine or
game. Windows guard pages/SEH, primary/GDI mapping, M2 repeated runtime replay,
M4 integration and M6 standalone build remain open. Next accept and relink the
probe under its own scope, recover the CRT dependencies, relocate recovered
data, then validate an end-to-end loader/lookup/formatting/morphology chain.
