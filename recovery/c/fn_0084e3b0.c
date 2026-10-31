#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 *param_1);
extern void CM_FASTCALL cm_fn_00682300(u32 *param_1);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;
/* Indirect slot 0 of the object at self+0xee: the listing loads the vtable
   from [object] and calls [vtable] after `mov ecx, object` with one pushed
   argument (value 1). The callee is thiscall and pops its own argument. */
typedef void (CM_THISCALL *cm_fn_0084e3b0_slot)(void *self, u32 arg);

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   Same SEH frame discipline as fn_007c8d50: one volatile
   { prev, handler, state } record linked into fs:[0] at entry and unlinked
   from its saved prev at exit. The state word is 0 before the 0x687970 call
   and -1 before the final 0x682300 call. Field offsets from self: 0x00 is the
   vtable pointer written at entry, 0x08 is a pointer released through
   0x49f450 and 0x944c94, 0xba is a u32 slot passed to 0x9452ca (zeroed after),
   0xee is an object pointer whose slot 0 is called if non-null. */
void CM_THISCALL cm_fn_0084e3b0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 field;
    u32 edi;
    u32 obj;
    u32 vtable;

    seh.state = -1;
    seh.handler = (void *)0x009626c8;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    *(volatile u32 *)base = 0x0096fa68;
    seh.state = 0;
    cm_fn_00687970((u8 *)self, (u8 *)0);

    field = *(volatile u32 *)(base + 0xba);
    if (field != 0) {
        cm_fn_009452ca(field);
        *(volatile u32 *)(base + 0xba) = 0;
    }

    edi = *(volatile u32 *)(base + 0x08);
    if (edi != 0) {
        cm_fn_0049f450((u32 *)edi);
        cm_fn_00944c94((void *)edi);
    }

    obj = *(volatile u32 *)(base + 0xee);
    if (obj != 0) {
        vtable = *(volatile u32 *)obj;
        (*(cm_fn_0084e3b0_slot volatile *)vtable)((void *)obj, 1);
    }

    seh.state = -1;
    cm_fn_00682300((u32 *)self);
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
