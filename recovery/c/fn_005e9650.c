/* WIP: coverage 0.4452; real virtual target005e8bf0 absent from inventory. */
#include "cmre.h"

/* Preferred-image VA 0x005e9650, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0068b100 reads stack slots 7/13/15/19 as WORD and 8/9/10/14/16/17/22
   as BYTE. Unknown upper register bits on those pushes are not initialized. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern void CM_FASTCALL cm_fn_00684230(u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32 self, u32 a1, u16 a2, u32 a3,
    u16 a4, u32 a5, u32 a6, u16 a7, u8 a8, u8 a9, u8 a10,
    u32 a11, u32 a12, u16 a13, u8 a14, u16 a15, u8 a16, u8 a17,
    u16 a18, u16 a19, u16 a20, u32 a21, u8 a22);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
typedef u32 (CM_THISCALL *StageCall)(u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_005e9650(u32 self, u32 index)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x414];
    u32 local = (u32)scratch, vtable, result, clubs, offset, target, base, value;
    u32 object, i, line, name, extension, text, a6, a7, a15, owner;
    u16 a13, a19;
    u8 a8, a9, a10, a16;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095a98e;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    vtable = D(self);
    B(local) = 2;
    B(local + 1) = 0;
    B(local + 2) = 0;
    B(local + 3) = 0;
    target = D(vtable + 0x3c);
    result = ((StageCall)target)(self, index, local + 0xc, local + 8, 0);
    D(local + 4) = result;
    clubs = cm_fn_00944e46(16);
    if (clubs == 0) {
        name = local + 0x14; extension = local + 0x214; line = 0x271;
        goto error;
    }
    offset = index * 4u + 4u;
    for (i = 0; i < 4; ++i) {
        base = D(self + 0xee);
        offset += 0x2c;
        value = D(base + offset - 0x2c);
        D(clubs + i * 4u) = value;
    }
    object = cm_fn_00944cf1(0xee);
    D(local + 0x10) = object;
    frame.state = 0;
    if (object != 0) {
        a19 = W(self + 0xd9);
        a16 = B(self + 0x51);
        a15 = D(local + 8);
        a13 = W(self + 0x40);
        a10 = B(self + 0xc4);
        a9 = B(self + 0xc3);
        a8 = B(self + 0xc2);
        a7 = D(local + 0xc);
        a6 = D(local + 4);
        owner = D(self + 4);
        result = cm_fn_0068b100(object, self, 4, clubs, 1, owner, a6, (u16)a7,
                     a8, a9, a10, self + 0xc5, local, a13, (u8)index,
                     (u16)a15, a16, 1, 0, a19, 0xffff, 0, 2);
    } else result = 0;
    base = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(base + index * 4u) = result;
    cm_fn_009452ca(clubs);
    value = D(local + 4);
    cm_fn_009452ca(value);
    base = D(self + 0xc);
    object = D(base + index * 4u);
    if (object != 0) {
        cm_fn_00684230(object);
        D(self + 0x30) = index;
        result = 1;
        goto done;
    }
    name = local + 0x314; extension = local + 0x114; line = 0x286;
error:
    cm_fn_00944cff(0x009cdab0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = 0;
done:
    value = frame.prev;
    CM_FS(u32, 0) = value;
    return result;
}
