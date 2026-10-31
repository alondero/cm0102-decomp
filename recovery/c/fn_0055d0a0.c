#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_STDCALL cm_fn_0055d0a0(u32 record, u32 output, u32 size)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 text, result;
    u16 count;
    u8 kind;
    if ((s32)size < 15) {
        cm_fn_00944cff(0x009b8f18, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x150e);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    kind = B(record + 0x10);
    if (kind == 5)
        return cm_fn_0066f4e0(output, 0x009c3568);
    if (kind == 6)
        return cm_fn_0066f4e0(output, 0x009c3554);
    if (kind == 0) {
        count = W(record + 0x11);
        return cm_fn_0066f4e0(output, 0x009c3534, (u32)(s32)(s16)count);
    }
    if (kind == 1) {
        count = W(record + 0x11);
        if (count == 1)
            return cm_fn_0066f4e0(output, 0x009c3528);
        return cm_fn_0066f4e0(output, 0x009c3504, (u32)(s32)(s16)count);
    }
    if (kind == 4) {
        count = W(record + 0x11);
        return cm_fn_0066f4e0(output, 0x009c34d4, (u32)(s32)(s16)count);
    }
    if (kind == 2) {
        count = W(record + 0x11);
        if (count == 1)
            return cm_fn_0066f4e0(output, 0x009c34c8);
        return cm_fn_0066f4e0(output, 0x009c34a8, (u32)(s32)(s16)count);
    }
    if (kind == 3)
        return cm_fn_0066f4e0(output, 0x009c349c);
    return cm_fn_0066f4e0(output, 0x009c3480);
}
