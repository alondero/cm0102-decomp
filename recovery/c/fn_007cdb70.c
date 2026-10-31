#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(int *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   The SEH frame is one volatile { prev, handler, state } record linked at
   entry and unlinked from its saved prev at exit. state is -1 at entry, 0
   after the vtable store and the first field read, and -1 before the final thiscall 0x518690.
   Field offsets from self: 0x00 is the vtable pointer written at entry;
   0xa7 and 0xa3 are u32 slots released through 0x9452ca when non-zero; 0xad
   is a pointer to an array of u32 slots with a signed 16-bit count at 0x3c.
   Non-null array slots are released in index order, then the array pointer
   itself is released (unconditionally, after the array loop). 0x08 is a
   pointer released through 0x49f450 (fastcall) and 0x944c94 (cdecl) when
   non-zero. */
void CM_THISCALL cm_fn_007cdb70(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 value;
    u32 edi_val;
    s32 i;

    seh.state = -1;
    seh.handler = (void *)0x0095f9f8;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    *(volatile u32 *)base = 0x0096e650;
    value = *(volatile u32 *)(base + 0xa7);
    seh.state = 0;
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0xa3);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0xad);
    if (value != 0) {
        if (*(volatile s16 *)(base + 0x3c) > 0) {
            i = 0;
            do {
                value = *(volatile u32 *)(*(volatile u32 *)(base + 0xad) + (u32)i * 4u);
                if (value != 0)
                    cm_fn_009452ca(value);
                i++;
            } while (i < (s32)*(volatile s16 *)(base + 0x3c));
        }
        cm_fn_009452ca(*(volatile u32 *)(base + 0xad));
    }

    edi_val = *(volatile u32 *)(base + 0x08);
    if (edi_val != 0) {
        cm_fn_0049f450((int *)edi_val);
        cm_fn_00944c94((void *)edi_val);
    }

    seh.state = -1;
    cm_fn_00518690(self);
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
