# Morphology language five: accepted application body

Target: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`. The analysis copy is hash-identical;
addresses are preferred-image VA, image base `0x00400000`.

Language five is now included in [full parent acceptance](LANG-MORPH-PARENT.md).
It adds `0x0067C2E5..0x0067FCF3`: 14,863 bytes / 4,767 instructions, plus the
shared guard `0x0067FD31..0x0067FD3E`, 14 bytes. The first two instructions are
dispatch CMP/JNE; the generated native body covers the other 4,765.
`recovery/lang/cm_lang_morph_five_body.inc` is byte-identical to the reviewed
isolated candidate body: 455 native C blocks, consolidated from 1,623 blocks
with 1,168 joins. This is native control flow, not an instruction interpreter.

Prefix case conversion retains signed character tests and ordered qualifier
comparisons. Suffix property2 is captured in DL once at `0x0067C942`, after the
initial `s` comparison. Six-pointer replacements use that captured signed low
byte, unsigned bounded to 0..5; unsupported values use the original space.
The first suffix pass copies through EBX into the local buffer, then falls
through to the second pass. First-pass miss clears P+0x14; the final shared
guard erases a nonempty caller suffix only when that flag is zero.
Duplicate comparisons and malformed original `no sel le` text are retained.

The del/de la prefix-property selector tests signed property1 with unsigned bounds 0..100.
Values 0 and 5 share a replacement, 1..4 select individual pointers, 6..99 select
the space, and 100 measures and group-copies an empty literal. Language nine's
100 action directly writes zero, so those paths are kept distinct.

The full O0/O2 pair each passes 323,545 comparisons, including all 5,126
isolated candidate cases and wider property, alias and mutation groups.
The historical [discovery note](LANG-MORPH-FIVE-DISCOVERY.md) describes the
candidate evidence and its frozen seventeen inputs. Its original unaccepted
report status remains historical; the full parent ledger supplies adoption.
The parent still depends on original probe/data and explicit CRT fixtures;
no live executable integration or standalone-build claim is made.
