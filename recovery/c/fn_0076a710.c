#include "cmre.h"

extern void CM_FASTCALL cm_fn_0058cf20(int *param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value. The SEH
   frame is one volatile record: state -1 at entry, then 0 after the first field
   read (self+0x1c), -1 before the final fastcall 0x68d040 on self+0x28.
   Field offsets from self: 0x1c pointer released through 0x58cf20 (fastcall)
   and 0x944c94 when non-zero; 0x00 pointer released through 0x68d040 (fastcall)
   and 0x944c94 when non-zero; 0x04, 0x08, 0x0c, 0x10, 0x24 and 0x36 are dwords
   released through 0x9452ca when non-zero. 0x20 is an array pointer: each
   6-byte slot in the array whose dword is non-null is released, the loop runs
   while the offset counter (in units of 1) is below G2 - 2*G1 (globals at
   0x00ae235c and 0x00ae2350), then the array pointer itself is released. */
void CM_THISCALL cm_fn_0076a710(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 edi_v;
    u32 value;
    u32 arr;
    u32 elem;
    u32 off;
    s32 i;
    s32 bound;

    seh.state = -1;
    seh.handler = (void *)0x0095dccb;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    edi_v = *(volatile u32 *)(base + 0x1c);
    seh.state = 0;
    if (edi_v != 0) {
        cm_fn_0058cf20((int *)edi_v);
        cm_fn_00944c94((void *)edi_v);
    }

    edi_v = *(volatile u32 *)(base + 0x00);
    if (edi_v != 0) {
        cm_fn_0068d040((int)edi_v);
        cm_fn_00944c94((void *)edi_v);
    }

    value = *(volatile u32 *)(base + 0x04);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0x08);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0x0c);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0x10);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0x20);
    if (value != 0) {
        i = 0;
        off = 0;
        bound = *(volatile s32 *)0x00ae235c - 2 * *(volatile s32 *)0x00ae2350;
        if (bound > 0) {
            do {
                arr = *(volatile u32 *)(base + 0x20);
                elem = *(volatile u32 *)(arr + off);
                if (elem != 0)
                    cm_fn_009452ca(elem);
                off += 6u;
                i++;
            } while (i < *(volatile s32 *)0x00ae235c - 2 * *(volatile s32 *)0x00ae2350);
        }
        cm_fn_009452ca(*(volatile u32 *)(base + 0x20));
    }

    value = *(volatile u32 *)(base + 0x24);
    if (value != 0)
        cm_fn_009452ca(value);

    value = *(volatile u32 *)(base + 0x36);
    if (value != 0)
        cm_fn_009452ca(value);

    seh.state = -1;
    cm_fn_0068d040((int)(base + 0x28));
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
