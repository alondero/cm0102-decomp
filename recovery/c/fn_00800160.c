#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00564080(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00800160(u32 self, u16 argument)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 buffer = self + 0x37fe;
    u32 position, capacity, data, value, context, text;
    D(self + 0x3802) = 0;
    position = D(buffer + 4);
    capacity = D(buffer + 8);
    if (position + 1u > capacity) {
        cm_fn_00944cff(0x009c3734, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x85);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        data = D(buffer);
        B(data + position) = 0x13;
        position = D(buffer + 4);
        D(buffer + 4) = position + 1u;
    }
    position = D(buffer + 4);
    capacity = D(buffer + 8);
    if (position + 2u > capacity) {
        cm_fn_00944cff(0x009c3734, 0, 0, local, local + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local, local + 0x100, 0x86);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        data = D(buffer);
        W(data + position) = argument;
        position = D(buffer + 4);
        D(buffer + 4) = position + 2u;
    }
    value = (u32)(s32)(s16)W(self + 0x382a) * 768u;
    value = (value & 0xffff0000u) | W(value + self + 0x7d4);
    if ((u16)value == 0) {
        context = D(self + 0x3840);
        return cm_fn_00564080(self + 0x3844, context, 0, buffer);
    }
    position = D(self + 0x3802);
    data = D(buffer);
    context = D(self + 0x3840);
    value = cm_fn_0077ac30(context, data, position, value);
    if (value == 0) {
        cm_fn_00944cff(0x00a7ddbc, 0, 0, local, local + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local, local + 0x100, 0x9df);
        value = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    return value;
}
