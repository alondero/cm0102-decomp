/* Preferred-image VA 0x008AA9D0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with three stack arguments, ret 0xc.
   Argument 1 is a byte checked as signed 0..19. Argument 3 selects between two
   record lookups. Returns 1 on success, 0 on failure. Field and value meanings
   remain unknown; record bases are read from dword +0x04 and dword +0x0de5. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

u32 CM_THISCALL cm_fn_008aa9d0(u8 *object, s8 arg1, u8 *arg2, u32 arg3)
{
    u8 *base;
    u8 *rec;
    s8 tag;

    if (arg1 < 0 || arg1 > 0x13) {
        return 0;
    }
    if (arg3 != 0) {
        if (*(u8 **)(object + 0x0de5) == NULL) {
            return 0;
        }
    }
    base = *(u8 **)(object + 0x04);
    if (base == NULL) {
        if (arg1 >= 0x0b) {
            return 0;
        }
        if (arg2 != NULL) {
            *arg2 = (u8)arg1;
        }
        return 1;
    }
    if (arg3 != 0) {
        s32 lead = (s8)object[0x0dea];
        s32 idx = (s32)arg1 + lead * 0x14;
        rec = base + 0x4796u + (u32)idx * 0x1beu;
    } else {
        rec = object + 0x2735u + (u32)(s32)arg1 * 0x1beu;
    }
    tag = (s8)rec[0x19];
    if (tag < 0) {
        if (base[0x8eb3] != 0 || (u8)tag != 0xfe) {
            return 0;
        }
    }
    if (arg2 != NULL) {
        *arg2 = (u8)tag;
        if ((u8)tag == 0xfe) {
            *arg2 = rec[0x1a];
        }
    }
    return 1;
}
