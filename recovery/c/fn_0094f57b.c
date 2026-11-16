/* Preferred-image VA 0x0094f57b; image base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   RET 0: the caller at 0x0094f5be pops both arguments, so the routine is cdecl
   with two DWORD arguments. 0094f5a4 overwrites the caller's copied second
   argument with the new control word; that stack slot is dead afterwards
   (fn_0094f5b0 pops it), so only the loaded word is observable.
   0094f580 FWAIT; 0094f581 FNSTCW word [ebp-4] saves the live control word;
   0094f584 pushes it (0094f5c6 reads only its low sixteen bits: BL and BH);
   0094f5a8 FLDCW word [ebp+0xc] loads the recomputed control word, whose low
   sixteen bits come from the 0094f658 result.
   Plain C cannot name FNSTCW/FLDCW, so the same word is read and written
   through the control-word field of a saved FPU environment image, which the
   original callees stay boundaries for.
   0094f57e/0094f5a7 save and restore ECX around both calls. Neither callee
   reads its incoming ECX (0094f5c6 and 0094f658 write ECX before reading it)
   and ECX is not a compared return register, so plain C leaves it to them. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_0094f5c6(u32 arg);
extern unsigned long long CM_CDECL cm_fn_0094f658(u32 arg);

u32 CM_CDECL cm_fn_0094f57b(u32 param_1, u32 param_2)
{
    volatile u8 env[512] __attribute__((aligned(16)));
    u32 cw, flags, value, new_cw;

    __builtin_ia32_fxsave((void *)env);            /* 0094f581 FNSTCW [ebp-4] */
    cw = (u32)env[0] | ((u32)env[1] << 8);
    flags = (u32)cm_fn_0094f5c6(cw);                /* 0094f587 */
    value = (flags & ~param_2) | (param_1 & param_2);
    new_cw = (u32)cm_fn_0094f658(value);            /* 0094f59e */
    env[0] = (u8)(new_cw & 0xffu);
    env[1] = (u8)((new_cw >> 8) & 0xffu);
    __builtin_ia32_fxrstor((void *)env);            /* 0094f5a8 FLDCW [ebp+0xc] */
    return value;
}
