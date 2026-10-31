#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fe4a0(u32, u32, u32, u32, u32, u32, u32);
/* Keep the original setter's full incidental EAX for the final call. */
extern u32 CM_THISCALL cm_fn_007ff130(u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_00788010(u32 view, u32 argument)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 result, allocation, text, index;
    result = cm_fn_007fe4a0(0x00b74450, view, 0x007881e0, 0x007892b0,
                            0, 0x007897a0, 0);
    if (result == 0)
        return result;
    allocation = (u32)cm_fn_00944cf1(30);
    if (allocation == 0) {
        cm_fn_00944cff(0x00a694ac, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x66);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    for (index = 0; index != 30; ++index)
        B(allocation + index) = index == 29 ? 0 : 0x20;
    cm_fn_00944c9f(allocation, 0x00a6949c);
    cm_fn_007ff130(0x00b74450, view, 0, argument, 0);
    for (index = 1; index != 8; ++index)
        cm_fn_007ff130(0x00b74450, view, index, 0, 0);
    cm_fn_007ff130(0x00b74450, view, 8, allocation, 1);
    index = 11;
    do {
        cm_fn_007ff130(0x00b74450, view, index - 2, 0, 0);
        cm_fn_007ff130(0x00b74450, view, index - 1, 0, 0);
        cm_fn_007ff130(0x00b74450, view, index, 0, 0);
        index += 3;
    } while (index <= 32);
    return cm_fn_007ff130(0x00b74450, view, 59, 0, 0);
}
