#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_00544fc0(u32 *param_1);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

/* Cdecl with one stack argument (ret 0). The argument is a dword value; several paths
   read the dword it points to. The routine links an SEH frame {prev, handler, state} at
   fs:[0]. The state is -1 on entry and is set to 0 after the allocation. Global 0x009b5984
   selects the fast result, arg + 0x38. Otherwise global 0x00ae2408 holds an object that is
   allocated on demand (0x40 bytes, eight dwords cleared at +0x20, then 0x544fc0 is called).
   The result is 79*Q + [object+0x20] + 0x34, where Q comes from table global 0x00ae23fc
   or from the argument's pointee, or 0 when the pointee is not below the object's first
   dword. Memory accesses are volatile so that they keep the listing order. */
u32 CM_CDECL cm_fn_00545770(u32 param_1)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u32 prev_head;
    u32 g1;
    u32 g2;
    u32 g3;
    u32 v;
    u32 q;
    u32 m;
    u32 w;
    u32 esi;
    u32 ecx_val;
    u32 result;
    u32 i;

    frame.state = -1;
    frame.handler = 0x009579dbU;
    prev_head = CM_FS(u32, 0);
    frame.prev = prev_head;
    CM_FS(u32, 0) = (u32)&frame;

    result = 0;
    g1 = CM_GLOBAL(volatile u32, 0x009b5984);
    if (g1 == 0 || g1 == 1) {
        result = param_1 + 0x38U;
    } else {
        g2 = CM_GLOBAL(volatile u32, 0x00ae2408);
        if (g2 == 0) {
            esi = (u32)cm_fn_00944cf1(0x40U);
            frame.state = 0;
            if (esi != 0) {
                for (i = 0; i < 8; i++) {
                    CM_GLOBAL(volatile u32, esi + 0x20U + 4 * i) = 0;
                }
                cm_fn_00544fc0((u32 *)esi);
                ecx_val = esi;
            } else {
                ecx_val = 0;
            }
            CM_GLOBAL(volatile u32, 0x00ae2408) = ecx_val;
            g2 = ecx_val;
        }
        if (g2 != 0) {
            g3 = CM_GLOBAL(volatile u32, 0x00ae23fc);
            if (g3 != 0) {
                v = CM_GLOBAL(volatile u32, param_1);
                q = CM_GLOBAL(volatile u32, g3 + v * 4U);
                w = CM_GLOBAL(volatile u32, g2 + 0x20U);
                result = 79U * q + w + 0x34U;
            } else {
                v = CM_GLOBAL(volatile u32, param_1);
                m = CM_GLOBAL(volatile u32, g2);
                if ((s32)v < (s32)m) {
                    w = CM_GLOBAL(volatile u32, g2 + 0x20U);
                    result = 79U * v + w + 0x34U;
                }
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
