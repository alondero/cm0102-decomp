#include "cmre.h"

extern void CM_THISCALL cm_fn_005a8f60(void *self);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3, void *param_4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   The SEH frame is one volatile record linked at entry and unlinked from its
   saved prev at exit. The state word is set to 4 right after the flag byte at
   self+0x8e9d is read, then to 3, 2, 1, 0 and -1 (byte writes for 3, 2, 1 and
   0; dword for 4 and -1) before each of the following calls in listing order.
   Calls: thiscall 0x5a8f60 on the object at self+0x8f7a (released through
   0x944c94, then zeroed, with self+0x8e9d cleared, only when the flag byte is
   non-zero and the pointer is non-null); fastcall-released pointer at
   self+0x478e (0x68d040, then 0x944c94, then zeroed); thiscall 0x5a8f60 on
   self+0xf5b2; four stdcall calls to 0x9462bb with (self+0xc3a8, 0x18e3, 2,
   0x5a53c0), (self+0x91e2, 0x18e3, 2, 0x5a53c0), (self+0x9032, 0xd0, 2,
   0x6dbb10), and (self+0x4796, 0x1be, 0x28, 0x6ebd60). Each 0x9462bb call
   is stdcall (the listing has no add esp after it) and the callee pops its
   own four arguments. */
void CM_THISCALL cm_fn_006b97a0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 al;
    u32 edi_v;

    seh.state = -1;
    seh.handler = (void *)0x0095d3da;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    al = *(volatile u8 *)(base + 0x8e9d);
    seh.state = 4;
    if (al != 0) {
        edi_v = *(volatile u32 *)(base + 0x8f7a);
        if (edi_v != 0) {
            cm_fn_005a8f60((void *)edi_v);
            cm_fn_00944c94((void *)edi_v);
            *(volatile u32 *)(base + 0x8f7a) = 0;
            *(volatile u8 *)(base + 0x8e9d) = 0;
        }
    }

    edi_v = *(volatile u32 *)(base + 0x478e);
    if (edi_v != 0) {
        cm_fn_0068d040((int)edi_v);
        cm_fn_00944c94((void *)edi_v);
        *(volatile u32 *)(base + 0x478e) = 0;
    }

    seh.state = 3;
    cm_fn_005a8f60((void *)(base + 0xf5b2));

    seh.state = 2;
    cm_fn_009462bb((u32)(base + 0xc3a8), 0x18e3u, 2, (void *)0x005a53c0);

    seh.state = 1;
    cm_fn_009462bb((u32)(base + 0x91e2), 0x18e3u, 2, (void *)0x005a53c0);

    seh.state = 0;
    cm_fn_009462bb((u32)(base + 0x9032), 0xd0u, 2, (void *)0x006dbb10);

    seh.state = -1;
    cm_fn_009462bb((u32)(base + 0x4796), 0x1beu, 0x28, (void *)0x006ebd60);

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
