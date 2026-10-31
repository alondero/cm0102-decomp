#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053dba0(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_00806260(u32 param_1, u32 param_2, u32 param_3);

/* Cdecl with two stack arguments (arg_04, arg_08), `ret 0`, returns u32 (0 or 1).
   Returns 0 when either argument is zero. Otherwise seven rounds k = 1..7: call
   0x53dba0(arg_04, 0xdd6bfc, 0xc8, k) and then 0x806260(0xdd6bfc, arg_08, 0);
   the first round whose second call returns non-zero makes the routine return 1.
   Both callees are cdecl; the caller removes the 28 pushed bytes of each round. */
u32 CM_CDECL cm_fn_00806080(u32 arg_04, u32 arg_08)
{
    u32 k;

    if (arg_04 == 0 || arg_08 == 0)
        return 0;

    for (k = 1; k <= 7; k++) {
        cm_fn_0053dba0(arg_04, 0x00dd6bfcu, 0xc8u, k);
        if (cm_fn_00806260(0x00dd6bfcu, arg_08, 0u) != 0)
            return 1;
    }
    return 0;
}
