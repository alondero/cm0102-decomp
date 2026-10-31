/* Preferred-image VA 0x005D8310, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: fastcall (ECX is the object), no stack arguments, ret 0.
   Reads dword +0x08 as a selector. Selector 0 and any value outside 1..12
   store byte 0 at +0x0c. Selectors 2, 3, 4, 5, 7 and 9 store 7; 6 stores 6.
   Every path returns 1 in EAX. Field and value meanings remain unknown. */
#include "cmre.h"

u32 CM_FASTCALL cm_fn_005d8310(u8 *object)
{
    s32 selector = *(s32 *)(object + 0x08);
    u8 value;

    switch (selector) {
    case 1:
        value = 0x18;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 9:
        value = 7;
        break;
    case 6:
        value = 6;
        break;
    case 8:
        value = 0x10;
        break;
    case 10:
        value = 0x15;
        break;
    case 11:
        value = 0xf;
        break;
    case 12:
        value = 0x19;
        break;
    default:
        value = 0;
        break;
    }
    *(u8 *)(object + 0x0c) = value;
    return 1;
}
