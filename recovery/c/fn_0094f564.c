/* Preferred-image VA0094F564, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0 terminal wrapper. The indexed "call"0094A5DD is actually a JMP,
   verified from pinned bytes. Native callees and their signal/import effects
   remain boundaries.00945945 calls00945956(code,1,0), reaching the original
   terminal ExitProcess import; no replacement termination logic is supplied. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094eae4(u32);
extern u32 CM_CDECL cm_fn_00951e15(u32);
extern void CM_CDECL cm_fn_00945945(u32) __attribute__((noreturn));
void CM_CDECL __attribute__((noreturn)) cm_fn_0094f564(void)
{
    cm_fn_0094eae4(10);
    cm_fn_00951e15(22);
    cm_fn_00945945(3);
}
