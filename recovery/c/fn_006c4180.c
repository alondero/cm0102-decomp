/* Preferred-image VA 0x006c4180, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
unsigned long long CM_STDCALL __attribute__((regparm(3))) cm_fn_006c4180(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 selector, u32 value)
{
    u32 eax, edx = incoming_edx;
    if ((u8)selector == 1u) {
        eax = (incoming_eax & 0xffffff00u) | *(volatile u8 *)(self + 0x8eb0u);
        eax -= (self & 0xffffff00u) | (u8)value;
    } else {
        edx = (incoming_edx & 0xffffff00u) | *(volatile u8 *)(self + 0x8eafu);
        eax = ((incoming_eax & 0xffffff00u) | (u8)value) - edx;
    }
    return ((unsigned long long)edx << 32) | eax;
}
