#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(int *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;
/* Slot 0 of the element's vtable, called thiscall with the element as ECX and
   one pushed argument (value 1). The callee pops its own argument. */
typedef void (CM_THISCALL *cm_fn_0040ad70_slot)(void *self, u32 arg);

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   Same SEH frame discipline as the 0x78a7a0 family: one volatile record,
   state -1 at entry, the field read at self+0xa7 before the state store to 0,
   and -1 again before the final thiscall 0x518690.
   Field offsets from self: 0x00 vtable (written at entry); 0xa7 and 0xa3 u32
   slots released when non-zero; 0xad array pointer with a signed 16-bit count
   at 0x3c (released as in the family, then the array itself released);
   0x30 is a signed count of elements in the pointer array at 0x0c; each
   non-null element gets slot 0 called (thiscall, element as ECX, argument 1);
   the 0x0c array is then released and zeroed when non-zero; 0x08 is released
   through 0x49f450 and 0x944c94 when non-zero; 0xb2 is released when non-zero.
   Loop 2 runs while the index is <= the count re-read each pass (so it runs
   count + 1 times for count >= 0 and is skipped when count < 0). */
void CM_THISCALL cm_fn_0040ad70(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 value;
    u32 edi_v;
    u32 arr;
    u32 elem;
    u32 vt;
    u32 i;

    seh.state = -1;
    seh.handler = (void *)0x00953778;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    *(volatile u32 *)base = 0x009674d4u;
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
                elem = *(volatile u32 *)(*(volatile u32 *)(base + 0xad) + i * 4u);
                if (elem != 0)
                    cm_fn_009452ca(elem);
                i++;
            } while ((s32)i < (s32)*(volatile s16 *)(base + 0x3c));
        }
        cm_fn_009452ca(*(volatile u32 *)(base + 0xad));
    }

    if ((s32)*(volatile s32 *)(base + 0x30) >= 0) {
        i = 0;
        do {
            arr = *(volatile u32 *)(base + 0x0c);
            elem = *(volatile u32 *)(arr + i * 4u);
            if (elem != 0) {
                vt = *(volatile u32 *)elem;
                (*(cm_fn_0040ad70_slot volatile *)vt)((void *)elem, 1);
            }
            i++;
        } while ((s32)i <= (s32)*(volatile s32 *)(base + 0x30));
    }

    arr = *(volatile u32 *)(base + 0x0c);
    if (arr != 0) {
        cm_fn_009452ca(arr);
        *(volatile u32 *)(base + 0x0c) = 0;
    }

    edi_v = *(volatile u32 *)(base + 0x08);
    if (edi_v != 0) {
        cm_fn_0049f450((int *)edi_v);
        cm_fn_00944c94((void *)edi_v);
    }

    value = *(volatile u32 *)(base + 0xb2);
    if (value != 0)
        cm_fn_009452ca(value);

    seh.state = -1;
    cm_fn_00518690(self);
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
