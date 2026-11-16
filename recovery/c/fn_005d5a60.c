/* fn_005d5a60, preferred-image VA 0x005d5a60 (image base 0x00400000).
   thiscall leaf: `ret 4` pops one DWORD stack argument, ECX is the receiver.

   Only AL of the argument is tested. A non-zero selector uses the pair
   numerator [ecx+0x20] / denominator [ecx+0x28]; zero uses [ecx+0x10] /
   [ecx+0x18]. The denominator is first compared (FCOMP) against the double
   at 0x9689a0 and only C3 (AH bit 6 after FNSTSW) is tested; when C3 is set
   the routine returns the shared constant double at 0x968998 in ST0.
   Otherwise it loads the numerator and divides by the denominator re-read
   from memory, returning the quotient in ST0.

   C3 after FCOMP means "equal" OR "unordered", so the original also takes the
   constant fallback for a NaN denominator.  Written as ordered relational
   comparisons the branch is taken for exactly that set (equal or unordered)
    This does NOT establish architectural x87 equivalence: FCOMP at 005d5a6b
    and 005d5a84 writes C0/C2/C3, whereas a compiler may lower these C
    comparisons to FCOMI/FCOMIP, which do not write those status bits.
    In particular, the equal fallback must retain C3=1, C2=C0=0 through
    the FLD at 005d5a9a and RET. The implementation below remains unresolved
    for that observable state; a passing semantic gate is insufficient.
    Plain C does not specify these x87 condition codes, and the worker rules
    prohibit inline assembly or code-generation shaping to enforce them.
   Load order, the second denominator read for the division and the single
   shared fallback block follow the listing. */

#include "cmre.h"

long double CM_THISCALL cm_fn_005d5a60(u32 self, u8 selector)
{
    if (selector != 0) {
        double denom = *(volatile double *)(self + 0x28u);
        if (denom < CM_GLOBAL(double, 0x009689a0u) ||
            denom > CM_GLOBAL(double, 0x009689a0u))
            return (long double)*(volatile double *)(self + 0x20u) /
                   (long double)*(volatile double *)(self + 0x28u);
    }
    else {
        double denom = *(volatile double *)(self + 0x18u);
        if (denom < CM_GLOBAL(double, 0x009689a0u) ||
            denom > CM_GLOBAL(double, 0x009689a0u))
            return (long double)*(volatile double *)(self + 0x10u) /
                   (long double)*(volatile double *)(self + 0x18u);
    }
    return (long double)CM_GLOBAL(double, 0x00968998u);
}
