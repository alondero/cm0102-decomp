#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_0048d410(volatile u8 *self, s32 index, s32 arg2)
{
    volatile u8 local_area[0x200];
    u32 object;
    s8 child_value;
    s32 upper = *(volatile s32 *)(self + 0x30);

    if (index > upper) {
        return 0;
    } else if (index < -1) {
        u32 error_text;
        cm_fn_00944cff(0x0099b80cu, 0u, 0u,
                       (void *)&local_area[0x100], (void *)&local_area[0]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x100], (void *)&local_area[0],
                       0x36eu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        object = 0;
    } else if (index == -1) {
        object = (u32)self;
    } else {
        u32 array = *(volatile u32 *)(self + 0x0c);
        object = *(volatile u32 *)(array + (u32)index * 4u);
    }

    if (*(volatile u8 *)(object + 0x43u) == 2u) {
        return 1;
    }
    {
        child_value = *(volatile s8 *)(object + 0xabu);
        if (arg2 < (s32)child_value) {
            return 1;
        }
        if (arg2 != (s32)child_value) {
            return 0;
        }
    }
    {
        s16 count = *(volatile s16 *)(object + 0x3cu);
        s32 signed_count = (s32)count;
        if ((s32)child_value != signed_count - 1) {
            return 0;
        }
        {
            u32 table = *(volatile u32 *)(object + 0xa3u);
            u32 table_address = table + (u32)signed_count * 104u - 0x61u;
            return *(volatile u16 *)table_address == 0x8cu;
        }
    }
}
