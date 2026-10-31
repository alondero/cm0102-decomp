/* Preferred-image VA 0x0053D740, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0) with two stack arguments: arg1 is an object pointer and
   arg2 is a pointer that may be NULL. Returns 1 or 0 in EAX (MOV EAX, 1 / XOR).
   Reads p = dword[arg1 + 0x1A] and q = dword[arg1 + 0x1E]. Returns 1 when
   byte[p + 0x7F] == 2, or dword[p] == dword[0x009CF4AC], or (q != NULL and
   byte[q + 0x7F] == 2). Otherwise, with arg2 != NULL and d = dword[arg2]:
   when d == dword[0x009CF314], compares dword[p + 0x71] with
   dword[0x00AE23A4] + 198 * dword[0x009CFA10] (ECX = G3 and EAX = G4 + 2*99*G3
   in the listing) and returns 1 on equality. Then returns 1 if d ==
   dword[0x009CF360]. If d != dword[0x009CF2C0] it returns 0. If d matches, it
   returns 1 when dword[p] equals dword[0x009CF348], dword[0x009CF408] or
   dword[0x009CF3A0]; otherwise 0. Field and global meanings remain unknown. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0053d740(u8 *arg1, u32 *arg2)
{
    u8 *p_b;
    u8 *p_c;
    u32 b_word;
    u32 d_word;
    u32 expect;
    u32 g3;
    u32 g4;

    p_b = *(u8 **)(arg1 + 0x1a);
    if (p_b[0x7f] == 2) {
        return 1;
    }
    b_word = *(u32 *)p_b;
    if (b_word == CM_GLOBAL(u32, 0x009cf4ac)) {
        return 1;
    }
    p_c = *(u8 **)(arg1 + 0x1e);
    if (p_c != 0 && p_c[0x7f] == 2) {
        return 1;
    }
    if (arg2 == 0) {
        return 0;
    }
    d_word = *arg2;
    if (d_word == CM_GLOBAL(u32, 0x009cf314)) {
        g3 = CM_GLOBAL(u32, 0x009cfa10);
        g4 = CM_GLOBAL(u32, 0x00ae23a4);
        expect = g4 + g3 * 198u;
        if (*(u32 *)(p_b + 0x71) == expect) {
            return 1;
        }
    }
    if (d_word == CM_GLOBAL(u32, 0x009cf360)) {
        return 1;
    }
    if (d_word != CM_GLOBAL(u32, 0x009cf2c0)) {
        return 0;
    }
    if (b_word == CM_GLOBAL(u32, 0x009cf348)) {
        return 1;
    }
    if (b_word == CM_GLOBAL(u32, 0x009cf408)) {
        return 1;
    }
    if (b_word == CM_GLOBAL(u32, 0x009cf3a0)) {
        return 1;
    }
    return 0;
}
