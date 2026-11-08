/* WIP: source reconstruction; differential and emission review pending.
   Preferred-image VA 00844780, base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define SB(a) ((s8)B(a))
#define SW(a) ((s16)W(a))
#define SD(a) ((s32)D(a))
#define Q(a) CM_GLOBAL(volatile double, (u32)(a))
#define F(a) CM_GLOBAL(volatile float, (u32)(a))
#define A(o) ((u32)&scratch.bytes[o])
#define L32(o) D(A(o))
#define L16(o) W(A(o))
#define L8(o) B(A(o))
#define L64(o) Q(A(o))
#define LF(o) F(A(o))
#define C(a) ((long double)Q(a))

extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 output, u32 source);
extern u8 CM_CDECL cm_fn_004b02e0(u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053d980(u32 a, u32 b, u32 c);
extern u32 CM_CDECL cm_fn_0053dba0(u32 p, u32 output, u32 capacity, u32 selector);
extern u32 CM_CDECL cm_fn_005407e0(u32 first, u32 second, u32 mode);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern u32 CM_CDECL cm_fn_00547810(u32 target, u32 object);
extern u32 CM_CDECL cm_fn_00547840(u32 target, u32 object);
extern u32 CM_CDECL cm_fn_00547d30(u32 object, u32 target);
extern u32 CM_CDECL cm_fn_00547d60(u32 object, u32 target);
extern u32 CM_THISCALL cm_fn_00597db0(u32 self, u32 kind);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg);
extern u8 CM_THISCALL cm_fn_007c6830(u32 self, u32 player, u32 club, u32 nation, u32 flag);
extern u32 CM_THISCALL cm_fn_0085e410(u32 self);
extern long double CM_STDCALL cm_fn_00864550(u32 p, u32 d, u32 zero, u32 minus1, u32 mode, u32 zero2);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 record, u32 output);

__attribute__((target("no-sse3")))
long double CM_THISCALL cm_fn_00844780(u32 self, u32 p, u32 d, u32 flag, u32 mode, u32 output)
{
    /* S-relative storage. The original constructor object at +9c and its
       following 500-byte buffer at +ec share the original allocation. */
    union {
        u8 bytes[0x2ec];
        struct {
            u8 locals[0x2e0];
            volatile struct { u32 prev, handler, state; } seh;
        } layout;
    } scratch;
    u32 current, helper, a, b, c, n, r, target, cursor, name;
    s32 x, y, z, value, offer, rating;
    s8 category, other_category;
    u8 byte, next_byte, wanted = (u8)mode;
    long double v, t, u, difference;
    long long destination_conversion;
    scratch.layout.seh.state = 0xffffffffu;
    scratch.layout.seh.handler = 0x0096253b;
    scratch.layout.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&scratch.layout.seh;
    L32(0x90) = self;
    L8(0x47) = 0;
    L32(0x78) = 0;
    L32(0x70) = 0;
    L32(0x7c) = 0;
    L32(0x80) = 0x3ff00000;
    if (output) D(output) = 0;
    if (!D(p + 0x69)) goto zero;
    if (!D(d + 0x53)) goto zero;
    if (!D(p + 0x1a)) goto zero;
    if (p) {
        /* Three original two-byte-at-a-time strcmp loops. The third helper
           call is reached only when the preceding comparison differed. */
        for (n = 0; n < 3; ++n) {
            cm_fn_0053dba0(p, A(0xec), 500, 1);
            name = n == 0 ? 0x00a82620 : n == 1 ? 0x00a82614 : 0x00a82604;
            cursor = A(0xec);
            for (;;) {
                byte = B(cursor);
                next_byte = B(name);
                if (byte != next_byte) break;
                if (!byte) break;
                byte = B(cursor + 1);
                next_byte = B(name + 1);
                if (byte != next_byte) break;
                cursor += 2;
                name += 2;
                if (!byte) break;
            }
            if (byte == next_byte) {
                if (D(p + 0x39)) goto zero;
                break;
            }
        }
    }
    category = SB(p + 0x3d);
    if (category == 0x11) goto zero;
    other_category = SB(p + 0x28);
    if (other_category == 0x11) goto zero;
    if (wanted != 5 && SD(p) >= (s32)(D(0xae2364) - 16)) goto zero;
    a = D(p + 0x24);
    if (a && other_category == 5) goto zero;
    if (wanted) {
        if (wanted != 5 && (category == 5 || category == 12 || other_category == 5 || other_category == 12)) {
            a = D(p + 0x69);
            if (SW(a + 10) > 0x109a || SW(a + 4) > 100) goto zero;
        }
        if (wanted == 10 && category != 10 && category) goto zero;
        if (other_category == 5 && D(p + 0x24)) goto zero;
        if (wanted != 15 && wanted != 12 && wanted != 5) {
            a = D(p + 0x61);
            if (a && category == 11) {
                a = D(p + 0x39);
                if (a && d != a) goto zero;
            }
        }
        if (wanted != 10 && category == 10) goto zero;
    }
    if (d) {
        if (wanted != 5 && (category == 5 || category == 12 || other_category == 5 || other_category == 12)) {
            a = D(p + 0x39);
            if (a) {
                b = D(d + 0x57);
                if (b) {
                    c = D(a + 0x57);
                    if (c && SW(c + 0x69) >= SW(b + 0x69)) goto zero;
                }
                if (SW(a + 0x80) > (s32)SW(d + 0x80) - 750) goto zero;
            }
        }
        if (SW(d + 0x80) < 2750) {
            a = D(d + 0x53);
            if (a) {
                b = D(p + 0x39);
                if (b && D(b + 0x53) == a) goto zero;
                if (cm_fn_0053d980(p, a, 1)) goto zero;
            }
        }
    }
    /* 844a87..844b30: retain the unrounded third weight while adding the
       binary32 spills, and compare that sum before loading the denominator. */
    LF(0x74) = wanted == 5 || wanted == 12 ? 0.0f : 1.0f;
    x = SB(p + 0x57);
    t = (long double)x;
    y = SB(p + 0x5b);
    LF(0x88) = (C(0x968dc0) - t * C(0x967878)) - (long double)y * C(0x967878);
    LF(0x84) = t * C(0x968a98) + C(0x9689b0);
    v = t * C(0x967888) + C(0x968968);
    LF(0x60) = v;
    v = v + LF(0x84);
    v = v + LF(0x88);
    v = v + LF(0x74);
    v = v + F(0x968e70);
    LF(0x8c) = v;
    if (!(v > C(0x9689a0))) LF(0x8c) = 0.5f;
    current = D(p + 0x39);
    L8(0x13) = B(p + 0x3d);
    L32(0x48) = current;
    if (!current || cm_fn_0053b900((u32 *)d)) {
        a = D(p + 0x24);
        if (a) {
            byte = B(p + 0x28);
            if (byte != 11) {
                current = a;
                L8(0x13) = byte;
                L32(0x48) = current;
            }
        }
    }
    if (current && !D(current + 0x53)) {
        v = C(0x968e38);
        goto done;
    }
    helper = cm_fn_004cf5f0(D(0xae1050), p, current);
    L32(0x58) = helper;
    if (d == current) {
        L32(0x14) = 0;
        L32(0x18) = 0x400c0000;
    } else {
        if (helper && current) {
            a = D(current + 0x53);
            if (a) {
                b = D(d + 0x53);
                if (b) {
                    byte = B(b + 0x77);
                    next_byte = B(a + 0x77);
                    if (byte == next_byte && (B(helper + 0x45) & 0x20)) goto zero;
                }
            }
        }
        L32(0x14) = 0;
        L32(0x18) = wanted == 5 || wanted == 12 ? 0x40020000 : 0x3ff80000;
        if (!current) {
            a = D(0xdd6ec8);
            b = D(0xdd6ecc);
            c = W(0xae2c92);
            L32(0x64) = a;
            L32(0x68) = b;
            if (L16(0x66) < c || (L16(0x66) == c && (s16)a <= SW(0xae2c90))) {
                n = W(0xdd6eca);
                L32(0x64) = D(p + 0x3e);
                L32(0x68) = D(p + 0x42);
                a = D(p + 0x3e);
                b = D(p + 0x42);
                L32(0x34) = a;
                L32(0x38) = b;
                if (L16(0x36) < n || (L16(0x36) == n && (s16)a < SW(0xdd6ec8))) {
                    cm_fn_00417c10(A(0x64), A(0x94), 0xdd6ec8);
                    c = W(0xae2c92);
                }
                x = (s32)(((u32)c - L16(0x66)) * 365 - (s32)(s16)L16(0x64) + (s32)SW(0xae2c90));
                if (x > 500) x = 500;
                x = (s32)((u32)x * (u32)x);
                v = (long double)x;
                a = D(p + 0x69);
                v = v / ((long double)SW(a + 10) + C(0x96f958));
                if (!(C(0x96d6a0) >= v)) v = C(0x96d6a0);
                L64(0x14) = v + L64(0x14);
            }
        }
    }
    r = cm_fn_00547d30(p, d);
    if (r) {
        L64(0x14) = (long double)L64(0x14) * C(0x968f78);
        if (output) D(output) = D(output) | 2;
    }
    r = cm_fn_00547d30(p, current);
    if (r) {
        t = L64(0x14);
        L64(0x14) = t + t;
        if (output) D(output) = D(output) | 4;
    }
    r = cm_fn_00547d60(p, current);
    if (r) {
        byte = B(current + 0x64);
        if (byte != 1 && B(d + 0x64) == 1) L64(0x14) = (long double)L64(0x14) * C(0x969520);
        else L64(0x14) = (long double)L64(0x14) * C(0x968968);
        if (output && L32(0x58) && !B(L32(0x58) + 0x49)) D(output) = D(output) | 8;
    }
    r = cm_fn_00547d60(p, d);
    if (r) {
        if (output) D(output) = D(output) | 0x10;
        v = (long double)L64(0x14) * C(0x968930);
        L64(0x14) = v;
    } else v = L64(0x14);
    if (current) {
        if (current != d && (d == D(current + 0xb3) || d == D(current + 0xb7) || d == D(current + 0xbb) ||
            (r = cm_fn_004b02e0(d, current), v = L64(0x14), (u8)r))) {
            a = D(d);
            b = D(0x9d0118);
            c = D(0x9d011c);
            if ((a == b || a == c) && (D(current) == b || D(current) == c))
                t = C(0x968968) - (long double)SB(p + 0x59) * C(0x967878);
            else if (D(D(d + 0x53)) == D(0x9cf360))
                t = C(0x967890) - (long double)SB(p + 0x59) * C(0x968958);
            else if ((d == D(current + 0xb3) || d == D(current + 0xb7) || d == D(current + 0xbb)) && D(d + 0x57) == D(current + 0x57)) {
                x = (s32)SW(d + 0x80) - SW(current + 0x80);
                y = x >> 31;
                x = (s32)(((u32)x ^ (u32)y) - (u32)y);
                if (x < 1250) t = C(0x969040) - (long double)SB(p + 0x59) * C(0x967878);
                else t = C(0x9689b0) - (long double)SB(p + 0x59) * C(0x968958);
            } else t = C(0x969520) - (long double)SB(p + 0x59) * C(0x967878);
            v = v * t;
            L64(0x14) = v;
            if (output && t < C(0x968e40)) D(output) = D(output) | 0x20;
        } else {
            a = D(D(d + 0x53));
            b = D(0x9cf2ac);
            c = D(0x9cf51c);
            if (a == c && D(D(current + 0x53)) == b && D(D(p + 0x1a)) == b) {
                v = v * C(0x967888);
                L64(0x14) = v;
                if (output) D(output) = D(output) | 2;
            } else if (a == b && D(D(current + 0x53)) == c && D(D(p + 0x1a)) == c) {
                v = v * C(0x967880);
                L64(0x14) = v;
                if (output) D(output) = D(output) | 2;
            }
        }
    }
    if (wanted != 5 && wanted != 12) {
        r = cm_fn_00547810(D(d + 0xcf), p);
        v = L64(0x14);
        if (r) {
            v = v + v;
            L64(0x14) = v;
            if (output) D(output) = D(output) | 0x40;
        }
        r = cm_fn_00547840(D(d + 0xcf), p);
        v = L64(0x14);
        if (r) {
            v = v * C(0x968f78);
            L64(0x14) = v;
            if (output) D(output) = D(output) | 0x80;
        }
    }
    if (current) {
        r = cm_fn_00547840(D(current + 0xcf), p);
        if (r) {
            t = L64(0x14);
            L64(0x14) = t + t;
            if (output) D(output) = D(output) | 0x100;
        }
        r = cm_fn_00547810(D(current + 0xcf), p);
        v = L64(0x14);
        if (r) {
            v = v * C(0x968968);
            L64(0x14) = v;
            if (output) D(output) = D(output) | 0x200;
        }
    }
    if (D(p + 0x61)) {
        r = cm_fn_008cca10(D(0xde1f54), p, 0);
        if (r) {
            if (output) D(output) = D(output) | 0x10000000;
            goto zero;
        }
        v = L64(0x14);
    }
    helper = L32(0x58);
    if (helper) {
        x = SB(helper + 0x44);
        if (d == current) {
            L8(0x13) = wanted;
            t = (long double)x * C(0x969028) + C(0x967890);
            v = v * t;
            L64(0x14) = v;
        } else {
            t = C(0x967890) - (long double)x * C(0x969028);
            v = v * t;
            L64(0x14) = v;
            if (output && !(B(helper + 0x4e) & 8)) {
                if (t > C(0x968940)) D(output) = D(output) | 0x400;
                else if (t < C(0x968968)) D(output) = D(output) | 0x800;
            }
        }
    } else if (d == current) L8(0x13) = wanted;
    if (d == current) goto current_rating;
    category = (s8)L8(0x13);
    a = (u32)(s32)category - 1;
    target = a > 15 ? 0x845307 : D(0x846810 + a * 4);
    switch (target) {
    case 0x8451ea: v = v * C(0x967898); break;
    case 0x845259:
        if (wanted != 5 || SW(d + 0x80) < 6000) v = v * C(0x967898);
        else v = v * C(0x968968);
        break;
    case 0x8451f5:
        if (current) {
            v = v * C(0x96a6a8);
            if (output) D(output) = D(output) | 0x4000000;
        } else v = v * C(0x96a5d0);
        break;
    case 0x845274:
        v = v * C(0x968940);
        if (output && (wanted == 5 || wanted == 12)) D(output) = D(output) | 0x8000000;
        break;
    case 0x8452c3:
        v = v * C(0x96d7d0);
        if (output && (wanted == 5 || wanted == 12)) D(output) = D(output) | 0x8000000;
        break;
    case 0x845296:
        t = (long double)SB(p + 0x57) * C(0x967888) + C(0x9689b0);
        v = v * t;
        if (output && (wanted == 5 || wanted == 12)) D(output) = D(output) | 0x8000000;
        break;
    case 0x8452d9:
        if (output && (wanted == 5 || wanted == 12 || wanted == 6 || wanted == 13 ||
            wanted == 14 || wanted == 15 || wanted == 7 || wanted == 8)) D(output) = D(output) | 0x8000000;
        break;
    case 0x84521e:
        x = SB(p + 0x57);
        if (x <= 1) x = 1;
        L32(0x4c) = x;
        v = v * (C(0x9688b0) / (long double)x);
        break;
    case 0x845307:
        v = v * C(0x9688b0);
        if (output) D(output) = D(output) | 0x8000000;
        break;
    }
    a = (u32)(s32)(s8)wanted;
    L32(0x5c) = a;
    target = a > 15 ? 0x8453b7 : D(0x846850 + 4 * a);
    switch (target) {
    case 0x8467ef: goto zero;
    case 0x845331: v = v * C(0x9688b0); L64(0x14) = v; break;
    case 0x84533c: v = v * C(0x96f950); L64(0x14) = v; break;
    case 0x845347:
        v = v * C(0x96f948);
        L64(0x14) = v;
        if (category == 5 || category == 12) L64(0x14) = (long double)L64(0x14) * C(0x968940);
        break;
    case 0x845367:
        L64(0x14) = v * C(0x968968);
        if (category == 5 || category == 12) L64(0x14) = (long double)L64(0x14) * C(0x968e00);
        break;
    case 0x84537d:
        L64(0x14) = v * C(0x968e00);
        if (category == 5 || category == 12) L64(0x14) = (long double)L64(0x14) * C(0x968e00);
        break;
    case 0x845385:
        if (category == 9 || category == 10) L64(0x14) = v * C(0x968e00);
        else L64(0x14) = v * C(0x9689e0);
        if (D(p + 0x61)) L64(0x14) = (long double)L64(0x14) * C(0x9689e0);
        if (category == 5 || category == 12) L64(0x14) = (long double)L64(0x14) * C(0x968e00);
        break;
    case 0x8453b7:
        L64(0x14) = v * C(0x967880);
        if (category == 5 || category == 12) L64(0x14) = (long double)L64(0x14) * C(0x968e00);
        break;
    }
    if (current && (category == 5 || category == 12)) {
        a = D(0xb67a44);
        if (a) {
            b = D(current);
            c = D(a);
            a = b * 73 + c;
            x = SW(a + 8);
            y = SW(a + 6);
            z = SB(p + 0x59);
            value = SB(p + 0x58);
            if (x + (z + value) * 500 + y < 8000) L64(0x14) = (long double)L64(0x14) * C(0x968dc0);
        }
    }
    a = D(d + 0x53);
    x = SB(a + 0x7e);
    if (x < 1 || x > 3) x = 3;
    L8(0x47) = (u8)(x * x);
    a = D(d + 0x69);
    if (a && (a = D(a + 0x38)) != 0) {
        x = SB(a + 0x33);
        rating = SW(d + 0x80);
        if (!x) value = rating / 2;
        else {
            y = x * rating / 15;
            t = (long double)rating * C(0x96d5e0);
            L64(0x34) = (long double)y;
            if (!(t > L64(0x34))) t = L64(0x34);
            value = (u32)(long long)t; /* __ftol 008454d2 */
        }
    } else {
        rating = SW(d + 0x80);
        t = (long double)rating * C(0x96d5e0);
        value = (u32)(long long)t; /* __ftol 008454f8 */
    }
    L32(0x30) = value;
    a = D(d + 0x57);
    if (a) {
        x = SW(a + 0x69);
        if (rating >= 7500 && SD(d + 0x73) > 35000) {
            x = x * 500;
            if (x <= rating) x = rating;
        } else {
            y = rating / 2 + x * 250;
            x = x * 500;
            if (x <= y) x = y;
        }
        L32(0x40) = x;
    } else L32(0x40) = (d & 0xffff0000) | (u16)rating;
    a = D(d + 0x1db);
    if ((s32)a <= -1) a = D(D(0xae2d14) + D(d) * 6);
    if ((s32)a > -1) {
        c = a * 107 + D(0xae23d0);
        L32(0x78) = c;
        if (c) {
            a = (c & 0xffff0000) | (u16)(W(c + 0x69) * 600);
            L32(0x4c) = a;
            goto destination_offer;
        }
    }
    t = (long double)(s16)L16(0x40) * C(0x9689b0);
    if (!(t < C(0x96d470))) t = C(0x96d470);
    L32(0x4c) = (u32)(long long)t; /* __ftol 008455e1 */
destination_offer:
    a = D(d + 0xcf);
    offer = 0;
    if (a && D(a + 0x69)) {
        offer = cm_fn_005407e0(a, p, 1);
        c = D(0xb67a44);
        if (c && wanted != 5) {
            if (!d) { x = 0; y = 0; }
            else {
                a = D(d);
                b = D(c);
                a = a * 73 + b;
                x = SW(a + 6);
                y = SW(a + 8);
            }
            t = (long double)x * C(0x968f80) + (long double)y * C(0x968f78);
            value = (u32)(long long)t; /* __ftol 00845674 */
            if ((s16)value < 5000) {
                t = C(0x9688a0) - (long double)(s16)value * C(0x96a618);
                u = (long double)(s16)offer;
                L64(0x34) = u / t;
                t = u - t * C(0x968908);
                if (L64(0x34) > t) t = L64(0x34);
            } else {
                t = (long double)(s16)value * C(0x96a5c0) + C(0x968968);
                u = (long double)(s16)offer;
                L64(0x34) = u * t;
                t = t * C(0x968908) + u;
                if (L64(0x34) < t) t = L64(0x34);
            }
            offer = (u32)(long long)t; /* __ftol 00845717 */
        }
    }
    rating = W(d + 0x80);
    r = cm_fn_0053b900((u32 *)d);
    if (!r) {
        r = cm_fn_005a0590(D(0xae2a38), d);
        if (r) {
            x = (s8)cm_fn_005986b0(r);
            a = (u32)x + 2;
            if (a <= 4) {
                target = D(0x846890 + a * 4);
                switch (target) {
                case 0x845763:
                    t = (long double)(s16)rating * C(0x968940);
                    rating = (u32)(long long)t; /* __ftol 00845774 */
                    if ((s16)rating < 5750) rating = 5750;
                    break;
                case 0x845789:
                    t = (long double)(s16)rating * C(0x968928);
                    rating = (u32)(long long)t; /* __ftol 0084579a */
                    if ((s16)rating < 4250) rating = 4250;
                    break;
                case 0x8457af:
                    t = (long double)(s16)rating * C(0x969520);
                    goto destination_category_cast;
                case 0x8457c2:
                    t = (long double)(s16)rating * C(0x968e40);
destination_category_cast:
                    rating = (u32)(long long)t; /* __ftol 008457d3 */
                    break;
                case 0x8457da: break;
                }
            }
        }
    }
    r = cm_fn_0053b900((u32 *)d);
    if (r && !cm_fn_005409b0((u32 *)d)) t = (long double)(s16)rating * C(0x96f940);
    else {
        a = D(d);
        b = L32(0x90);
        b = D(b + 0xdc);
        x = SW(a * 9 + b + 5);
        t = (long double)x * C(0x968e00) + (long double)(s16)rating * C(0x968e48);
    }
    value = (u32)(long long)t; /* __ftol 0084583f */
    t = (long double)(s16)L16(0x4c) * LF(0x60);
    t = t + (long double)(s16)L16(0x40) * LF(0x84);
    t = t + (long double)(s16)offer * LF(0x74);
    t = t + (long double)(s16)L16(0x30) * LF(0x88);
    t = t + (long double)(s16)value;
    t = t / LF(0x8c);
    rating = (u32)(long long)t; /* __ftol 008458a4 */
    r = cm_fn_0053b900((u32 *)d);
    if (r && D(d + 0x53) == D(p + 0x1a)) {
        t = (long double)(s16)rating * C(0x96f940);
        rating = (u32)(long long)t; /* __ftol 008458d4 */
    }
    r = cm_fn_0053b900((u32 *)d);
    if (!r) {
        byte = B(d + 0x64);
        if (byte != 1) {
            x = (s16)rating;
            y = B(p + 0x22);
            z = x - (y * y + 400) * 5;
            rating = x / 2;
            if (rating >= z) rating = z;
            if (byte == 3) {
                x = (s16)rating;
                rating = x - 2000;
                if (x / 2 < rating) rating = x / 2;
            }
        }
    }
    t = (long double)(s16)rating * C(0x968940) - C(0x9688e8);
    destination_conversion = (long long)t; /* __ftol 00845950 */
    L32(0x34) = (u32)destination_conversion;
    a = D(d + 0x53);
    if (a) {
        x = (s16)L16(0x34) + 5000;
        t = (long double)x;
        b = D(p + 0x61);
        L64(0x4c) = (t * C(0x968d78)) * t;
        if (b) {
            /* EDX is the high DWORD returned by __ftol at 00845950. */
            n = ((u32)((unsigned long long)destination_conversion >> 32) & 0xffffff00) | B(a + 0x77);
            t = (long double)(s8)cm_fn_007c6830(D(0xb74350), p, d, n, 1);
        } else t = C(0x9688b0);
        L64(0x24) = t * C(0x967880);
        t = C(0x96f938) - t * C(0x96a648);
        t = (long double)L64(0x4c) / t;
        if (L64(0x24) > t) t = L64(0x24);
        t = t + C(0x968918);
        L64(0x24) = t;
        v = t * L64(0x14);
        L64(0x14) = v;
        if (output && L64(0x24) < C(0x968e48)) D(output) = D(output) | 0x10000;
    } else v = L64(0x14);
    current = L32(0x48);
    if (current) {
        a = D(d + 0x53);
        if (a) {
            r = cm_fn_0053d980(p, a, 1);
            v = L64(0x14);
            if (!r) goto destination_modifiers_done;
        }
        if (D(current + 0x53) == D(d + 0x53)) goto destination_modifiers_done;
    }
    if (wanted != 5 && wanted != 12) {
        a = D(d + 0xcf);
        if (a && (a = D(a + 0x1a)) != 0) {
            r = cm_fn_0053d980(p, a, 1);
            if (!r) {
                a = L32(0x5c) - 2;
                target = 0x845ad0;
                if (a <= 13) {
                    c = B(0x8468b8 + a);
                    target = D(0x8468a4 + c * 4);
                }
                v = L64(0x14);
                switch (target) {
                case 0x845aa0: v = v * C(0x969520); break;
                case 0x845aac: v = v * C(0x969040); break;
                case 0x845ab8: v = v * C(0x96a5d0); break;
                case 0x845ac4: v = v * C(0x968968); break;
                case 0x845ad0: v = v * C(0x967898); break;
                }
                goto destination_modifier_store;
            }
            v = L64(0x14);
        }
    }
    a = L32(0x5c) - 2;
    target = a > 13 ? 0x845b13 : D(0x8468c8 + a * 4);
    switch (target) {
    case 0x845afb: v = v * C(0x9689e0); break;
    case 0x845af3: v = v * C(0x968e80); break;
    case 0x845b03: v = v * C(0x96d6d8); break;
    case 0x845b0b: v = v * C(0x968918); break;
    case 0x845b13: v = v * C(0x96a350); break;
    }
destination_modifier_store:
    L64(0x14) = v;
destination_modifiers_done:
    if (!current) {
        v = v * C(0x968e18);
        L64(0x14) = v;
        goto no_current;
    }
    a = W(p + 0x40);
    b = W(0xae2c92);
    c = W(p + 0x3e);
    x = SB(p + 0x59);
    y = (s32)((b - a) * 365 - (s32)(s16)c + (s32)SW(0xae2c90));
    if (y < (x + 8) * 15) goto zero;
    y = (s32)((u32)y * (u32)y);
    y = y / 20;
    z = x + 6;
    if (z < 1) z = 1;
    y = y / z;
    L32(0x4c) = y;
    if (y < 0) goto zero;
    if (y > 400) L32(0x4c) = 400;
    else if (!y) goto zero;
    L64(0x7c) = (long double)(s32)L32(0x4c) * C(0x969028);
current_rating:
    if (!current) goto no_current;
    a = D(current + 0x53);
    x = 3;
    if (a) {
        x = SB(a + 0x7e);
        if (x < 1 || x > 3) x = 3;
    }
    L8(0x46) = x * x;
    a = D(current + 0x69);
    if (a && (a = D(a + 0x38)) != 0) {
        x = SB(a + 0x33);
        rating = SW(current + 0x80);
        L32(0x5c) = (current & 0xffff0000) | (u16)rating;
        if (!x) value = rating / 2;
        else {
            y = x * rating / 15;
            L32(0x30) = rating;
            t = (long double)rating * C(0x96d5e0);
            L64(0x24) = (long double)y;
            if (!(t > L64(0x24))) t = L64(0x24);
            value = (u32)(long long)t; /* __ftol 00845ce3 */
        }
    } else {
        rating = SW(current + 0x80);
        L32(0x30) = rating;
        L32(0x5c) = (current & 0xffff0000) | (u16)rating;
        t = (long double)rating * C(0x96d5e0);
        value = (u32)(long long)t; /* __ftol 00845ce3 */
    }
    L32(0x4c) = value;
    a = D(current + 0x57);
    if (a) {
        x = SW(a + 0x69);
        if (rating >= 7500 && SD(current + 0x73) > 35000) {
            x = x * 500;
            if (x <= rating) x = rating;
        } else {
            y = rating / 2 + x * 250;
            x = x * 500;
            if (x <= y) x = y;
        }
        L32(0x30) = x;
    } else L32(0x30) = L32(0x5c);
    a = D(current + 0x1db);
    if ((s32)a <= -1 || (a == D(0x9cf6f8) && (s16)rating >= 5000))
        a = D(D(0xae2d14) + D(current) * 6);
    if ((s32)a > -1) {
        c = a * 107 + D(0xae23d0);
        L32(0x70) = c;
        if (c) {
            a = (c & 0xffff0000) | (u16)(W(c + 0x69) * 600);
            L32(0x1c) = a;
            goto current_offer;
        }
    }
    t = (long double)(s16)L16(0x30) * C(0x9689b0);
    if (!(t < C(0x96d470))) t = C(0x96d470);
    L32(0x1c) = (u32)(long long)t; /* __ftol 00845dda */
current_offer:
    a = D(current + 0xcf);
    if (!a) {
        t = (long double)(s16)L16(0x30) * C(0x969040);
        goto current_offer_cast;
    }
    a = D(a + 0x69);
    if (!a) {
        t = (long double)(s16)L16(0x30) * C(0x969040);
        goto current_offer_cast;
    }
    c = D(0xb67a44);
    offer = SW(a + 10);
    if (c && wanted != 5) {
        a = D(current);
        b = D(c);
        a = a * 73 + b;
        x = SW(a + 6);
        y = SW(a + 8);
        t = (long double)x * C(0x968f80) + (long double)y * C(0x968f78);
        value = (u32)(long long)t; /* __ftol 00845e78 */
        if ((s16)value < 5000) {
            t = C(0x9688a0) - (long double)(s16)value * C(0x96a618);
            u = (long double)(s16)offer;
            L64(0x24) = u / t;
            t = u - t * C(0x968908);
            if (L64(0x24) > t) t = L64(0x24);
        } else {
            t = (long double)(s16)value * C(0x96a5c0) + C(0x968968);
            u = (long double)(s16)offer;
            L64(0x24) = u * t;
            t = t * C(0x968908) + u;
            if (L64(0x24) < t) t = L64(0x24);
        }
current_offer_cast:
        offer = (u32)(long long)t; /* __ftol 00845f1b */
    }
    rating = L32(0x5c);
    r = cm_fn_0053b900((u32 *)current);
    if (!r) {
        r = cm_fn_005a0590(D(0xae2a38), current);
        if (r) {
            x = (s8)cm_fn_005986b0(r);
            a = (u32)x + 2;
            if (a <= 4) {
                target = D(0x846900 + a * 4);
                switch (target) {
                case 0x845f64:
                    t = (long double)(s16)rating * C(0x968940);
                    rating = (u32)(long long)t; /* __ftol 00845f75 */
                    if ((s16)rating < 5750) rating = 5750;
                    break;
                case 0x845f8a:
                    t = (long double)(s16)rating * C(0x968928);
                    rating = (u32)(long long)t; /* __ftol 00845f9b */
                    if ((s16)rating < 4750) rating = 4750;
                    break;
                case 0x845fb0:
                    t = (long double)(s16)rating * C(0x9689d8);
                    goto current_category_cast;
                case 0x845fc3:
                    t = (long double)(s16)rating * C(0x969040);
current_category_cast:
                    rating = (u32)(long long)t; /* __ftol 00845fd4 */
                    break;
                case 0x845fdb: break;
                }
            }
        }
    }
    r = cm_fn_0053b900((u32 *)current);
    if (r && !cm_fn_005409b0((u32 *)current)) t = (long double)(s16)rating * C(0x969588);
    else {
        a = D(current);
        b = L32(0x90);
        b = D(b + 0xdc);
        x = SW(a * 9 + b + 5);
        t = (long double)x * C(0x968e00) + (long double)(s16)rating * C(0x968e48);
    }
    value = (u32)(long long)t; /* __ftol 00846040 */
    t = (long double)(s16)L16(0x1c) * LF(0x60);
    t = t + (long double)(s16)L16(0x30) * LF(0x84);
    t = t + (long double)(s16)offer * LF(0x74);
    t = t + (long double)(s16)L16(0x4c) * LF(0x88);
    t = t + (long double)(s16)value;
    t = t / LF(0x8c);
    rating = (u32)(long long)t; /* __ftol 008460a5 */
    r = cm_fn_0053b900((u32 *)current);
    if (!r) {
        a = L32(0x48);
        byte = B(a + 0x64);
        if (byte != 1) {
            x = (s16)rating;
            y = B(p + 0x22);
            z = x - (y * y + 400) * 5;
            rating = x / 2;
            if (rating >= z) rating = z;
            if (byte == 3) {
                x = (s16)rating;
                rating = x - 2000;
                if (x / 2 < rating) rating = x / 2;
            }
        }
    }
    t = (long double)(s16)rating * C(0x968940) - C(0x9688e8);
    rating = (u32)(long long)t; /* __ftol 0084611d */
    c = L32(0x48);
    a = D(c + 0x53);
    if (a) {
        if (d == c) { v = L64(0x14); goto final_adjustment; }
        x = (s16)rating + 5000;
        t = (long double)x;
        b = D(p + 0x61);
        L64(0x1c) = (t * C(0x968d78)) * t;
        if (b) {
            n = (a & 0xffffff00) | B(a + 0x77);
            t = (long double)(s8)cm_fn_007c6830(D(0xb74350), p, c, n, 1);
        } else t = C(0x9688b0);
        L64(0x24) = t * C(0x967880);
        t = C(0x96f938) - t * C(0x96a648);
        t = (long double)L64(0x1c) / t;
        if (L64(0x24) > t) t = L64(0x24);
        t = t + C(0x968918);
        v = (long double)L64(0x14) / t;
        L64(0x14) = v;
        goto compare_ratings;
    }
    v = L64(0x14);
    goto compare_ratings;
no_current:
    a = D(p + 0x1a);
    x = SB(a + 0x7e);
    if (x < 1 || x > 3) x = 3;
    L8(0x46) = x * x;
    a = D(p + 0x69);
    x = SW(a + 12);
    y = SW(a + 10);
    z = SW(a + 8);
    x = (s16)((x + y + z) / 3);
    t = (long double)x * C(0x968940) - C(0x9688e8);
    rating = (u32)(long long)t; /* __ftol 00845c29 */
compare_ratings:
    current = L32(0x48);
    category = (s8)L8(0x13);
    if (d == current) goto final_adjustment;
    a = D(d + 0x57);
    if (!a || D(a) == D(0x9cf764)) v = v * C(0x967880);
    a = D(p + 0x69);
    x = SW(a + 12);
    y = SW(a + 10);
    z = SW(a + 8);
    x = SW(d + 0x80) - (x + y + z) / 3;
    t = (long double)x * C(0x96a6b8) + C(0x967890);
    L64(0x24) = t;
    x = (s16)L16(0x34);
    y = (s16)rating;
    if (x > y) {
        t = (long double)(x - y);
        u = (long double)SB(p + 0x57) * C(0x96f930) + C(0x969040);
        difference = t * u;
    } else if (wanted != 5 && wanted != 12) difference = (long double)(x - y) * C(0x968940);
    else if (v > C(0x9688b0)) {
        t = C(0x9689c0) - v * C(0x967898);
        if (t < C(0x967880)) t = C(0x967880);
        else if (t > C(0x967890)) t = C(0x967890);
        difference = t * (long double)(x - y);
    } else difference = (long double)(x - y);
    if (difference > C(0x9689a0) && L64(0x7c) < C(0x967890)) {
        t = difference * C(0x968a98);
        if (C(0x9688a8) < t) t = C(0x9688a8);
        t = ((C(0x968f50) - t) * L64(0x7c) + t) * C(0x96d1d0);
        v = v * t;
        if (output && t < C(0x968e48)) D(output) = D(output) | 0x100000;
    }
    if (category != 5 && category != 12 && (wanted == 5 || wanted == 12) && !(difference >= C(0x9689a0))) {
        if (category == 8 || category == 15 || category == 6) {
            t = C(0x967890) - v * C(0x968a90);
            if (!(t >= C(0x968e00))) t = C(0x968e00);
        } else {
            t = C(0x967890) - v * C(0x967880);
            if (!(t >= C(0x968918))) t = C(0x968918);
        }
        difference = t * difference;
    }
    t = difference * C(0x96f928) + C(0x967890);
    L64(0x14) = t;
    if (!(t >= C(0x96a350))) {
        L32(0x14) = 0xd2f1a9fc;
        L32(0x18) = 0x3f50624d;
    }
    v = v * L64(0x14);
    if (output) {
        if (L64(0x14) < C(0x968e48)) {
            a = 0x800000;
            if (LF(0x60) > C(0x968978) && L32(0x70) && !L32(0x78) && v > C(0x968928) &&
                (category == 5 || category == 12)) {
                b = L32(0x58);
                if (!b || !B(b + 0x49)) a = 0x400000;
            }
            D(output) = D(output) | a;
        } else if (L64(0x14) > C(0x968940) && LF(0x60) > C(0x968978) && !L32(0x70) && L32(0x78) &&
            (wanted == 5 || wanted == 12)) {
            b = L32(0x58);
            if (!b || !B(b + 0x49)) D(output) = D(output) | 0x200000;
        }
    }
    current = L32(0x48);
    if (current) {
        byte = L8(0x46);
        next_byte = L8(0x47);
        if (byte != next_byte) {
            x = (s16)L16(0x30);
            if (x <= 500) x = 500;
            t = (long double)(s8)byte;
            t = t * (long double)(s16)L16(0x40);
            u = (long double)x;
            u = u * (long double)(s8)next_byte;
            t = t / u;
            if (C(0x967898) > t) t = C(0x967898);
            else if (C(0x9688b0) < t) t = C(0x9688b0);
            a = D(current + 0x53);
            b = D(d + 0x53);
            b = D(b + 0x71);
            a = D(a + 0x71);
            v = v * t;
            if (b != a && t < C(0x967890)) {
                v = v * C(0x968918);
                if (output) D(output) = D(output) | 0x2000000;
            }
        } else {
            a = D(p + 0x1a);
            a = D(a + 0x71);
            if (a) {
                b = D(0x9cfa10);
                c = D(a);
                if (c == b) {
                    a = D(d + 0x53);
                    a = D(a + 0x71);
                    if (a && D(a) != b) {
                        c = D(current + 0x53);
                        if (a != D(c + 0x71)) {
                            v = v * C(0x968e00);
                            if (output) D(output) = D(output) | 0x2000000;
                        }
                    }
                }
            }
        }
    }
    v = v * L64(0x24);
    L64(0x14) = v;
    if (output && L64(0x24) < C(0x968e48)) D(output) = D(output) | 0x800000;
final_adjustment:
    if (!flag || !(v >= C(0x968f98))) goto done;
    cm_fn_0085e410(A(0x9c));
    a = D(0xae2a38);
    scratch.layout.seh.state = 0;
    r = cm_fn_005a0590(a, d);
    if (!r) goto zero;
    u = cm_fn_00864550(p, d, 0, 0xffffffffu, mode, 0);
    if (C(0x967880) > L64(0x14)) t = C(0x967880);
    else t = L64(0x14);
    L64(0x24) = C(0x968e38) / t;
    if (!(L64(0x24) >= C(0x9689d8))) {
        L32(0x24) = 0xcccccccd;
        L32(0x28) = 0x3feccccc;
    }
    /* 008466e9 has one live pushed argument: [esp+80] is S+7c. */
    L64(0x7c) = (long double)L64(0x24) * u;
    a = cm_fn_00597db0(r, mode);
    L32(0x34) = a;
    L64(0x64) = (long double)(s32)a;
    if (L64(0x7c) > L64(0x64) && L64(0x7c) > C(0x9689a0)) {
        v = ((long double)L64(0x64) / L64(0x7c)) * L64(0x14);
        t = L64(0x64);
    } else {
        v = L64(0x14);
        t = L64(0x7c);
    }
    a = L32(0x58);
    if (a) {
        u = (long double)SD(a + 12);
        if (u < C(0x9688b0)) u = C(0x9688b0);
        t = t / u;
        if (!(C(0x967880) > t)) {
            if (!(C(0x9688b0) >= t)) {
                t = C(0x9688b0) / L64(0x24);
                v = v * t;
            } else if (!(t < C(0x967890))) {
                t = t / L64(0x24);
                v = v * t;
            }
        }
    }
    goto done;
zero:
    v = C(0x9689a0);
done:
    CM_FS(u32, 0) = scratch.layout.seh.prev;
    return v;
}
