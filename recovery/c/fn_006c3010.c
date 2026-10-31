#include "cmre.h"

/* Thiscall with three stack arguments (ret 0x0c). ECX is the object pointer
   (self, copied to esi). arg1 (edi) is the first stack argument, arg2 and
   arg3 are the second and third. Returns the low byte (al): 0 or 1.

   Stack slots reassigned by the original: the dword at arg3 is overwritten by
   the float of the first call result (fst at 0x006c3034); the dword at arg1
   is overwritten inside the loop by the float of the second call result
   (fst at 0x006c308e) and reloaded at 0x006c30aa. Both are modelled as locals
   because no other original instruction reads those slots. */

extern long double CM_THISCALL cm_fn_006f09b0(u32 self, u32 arg1, u32 arg2);
extern u8 CM_THISCALL cm_fn_00714d30(volatile u8 *self);
extern u32 CM_FASTCALL cm_fn_00717410(u32 self);

u8 CM_THISCALL cm_fn_006c3010(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    s8 cVar1;
    u8 cVar3;
    u32 rec;
    long double fVar5;
    long double fVar6;
    float slot1;
    float slot3;
    int ordered_nonequal;

    /* Receiver for both calls to 0x006f09b0 at 0x006c302d and 0x006c3087:
       this = arg1 for the first call, this = rec for the loop call. */
    cVar1 = (s8)cm_fn_00714d30((volatile u8 *)arg1);
    fVar5 = cm_fn_006f09b0(arg1, arg2, arg3);
    slot3 = (float)fVar5;
    /* fcomp against 0.0 sets C3 for equal or unordered; a jump on ZF=1
       (test ah,0x40) means ordered and not equal. */
    ordered_nonequal = (fVar5 < 0.0L) || (fVar5 > 0.0L);
    if (ordered_nonequal || (u16)cm_fn_00717410(arg1) == 1) {
        cVar3 = 0;
        rec = self + 0x4796u + (u32)((s32)cVar1 * 0x22d8);
        do {
            if (((*(volatile s8 *)(rec + 0x19u)) >= 0) && (arg1 != rec) &&
                (*(volatile u8 *)(rec + 0x2bu) == 0)) {
                fVar6 = cm_fn_006f09b0(rec, arg2, arg3);
                slot1 = (float)fVar6;
                /* test ah,1 after fcomp: st0 below slot3, or unordered. */
                if (!(fVar6 >= (long double)slot3)) {
                    return 0;
                }
                if ((u16)cm_fn_00717410(arg1) == 1) {
                    /* fld slot1; fcomp slot3; C3 set (equal or unordered). */
                    if (!(((long double)slot1 < (long double)slot3) ||
                          ((long double)slot1 > (long double)slot3))) {
                        return 0;
                    }
                }
            }
            cVar3 = (u8)(cVar3 + 1);
            rec += 0x1beu;
        } while (cVar3 < 0x14);
    }
    return 1;
}
