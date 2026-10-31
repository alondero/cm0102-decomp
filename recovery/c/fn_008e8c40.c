#include "cmre.h"

/* Two stack arguments, popped by the callee (ret 8). Returns 0 or 1 in EAX.
   The local scratch is 0x28 bytes; the SEH record sits directly above it.
   Scratch offsets: 0x00 and 0x04 hold a record copied from a callee result,
   0x08, 0x10, 0x18 and 0x20 are the pointers passed as the second argument. */

extern u32 CM_THISCALL cm_fn_0040a7d0(int param_1, u32 param_2, u32 param_3);
/* The original returns a pointer in EAX (the listing reads [eax+2] after the
   call), so the declared return is u32 rather than the lifted void. */
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);

#define SLOT_U32(off) (*(volatile u32 *)(locals + (off)))
#define SLOT_U16(off) (*(volatile u16 *)(locals + (off)))

u32 CM_STDCALL cm_fn_008e8c40(u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 locals[0x28];
    u32 x;
    u32 p;
    u32 rec0;
    u32 rec4;
    u32 q;
    u32 w;
    u32 r;
    u32 retval;
    u16 cx;
    u16 dx;
    u8 a4;
    s8 c10;

    seh.state = -1;
    seh.handler = 0x009649f0u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    a4 = *(volatile u8 *)(arg2 + 4);
    c10 = (s8)*(volatile u8 *)(arg2 + 0xa);
    /* The original loads AL and then AX, so the upper half of the pushed
       value is the upper half of the previous SEH head (EAX on entry). */
    x = (seh.prev & 0xffff0000u) | *(volatile u16 *)(arg1 + 2);

    if ((s8)a4 <= c10) {
        p = cm_fn_0040a7d0((int)arg2, (u32)(locals + 0x10), x);
        rec4 = *(volatile u32 *)(p + 4);
        rec0 = *(volatile u32 *)p;
        SLOT_U32(4) = rec4;
        seh.state = 2;
        SLOT_U32(0) = rec0;
        q = cm_fn_0054c5b0((short *)arg1, (u32 *)(locals + 8), 1);
        cx = *(volatile u16 *)(q + 2);
        dx = SLOT_U16(2);
        if (dx < cx) {
            r = 1u;
        } else if (dx > cx) {
            r = 0u;
        } else {
            r = ((s16)(u16)rec0 < *(volatile s16 *)q) ? 1u : 0u;
        }
        seh.state = -1;
        if (r == 0) {
            retval = 0u;
            goto unlink;
        }
        w = *(volatile u16 *)(arg1 + 2);
        p = cm_fn_0040a7d0((int)(arg2 + 6), (u32)(locals + 0x18), w);
        rec0 = *(volatile u32 *)p;
        rec4 = *(volatile u32 *)(p + 4);
        SLOT_U32(0) = rec0;
        SLOT_U32(4) = rec4;
        seh.state = 3;
        q = cm_fn_0054c8e0((short *)arg1, (u32 *)(locals + 0x20), 1);
        goto compare2;
    } else {
        p = cm_fn_0040a7d0((int)arg2, (u32)(locals + 8), x);
        rec0 = *(volatile u32 *)p;
        rec4 = *(volatile u32 *)(p + 4);
        seh.state = 0;
        SLOT_U32(0) = rec0;
        SLOT_U32(4) = rec4;
        q = cm_fn_0054c5b0((short *)arg1, (u32 *)(locals + 0x10), 1);
        cx = *(volatile u16 *)(q + 2);
        dx = SLOT_U16(2);
        if (dx < cx) {
            r = 1u;
        } else if (dx > cx) {
            r = 0u;
        } else {
            r = ((s16)(u16)rec0 < *(volatile s16 *)q) ? 1u : 0u;
        }
        seh.state = -1;
        if (r != 0) {
            retval = 1u;
            goto unlink;
        }
        /* EAX is 0 here, so the word loaded into AX is zero-extended. */
        w = *(volatile u16 *)(arg1 + 2);
        p = cm_fn_0040a7d0((int)(arg2 + 6), (u32)(locals + 0x10), w);
        rec0 = *(volatile u32 *)p;
        rec4 = *(volatile u32 *)(p + 4);
        SLOT_U32(0) = rec0;
        SLOT_U32(4) = rec4;
        seh.state = 1;
        q = cm_fn_0054c8e0((short *)arg1, (u32 *)(locals + 8), 1);
        goto compare2;
    }

compare2:
    cx = *(volatile u16 *)(q + 2);
    dx = SLOT_U16(2);
    if (dx > cx) {
        retval = 1u;
    } else if (dx < cx) {
        retval = 0u;
    } else {
        retval = ((s16)(u16)rec0 > *(volatile s16 *)q) ? 1u : 0u;
    }

unlink:
    CM_FS(u32, 0) = seh.prev;
    return retval;
}
