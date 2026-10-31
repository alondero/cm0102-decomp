#include "cmre.h"

extern void CM_THISCALL cm_fn_00687430(u32 self, u32 local_value,
                                       u32 entry, u32 zero1, u32 zero2);
extern void CM_THISCALL cm_fn_00684230(u32 self);
extern void cm_fn_005e8290(const char *message, const char *buffer, int mode);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *buffer1, void *buffer2);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_004019c0(u32 self)
{
    u8 local_area[0x204];
    u32 result;

    *(volatile u16 *)(self + 0x3eu) = 4u;
    result = cm_fn_00944e46(0xecu);
    *(volatile u32 *)(self + 0xb1u) = result;
    if (result == 0u) {
        (void)cm_fn_00944cff(0x009870f8u, 0u, 0u,
                             local_area + 0x104, local_area + 4);
        result = (u32)(unsigned long)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result,
                             (u32)(unsigned long)(local_area + 0x104),
                             (u32)(unsigned long)(local_area + 4), 0x1f1u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0u;
    }

    {
        volatile u8 *index_byte = local_area;
        *index_byte = 0u;
        if (*(volatile s16 *)(self + 0x3eu) > 0) {
            do {
                u32 table = *(volatile u32 *)(self + 0xeeu);
                s8 signed_index = (s8)*index_byte;
                u32 entry = *(volatile u32 *)(table + (u32)(s32)signed_index * 4u);
                u32 local_value = *(volatile u32 *)local_area;
                cm_fn_00687430(self, local_value, entry, 0u, 0u);
                *index_byte = (u8)(*index_byte + 1u);
            } while ((s16)(s8)*index_byte < *(volatile s16 *)(self + 0x3eu));
        }
    }
    cm_fn_00684230(self);
    return 1u;
}
