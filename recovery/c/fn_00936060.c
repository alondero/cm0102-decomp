/* Preferred-image VA 0x00936060; base 0x00400000.
   CM0102 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Caller 0x00934d8d..95 pushes three arguments and cleans 12 bytes.
   Both successors overwrite EAX before use; the routine has no return value.
   Pointer/field meanings remain unknown. Reads and writes retain original widths,
   order and reloads, including unaligned pointer fields +6 and +10. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define S16(a) ((s32)(s16)R16(a))

void CM_CDECL cm_fn_00936060(u32 object, u32 records, u32 result)
{
    u32 count = 0, outer = 0, cursor, bound, index, left, right;
    u32 p, q, a, b, c, d, n, changed, pairs;
    u8 first_key, second_key, side;
    if (S16(object + 4) > 0) {
        do {
            if ((s32)count >= (s32)R32(object + 0x54)) break;
            p = R32(object + 10);
            bound = (u32)S16(p + outer * 2);
            p = R32(object + 6);
            index = (u32)S16(p + outer * 2);
            if ((s32)index < (s32)bound) {
                cursor = records + count * 8;
                do {
                    p = R32(object + 0x34);
                    left = (u32)S16(p + index * 2);
                    p = R32(object + 0x38);
                    right = (u32)S16(p + index * 2);
                    if ((s32)right >= 0 || (s32)left >= 0) {
                        p = R32(object + 0x28);
                        if (R8(p + index * 2) & 0x40) {
                            R8(cursor) = 1;
                            R8(cursor + 3) = 1;
                            R16(cursor + 4) = (u16)index;
                            ++count;
                            R16(cursor + 6) = (u16)index;
                            cursor += 8;
                        }
                        if ((s32)left >= 0) {
                            p = R32(object + 0x34);
                            a = (u32)S16(p + left * 2);
                            if (a != index) {
                                p = R32(object + 0x38);
                                a = (u32)S16(p + left * 2);
                            }
                            if (a == index) {
                                R8(cursor) = 3;
                                R8(cursor + 3) = 5;
                                R8(cursor + 2) = 1;
                                R16(cursor + 4) = (u16)left;
                                ++count;
                                R16(cursor + 6) = (u16)index;
                                cursor += 8;
                            }
                        }
                        if ((s32)right >= 0) {
                            if (right == left) {
                                p = R32(object + 0x4c);
                                q = R32(object + 0x44);
                                n = R32(object + 0x40);
                                a = R32(p + index * 4);
                                a *= R32(q + index * 4);
                                p = R32(object + 0x48);
                                b = R32(p + index * 4);
                                b *= R32(n + index * 4);
                                a += b;
                                a &= 0xffffc000u;
                                if ((s32)a > 0x0ddb4000) goto advance;
                            }
                            p = R32(object + 0x34);
                            a = (u32)S16(p + right * 2);
                            if (a != index) {
                                p = R32(object + 0x38);
                                a = (u32)S16(p + right * 2);
                            }
                            if (a == index) {
                                R8(cursor) = 3;
                                R8(cursor + 3) = 5;
                                R8(cursor + 2) = 0;
                                R16(cursor + 4) = (u16)right;
                                R16(cursor + 6) = (u16)index;
                                a = R32(object + 0x54);
                                ++count;
                                cursor += 8;
                                if ((s32)count >= (s32)a) break;
                            }
                        }
                    }
advance:
                    ++index;
                } while ((s32)index <= (s32)bound);
            }
            ++outer;
        } while ((s32)outer < S16(object + 4));
    }
    if ((s32)count > 0) {
        n = count;
        cursor = records + 1;
        do {
            side = R8(cursor + 1);
            index = (u32)S16(cursor + 5);
            if (side) {
                p = R32(object + 0x40);
                q = R32(object + 0x44);
                a = R32(p + index * 4);
            } else {
                p = R32(object + 0x48);
                q = R32(object + 0x4c);
                a = R32(p + index * 4);
            }
            b = R32(q + index * 4);
            if ((s32)a > 0x3dd2 || (s32)a < -0x3dd2)
                R8(cursor) = 2;
            else if ((s32)b > 0x3dd2 || (s32)b < -0x3dd2)
                R8(cursor) = 1;
            else
                R8(cursor) = 0xff;
            cursor += 8;
        } while (--n != 0);
    }
    pairs = count - 1;
    if ((s32)pairs > 0) {
        do {
            changed = 0;
            cursor = records + 8;
            n = pairs;
            do {
                first_key = R8(cursor - 5);
                second_key = R8(cursor + 3);
                if (first_key > second_key) {
                    a = R32(cursor);
                    b = R32(cursor - 8);
                    c = R32(cursor - 4);
                    R32(cursor - 8) = a;
                    d = R32(cursor + 4);
                    changed = 1;
                    R32(cursor - 4) = d;
                    R32(cursor) = b;
                    R32(cursor + 4) = c;
                }
                cursor += 8;
            } while (--n != 0);
        } while (changed);
    }
    R32(result) = (u32)(s32)(s16)count;
}
