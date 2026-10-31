#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 slot,
                                       u32 value, u32 arg3);
extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

void CM_CDECL cm_fn_00472900(volatile u8 *input, u32 arg2)
{
    volatile u8 local_area[0x200];

    if (input == 0) {
        u32 error_text;
        cm_fn_00944cff(0x0098e598u, 0u, 0u,
                       (void *)&local_area[0x100], (void *)&local_area[0]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x100], (void *)&local_area[0],
                       0x347du);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return;
    }

    if (cm_fn_007fe600(0x00b74450u, 0x00472a00u, 0x004735f0u,
                       1u, 0u, 0u) != 0) {
        u32 allocation = cm_fn_00944cf1(0xd1u);
        volatile u32 *source = (volatile u32 *)input;
        volatile u32 *destination = (volatile u32 *)allocation;
        u32 index;
        for (index = 0; index != 0x34u; ++index) {
            u32 value = source[index];
            destination[index] = value;
        }
        {
            u8 final_byte = *(volatile u8 *)(input + 0xd0u);
            *(volatile u8 *)(allocation + 0xd0u) = final_byte;
        }
        cm_fn_007ff270(0x00b74450u, 0u, (u32)input, 0u);
        cm_fn_007ff270(0x00b74450u, 1u, arg2, 0u);
        cm_fn_007ff270(0x00b74450u, 2u, allocation, 0u);
    }
}
