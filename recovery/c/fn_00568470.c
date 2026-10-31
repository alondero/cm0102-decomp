#include "cmre.h"

extern u32 CM_STDCALL cm_fn_00568290(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 arg1, u32 arg2,
                                    u32 arg3);
extern u32 CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 arg1, u32 arg2, ...);

u32 CM_THISCALL cm_fn_00568470(u32 self, u32 arg1)
{
    volatile u8 local_area[0x200];
    u32 field = *(volatile u32 *)(self + 0x12f505u);

    if (field == 0) {
        u32 year;
        *(volatile u32 *)(self + 0x12f555u) = 1u;
        year = CM_GLOBAL(volatile u16, 0x00ae2914u);
        return cm_fn_00568290(arg1, year, 0u);
    }

    {
        u32 year = (self & 0xffff0000u)
                   | CM_GLOBAL(volatile u16, 0x00ae2914u);
        u32 arg_b = CM_GLOBAL(volatile u32, 0x00ae28fcu);
        u32 arg_a = CM_GLOBAL(volatile u32, 0x00ae28f8u);
        u32 result = cm_fn_0077ac30(arg1, arg_a, arg_b, year);
        if (result != 0) {
            return result;
        }
    }

    {
        u32 error_text;
        cm_fn_00944cff(0x009c36ecu, 0u, 0u,
                       (void *)&local_area[0x100], (void *)&local_area[0]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x100], (void *)&local_area[0],
                       0xc71u);
        cm_fn_0066f4e0(0x00ae28bcu, 0x009c3814u);
        cm_fn_0066f4e0(0x00ae26c4u, 0x009c37b0u, 0x00ae24d0u);
        return cm_fn_005e8290((const char *)0x00ae28bcu,
                              (const char *)0x00ae26c4u, 0);
    }
}
