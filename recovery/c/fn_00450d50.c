#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00448110(u32, u32);
extern u32 CM_THISCALL cm_fn_00448170(u32, u32);
extern u32 CM_THISCALL cm_fn_00448230(u32, u32);
extern u32 CM_THISCALL cm_fn_004484d0(u32, u32);
extern u32 CM_THISCALL cm_fn_00448500(u32, u32);
extern u32 CM_THISCALL cm_fn_00448530(u32, u32);
extern u32 CM_THISCALL cm_fn_00448550(u32, u32);
extern u32 CM_THISCALL cm_fn_004485b0(u32, u32);
extern u32 CM_THISCALL cm_fn_00451200(u32, u32);
extern u32 CM_THISCALL cm_fn_00451570(u32, u32);
extern u32 CM_THISCALL cm_fn_00452710(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004528d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00452a90(u32, u32);
extern u32 CM_CDECL cm_fn_00452fe0(u32);
extern u32 CM_THISCALL cm_fn_004a1fb0(u32, u32);
extern u32 CM_THISCALL cm_fn_004ab5b0(u32, u32);
extern u32 CM_THISCALL cm_fn_0058f120(u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u16 CM_CDECL cm_fn_00669950(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1d0(u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);

/* Preferred-image VA00450d50/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full EAX. Scratch208: saved receiverL0, second recordL4,
   error buffersL8/L108. All callees remain original boundaries. */
u32 CM_THISCALL cm_fn_00450d50(u32 self, volatile u32 club_arg, u32 count_arg)
{
    u32 scratch[0x208 / 4];
    u32 l = (u32)scratch, count = count_arg, club = club_arg;
    u32 first, second, allocation, dst, n, a, b, owner, list, item, version;
    u32 error_first, error_second, line;
    u16 year;
    D(count) = 0;
    D(l) = self;
    if (club == 0) {
        cm_fn_00944cff(0x0098b6a0, 0, 0, l + 0x108, l + 8);
        error_first = l + 0x108;
        error_second = l + 8;
        line = 0x14c1;
        goto error;
    }
    second = cm_fn_004528d0(self, club, 0);
    D(l + 4) = second;
    first = cm_fn_00452710(self, club, 0);
    allocation = (u32)cm_fn_00944e46(0x6ef64);
    if (allocation == 0) {
        cm_fn_00944cff(0x0098b6a0, 0, 0, l + 8, l + 0x108);
        error_first = l + 8;
        error_second = l + 0x108;
        line = 0x14cd;
        goto error;
    }
    n = D(count);
    W(allocation + n * 0x38du) = 0;
    n = D(count);
    dst = allocation + n * 0x38du + 2;
    if (first != 0) {
        D(dst) = D(first);
        W(dst + 4) = W(first + 4);
        B(dst + 6) = B(first + 6);
        D(dst + 7) = D(first + 7);
        W(dst + 0xb) = W(first + 0xb);
        B(dst + 0xd) = B(first + 0xd);
        D(dst + 0xe) = D(first + 0xe);
        cm_fn_00448110(dst + 0x12, first + 0x12);
        D(dst + 0x31) = D(first + 0x31);
        cm_fn_00448110(dst + 0x35, first + 0x35);
        cm_fn_004ab5b0(dst + 0x54, first + 0x54);
        D(dst + 0x77) = D(first + 0x77);
        W(dst + 0x7b) = W(first + 0x7b);
        cm_fn_00448110(dst + 0x7d, first + 0x7d);
        cm_fn_00448110(dst + 0x9c, first + 0x9c);
        cm_fn_00448110(dst + 0xbb, first + 0xbb);
        cm_fn_00448110(dst + 0xda, first + 0xda);
        cm_fn_00448110(dst + 0xf9, first + 0xf9);
        cm_fn_00448170(dst + 0x118, first + 0x118);
        cm_fn_00448170(dst + 0x12c, first + 0x12c);
        cm_fn_00448170(dst + 0x140, first + 0x140);
        cm_fn_00448170(dst + 0x154, first + 0x154);
        cm_fn_004484d0(dst + 0x168, first + 0x168);
        cm_fn_004484d0(dst + 0x183, first + 0x183);
        cm_fn_00451570(dst + 0x19e, first + 0x19e);
        cm_fn_004484d0(dst + 0x1d6, first + 0x1d6);
        cm_fn_00448230(dst + 0x1f1, first + 0x1f1);
        cm_fn_004484d0(dst + 0x210, first + 0x210);
        cm_fn_00448500(dst + 0x22b, first + 0x22b);
        cm_fn_00448530(dst + 0x247, first + 0x247);
        cm_fn_00448530(dst + 0x261, first + 0x261);
        cm_fn_004a1fb0(dst + 0x27b, first + 0x27b);
        cm_fn_004a1fb0(dst + 0x2ba, first + 0x2ba);
        cm_fn_00448530(dst + 0x2f9, first + 0x2f9);
        cm_fn_00448550(dst + 0x313, first + 0x313);
        cm_fn_00448550(dst + 0x33c, first + 0x33c);
        D(dst + 0x365) = D(first + 0x365);
        W(dst + 0x369) = W(first + 0x369);
        D(dst + 0x36b) = D(first + 0x36b);
        W(dst + 0x36f) = W(first + 0x36f);
        cm_fn_004485b0(dst + 0x371, first + 0x371);
        club = club_arg;
    } else {
        cm_fn_00452fe0(dst);
    }
    n = D(count);
    second = D(l + 4);
    D(count) = n + 1;
    if (second != 0) {
        long double value = CM_GLOBAL(volatile float, second + 0x1f1);
        long double limit = CM_GLOBAL(volatile double, 0x00968948);
        /* FCOMP C3 is set for equality and unordered operands. */
        if (!__builtin_islessgreater(value, limit)) {
            owner = D(l);
            cm_fn_00452a90(owner, club);
        }
    }
    year = cm_fn_00669950(club);
    n = D(count);
    W(allocation + n * 0x38du) = year;
    n = D(count);
    dst = allocation + n * 0x38du + 2;
    if (second != 0)
        cm_fn_00451200(dst, second);
    else
        cm_fn_00452fe0(dst);
    n = D(count);
    D(count) = n + 1;
    a = D(club);
    owner = D(l);
    b = D(owner + 0x1c);
    owner = D(owner);
    b += a * 0x6au;
    a = D(b + 0xc);
    b = D(b + 8);
    list = cm_fn_0058f120(owner, b, a);
    if (list != 0) {
        D(list + 0xa) = D(list + 6);
        item = cm_fn_0068d1d0(list);
        while (item != 0) {
            n = D(count);
            if (n == 500)
                break;
            dst = allocation + n * 0x38du;
            D(count) = n + 1;
            year = W(item);
            W(dst) = year;
            cm_fn_00451200(dst + 2, item + 2);
            item = cm_fn_0068d1d0(list);
        }
        cm_fn_0068d040(list);
        cm_fn_00944c94(list);
    }
    return allocation;
error:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, error_first, error_second, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
