#include "cmre.h"

extern u32 *cm_fn_0090bde0(u32 param_1, u32 param_2);
extern u8 cm_fn_0090c120(u32 param_1);

/* Cdecl with one stack argument (ret 0), returning a byte. The argument selects one of
   several string constants (0 and 1 and 11 and 12 use the same one). For each selected
   constant the routine calls 0x0090bde0 with the format global 0x00987da0 and the string,
   and returns the byte from 0x0090c120 on that result. Any other argument returns 0. */
u32 CM_CDECL cm_fn_00535930(u32 param_1)
{
    u32 str;
    u32 *p;

    if (param_1 == 0 || param_1 == 1) {
        str = 0x009b632cU;
    } else if (param_1 == 2) {
        str = 0x009b633cU;
    } else if (param_1 == 3) {
        str = 0x009b6334U;
    } else if (param_1 == 4) {
        str = 0x009b6324U;
    } else if (param_1 == 5) {
        str = 0x009b630cU;
    } else if (param_1 == 6) {
        str = 0x009b62fcU;
    } else if (param_1 == 7) {
        str = 0x009b6344U;
    } else if (param_1 == 8) {
        str = 0x009b6314U;
    } else if (param_1 == 9) {
        str = 0x009b631cU;
    } else if (param_1 == 10) {
        str = 0x009b6304U;
    } else if (param_1 == 11 || param_1 == 12) {
        str = 0x009b632cU;
    } else {
        return 0;
    }
    p = cm_fn_0090bde0(0x00987da0U, str);
    return cm_fn_0090c120((u32)p);
}
