#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_00945934(u32 param_1);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

u32 cm_fn_0054da00(u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u8 scratch[0x200];
    u32 element_size;
    u32 result;

    element_size = *(volatile u32 *)(arg4 + 0x37u);
    if (*(volatile u32 *)(arg4 + 0x3fu) != 1u) {
        (void)cm_fn_00944cff(0x009b8dc0u, 0u, 0u,
                             scratch + 0x100u, scratch);
        result = (u32)(unsigned long)cm_fn_0090d130();
        (void)cm_fn_00944c9f((u32)(unsigned long)&CM_GLOBAL(u8, 0x00ae24d0),
                             0x009870e8u, result,
                             (u32)(unsigned long)(scratch + 0x100u),
                             (u32)(unsigned long)scratch, 0x23du);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)&CM_GLOBAL(u8, 0x00ae24d0), 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }
    if (arg2 != 0u) {
        result = cm_fn_00932c30(arg3, 0x4eu, element_size, arg1);
        return (u32)(result == element_size);
    }
    result = cm_fn_00945bfe(arg3, 0x4eu, element_size, arg1);
    return (u32)(result == element_size);
}
