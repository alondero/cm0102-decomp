/* Preferred-image VA 00947B2E, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1 terminal diagnostic wrapper. The argument home is read after the
   optional original callback path. Original code and the ExitProcess import
   remain explicit boundaries; no host termination behavior is implemented. */
#include "cmre.h"
extern void CM_CDECL cm_fn_0094eaab(void);
extern void CM_CDECL cm_fn_0094eae4(u32);
typedef void (CM_STDCALL *exit_process_00947b2e)(u32) __attribute__((noreturn));
void CM_CDECL __attribute__((noreturn))
cm_fn_00947b2e(volatile u32 diagnostic_home)
{
    if (CM_GLOBAL(volatile u32,0x00de3000u)==1u)
        cm_fn_0094eaab();
    cm_fn_0094eae4(diagnostic_home);
    ((exit_process_00947b2e)CM_GLOBAL(volatile u32,0x00967164u))(0xffu);
}
