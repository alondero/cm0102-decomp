#include "cmre.h"

extern u32 *CM_THISCALL cm_fn_007af580(u32 *param_1, u32 *param_2);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

/* Thiscall with no stack arguments; always returns 1. Fields used by offset: +0x1ee is a
   base pointer B, +0x1f2 a dword cleared with it, +0x1f6 a signed record count C. Records
   are 0x2d (45) bytes. A record whose byte at +0x20 has bit 1 set is removed: the callee at
   0x007af580 gets (B + off, B + 45*C - 45) and the count drops by one, and the later
   records slide down. The compacted table is then passed to 0x00945333 with the global
   0x007bce50 as its last argument, or freed when empty. */
u32 CM_THISCALL cm_fn_007b11b0(u8 *param_1)
{
    s32 count;
    s32 i;
    u32 off;
    u32 base;

    count = *(s32 *)(param_1 + 0x1f6);
    if (count != 0) {
        if (count > 0) {
            i = 0;
            off = 0;
            do {
                base = *(u32 *)(param_1 + 0x1ee);
                if ((*(u8 *)(base + off + 0x20) & 2) != 0) {
                    cm_fn_007af580((u32 *)(base + off),
                                   (u32 *)(base + (u32)count * 45U - 0x2dU));
                    *(s32 *)(param_1 + 0x1f6) = *(s32 *)(param_1 + 0x1f6) - 1;
                    i--;
                    off -= 0x2dU;
                }
                count = *(s32 *)(param_1 + 0x1f6);
                i++;
                off += 0x2dU;
            } while (i < count);
        }
        count = *(s32 *)(param_1 + 0x1f6);
        if (count != 0) {
            base = *(u32 *)(param_1 + 0x1ee);
            cm_fn_00945333((u32 *)base, (u32 *)count, 0x2dU, (void *)0x007bce50);
        } else {
            base = *(u32 *)(param_1 + 0x1ee);
            cm_fn_009452ca(base);
            *(u32 *)(param_1 + 0x1ee) = 0;
            *(u32 *)(param_1 + 0x1f2) = 0;
        }
    }
    return 1;
}
