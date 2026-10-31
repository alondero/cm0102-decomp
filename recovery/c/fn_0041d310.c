#include "cmre.h"

/* Preferred-image VA0041D310/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Each of eight rows follows its original date
   tuples, constants and field-write order; date callees remain original. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0041d310(u32 self, u32 selector, u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    static const struct {
        u32 first[4], last[5];
        u16 value7, valued, valuef, value18, value1a, value1c, value1e;
        u8 value17, value20, value21, value22;
    } rows[8] = {
        {{6,7,0,6}, {12,7,0,5,2},30,0x401,0,76,38,76,0,4,0,1,0},
        {{13,7,0,6},{19,7,0,5,2},40,0x403,0,56,28,18,76,4,0,1,0},
        {{20,7,0,6},{26,7,0,5,2},50,0x403,0,28,14,0,0,4,0,1,0},
        {{15,8,0,6},{5,10,0,6,2},60,3,0,32,16,18,94,4,0,1,0},
        {{6,10,0,0},{29,10,0,2,2},70,3,0,16,8,0,0,4,0,1,0},
        {{30,10,0,3},{24,0,1,2,2},120,2,3,8,4,0,0,6,1,1,0},
        {{25,0,1,3},{28,2,1,2,2},130,0,3,4,2,0,0,6,0,2,14},
        {{12,3,1,3},{27,4,1,6,2},150,2,3,2,1,0,0,6,1,1,6}
    };
    u8 scratch[0x200];
    u32 L = (u32)scratch, p, i, r, year, version;
    if ((u8)selector != 0xff) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 8;
    W(zero_out) = 0;
    p = cm_fn_00944e46(0x340);
    if (p == 0) {
        cm_fn_00944cff(0x989ae0, 0, 0, (void *)(L + 0x100), (void *)L);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, L + 0x100, L, 0xa9);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    for (i = 0; i != 8; ++i) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, i, rows[i].first[0], rows[i].first[1],
                      rows[i].first[2], rows[i].first[3], year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, i, rows[i].last[0], rows[i].last[1], rows[i].last[2],
                      rows[i].last[3], rows[i].last[4], year, i == 7 ? 4 : 0);
        r = p + i * 104u;
        W(r + 7) = rows[i].value7;
        W(r + 9) = (u16)i;
        W(r + 0xb) = 0;
        W(r + 0xd) = rows[i].valued;
        if (i == 2 || i == 3) W(r + 0xf) = rows[i].valuef;
        B(r + 0x17) = rows[i].value17;
        W(r + 0x18) = rows[i].value18;
        W(r + 0x1a) = rows[i].value1a;
        W(r + 0x1c) = rows[i].value1c;
        if (i != 2 && i != 3) W(r + 0xf) = rows[i].valuef;
        W(r + 0x1e) = rows[i].value1e;
        B(r + 0x20) = rows[i].value20;
        B(r + 0x21) = rows[i].value21;
        B(r + 0x22) = rows[i].value22;
        D(r + 0x5c) = 0;
        D(r + 0x60) = i < 3 ? 0 : 0xffffffff;
        D(r + 0x64) = 0;
    }
    return p;
}
