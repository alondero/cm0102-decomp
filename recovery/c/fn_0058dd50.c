#include "cmre.h"
/* Preferred-image VAs, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void *CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_THISCALL cm_fn_0058d610(u32, u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define F(a) CM_GLOBAL(volatile double, (a))
#define S(a) CM_GLOBAL(volatile float, (a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0058dd50(u32 self)
{
    /* L is ESP immediately after SUB 204. Scratch bytes are uninitialized. */
    u8 scratch[0x204];
    u32 a, b, c, d, offset, index;
    long double saved, conversion;
    volatile long long converted;
    a = D(0x00ae2350);
    a = (u32)cm_fn_00945501(a, 0x2c);
    D(self) = a;
    if (!a) {
        cm_fn_00944cff(0x009c74f4, 0, 0, scratch + 0x104, scratch + 4);
        a = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, a,
                      scratch + 0x104, scratch + 4, 0x2da);
        a = cm_fn_005e8290((const char *)0x009870e0,
                          (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return a;
    }
    a = D(0x00ae2350);
    index = 0;
    offset = 0;
    if ((s32)a > 0) {
        D((u32)scratch) = 0;
        do {
            d = D(self);
            c = D((u32)scratch);
            D(offset + d) = index;
            a = D(0x00ae23a8);
            saved = (long double)F(c + a + 0xa8);
            saved *= F(0x0096a348);
            conversion = saved;
            conversion *= F(0x00968908);
            /* ADR0006: physical __ftol CALL 0058de0b consumes ST0;
               the duplicated lower value survives for FSTP m32 0058de27. */
            converted = (long long)conversion;
            a = (u32)converted;
            c = offset + 0x14;
            d = 6;
            do {
                b = D(self);
                c += 4;
                --d;
                D(c + b - 4) = a;
            } while (d != 0);
            d = D(self);
            ++index;
            S(offset + d + 0xc) = (float)saved;
            d = D((u32)scratch);
            a = D(0x00ae2350);
            d += 0x122;
            offset += 0x2c;
            D((u32)scratch) = d;
        } while ((s32)index < (s32)a);
    }
    a = cm_fn_0058d610(self, (u32)scratch);
    d = D((u32)scratch);
    c = 0;
    if ((s32)d > 0) {
        d = a;
        do {
            b = D(d);
            d += 4;
            offset = (offset & 0xffff0000u) | W(b + 0x10);
            W(b + 0x12) = (u16)offset;
            b = D(d - 4);
            index = c + 1;
            ++c;
            W(b + 0x10) = (u16)index;
            index = D((u32)scratch);
        } while ((s32)c < (s32)index);
    }
    b = (u32)cm_fn_009452ca(a);
    /* Native 0058de7f forwards EAX=a. At a==0, 009452d4 branches
       directly to 00945330..332 without reading or changing EAX.
       Preserve that original zero result while retaining the helper call. */
    return a == 0 ? a : b;
}
