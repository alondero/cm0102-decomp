/* Preferred-image VA 0x006A8770, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stack arguments (stdcall, ret 0xC): [esp+4] signed byte selector, [esp+8]
   dword target, [esp+0xC] signed word threshold.
   Exits with EAX = target when target is zero, when the signed word threshold is
   below 15, or when selector-1 is outside 0..5 (signed byte selector, unsigned
   compare). Otherwise a jump table on selector 1..6 pushes one of six fixed
   addresses (0x00A0FD70, 0x00A0FD54, 0x00A0FD30, 0x00A0FD08, 0x00A0FCE8,
   0x00A0FCC8) after target and calls 0066F4E0 (cdecl, two arguments), then
   returns with EAX as left by that callee.
   The fixed addresses are passed as values, not read. Callee meaning unknown. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2);

u32 CM_STDCALL cm_fn_006a8770(s8 selector, u32 target, s16 threshold)
{
    if (target == 0) {
        return target;
    }
    if (threshold < 15) {
        return target;
    }
    switch (selector) {
    case 1:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fd70);
    case 2:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fd54);
    case 3:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fd30);
    case 4:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fd08);
    case 5:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fce8);
    case 6:
        return (u32)cm_fn_0066f4e0((int)target, (char *)0x00a0fcc8);
    default:
        return target;
    }
}
