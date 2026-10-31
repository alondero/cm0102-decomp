#include "cmre.h"

/* Preferred-image VA007CDC40/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Each row's date arguments, field values and
   write order are checked against its own original instructions. */
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

u32 CM_THISCALL cm_fn_007cdc40(u32 self, u32 selector, u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    static const struct {
        u32 first[4], last[5];
        u16 value7, valued, valuef, value18, value1a, value1c, value1e;
        u8 value17, value20, value22;
    } rows[8] = {
        {{10,8,0,4},{19,8,0,6,1},20,2,3,128,64,128,0,4,1,3},
        {{23,8,0,3},{5,9,0,1,1},30,2,3,82,41,18,128,4,1,8},
        {{18,9,0,0},{14,10,0,6,1},40,2,3,58,29,17,146,4,1,17},
        {{29,11,0,0},{9,0,1,6,1},50,2,3,30,15,1,163,4,1,0},
        {{10,0,1,0},{26,0,1,2,2},60,2,3,15,7,0,0,4,1,7},
        {{3,1,1,3},{9,1,1,2,2},120,2,3,8,4,0,0,6,1,7},
        {{20,2,1,0},{12,3,1,2,2},130,2,3,4,2,0,0,6,1,7},
        {{13,3,1,3},{21,4,1,6,1},150,3,0,2,1,0,0,6,0,4}
    };
    u8 scratch[0x200];
    u32 L = (u32)scratch, p, i, r, year, version;
    if ((u8)selector != 0xff) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 8;
    W(zero_out) = 0;
    p = cm_fn_00944e46(0x340);
    if (p == 0) {
        cm_fn_00944cff(0xa786f0, 0, 0, (void *)(L + 0x100), (void *)L);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, L + 0x100, L, 0xa3);
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
        B(r + 0x21) = 1;
        B(r + 0x22) = rows[i].value22;
        D(r + 0x5c) = 0;
        D(r + 0x60) = 0xffffffff;
        D(r + 0x64) = 0;
    }
    return p;
}
