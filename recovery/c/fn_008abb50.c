#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_008aa9d0(u32, s8, u32, u32);
extern u32 CM_THISCALL cm_fn_008ab480(u32, u32, u32);
extern u8 CM_THISCALL cm_fn_008aba70(u32, u8, u32);
extern u32 CM_THISCALL cm_fn_005b57c0(u32, u32);
extern u32 CM_THISCALL cm_fn_005b58e0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008abb50(u32 self, u32 index, u32 mode)
{
    u8 scratch[0x208];
    u32 value, receiver, row, base, flags, kind, text;
    u8 selected;
    if ((s8)index < 0 || (s8)index > 0x13) {
        cm_fn_00944cff(0x00a8b94c, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x108, (u32)scratch + 8, 0x1f11);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if (D(self + 0xde5) == 0)
        return 0;
    if (cm_fn_008aa9d0(self, (s8)index, (u32)scratch, mode) == 0)
        return 0;
    if (cm_fn_008ab480(self, index, mode) == 0)
        return 0;
    if (mode == 0) {
        value = D((u32)scratch);
        receiver = self + 0xe44;
    } else {
        row = (u32)(s32)(s8)B(self + 0xdea);
        value = D((u32)scratch);
        base = D(self + 4);
        receiver = base + row * 6371u + 0x91e2;
    }
    flags = cm_fn_005b57c0(receiver, value);
    if ((flags & 4u) == 0)
        return 0;
    if ((flags & 8u) != 0) {
        value = D((u32)scratch);
        if (mode == 0) {
            receiver = self + 0xe44;
        } else {
            row = (u32)(s32)(s8)B(self + 0xdea);
            base = D(self + 4);
            receiver = base + row * 6371u + 0x91e2;
        }
        return cm_fn_005b58e0(receiver, value);
    }
    if ((flags & 0x10u) != 0) {
        selected = cm_fn_008aba70(self, (u8)index, mode);
        /* The original initializes only AL's byte in this stack DWORD. */
        B((u32)scratch + 4) = selected;
        if ((s8)selected > -1) {
            kind = D((u32)scratch + 4);
            return cm_fn_008ab480(self, kind, mode == 0);
        }
    }
    return 0;
}
