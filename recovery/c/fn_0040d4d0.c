#include "cmre.h"

extern void cm_fn_005e8290(const char *message, const char *buffer, int mode);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *buffer1, void *buffer2);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_0040d4d0(u32 self)
{
    u8 scratch[0x200];
    u32 result;
    u32 index;

    *(volatile u16 *)(self + 0x3eu) = 0x10u;
    result = cm_fn_00944e46(0x60u);
    *(volatile u32 *)(self + 0xa7u) = result;
    if (result == 0u) {
        (void)cm_fn_00944cff(0x00987b60u, 0u, 0u,
                             scratch + 0x100, scratch);
        result = (u32)(unsigned long)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result,
                             (u32)(unsigned long)(scratch + 0x100),
                             (u32)(unsigned long)scratch, 0x185u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0xffffffffu;
    }

    index = 0u;
    if ((s16)*(volatile u16 *)(self + 0x3eu) > 0) {
        do {
            s16 signed_index = (s16)index;
            u32 source = *(volatile u32 *)(self + 0xb2u);
            u32 output = *(volatile u32 *)(self + 0xa7u);
            u32 source_value = *(volatile u32 *)(source + (u32)(s32)signed_index * 4u);
            u32 output_offset = (u32)(s32)signed_index * 6u;
            *(volatile u32 *)(output + output_offset) = source_value;
            output = *(volatile u32 *)(self + 0xa7u);
            *(volatile u8 *)(output + output_offset + 4u) = 0u;
            output = *(volatile u32 *)(self + 0xa7u);
            *(volatile u8 *)(output + output_offset + 5u) = 0u;
            ++index;
        } while ((s16)index < *(volatile s16 *)(self + 0x3eu));
    }
    return 1u;
}
