#include "cmre.h"

extern u32 CM_STDCALL cm_fn_004c6280(u32 *output);
extern void CM_THISCALL cm_fn_004c6430(u32 self, u32 arg2, u32 arg3);
extern void cm_fn_005e8290(const char *arg1, const char *arg2, int arg3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_004c61b0(u32 self, u32 trigger)
{
    u8 local_area[0x204];
    u32 *local_output = (u32 *)local_area;
    u32 buffer_a = (u32)local_area + 4u;
    u32 buffer_b = (u32)local_area + 0x104u;

    if (trigger != 0)
        return;

    u16 global_value = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    u16 previous_value = CM_GLOBAL(volatile u16, self + 0x6fu);
    if (global_value == previous_value)
        return;

    CM_GLOBAL(volatile u16, self + 0x6fu) = global_value;
    u32 status = cm_fn_004c6280(local_output);
    if ((u16)status == 0) {
        cm_fn_00944cff(0x009a7ed8u, 0u, 0u, (void *)buffer_b,
                       (void *)buffer_a);
        u32 format_string = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, format_string,
                             buffer_b, buffer_a, 0x9bu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return;
    }

    u32 output_value = CM_GLOBAL(volatile u32, (u32)local_output);
    cm_fn_004c6430(self, output_value, status);
    output_value = CM_GLOBAL(volatile u32, (u32)local_output);
    (void)cm_fn_009452ca(output_value);
}
