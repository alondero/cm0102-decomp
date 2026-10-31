#include "cmre.h"
/* Preferred-image VA 0x008b4450, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All five direct callers discard EAX: 008b1d2a -> MOVSX EAX,BL at
   008b1d31; 008b4349/008b43e8 -> MOV EAX,[ESP+10] at 434e/43ed;
   008b4384/008b4427 -> 008b49b0, which overwrites EAX at 008b49c1.
   After 008b4626, MOVSX DX,AL preserves EDX's high half. That half
   affects only EAX's high half in LEA; every remaining comparison uses AX.
   The void result therefore permits retaining only the observed low word. */
extern u16 CM_CDECL cm_fn_005b7d20(u32);
/* Forward the ECX loaded by 008b4620, including any partial-register
   forwarding inside the original helper; two stack arguments, RET 8. */
extern u8 CM_THISCALL cm_fn_008b4770(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_008b4450(u32 self, u32 entry, u32 mode)
{
    volatile u8 scratch[0x204];
    u32 id, base, player, profile, text, score = 0;
    u16 kind, tags;
    s8 age, measured;
    s16 threshold;
    u8 value;
    D((u32)scratch) = self;
    id = D(entry);
    base = D(0x00ae23bc);
    player = base + id * 110u;
    profile = D(player + 0x61u);
    if (profile == 0) {
        cm_fn_00944cff(0x00a8ccd0, 0, 0,
                       (u32)(scratch + 0x104), (u32)(scratch + 4));
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       (u32)(scratch + 0x104), (u32)(scratch + 4), 0x80a);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        B(entry + 7u) = 0;
        return;
    }
    kind = W(entry + 4u);
    tags = cm_fn_005b7d20(profile);
    if (kind != 1 && tags != 1) {
        if (kind == 0x40) score = 20;
        else if (kind == 0x20) score = (tags & 0x50u) ? 100 : 30;
        else if (kind == 0x10) {
            if (tags & 0x40u) score = 80;
            else score = (tags & 0x38u) ? 130 : 40;
        } else if (kind == 8) {
            if (tags & 0x40u) score = 50;
            else if (tags & 0x20u) score = 70;
            else score = (tags & 0x18u) ? 130 : 50;
        } else if (kind == 4) {
            if (tags & 0x60u) score = 40;
            else if (tags & 0x10u) score = 60;
            else score = (tags & 8u) ? 100 : 120;
        } else if (kind == 2) {
            if (tags & 0x40u) score = 30;
            else if (tags & 0x20u) score = 50;
            else if (tags & 0x10u) score = 60;
            else score = (tags & 8u) ? 120 : 130;
        } else score = 60;
    }
    age = (s8)B(player + 0x18u);
    if (age < 17) score += 50u;
    else if (age < 19) score += 40u;
    else if (age < 22) score += 30u;
    else if (age < 23) score -= 20u;
    else score -= 60u;
    self = D((u32)scratch);
    measured = (s8)cm_fn_008b4770(self, player, kind);
    threshold = (s16)(u16)(score + 2u * (u32)(s32)measured - 20u);
    if (mode != 0) {
        if (threshold > 150) value = 5;
        else if (threshold > 120) value = 7;
        else if (threshold > 90) value = 10;
        else if (threshold > 60) value = 20;
        else if (threshold > 40) value = 30;
        else if (threshold > 20) value = 40;
        else value = 80;
    } else {
        if (threshold > 150) value = 0xb0;
        else if (threshold > 120) value = 0xd8;
        else if (threshold > 90) value = 0xe2;
        else if (threshold > 60) value = 0xec;
        else if (threshold > 40) value = 0xf6;
        else if (threshold > 20) value = 0xf9;
        else value = 0xfb;
    }
    B(entry + 7u) = value;
}
