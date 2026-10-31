#include "cmre.h"

extern u32 CM_CDECL cm_fn_00947480(u32 string_va);
extern u32 CM_CDECL cm_fn_009478d8(const volatile u8 *value, u8 mode);
extern u32 CM_CDECL cm_fn_0094dbe1(u32 zero, u32 command, u32 *scratch,
                                    u32 final_zero);
extern u32 CM_CDECL cm_fn_0094dd44(u32 zero, u32 environment, u32 *scratch,
                                    u32 final_zero);

u32 CM_CDECL cm_fn_009473ea(s32 parameter)
{
    u32 environment = cm_fn_00947480(0x00971414u);
    if (parameter == 0) {
        if (environment == 0)
            return 0;
        return cm_fn_009478d8((const volatile u8 *)(u32)environment, 0) == 0
                   ? 1u
                   : 0u;
    }

    volatile u32 local[4];
    local[0] = environment;
    local[1] = 0x00971410u;
    local[2] = (u32)parameter;
    local[3] = 0;

    if (environment != 0) {
        u32 result = cm_fn_0094dd44(0, environment, (u32 *)&local[0], 0);
        if (result != 0xffffffffu)
            return result;
        u32 error = CM_GLOBAL(volatile u32, 0x00de2f8cu);
        if (error != 2u && error != 0x0du)
            return 0xffffffffu;
    }

    u32 command = (CM_GLOBAL(volatile u8, 0x00de2f99u) & 0x80u) != 0
                      ? 0x00971404u
                      : 0x009713fcu;
    local[0] = command;
    return cm_fn_0094dbe1(0, command, (u32 *)&local[0], 0);
}
