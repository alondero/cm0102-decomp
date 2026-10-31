# RNG init and bounded PRNG (official 3.9.68)

Input: `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Addresses are **preferred-image VAs**, image base `0x00400000`.

This is the first recovered application slice. Recovered C lives in
`recovery/rng/`. The original-versus-recovered harness is
`recovery/rng/test_cm_rng.py`. Machine-readable dumps are in
`CM0102/Analysis/rng-slice/` (local, ignored).

## Status

| Routine | VA | Bytes | Status |
| --- | --- | --- | --- |
| Bounded game PRNG | `0x0090CFC0` | 221 | identified, decompiled from instructions, compiled, validated against original bytes in Unicorn |
| Seed / table cursor init | `0x0090D0A0` | 134 | same |
| LCG store (`srand`) | `0x009469FA` | 10 | same; MSVC LCG helper |
| LCG step (`rand`) | `0x00946A04` | 30 | same; MSVC LCG helper |
| `time()` helper | `0x00946EBB` | 220 | identified; CRT/OS residual, stubbed in the harness |
| sprintf-style writer | `0x00944C9F` | 82 | identified; CRT residual, stubbed in the harness |

Not integrated into the original process (M4). The table at `0x00AA2454` is
original data, not recovered code.

The [GDI replay extension](RUNTIME-REPLAY.md) verifies correspondence of these
four complete bodies and table/scale/format constants on the separate GDI binary.
It adds bounded live readings with actual module bases; the two posted-input
sequences diverge and do not establish deterministic consumed-input replay.
External callee bodies, live replacement and whole-game parity remain open.

## ABI

`0x0090D0A0` and `0x0090CFC0` are **cdecl**, one 32-bit stack argument, result in
EAX, caller pops. Confirmed by `add esp, 4` at every direct call site inspected
and by `ret` (not `ret n`) at both function ends.

`0x0090D0A0(seed)`:

- If `seed == 0`, call `0x00946EBB(0)` (`time()`, IAT `GetLocalTime` /
  `GetSystemTime` / `GetTimeZoneInformation` at `0x00967148` / `0x0096714C` /
  `0x00967150`) and use that value.
- Store the seed at `0x00AD6C30` via `0x009469FA`.
- Write `Random Number seed: %d` (format at `0x00AD437C`) into `0x00DE1F64` via
  `0x00944C9F`. `%d` treats the seed as a signed 32-bit integer.
- Draw two MSVC LCG values (`seed = seed * 0x343FD + 0x269EC3`, then
  `(int32_t)seed >> 16 & 0x7FFF`).
- Set cursor `0x00DE1F60 = 0x00AA2454 + 4 * (first % 0xC738)`. `0xC738` is 51000.
- Set table-end pointer `0x00DE2798 = 0x00AD4130` (last of 51000 dwords).
- Set mix `0x00DE1F5C = second % 0xFFFF`.
- If the cursor is outside `[0x00AA2454, 0x00AD4130]`, clamp it to the base.

`0x0090CFC0(bound)`:

- `bound == 0` returns 0.
- Advance the cursor by one dword. If it passes `0x00DE2798`, wrap to
  `0x00AA2454` and replace mix with `rand() & 0xFFFF`.
- For `|bound| <= 65535`: return `(mix + table[cursor]) % bound`, then negate
  the remainder when `bound <= 0`. The add/idiv is 32-bit wrap then x86 `CDQ` /
  `IDIV`.
- For larger `|bound|`: scale `(mix + table[cursor]) % 65535` by the qword
  `1/65535` at `0x00970A70` (`0x3EF0001000100010`), `__ftol` at `0x00945640`,
  recurse with a magic-number bound, and fold. The harness covers this path.

The 51000-entry table is file-backed `.data` (file offset `0x006A2454`), 35316
distinct values, a single zero. It is a shuffled constant pool, not BSS.

## Callers

`0x0090D0A0` has exactly three `E8` sites:

| Site VA | Role |
| --- | --- |
| `0x005CC6B2` | Application wrapper `0x005CC6B0` pushes 0 (time seed) before `0x005CCCD0` |
| `0x0081B99E` | `Setup.cpp` (`0x00A801DC`): parsed `-seed` integer (`0x00A801D4`, sscanf `%d` at `0x0098806C`) |
| `0x0081B9B2` | Same function, fallback `push 0` when `-seed` is absent or does not parse |

`0x005CCCD0` later calls `0x0081B890` (`Setup.cpp`). That re-runs init, so the
wrapper's first `0x0090D0A0(0)` only covers work before Setup (splash
`0x0090CFC0(20)` choosing `eidos.rgn` vs `savechip.rgn`). A `-seed N` start
still spends a time-based seed on that splash, then re-seeds for the game.

`0x0090CFC0` has 3772 direct `E8` sites in `.text`. It is the process-wide
bounded generator the community thread described; the thread's relocated listing
was not used as specification. The original instructions above are.

## Recovered source and harness

```powershell
py -3.13 recovery\rng\test_cm_rng.py
python scripts\dump_rng_slice.py
```

The test compiles `recovery/rng/cm_rng.c` with MinGW gcc (`-shared -O0 -Wall`)
and compares it with Unicorn emulation of the original bytes mapped from the
analysis copy. Stubs: `0x00946EBB` patched to `mov eax, imm32; ret`;
`0x00944C9F` hooked to write the seed log line. `__ftol` and the LCG helpers run
as original instructions.

Passing corpus: explicit seeds including `0x80000000` / `0xFFFFFFFF`; seed 0 with
a fake timestamp; 32 LCG steps; bounds `{0, ±1, ±3, ±20, 2, 4, 8, 20, 100, 255,
32767, 65535, 65536, 70000, 100000, 1e6}` with repeated draws and full state
compare (`lcg`, `mix`, `cursor`, `table_end`, log text).

## Residuals and uncertainty

- `time()` and sprintf remain original CRT/OS. The recovered init takes a
  `time0` callback and uses `snprintf` for the one format string.
- The table is loaded from the original image. Recovered `cm_rng_random` indexes
  it as `cursor = 0x00AA2454 + 4*i`.
- Owning translation unit of the RNG pair is not proven. The `-seed` parser is
  `cm3\code\Setup.cpp`. Path joining around `0x0090BDE0` is SI platform code.
  No path string is referenced from inside `0x0090CFC0` / `0x0090D0A0`.
- Unicorn uses the mapped original `__ftol` and x87; recovered large-bound math
  uses IEEE-754 double times the exact scale qword. They agreed on the tested
  large bounds; that is not a proof for every 80-bit edge case.
- Community post p162260 describes a patched reloc of `0x0090CFC0`. It was not
  used as the specification.

Reproduce dumps from `scripts/dump_rng_slice.py`. Do not treat Ghidra
pseudocode as this source.
