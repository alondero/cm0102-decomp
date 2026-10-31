#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u32 *param_1);
extern void CM_FASTCALL cm_fn_00682300(u32 *param_1);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   The SEH frame is one volatile { prev, handler, state } record linked into
   fs:[0] before the first store and unlinked from its saved prev last.
   Field offsets from self: 0x00 is the vtable pointer written at entry,
   0x08 is a pointer released through 0x49f450 and 0x944c94, 0xba and 0xee
   are u32 slots passed to 0x9452ca (0xba is zeroed after release). The
   state word is set to 0 before 0x687970 and to -1 before 0x682300. */
void CM_THISCALL cm_fn_007c8d50(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 field;
    u32 edi;

    seh.state = -1;
    seh.handler = (void *)0x0095f758;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    *(volatile u32 *)base = 0x0096e360;
    seh.state = 0;
    cm_fn_00687970((u8 *)self, (u8 *)0);

    field = *(volatile u32 *)(base + 0xee);
    if (field != 0)
        cm_fn_009452ca(field);

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

    seh.state = -1;
    cm_fn_00682300((u32 *)self);
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
