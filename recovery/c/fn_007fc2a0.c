#include "cmre.h"

/* cdecl comparison of two object pointers a and b (two stack arguments).
   A null pointer sorts first (-1 for a, +1 for b). Object fields by byte
   offset: 0x66 byte tag (0xff means empty), 0x2b dword, 0x50 dword,
   0x5c signed byte, 0x5d signed byte. The listing's "or eax, -1" writes an
   all-ones result, so the result does not depend on the entry EAX value. */

s32 CM_CDECL cm_fn_007fc2a0(u32 a, u32 b)
{
    u32 x;
    u32 y;
    s8 ca;
    s8 cb;

    if (a == 0u) {
        return -1;
    }
    if (b == 0u) {
        return 1;
    }
    if (*(volatile u8 *)(a + 0x66u) == 0xffu) {
        return -1;
    }
    if (*(volatile u8 *)(b + 0x66u) == 0xffu) {
        return 1;
    }

    x = *(volatile u32 *)(a + 0x2bu);
    y = *(volatile u32 *)(b + 0x2bu);
    if (x != y) {
        return (s32)(x - y);
    }

    x = *(volatile u32 *)(a + 0x50u);
    y = *(volatile u32 *)(b + 0x50u);
    if (x != y) {
        return (s32)(x - y);
    }

    ca = *(volatile s8 *)(a + 0x5cu);
    cb = *(volatile s8 *)(b + 0x5cu);
    if (ca != cb) {
        return (s32)ca - (s32)cb;
    }
    return (s32)*(volatile s8 *)(a + 0x5du) - (s32)*(volatile s8 *)(b + 0x5du);
}
