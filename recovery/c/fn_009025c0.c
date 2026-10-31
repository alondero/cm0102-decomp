#include "cmre.h"

/* Preferred-image VA009025C0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Allocate nine records, then report eight or nine
   after the optional first row. Every signed BYTE B2 read stays fresh. */
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

u32 CM_THISCALL cm_fn_009025c0(u32 self, u32 selector, volatile u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    static const struct {
        u32 first[4], last[5];
        u16 value7, valueb, valued, value18, value1a, value1c;
        u8 value17, value21, value22;
    } rows[8] = {
        {{28,6,0,4},{10,7,0,3,2},220,4,0,82,41,0,7,2,14},
        {{25,7,0,4},{14,8,0,3,2},10,4,0x400,96,48,55,7,2,14},
        {{29,8,0,4},{19,9,0,3,2},20,4,0,48,24,0,7,2,14},
        {{3,10,0,4},{23,10,0,3,2},30,0,0,32,16,8,8,2,14},
        {{8,11,0,4},{1,2,1,3,2},40,0,0,16,8,0,8,2,7},
        {{9,2,1,4},{15,2,1,3,2},120,0,0,8,4,0,8,2,7},
        {{23,2,1,4},{5,3,1,3,2},130,0,0,4,2,0,8,2,14},
        {{20,3,1,4},{16,4,1,2,2},150,0,0x83,2,1,0,8,1,0}
    };
    u8 scratch[0x200];
    u32 L = (u32)scratch, p, i, index = 0, r, year, version, count_pointer;
    s32 number;
    if ((u8)selector != 0xff) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    count_pointer = count_out;
    W(count_pointer) = 9;
    W(zero_out) = 0;
    p = cm_fn_00944e46(0x3a8);
    if (p == 0) {
        cm_fn_00944cff(0xaa19e0, 0, 0, (void *)(L + 0x100), (void *)L);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, L + 0x100, L, 0xe0);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    if ((s8)B(self + 0xb2) > 0) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, 0, 28, 5, 0, 2, year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, 0, 6, 6, 0, 3, 2, year, 0);
        W(p + 7) = 160; W(p + 9) = 0; W(p + 0xb) = 0;
        W(p + 0xd) = 0x200; W(p + 0xf) = 3; B(p + 0x17) = 7;
        number = (s8)B(self + 0xb2); W(p + 0x18) = (u16)number;
        number = (s8)B(self + 0xb2);
        index = 1;
        W(p + 0x1a) = (u16)(number / 2);
        number = (s8)B(self + 0xb2); W(p + 0x1c) = (u16)number;
        W(p + 0x1e) = 0;
        B(p + 0x20) = 0; B(p + 0x21) = 2; B(p + 0x22) = 14;
        D(p + 0x5c) = 0; D(p + 0x60) = 0xffffffff; D(p + 0x64) = 0;
    }
    for (i = 0; i != 8; ++i) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, index, rows[i].first[0], rows[i].first[1],
                      rows[i].first[2], rows[i].first[3], year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, index, rows[i].last[0], rows[i].last[1], rows[i].last[2],
                      rows[i].last[3], rows[i].last[4], year, i == 7 ? 4 : 0);
        r = p + index * 104u;
        W(r + 7) = rows[i].value7;
        W(r + 9) = (u16)index;
        W(r + 0xb) = rows[i].valueb;
        W(r + 0xd) = rows[i].valued;
        if (i < 2) W(r + 0xf) = 3;
        B(r + 0x17) = rows[i].value17;
        W(r + 0x18) = rows[i].value18;
        W(r + 0x1a) = rows[i].value1a;
        if (i == 0) {
            number = (s8)B(self + 0xb2);
            ++index;
            W(r + 0x1c) = (u16)(82 - number / 2);
        } else W(r + 0x1c) = rows[i].value1c;
        if (i >= 2) W(r + 0xf) = i == 7 ? 0 : 3;
        if (i == 0) {
            number = (s8)B(self + 0xb2);
            W(r + 0x1e) = (u16)number;
        } else if (i == 1 || i == 3) {
            number = (s8)B(self + 0xb2);
            ++index;
            W(r + 0x1e) = (u16)(number / 2 + (i == 1 ? 82 : 137));
        } else W(r + 0x1e) = 0;
        B(r + 0x20) = 0;
        B(r + 0x21) = rows[i].value21;
        B(r + 0x22) = rows[i].value22;
        D(r + 0x5c) = 0;
        D(r + 0x60) = i == 7 ? 1500000 : 0xffffffff;
        D(r + 0x64) = 0;
        if (i == 2 || i >= 4) ++index;
    }
    count_pointer = count_out;
    W(count_pointer) = (u16)index;
    return p;
}
