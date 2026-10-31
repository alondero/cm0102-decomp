#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *arg1, const char *arg2, s32 arg3);
extern u32 CM_THISCALL cm_fn_005fe290(u32 self, u32 arg1, u32 arg2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_005fe420(void *self, u32 value)
{
    volatile u8 scratch[0x200];
    u32 base = (u32)&scratch[0];

    if (value == 0u) {
        (void)cm_fn_00944cff(0x009cf11cu, 0u, 0u,
                             (void *)(base + 0x100u), (void *)base);
        {
            char *text = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                                 base + 0x100u, base, 0x6a5u);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    for (u32 index = 0u; index < 0x10u; ++index) {
        s32 date = CM_GLOBAL(volatile s32, 0x00ae2364u);
        u32 signed_index = (u32)(s32)(s8)index;
        u32 x = signed_index + (u32)date - 0x10u;
        u32 row_offset = x * 110u;
        u32 table = CM_GLOBAL(volatile u32, 0x00ae23bcu);
        u8 active = *(volatile u8 *)(table + row_offset + 0x5fu);
        u32 row = table + row_offset;
        if (active != 0u) {
            u32 matched = cm_fn_005fe290((u32)self, value, row);
            if (matched != 0u)
                return 1u;
        }
    }
    return 0u;
}
