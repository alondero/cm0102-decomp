/* Preferred-image VA 0x0062be00 (image base 0x00400000). Thiscall (ECX is
   copied to ESI before any read) with three stack arguments and `ret 0xc`.

   Frame: the SEH record {prev, handler 0x0095b0db, state} sits at the top of
   the locals; state is -1 on entry, 0 around the allocation/initialiser
   path and -1 again after it. The 12-byte scratch array holds the listing's
   local buffer at offset 4: a dword key, then one byte from *arg2 (offset 8)
   and one byte from *arg3 (offset 9); the callee reads six bytes from there.
   Offset 0 keeps the allocation result that the listing writes but never reads. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

void CM_THISCALL cm_fn_0062be00(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 scratch[12];
    u32 ptr0;
    u32 eax_value;
    u32 node;
    u32 nkey;
    u32 key;
    u32 p;
    u8 cl;
    u8 dl;
    s32 sum;

    frame.prev = CM_FS(u32, 0);
    frame.handler = 0x0095b0dbu;
    frame.state = -1;
    CM_FS(u32, 0) = (u32)&frame;

    if (*(volatile u32 *)(self + 0x28u) == 0) {
        ptr0 = (u32)cm_fn_00944cf1(0xeu);
        *(volatile u32 *)&scratch[0] = ptr0;
        frame.state = 0;
        if (ptr0 != 0) {
            eax_value = cm_fn_0068d020((u16 *)ptr0);
        } else {
            eax_value = 0;
        }
        frame.state = -1;
        *(volatile u32 *)(self + 0x28u) = eax_value;
    }

    p = *(volatile u32 *)(self + 0x28u);
    *(volatile u32 *)(p + 0xau) = *(volatile u32 *)(p + 2u);

    node = cm_fn_0068d1b0(*(volatile u32 *)(self + 0x28u));
    while (node != 0) {
        nkey = *(volatile u32 *)node;
        key = *(volatile u32 *)arg1;
        if (nkey == key) {
            goto found;
        }
        node = cm_fn_0068d1b0(*(volatile u32 *)(self + 0x28u));
    }

    key = *(volatile u32 *)arg1;
    *(volatile u32 *)&scratch[4] = key;
    dl = *(volatile u8 *)arg3;
    cl = *(volatile u8 *)arg2;
    scratch[9] = dl;
    scratch[8] = cl;
    p = *(volatile u32 *)(self + 0x28u);
    (void)cm_fn_0068d0d0((volatile u8 *)p, (const u8 *)&scratch[4], 6);
    goto epilogue;

found:
    cl = *(volatile u8 *)(node + 4u);
    if (cl == *(volatile u8 *)arg2) {
        dl = *(volatile u8 *)(node + 5u);
        cl = *(volatile u8 *)arg3;
        sum = (s32)(s8)cl + (s32)(s8)dl;
        if (sum > 100) {
            cl = *(volatile u8 *)(node + 4u);
            *(volatile u8 *)(node + 5u) = 0x64;
            *(volatile u8 *)arg2 = cl;
            dl = *(volatile u8 *)(node + 5u);
            *(volatile u8 *)arg3 = dl;
        } else {
            cl = (u8)(cl + dl);
            *(volatile u8 *)(node + 5u) = cl;
            cl = *(volatile u8 *)(node + 4u);
            *(volatile u8 *)arg2 = cl;
            dl = *(volatile u8 *)(node + 5u);
            *(volatile u8 *)arg3 = dl;
        }
    } else {
        dl = *(volatile u8 *)(node + 5u);
        cl = *(volatile u8 *)arg3;
        if ((s8)cl > (s8)dl) {
            *(volatile u8 *)(node + 5u) = cl;
            dl = *(volatile u8 *)arg2;
            *(volatile u8 *)(node + 4u) = dl;
        }
        cl = *(volatile u8 *)(node + 4u);
        *(volatile u8 *)arg2 = cl;
        dl = *(volatile u8 *)(node + 5u);
        *(volatile u8 *)arg3 = dl;
    }

epilogue:
    CM_FS(u32, 0) = frame.prev;
}
