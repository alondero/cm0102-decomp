#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern s32 CM_CDECL cm_fn_00932e10(u32 *slot, s32 width, s32 mode,
                                    u32 value);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

void CM_THISCALL cm_fn_005b0170(volatile u32 *self, u32 input)
{
    volatile u8 local_area[0x204];
    volatile u32 *local_value = (volatile u32 *)&local_area[0];
    volatile u8 *buffer_a = &local_area[4];
    volatile u8 *buffer_b = &local_area[0x104];
    u32 first_base = *(volatile u32 *)(self + 1);
    u32 initial = *(volatile u32 *)first_base;
    *local_value = initial;
    s32 result = cm_fn_00932e10((u32 *)local_value, 4, 1, input);
    if (result == 1) {
        u32 global_value = CM_GLOBAL(volatile u32, 0x00ae2364u);
        u32 base = *self;
        result = cm_fn_00932e10((u32 *)base, 9, (s32)global_value, input);
        global_value = CM_GLOBAL(volatile u32, 0x00ae2364u);
        if ((u32)result == global_value) {
            return;
        }

        cm_fn_00944cff(0x009cc27cu, 0u, 0u,
                       (void *)buffer_a, (void *)buffer_b);
        {
            u32 error_text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                           (void *)buffer_a, (void *)buffer_b, 0x25bu);
        }
    } else {
        cm_fn_00944cff(0x009cc27cu, 0u, 0u,
                       (void *)buffer_b, (void *)buffer_a);
        {
            u32 error_text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                           (void *)buffer_b, (void *)buffer_a, 0x254u);
        }
    }
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
}
