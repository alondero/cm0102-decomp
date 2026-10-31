#include "cmre.h"

/* Preferred-image VA00905230, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX. Scratch200 diagnostic buffers L0/L100.
   Mode98f928 is the pending second argument of 945b00, not a third
   argument of 90bde0. Keep signedBYTE counts, fixed allocation78h,
   original fread argument order and the original failure cleanup. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_0090bde0(u32, u32);
extern u32 CM_CDECL cm_fn_00945b00(u32, u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00905230(u32 self)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch, value, stream, count, offset, id, base, pointer;
    u32 name, extension, line;
    u8 index;
    value = cm_fn_0090bde0(0x00987da0, 0x00aa1a50);
    stream = cm_fn_00945b00(value, 0x0098f928);
    if (stream == 0) return 0;
    value = cm_fn_00945bfe(self + 0x438, 1, 1, stream);
    if (value != 1) {
        name = l + 0x100; extension = l; line = 0x5f4;
        goto diagnostic;
    }
    count = (u32)(s32)(s8)B(self + 0x438);
    value = cm_fn_00945bfe(self, 0x12, count, stream);
    count = (u32)(s32)(s8)B(self + 0x438);
    if (value != count) {
        name = l; extension = l + 0x100; line = 0x5fd;
        goto diagnostic;
    }
    value = cm_fn_00945bfe(self + 0x439, 2, 1, stream);
    if (value != 1) {
        name = l; extension = l + 0x100; line = 0x605;
        goto diagnostic;
    }
    index = 0;
    if ((s8)B(self + 0x438) > 0) {
        do {
            offset = (u32)(s32)(s8)index * 18u;
            id = D(self + offset);
            base = D(0x00ae23a8);
            ++index;
            pointer = base + id * 0x122u;
            D(self + offset + 4) = pointer;
            count = B(self + 0x438);
        } while ((s8)index < (s8)(u8)count);
    }
    value = cm_fn_00945bfe(self + 0x43f, 1, 1, stream);
    if (value != 1) {
        name = l; extension = l + 0x100; line = 0x613;
        goto diagnostic;
    }
    if (B(self + 0x43f) != 0) {
        pointer = D(self + 0x43b);
        if (pointer != 0) cm_fn_009452ca(pointer);
        pointer = (u32)cm_fn_00944e46(0x78);
        D(self + 0x43b) = pointer;
        if (pointer == 0) {
            name = l; extension = l + 0x100; line = 0x623;
            goto diagnostic;
        }
        count = (u32)(s32)(s8)B(self + 0x43f);
        value = cm_fn_00945bfe(pointer, 4, count, stream);
        count = (u32)(s32)(s8)B(self + 0x43f);
        if (value != count) {
            name = l; extension = l + 0x100; line = 0x62b;
            goto diagnostic;
        }
    }
    value = cm_fn_00945bfe(self + 0x440, 0xc, 0x32, stream);
    if (value != 0x32) {
        name = l; extension = l + 0x100; line = 0x634;
        goto diagnostic;
    }
    cm_fn_00945a09(stream);
    return 1;
diagnostic:
    cm_fn_00944cff(0x00aa1a1c, 0, 0, name, extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
