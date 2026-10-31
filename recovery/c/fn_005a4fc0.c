#include "cmre.h"

/* fn_005a4fc0: thiscall (ECX = object, read before written) with one stack argument (ret 4).
   Preferred-image VA 0x005a4fc0, image base 0x00400000. Returns the object (ESI) in EAX.
   SEH frame {prev, handler 0x009592cc, state} is linked first and unlinked before the return.
   The object's dwords at +0x00, +0x04 and +0x08 are set. A nonzero first argument selects the
   path that calls 0x005a5180 with the object as ECX first; a zero argument selects the other.
   The state field takes 0 and 1 on the first path and 2 and 3 on the second, with -1 stored
   before each store to +0x00. The stack argument slot is reused as a local by the listing,
   but its value is only read before that point. */

extern u32 CM_THISCALL cm_fn_005a5180(u32 *param_1, int param_2);
extern u32 CM_FASTCALL cm_fn_0068d020(u32 param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_005a4fc0(u8 *self, u32 arg1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 edi;
    u32 p;
    u32 eax;

    seh.state = -1;
    seh.handler = 0x009592ccu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    *(volatile u32 *)(self + 0x00) = 0u;
    *(volatile u32 *)(self + 0x04) = 0u;

    if (arg1 != 0u) {
        eax = cm_fn_005a5180((u32 *)self, (int)arg1);
        if ((u8)eax != 0u) {
            CM_FS(u32, 0) = seh.prev;
            return (u32)self;
        }
        {
            edi = *(volatile u32 *)(self + 0x00);
            if (edi != 0u) {
                cm_fn_0068d040((int)edi);
                cm_fn_00944c94(edi);
            }
            edi = *(volatile u32 *)(self + 0x04);
            if (edi != 0u) {
                cm_fn_0068d040((int)edi);
                cm_fn_00944c94(edi);
            }

            p = (u32)cm_fn_00944cf1(0x0eu);
            seh.state = 0;
            if (p != 0u) {
                eax = cm_fn_0068d020(p);
            } else {
                eax = 0u;
            }
            seh.state = -1;
            *(volatile u32 *)(self + 0x00) = eax;

            p = (u32)cm_fn_00944cf1(0x0eu);
            seh.state = 1;
            if (p != 0u) {
                eax = cm_fn_0068d020(p);
            } else {
                eax = 0u;
            }
            *(volatile u32 *)(self + 0x04) = eax;
        }
    } else {
        p = (u32)cm_fn_00944cf1(0x0eu);
        seh.state = 2;
        if (p != 0u) {
            eax = cm_fn_0068d020(p);
        } else {
            eax = 0u;
        }
        seh.state = -1;
        *(volatile u32 *)(self + 0x00) = eax;

        p = (u32)cm_fn_00944cf1(0x0eu);
        seh.state = 3;
        if (p != 0u) {
            eax = cm_fn_0068d020(p);
        } else {
            eax = 0u;
        }
        *(volatile u32 *)(self + 0x04) = eax;
    }

    eax = CM_GLOBAL(u32, 0x00ae235c);
    *(volatile u32 *)(self + 0x08) = (u32)cm_fn_00945501(eax, 1u);

    CM_FS(u32, 0) = seh.prev;
    return (u32)self;
}
