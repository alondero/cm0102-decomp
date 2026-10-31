#include "cmre.h"
/* Preferred-image VA 0x007c2150, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Long-double intermediates retain the original x87 evaluation precision. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile s16 *)(u32)(a))
#define F(a) CM_GLOBAL(volatile float, a)
#define Q(a) CM_GLOBAL(volatile double, a)

long double CM_STDCALL cm_fn_007c2150(u32 stats, s8 kind, float incoming)
{
    u8 scratch[0x208];
    u8 *first, *second;
    u32 target, offset, count, line, numerator;
    s32 integer;
    s16 denominator;
    long double value, extra;
    char *version;
    if (stats == 0) {
        first = scratch + 0x108; second = scratch + 8; line = 0x6d9;
        goto error;
    }
    if ((u32)((s32)kind - 1) > 21u)
        goto invalid;
    target = CM_GLOBAL(volatile u32, 0x007c254cu + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x007c21ad: offset = 0; break;
    case 0x007c21c3: offset = 2; break;
    case 0x007c21da: offset = 3; break;
    case 0x007c21f1: offset = 4; break;
    case 0x007c2208: offset = 6; break;
    case 0x007c221f: offset = 7; break;
    case 0x007c2236: offset = 8; break;
    case 0x007c224d: offset = 9; break;
    case 0x007c2264: offset = 10; break;
    case 0x007c227b: offset = 11; break;
    case 0x007c2292: offset = 12; break;
    case 0x007c22a9: offset = 13; break;
    case 0x007c22c0:
        count = B(stats);
        if (count == 0) goto zero;
        integer = W(stats + 14u);
        return (long double)integer / (long double)count;
    case 0x007c22ee:
        denominator = W(stats + 16u);
        if (denominator <= 0) goto zero;
        integer = W(stats + 18u);
        value = (long double)integer / (long double)denominator;
        return value * (long double)F(0x00968960);
    case 0x007c2322:
        count = B(stats);
        if (count == 0) goto zero;
        integer = W(stats + 20u);
        return (long double)integer / (long double)count;
    case 0x007c2350:
        count = B(stats);
        if (count == 0) goto zero;
        integer = W(stats + 22u);
        return (long double)integer / (long double)count;
    case 0x007c237e:
        denominator = W(stats + 24u);
        if (denominator <= 0) goto zero;
        integer = W(stats + 26u);
        value = (long double)integer / (long double)denominator;
        return value * (long double)F(0x00968960);
    case 0x007c23b2:
        count = B(stats);
        integer = W(stats + 14u);
        if (count < 20) {
            value = (long double)(20u - count);
            value *= (long double)incoming;
            value += (long double)integer;
            return value * (long double)F(0x0096d61c);
        }
        return (long double)integer / (long double)count;
    case 0x007c2415:
        count = B(stats);
        if (count == 0)
            return (long double)F(0x0096b004);
        numerator = B(stats + 9u);
        offset = B(stats + 10u);
        numerator = numerator * 2u - offset + count;
        value = (long double)(s32)numerator;
        if (count < 20) {
            extra = (long double)(20u - count);
            extra *= (long double)Q(0x00968940);
            value += extra;
            return value * (long double)F(0x0096d61c);
        }
        return value / (long double)count;
    case 0x007c24a5:
        count = B(stats);
        if (count == 0) goto zero;
        integer = W(stats + 24u);
        return (long double)integer / (long double)count;
    case 0x007c24d3:
        integer = *(volatile s32 *)(stats + 28u);
        return (long double)integer;
    default: goto invalid;
    }
    count = B(stats + offset);
    return (long double)count;
invalid:
    first = scratch + 8; second = scratch + 0x108; line = 0x7ab;
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
zero:
    return (long double)F(0x00968970);
}
