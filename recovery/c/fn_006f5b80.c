/* fn_006f5b80 (VA 0x006f5b80, 171 bytes). Thiscall with one stack byte (ret 4).
   ECX is the object for the whole routine (no write before the calls).
   The local saved-ECX slot at [esp] (push ecx) is modelled as `slot`; its
   low byte receives the adjusted level byte before each path. Both callees
   are forwarded with their EAX left in place (the routine ends with pop ecx;
   ret 4, which does not touch EAX). Entry EAX/EDX upper bits leak into some
   pushed dwords; C has no access to them, so those values are the zero-extended
   low byte (see the notes at each call). */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006f09f0(u32 self, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern u32 CM_THISCALL cm_fn_006f46b0(u32 self, u32 arg2, u32 arg3);

u32 CM_THISCALL cm_fn_006f5b80(u32 self, u8 param_2)
{
    u32 slot;
    s8 level;
    u8 flag;
    u8 b102;
    u32 ptr;
    u8 b;

    slot = self;
    flag = *(volatile u8 *)(self + 0x27);
    level = *(volatile s8 *)(self + 0x103);
    *(u8 *)&slot = (u8)level;

    if (flag == 1) {
        if (level < 11) {
            level = (s8)(level + 1);
            *(u8 *)&slot = (u8)level;
        }
        if (param_2 != 0) {
            ptr = *(volatile u32 *)(self + 0x19e);
            b = *(volatile u8 *)(ptr + 0x8eb0);
            b102 = *(volatile u8 *)(self + 0x102);
            b = (u8)(b + 1);
            /* push edx (arg3 keeps entry EDX upper bits: approximated) ; push eax (arg2 exact) */
            return cm_fn_006f46b0(self, (ptr & 0xffffff00u) | b102, (u32)b);
        }
        b102 = *(volatile u8 *)(self + 0x102);
        /* push eax (arg2 keeps entry EAX upper bits: approximated) */
        return cm_fn_006f09f0(self, (u32)b102, slot, 0xffffffffu, 0, 0);
    }

    if (level > 0) {
        level = (s8)(level - 1);
        *(u8 *)&slot = (u8)level;
    }
    if (param_2 != 0) {
        ptr = *(volatile u32 *)(self + 0x19e);
        b = *(volatile u8 *)(ptr + 0x8eaf);
        b102 = *(volatile u8 *)(self + 0x102);
        b = (u8)(b - 1);
        /* arg2 = edx = (ptr & ~0xff) | b102 exact; arg3 = eax keeps entry EAX upper bits: approximated */
        return cm_fn_006f46b0(self, (ptr & 0xffffff00u) | b102, (u32)b);
    }
    b102 = *(volatile u8 *)(self + 0x102);
    /* arg2 = edx keeps entry EDX upper bits: approximated */
    return cm_fn_006f09f0(self, (u32)b102, slot, 0xffffffffu, 0, 0);
}
