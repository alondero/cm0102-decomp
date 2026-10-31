#include "cmre.h"

/* fn_007671c0: thiscall (ECX = object, read before written) with four stack arguments (ret 0x10).
   Preferred-image VA 0x007671c0, image base 0x00400000.
   Object: 12 records of 11 bytes starting at this. Within a record: 0x1d dword (set to arg1),
   0x21 dword (set to arg2), 0x25 byte matched against arg3, 0x26 word matched against arg4.
   The object also reads 0x05 dword, 0x15 and 0x17 words and 0x19 dword, and uses the
   globals at 0x00ae23b4 (table base, 581-byte rows) and 0x00ae23bc (table base, 110-byte rows).
   Arguments are kept as 32-bit values because the listing pushes full registers; only their
   low bytes or words are consumed by the callees' char/short parameters.
   Callees: 0x00412f00 cdecl (five words), 0x00767330 thiscall (one char), 0x0086c8c0 thiscall
   with five stack words (ret 0x14) and receiver at the global 0x00dd7f00. */

extern void CM_CDECL cm_fn_00412f00(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern void CM_THISCALL cm_fn_00767330(int param_1, char param_2);
extern void CM_THISCALL cm_fn_0086c8c0(u32 self, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);

void CM_THISCALL cm_fn_007671c0(u8 *self, u32 a1, u32 a2, u32 a3, u32 a4)
{
    u32 idx;
    u8 *rec;
    u8 *pa;
    u32 iv3;
    u32 x19;
    u32 w15;
    u32 w17;
    u32 hi;
    u32 t5;
    u32 ptr;
    u32 cur;

    idx = 0;
    for (;;) {
        rec = self + idx * 11u;
        if (*(volatile u8 *)(rec + 0x25u) == (u8)a3 && *(volatile u16 *)(rec + 0x26u) == (u16)a4) {
            break;
        }
        idx = idx + 1u;
        if (idx >= 12u) {
            return;
        }
    }

    pa = rec + 0x1d;
    *(volatile u32 *)pa = a1;
    *(volatile u32 *)(rec + 0x21u) = a2;

    if ((s32)a1 > -1) {
        if ((s32)a2 <= -1) {
            iv3 = 0u;
        } else {
            iv3 = CM_GLOBAL(u32, 0x00ae23b4) + a2 * 581u;
        }
        x19 = *(volatile u32 *)(self + 0x19u);
        w17 = *(volatile u16 *)(self + 0x17u);
        w15 = *(volatile u16 *)(self + 0x15u);
        hi = (x19 >> 16) & 0xffffu;
        cm_fn_00412f00(CM_GLOBAL(u32, 0x00ae23bc) + a1 * 110u, iv3,
                       (hi << 16) | w15, (hi << 16) | w17, x19);
    }

    cm_fn_00767330((int)self, (char)idx);

    cur = *(volatile u32 *)(rec + 0x21u);
    if ((s32)cur != -1) {
        t5 = *(volatile u32 *)(self + 0x05u);
        ptr = CM_GLOBAL(u32, 0x00ae23b4) + cur * 581u;
    } else {
        t5 = *(volatile u32 *)(self + 0x05u);
        ptr = 0u;
    }

    cur = *(volatile u32 *)pa;
    cm_fn_0086c8c0(CM_GLOBAL(u32, 0x00dd7f00),
                   CM_GLOBAL(u32, 0x00ae23bc) + cur * 110u,
                   ptr, t5, a3, a4);
}
