#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Each of the eight direct callers consumes only AL. */
u8 CM_THISCALL cm_fn_00868020(u32 self, volatile u32 output, s32 size)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 text;
    u8 first, second;
    if (size < 0x14) {
        cm_fn_00944cff(0x00a8360c, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x100, local, 0x2178);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    first = B(self + 0x1d);
    if (first != 0) {
        if (first != 1)
            B(self + 0x1d) = 1;
        cm_fn_0066f4e0(output, 0x00a83b34, 0x00ad9c64);
        return B(self + 0x1d);
    }
    first = B(self + 0x1c);
    if (first != 0) {
        second = B(self + 0x1f);
        if (second != 0) {
            if (first == 2) {
                cm_fn_0066f4e0(output, 0x00a83aec, 0x00ad9c64);
                return 2;
            }
            if (second == 2) {
                cm_fn_0066f4e0(output, 0x00a83aac, 0x00ad9c64);
                return 2;
            }
            cm_fn_0066f4e0(output, 0x00a83a54, 0x00ad9c64);
            return 1;
        }
        cm_fn_0066f4e0(output, 0x00a83aec, 0x00ad9c64);
        return B(self + 0x1c);
    }
    first = B(self + 0x1f);
    if (first != 0) {
        cm_fn_0066f4e0(output, 0x00a83aac, 0x00ad9c64);
        return B(self + 0x1f);
    }
    first = B(self + 0x20);
    if (first != 0) {
        cm_fn_0066f4e0(output, 0x00a83a10, 0x00ad9c64);
        return B(self + 0x20);
    }
    cm_fn_00944c9f(output, 0x0098ba0c);
    return 0;
}
