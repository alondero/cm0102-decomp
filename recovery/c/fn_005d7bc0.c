#include "cmre.h"

extern u8 CM_CDECL cm_fn_007a1750(void);
extern s32 CM_CDECL cm_fn_007a16d0(void);
extern u32 CM_THISCALL cm_fn_005d7c60(u8 *self, u32 mask);
extern u8 CM_CDECL cm_fn_0084c7f0(void);

u32 CM_THISCALL cm_fn_005d7bc0(u8 *self)
{
    u8 mode = cm_fn_007a1750();
    s32 value = cm_fn_007a16d0();
    u32 mask;

    *(volatile u32 *)(self + 4) = 0x212552a9u;

    if (mode == 2 || value < 0x3f)
        mask = 2;
    else
        mask = 1;
    (void)cm_fn_005d7c60(self, mask);

    if (mode == 0) {
        (void)cm_fn_005d7c60(self, 0x20000u);
        (void)cm_fn_005d7c60(self, 0x40000u);
    } else {
        (void)cm_fn_005d7c60(self, 0x10000u);
        if (mode == 2) {
            (void)cm_fn_005d7c60(self, 0x80000u);
        } else {
            (void)cm_fn_005d7c60(self, 0x40000u);
        }
    }

    if (cm_fn_0084c7f0() != 0)
        (void)cm_fn_005d7c60(self, 0x1000000u);
    else
        (void)cm_fn_005d7c60(self, 0x2000000u);
    return 1;
}
