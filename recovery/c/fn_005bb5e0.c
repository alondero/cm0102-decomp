#include "cmre.h"

/* fn_005bb5e0: thiscall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x005bb5e0, image base 0x00400000.
   Object fields by byte offset from the object pointer in ECX:
   0x00 vtable pointer (written by this routine), 0x08 pointer passed to
   0x0049f450 and 0x00944c94, 0xba pointer released through 0x009452ca and then
   cleared, 0xee and 0xf2 element pointers whose slot 0 is called with one argument. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern void CM_THISCALL cm_fn_00687970(u32 self, u32 buffer);
extern void CM_FASTCALL cm_fn_0049f450(u32 self);
extern void CM_FASTCALL cm_fn_00682300(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Virtual slot 0, called with the element as ECX and one stack argument that the
   callee pops. */
typedef void (CM_THISCALL *cm_slot0_fn)(u32 self, u32 arg);

void CM_THISCALL cm_fn_005bb5e0(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 released;
    u32 extra;
    u32 element;
    u32 vtable;

    seh.state = -1;
    seh.handler = 0x00959998u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    *(volatile u32 *)self = 0x0096aa24u;
    seh.state = 0;
    cm_fn_00687970(self, 0u);

    released = OBJ_U32(0xba);
    if (released != 0) {
        cm_fn_009452ca(released);
        OBJ_U32(0xba) = 0;
    }

    extra = OBJ_U32(0x08);
    if (extra != 0) {
        cm_fn_0049f450(extra);
        cm_fn_00944c94(extra);
    }

    element = OBJ_U32(0xee);
    if (element != 0) {
        vtable = *(volatile u32 *)element;
        cm_slot0_fn slot = (cm_slot0_fn)(*(volatile u32 *)vtable);
        slot(element, 1u);
    }

    element = OBJ_U32(0xf2);
    if (element != 0) {
        vtable = *(volatile u32 *)element;
        cm_slot0_fn slot = (cm_slot0_fn)(*(volatile u32 *)vtable);
        slot(element, 1u);
    }

    seh.state = -1;
    cm_fn_00682300(self);
    CM_FS(u32, 0) = seh.prev;
}
