#include "cmre.h"

extern void CM_THISCALL cm_fn_00412e40(void *self, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
extern u32 CM_THISCALL cm_fn_00766f50(void *self, u32 param_2);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), twelve stack arguments (`ret 0x30`), named by entry
   offset (arg_04 .. arg_30), returns self. The SEH frame is one volatile
   record linked at entry and unlinked at exit; state is 0 from the vtable
   store on.
   The first call (0x412e40) gets self, arg_04, arg_08, arg_0c, arg_10, arg_14,
   arg_18, arg_2c and arg_30. Then the loop writes 11-byte records at
   self+0x25 + 11*k for k = 0,1,...: a byte cl at +0x25, a word dx at +0x26,
   a dword -1 at +0x1d, and a dword -1 at self+11*(k+3). The byte index k is
   stored at self+0xa1 and incremented each pass. cl starts at arg_1c and
   counts to 11 (then wraps to 0 with dx incremented); dx starts at arg_20 and
   bp is arg_28 (word). The loop ends when dx is above bp (unsigned, dx != bp),
   or when dx == bp and cl exceeds arg_24 (signed byte), or when k+1 >= 12
   (signed). A non-zero arg_2c triggers the thiscall 0x766f50. */
u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c,
                               u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c,
                               u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c,
                               u32 arg_30)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 cl;
    u16 dx;
    u16 bp;
    s32 k;
    u8 al;

    seh.state = -1;
    seh.handler = (void *)0x0095db88;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_00412e40(self, arg_04, arg_08, arg_0c, arg_10, arg_14, arg_18, arg_2c, arg_30);

    bp = (u16)arg_28;
    cl = (u8)arg_1c;
    dx = (u16)arg_20;
    seh.state = 0;
    *(volatile u32 *)base = 0x0096da00u;
    *(volatile u8 *)(base + 4) = 2;
    *(volatile u8 *)(base + 0xa1) = 0;

    do {
        k = (s32)(s8)*(volatile u8 *)(base + 0xa1);
        *(volatile u8 *)(base + (u32)k * 11u + 0x25) = cl;
        *(volatile u16 *)(base + (u32)k * 11u + 0x26) = dx;
        *(volatile u32 *)(base + (u32)k * 11u + 0x1d) = 0xffffffffu;
        *(volatile u32 *)(base + (u32)(k + 3) * 11u) = 0xffffffffu;
        al = (u8)(k + 1);
        cl = (u8)(cl + 1);
        *(volatile u8 *)(base + 0xa1) = al;
        if ((s8)cl > 0x0b) {
            cl = 0;
            dx++;
        }
        if (dx == bp) {
            /* Original 766ee9: ESP is entryESP-0x1c, so +0x40
               is entry argument +0x24, the end month. */
            if ((s8)cl > (s8)(u8)arg_24)
                break;
        } else if (dx > bp) {
            break;
        }
        if (!((s8)al < 0x0c))
            break;
    } while (1);

    if (arg_2c != 0)
        cm_fn_00766f50(self, arg_2c);

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return (u32)self;
}
