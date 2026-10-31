#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e8290(const char *format, const char *message, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932e10(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_009314e0(u8 *self, u32 arg)
{
    u32 scratch[0x80];
    u32 *id_slot = (u32 *)(self + 4);
    u32 check = cm_fn_00932e10((u32)id_slot, 4, 1, arg);

    if (check == 1) {
        u32 pointed_id = *(volatile u32 *)id_slot;
        if (pointed_id == 0)
            return 0;

        u32 self_value = *(volatile u32 *)self;
        u32 result = cm_fn_00932e10(self_value, 0x15, pointed_id, arg);
        u32 reloaded_id = *(volatile u32 *)id_slot;
        if (result == reloaded_id)
            return result;

        cm_fn_00944cff(0x00ad6a38u, 0, 0, scratch, scratch + 0x40);
        u32 text = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             text, scratch, scratch + 0x40, 0x8fu);
    } else {
        cm_fn_00944cff(0x00ad6a38u, 0, 0, scratch + 0x40, scratch);
        u32 text = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             text, scratch + 0x40, scratch, 0x86u);
    }

    u32 error_result = cm_fn_005e8290((const char *)0x009870e0,
                                      (const char *)0x00ae24d0, 0);
    *(volatile u32 *)0x00b67a34 = 0;
    return error_result;
}
