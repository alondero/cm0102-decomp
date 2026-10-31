/* Preferred-image VA 0x00717450, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38f0(u32, u32, u32, u32);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00717450(u32 eax, u32 edx, u32 self)
{
    u32 target = *(volatile u32 *)(self + 0x19eu);
    (void)eax;
    if (!target) return 0;
    return cm_fn_006c38f0(self, edx, target, self);
}
