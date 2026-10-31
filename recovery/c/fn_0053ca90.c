#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b3d0(u32 arg1);
extern u32 CM_CDECL cm_fn_00540a50(u32 arg1, u32 arg2, u32 arg3);
extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

u32 CM_CDECL cm_fn_0053ca90(volatile u8 *self)
{
    volatile u8 local_area[0x200];
    u32 current = (u32)self;

    while (current != 0) {
        u32 field = *(volatile u32 *)(current + 0xcfu);
        if (field != 0) {
            return *(volatile u32 *)(current + 0xcfu);
        }

        {
            u32 candidate = cm_fn_00540a50(current, 0u, 1u);
            if (candidate != 0) {
                u32 candidate_field = *(volatile u32 *)(candidate + 0xcf);
                if (candidate_field != 0) {
                    return *(volatile u32 *)(candidate + 0xcf);
                }
            }
        }

        {
            u32 upper = CM_GLOBAL(volatile u32, 0x00ae235cu);
            u32 lower = CM_GLOBAL(volatile u32, 0x00ae2350u);
            s32 value = *(volatile s32 *)current;
            u32 threshold = upper - lower;
            if (value < (s32)threshold) {
                return 0;
            }
        }

        {
            u32 key = *(volatile u32 *)(current + 0x53u);
            u32 next = cm_fn_0053b3d0(key);
            if (next == 0) {
                break;
            }
            current = next;
        }
    }

    {
        u32 error_text;
        cm_fn_00944cff(0x009b5ac8u, 0u, 0u,
                       (void *)&local_area[0x100], (void *)&local_area[0]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x100], (void *)&local_area[0],
                       0x394au);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    }
    return 0;
}
