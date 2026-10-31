#include "cmre.h"
/* Preferred-image VA 0x005aff00, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u8 CM_STDCALL cm_fn_005b03e0(u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern void CM_THISCALL cm_fn_00802f80(u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_005aff00(u32 self, u32 left)
{
    u8 scratch[0xd0];
    u32 count, pointer, index, offset, player, argument;
    u8 result;
    count = D(0x00ae2364);
    pointer = (u32)cm_fn_00944e46(count * 9u);
    D(self) = pointer;
    if (pointer == 0) return self;
    if (left == 0) return self;
    D(self + 4u) = left;
    cm_fn_0066f4e0((u32)(scratch + 8), 0x009cc2ac);
    count = D(0x00ae2364);
    cm_fn_00802f80(0x00b74450, (u32)(scratch + 8), 0, count, 0xffffffffu);
    count = D(0x00ae2364);
    player = D(0x00ae23bc);
    index = 0;
    if ((s32)count > 0) {
        offset = 0;
        do {
            pointer = D(self);
            cm_fn_00417c10(pointer + offset + 1u, (u32)scratch, 0x00ae2c90);
            argument = D(self + 4u);
            result = cm_fn_005b03e0(argument, player);
            pointer = D(self);
            *(volatile u8 *)(pointer + offset) = result;
            if ((s32)index % 1000 == 0) {
                count = D(0x00ae2364);
                cm_fn_00802f80(0x00b74450, (u32)(scratch + 8), index, count, 0xffffffffu);
            }
            count = D(0x00ae2364);
            ++index;
            offset += 9u;
            player += 110u;
        } while ((s32)index < (s32)count);
    }
    cm_fn_00802f80(0x00b74450, (u32)(scratch + 8), count, count, 0xffffffffu);
    return self;
}
