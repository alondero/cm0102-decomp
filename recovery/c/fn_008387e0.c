#include "cmre.h"
/* Preferred-image VA 0x008387e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scan the entire returned string before deciding whether it is empty. */
extern u32 CM_THISCALL cm_fn_005fde40(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32, u32);
extern u32 CM_THISCALL cm_fn_005fe140(u32, u32, u32);
extern void CM_THISCALL cm_fn_007ff8f0(u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_008387e0(void)
{
    u32 i, index, base, person, receiver, pointer, remaining, slot, result = 0;
    u8 byte;
    for (i = 0; i < 16; ++i) {
        index = D(0x00ae2364) - i - 1u;
        base = D(0x00ae23bc);
        person = base + index * 110u;
        if (B(person + 0x5f) != 1) continue;
        receiver = D(0x00b63c98);
        pointer = cm_fn_005fde40(receiver, person);
        remaining = 0xffffffffu;
        do {
            byte = B(pointer);
            ++pointer;
            --remaining;
        } while (remaining && byte);
        if ((~remaining - 1u) == 0 && D(0x00dd712c) == 0) {
            index = (u32)(s32)(s16)W(0x00b77c7a);
            slot = 0x00b74c26 + index * 768u;
            if (!D(slot)) {
                D(slot) = person;
                D(0x00dd6bf0) = 0x00838a80;
                D(0x00dd6bf4) = 0x00838c00;
            } else cm_fn_007ff8f0(0x00b74450, 0x00838a80, 0x00838c00, person);
        } else {
            receiver = D(0x00b63c98);
            if ((u32)cm_fn_005fd210(receiver, person)) result = 1;
            else {
                receiver = D(0x00b63c98);
                cm_fn_005fe140(receiver, 1, person);
            }
        }
    }
    receiver = D(0x00b63c98);
    return D(receiver + 8) ? 1 : result;
}
