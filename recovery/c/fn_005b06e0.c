#include "cmre.h"
/* Preferred-image VA005b06e0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/RET8. Sole indexed caller 005b02cb..02cf consumes AL only.
   Scratch L3..7 are flags; Lc is the reused first stack argument slot.
   004cf5a0 has a physical ECX receiver and one stack argument. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);

u8 CM_STDCALL cm_fn_005b06e0(u32 first, u32 second)
{
    u8 scratch[0x14];
    u32 l = (u32)scratch, a, c, club, other, p, q, result, id, base;
    s32 tenth;
    long double product;
    D(l + 0xc) = first;
    a = D(first + 0x1a);
    B(l + 5) = 0;
    c = D(second + 0x1a);
    B(l + 4) = 0; B(l + 3) = 0; B(l + 6) = 0; B(l + 7) = 0;
    if (a == c) B(l + 5) = 1;
    club = D(first + 0x39);
    if (club != 0) {
        p = D(second + 0x39);
        if (p != 0) {
            q = D(club + 0x53);
            if (q == D(p + 0x53)) B(l + 4) = 1;
        }
        if (D(club + 0x53) == c) B(l + 6) = 1;
    }
    other = D(second + 0x39);
    if (other != 0 && D(other + 0x53) == a) B(l + 7) = 1;
    if (club != 0 && other != 0) {
        a = D(club + 0x57); p = D(other + 0x57);
        if (a == p) B(l + 3) = 1;
    }
    if (second != 0 && c != 0) {
        tenth = (s32)(s16)W(c + 0x8e) / 10;
        D(l + 0xc) = (u32)tenth;
        p = B(second + 0x22);
        product = (long double)(s32)D(l + 0xc);
        D(l + 0xc) = p;
        product *= (long double)(s32)D(l + 0xc);
        if (product > (long double)CM_GLOBAL(volatile double, 0x0096a790))
            return 3;
        p = D(second + 0x61);
        if (p != 0) {
            if ((s16)W(p + 0xd) > 0x1482) return 3;
        } else {
            p = D(second + 0x69);
            if (p != 0 && (s16)W(p + 0xc) > 0x1482) return 3;
        }
    }
    if (other != 0 && other == club) return 3;
    p = D(second + 0x24);
    if (p != 0 && p == D(first + 0x24)) return 3;
    p = G(0x00ae1050);
    result = cm_fn_004cf5a0(p, second);
    G(0x00ae1080) = result;
    if (result != 0) {
        id = D(result + 4); base = G(0x00ae23b4);
        result = base + id * 581u;
    }
    if (result == D(first + 0x39)) return 3;
    p = D(second + 0x39);
    if (p != 0) {
        result = cm_fn_00540a50(p, l + 0xc, 1);
        q = D(first + 0x39);
        if (result == q) return 3;
    }
    p = G(0x00ae1050);
    result = cm_fn_004cf5a0(p, second);
    G(0x00ae1080) = result;
    if (result != 0) {
        id = D(result + 4); base = G(0x00ae23b4);
        result = base + id * 581u;
        if (result != 0) {
            p = G(0x00ae1050);
            result = cm_fn_004cf5a0(p, second);
            G(0x00ae1080) = result;
            if (result != 0) {
                id = D(result + 4); base = G(0x00ae23b4);
                result = base + id * 581u;
            }
            result = cm_fn_00540a50(result, l + 0xc, 1);
            q = D(first + 0x39);
            if (result == q) return 3;
        }
    }
    if (B(l + 3) != 0) return 2;
    if (B(l + 4) != 0) {
        p = D(second + 0x61);
        if (p != 0) {
            if ((s16)W(p + 0xb) > 0x109a) return 2;
        } else {
            p = D(second + 0x69);
            if (p != 0 && (s16)W(p + 0xa) > 0x109a) return 2;
        }
        if ((s8)B(second + 0x18) > 0x15) return 1;
    }
    if (B(l + 5) != 0 || B(l + 6) != 0) {
        p = D(second + 0x61);
        if (p != 0) {
            if ((s16)W(p + 0xb) > 0x109a) return 2;
        } else {
            p = D(second + 0x69);
            if (p != 0 && (s16)W(p + 0xa) > 0x109a) return 2;
        }
        if ((s8)B(second + 0x18) > 0x15) return 1;
    }
    if (B(l + 7) != 0) {
        p = D(second + 0x61);
        if (p != 0) {
            if ((s16)W(p + 0xb) > 0x109a) return 1;
        } else {
            p = D(second + 0x69);
            if (p != 0 && (s16)W(p + 0xa) > 0x109a) return 1;
        }
    }
    return 0;
}
