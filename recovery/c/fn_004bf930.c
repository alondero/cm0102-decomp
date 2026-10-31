#include "cmre.h"

extern u32 CM_CDECL cm_fn_004bfa30(u32 field, u32 mixed_arg1,
                                  u32 mixed_arg2, u32 arg2,
                                  u32 arg3, s32 arg4);
extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

u32 CM_CDECL cm_fn_004bf930(volatile u8 *arg1, u32 arg2,
                            volatile u32 *arg3, s32 arg4)
{
    volatile u8 local_area[0x200];

    if (arg1 == 0 || arg3 == 0 || arg4 < 0x67) {
        u32 error_text;
        cm_fn_00944cff(0x0099ec20u, 0u, 0u,
                       (void *)&local_area[0], (void *)&local_area[0x100]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0], (void *)&local_area[0x100],
                       0x2587u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        u32 field = *(volatile u32 *)(arg1 + 0x14u);
        if (field == 0) {
            u32 error_text;
            cm_fn_00944cff(0x0099ec20u, 0u, 0u,
                           (void *)&local_area[0x100], (void *)&local_area[0]);
            error_text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                           (void *)&local_area[0x100], (void *)&local_area[0],
                           0x258fu);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
            return 0;
        }

        {
            u16 word30 = *(volatile u16 *)(arg1 + 0x30u);
            u32 mixed_arg2 = (arg2 & 0xffff0000u) | word30;
            u16 word32 = *(volatile u16 *)(arg1 + 0x32u);
            u32 mixed_arg1 = ((u32)arg1 & 0xffff0000u) | word32;
            return cm_fn_004bfa30(field, mixed_arg1, mixed_arg2,
                                  arg2, (u32)arg3, arg4);
        }
    }
}
