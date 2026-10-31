#include "cmre.h"

/* fn_00583a00: thiscall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x00583a00, image base 0x00400000.
   Object fields: 0x00 vtable pointer (written here as 0x0096a1d0), 0x08 pointer passed to
   0x0049f450 (fastcall) and 0x00944c94, 0x0c table of object pointers (slot 0 called with one
   argument; count at 0x30, signed, inclusive), 0x3c signed 16-bit count of the 0x0ad table,
   0x30 signed count, 0xa3, 0xa7 and 0xad pointers released through 0x009452ca.
   SEH frame {prev, handler 0x00958a98, state} is linked first and unlinked last. */

extern void CM_FASTCALL cm_fn_0049f450(u32 self);
extern void CM_THISCALL cm_fn_00518690(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Slot 0 of an object's vtable, called with the object as ECX and one stack argument that
   the callee pops. */
typedef void (CM_THISCALL *cm_slot0_fn)(u32 self, u32 arg);

void CM_THISCALL cm_fn_00583a00(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 eax;
    u32 t2;
    u32 obj;
    u32 vt;
    u32 target;
    u32 p;
    u32 i;
    s16 count;

    seh.state = -1;
    seh.handler = 0x00958a98u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    *(volatile u32 *)self = 0x0096a1d0u;
    eax = *(volatile u32 *)(self + 0xa7u);
    seh.state = 0;
    if (eax != 0u) {
        cm_fn_009452ca(eax);
    }
    eax = *(volatile u32 *)(self + 0xa3u);
    if (eax != 0u) {
        cm_fn_009452ca(eax);
    }

    if (*(volatile u32 *)(self + 0xadu) != 0u) {
        i = 0;
        if ((s16)*(volatile s16 *)(self + 0x3cu) > 0) {
            do {
                eax = *(volatile u32 *)(*(volatile u32 *)(self + 0xadu) + i * 4u);
                if (eax != 0u) {
                    cm_fn_009452ca(eax);
                }
                count = *(volatile s16 *)(self + 0x3cu);
                i = i + 1u;
            } while ((s32)i < (s32)count);
        }
        cm_fn_009452ca(*(volatile u32 *)(self + 0xadu));
    }

    i = 0;
    if ((s32)*(volatile s32 *)(self + 0x30u) >= 0) {
        do {
            t2 = *(volatile u32 *)(self + 0x0cu);
            obj = *(volatile u32 *)(t2 + i * 4u);
            if (obj != 0u) {
                vt = *(volatile u32 *)obj;
                target = *(volatile u32 *)vt;
                ((cm_slot0_fn)target)(obj, 1u);
            }
            i = i + 1u;
        } while ((s32)i <= (s32)*(volatile s32 *)(self + 0x30u));
    }

    t2 = *(volatile u32 *)(self + 0x0cu);
    if (t2 != 0u) {
        cm_fn_009452ca(t2);
        *(volatile u32 *)(self + 0x0cu) = 0u;
    }

    p = *(volatile u32 *)(self + 0x08u);
    if (p != 0u) {
        cm_fn_0049f450(p);
        cm_fn_00944c94(p);
    }

    seh.state = -1;
    cm_fn_00518690(self);
    CM_FS(u32, 0) = seh.prev;
}
