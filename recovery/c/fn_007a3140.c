#include "cmre.h"

/* The string compare 0x00946fa0 is a cdecl (str1, str2, max) call. Each step
   passes arg_1 as str1, a fixed string address from the 0x00a6b2f8..0x00a6b3a0
   table as str2, and max 0x64. A zero result returns the step's constant; any
   other result falls through to the next step. The final step returns 0x40 on a
   match and 0 otherwise (neg/sbb/and/add sequence). */

extern s32 CM_CDECL cm_fn_00946fa0(u32 str1, u32 str2, u32 max);

u32 CM_CDECL cm_fn_007a3140(u32 arg_1)
{
    s32 r;

    r = cm_fn_00946fa0(arg_1, 0x00a6b3a0u, 0x64u);
    if (r == 0) {
        return 1u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b390u, 0x64u);
    if (r == 0) {
        return 2u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b384u, 0x64u);
    if (r == 0) {
        return 0x200u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b374u, 0x64u);
    if (r == 0) {
        return 4u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b364u, 0x64u);
    if (r == 0) {
        return 8u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b35cu, 0x64u);
    if (r == 0) {
        return 0x10u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b350u, 0x64u);
    if (r == 0) {
        return 0x100u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b348u, 0x64u);
    if (r == 0) {
        return 0x100u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b33cu, 0x64u);
    if (r == 0) {
        return 0x1000u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b32cu, 0x64u);
    if (r == 0) {
        return 0x800u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b324u, 0x64u);
    if (r == 0) {
        return 0x20u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b314u, 0x64u);
    if (r == 0) {
        return 0x2000u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b308u, 0x64u);
    if (r == 0) {
        return 0x400u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b300u, 0x64u);
    if (r == 0) {
        return 0x80u;
    }
    r = cm_fn_00946fa0(arg_1, 0x00a6b2f8u, 0x64u);
    if (r == 0) {
        return 0x40u;
    }
    return 0u;
}
