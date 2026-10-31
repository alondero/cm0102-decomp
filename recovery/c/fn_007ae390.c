#include "cmre.h"

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_FASTCALL cm_fn_00806450(u32 *param_1);
extern void CM_FASTCALL cm_fn_0081b800(u32 *param_1);
extern u16 *CM_THISCALL cm_fn_00549f70(u16 *self, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
extern void CM_THISCALL cm_fn_00417c10(void *self, u32 a, u32 b);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`; returns self in EAX.
   The SEH frame is one volatile record. The state word is -1 at entry, then
   0..5 before each of the six fastcall/helper calls on fields, 6 before the
   0x549f70 call, and 7 before the 0x417c10 call (never reset). The scratch
   block frame[20] holds the 0x549f70 output buffer at frame+4 (its result is
   passed to 0x417c10 as the value) and the 0x417c10 output pointer at frame+12.
   Field offsets from self: 0x00 is the dword -1; 0x04 a byte -1; 0x05 the
   dword 0x24a9e; 0x09 byte 1; 0x0a byte 4; 0x0b, 0x70, 0x139 zero bytes; 0x16c
   the global at 0x00dd7120; 0x180, 0x18c, 0x190, 0x195, 0x1a1 are -1, 0x194 a
   byte -1, 0x199 and 0x19d zero; after the calls 0x1b5 (byte), 0x1ee, 0x1f2,
   0x1f6, 0x170 are zero and 0x2a4 is 1 and 0x2a8, 0x2ac zero. */
u32 CM_THISCALL cm_fn_007ae390(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 frame[20];
    u32 g;
    u16 *r;

    seh.state = -1;
    seh.handler = (void *)0x0095f2ea;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0054a110((u16 *)(base + 0x184));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(base + 0x1a5));
    seh.state = 1;
    cm_fn_0054a110((u16 *)(base + 0x1ad));
    seh.state = 2;
    cm_fn_0054a110((u16 *)(base + 0x1b6));
    seh.state = 3;
    cm_fn_0054a110((u16 *)(base + 0x1be));
    seh.state = 4;
    cm_fn_00806450((u32 *)(base + 0x1fa));
    seh.state = 5;
    cm_fn_0081b800((u32 *)(base + 0x293));

    *(volatile u32 *)(base + 0x05) = 0x00024a9eu;
    *(volatile u32 *)(base + 0x00) = 0xffffffffu;
    *(volatile u8 *)(base + 0x04) = 0xffu;
    *(volatile u8 *)(base + 0x09) = 1;
    *(volatile u8 *)(base + 0x0a) = 4;
    *(volatile u8 *)(base + 0x0b) = 0;
    *(volatile u8 *)(base + 0x70) = 0;
    *(volatile u8 *)(base + 0x139) = 0;
    g = *(volatile u32 *)0x00dd7120;
    *(volatile u32 *)(base + 0x16c) = g;
    seh.state = 6;
    *(volatile u32 *)(base + 0x180) = 0xffffffffu;
    *(volatile u32 *)(base + 0x18c) = 0xffffffffu;
    *(volatile u8 *)(base + 0x194) = 0xffu;
    *(volatile u32 *)(base + 0x190) = 0xffffffffu;
    *(volatile u32 *)(base + 0x195) = 0xffffffffu;
    *(volatile u32 *)(base + 0x199) = 0;
    *(volatile u32 *)(base + 0x19d) = 0;
    *(volatile u32 *)(base + 0x1a1) = 0xffffffffu;

    r = cm_fn_00549f70((u16 *)(frame + 4), 0x1bu, 4u, 0x7b1u, 0xffffffffu);

    seh.state = 7;
    cm_fn_00417c10((void *)(base + 0x1a5), (u32)(frame + 12), (u32)r);

    *(volatile u8 *)(base + 0x1b5) = 0;
    *(volatile u32 *)(base + 0x1ee) = 0;
    *(volatile u32 *)(base + 0x1f2) = 0;
    *(volatile u32 *)(base + 0x1f6) = 0;
    *(volatile u32 *)(base + 0x2a4) = 1;
    *(volatile u32 *)(base + 0x2a8) = 0;
    *(volatile u32 *)(base + 0x2ac) = 0;
    *(volatile u32 *)(base + 0x170) = 0;

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return (u32)self;
}
