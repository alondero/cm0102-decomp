#include "cmre.h"
extern int CM_CDECL cm_fn_0090cfc0(int);
#define R32(a) CM_GLOBAL(volatile u32, (a))
#define R16(a) CM_GLOBAL(volatile s16, (a))
#define R8(a) CM_GLOBAL(volatile s8, (a))
#define D(a) CM_GLOBAL(volatile double, (a))
/* Preferred-image VAs; primary SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059b7e0(u32 self, u32 item)
{
    u32 a, p, q, flag, factor, value, index, base, low, b, c, sum, high;
    s32 score;
    long double t;
    volatile long long converted;
    a = R32(item + 0xbf);
    if (a) {
        a = R32(a + 0x69);
        if (a) {
            flag = (u8)R8(item + 0x6d);
            a = (u32)(s32)R8(a + 0xf);
            score = R16(item + 0x80);
            a = a * 25u + (u32)score;
            if (!flag) {
                t = (long double)(s32)a;
                t = t * D(0x0096a5d0);
                /* registered __ftol, physical call 0x0059b82e: low DWORD consumed */
                converted = (long long)t; a = (u32)converted;
            }
            t = (long double)(s32)R32(item + 0x73);
            t = t * (long double)score;
            if ((s32)a > 0x157c) t = t * D(0x0096a6d0);
            else if ((s32)a > 0xfa0) t = t * D(0x0096a6c8);
            else t = t * D(0x0096a6b8);
            goto convert;
        }
    }
    score = R16(item + 0x80);
    p = R32(item + 0x53);
    if (p) {
        q = R32(p);
        value = R32(0x009cf36c); /* native MOV EDX,m32 at 59b87f/8ce/90e */
        flag = q == value;
    } else flag = 0;
    if (score > 0x1482) factor = flag ? 0x0096a6c0u : 0x0096a6d0u;
    else if (score > 0xea6) factor = flag ? 0x0096a6d0u : 0x0096a6c8u;
    else factor = flag ? 0x0096a6c8u : 0x0096a6b8u;
    t = (long double)(s32)R32(item + 0x73);
    t = t * (long double)score;
    t = t * D(factor);
convert:
    /* registered __ftol, physical call 0x0059b946: low DWORD consumed */
    converted = (long long)t; a = (u32)converted;
    value = (u32)cm_fn_0090cfc0(11);
    value = (value + 95u) * a;
    t = (long double)(s32)value;
    t = t * D(0x00967898);
    /* registered __ftol, physical call 0x0059b96b: low DWORD then CDQ */
    converted = (long long)t; a = (u32)converted;
    index = R32(item);
    base = R32(self);
    base = base + index * 359u;
    low = R32(base);
    b = R32(base + 0xbc);
    c = R32(base + 0x15c);
    b += a;
    c += a;
    sum = low + a;
    R32(base + 0x15c) = c;
    R32(base) = sum;
    high = R32(base + 4);
    high += (0u - (a >> 31)) + (sum < low);
    R32(base + 0xbc) = b;
    R32(base + 4) = high;
    return high;
}
