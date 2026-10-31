#include "cmre.h"

extern int CM_CDECL cm_fn_00946fa0(char *_Str1, char *_Str2, u32 _MaxCount);

/* Stdcall with one stack argument (ret 4) returning a byte. The argument string is compared
   with seven constant strings, each limited to 100 characters, in listing order. The first
   equal string selects the code: 1, 7, 0, 3, 5, 4 for the first six; for the last
   (0x00a79464) the result is 2 when equal and 0xff otherwise. */
u8 CM_STDCALL cm_fn_007daa70(char *param_1)
{
    int r;

    r = cm_fn_00946fa0(param_1, (char *)0x00a794a8, 0x64U);
    if (r == 0) {
        return 1;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a79498, 0x64U);
    if (r == 0) {
        return 7;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a79490, 0x64U);
    if (r == 0) {
        return 0;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a79488, 0x64U);
    if (r == 0) {
        return 3;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a7947c, 0x64U);
    if (r == 0) {
        return 5;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a79470, 0x64U);
    if (r == 0) {
        return 4;
    }
    r = cm_fn_00946fa0(param_1, (char *)0x00a79464, 0x64U);
    if (r != 0) {
        return (u8)0xffU;
    }
    return 2;
}
