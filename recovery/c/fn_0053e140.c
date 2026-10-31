#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))

/* The caller supplies an additional -1 after the six consumed arguments. */
extern u32 CM_CDECL cm_fn_0053e2e0(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006005e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00600740(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person)
{
    u8 scratch[0x208];
    u32 first_name, second_name, preferred, object, offset, base, text;
    if (output == 0 || (s16)size < 0x69) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x108, (u32)scratch + 8, 0x3cd7);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if ((s32)first > -1)
        first_name = D(0x00ae23f0) + first * 60u;
    else
        first_name = 0;
    if ((s32)second > -1)
        second_name = D(0x00ae23f4) + second * 60u;
    else
        second_name = 0;
    if ((s32)person > -1) {
        object = D(0x00b63c98);
        if (object != 0) {
            offset = person * 110u;
            base = D(0x00ae23bc);
            if (cm_fn_00600740(object, base + offset, 0) != 0) {
                base = D(0x00ae23bc);
                if (cm_fn_0053e990(base + offset, first, second, third, 0) != 0) {
                    base = D(0x00ae23bc);
                    object = D(0x00b63c98);
                    preferred = cm_fn_006005e0(object, offset + base, 0);
                    goto format;
                }
            }
        }
    }
    if ((s32)third > -1)
        preferred = D(0x00ae23f8) + third * 60u;
    else
        preferred = 0;
format:
    return cm_fn_0053e2e0(first_name, second_name, preferred, output, size, kind, 0xffffffffu);
}
