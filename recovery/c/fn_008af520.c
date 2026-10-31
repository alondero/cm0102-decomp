#include "cmre.h"

extern void CM_THISCALL cm_fn_00412e40(void *self, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
extern void CM_FASTCALL cm_fn_0054a110(void *p);
extern u32 CM_THISCALL cm_fn_008af660(void *self, u32 arg);
extern void CM_FASTCALL cm_fn_008afdc0(void *self);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self) with ten stack arguments (`ret 0x28`), named by entry
   offset (arg_04 .. arg_28). Returns self (EAX = ESI at exit).
   The SEH frame is one volatile record linked at entry and unlinked from its
   saved prev at exit. State values, in listing order: -1 at entry, 0 before
   the fastcall at self+0x1d, 1 before the one at self+0x4e+0x1f4, 2 before
   the one at self+0x4e+0x1fc, 3 after the argument stores below.
   The listing stores self+0x4e into the arg_28 stack slot after state 1;
   that slot is never read again, so it is not modelled as a field.
   Field offsets from self: 0x25 and 0x25c and 0x253 receive the arg_1c byte,
   the arg_20 byte and the constant 3; 0x252 and 0x04 get zero bytes; 0x254 and
   0x258 get -1; the vtable at offset 0 is 0x009703f0; a table of 20 records of
   25 bytes starts at self+0x52 with the stores at -4, 0, +4, +8, +0xc (-1) and
   +0x11 (0, 4 bytes). The optional final call (arg_24 != 0) is to 0x8af660. */
u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c,
                               u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c,
                               u32 arg_20, u32 arg_24, u32 arg_28)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 edi_v;
    u32 cursor;
    u32 n;

    (void)arg_28;

    seh.state = -1;
    seh.handler = (void *)0x009637cc;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_00412e40(self, arg_04, arg_08, arg_0c, arg_10, arg_14, arg_18, arg_24, arg_28);

    seh.state = 0;
    cm_fn_0054a110((void *)((u32)self + 0x1d));

    edi_v = (u32)self + 0x4e;
    seh.state = 1;
    cm_fn_0054a110((void *)(edi_v + 0x1f4));

    seh.state = 2;
    cm_fn_0054a110((void *)(edi_v + 0x1fc));

    *(volatile u8 *)(base + 0x25) = (u8)arg_1c;
    seh.state = 3;
    *(volatile u8 *)(base + 0x253) = 3;
    *(volatile u8 *)(base + 0x25c) = (u8)arg_20;
    *(volatile u32 *)base = 0x009703f0u;
    *(volatile u8 *)(base + 0x04) = 0;
    *(volatile u32 *)(base + 0x254) = 0xffffffffu;
    *(volatile u32 *)(base + 0x258) = 0xffffffffu;

    cursor = (u32)self + 0x52;
    n = 0x14;
    do {
        *(volatile u32 *)(cursor - 4) = 0xffffffffu;
        *(volatile u32 *)(cursor) = 0xffffffffu;
        *(volatile u32 *)(cursor + 4) = 0xffffffffu;
        *(volatile u32 *)(cursor + 8) = 0xffffffffu;
        *(volatile u32 *)(cursor + 0xc) = 0xffffffffu;
        *(volatile u32 *)(cursor + 0x11) = 0;
        cursor += 0x19;
        n--;
    } while (n != 0);

    *(volatile u8 *)(base + 0x252) = 0;
    cm_fn_008afdc0(self);
    if (arg_24 != 0)
        cm_fn_008af660(self, arg_24);

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return (u32)self;
}
