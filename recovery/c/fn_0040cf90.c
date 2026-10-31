#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521f10(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA0040cf90/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Scratch200: error buffersL0/L100. All six
   initializer rows and three extra calls checked against their instructions. */
u32 CM_THISCALL cm_fn_0040cf90(u32 self, u32 mode_arg,
    volatile u32 count_arg, volatile u32 code_arg, volatile u32 optional_arg)
{
    static const struct {
        u32 date1[4], date2[5], date_last;
        u32 value7, valued, valuef, byte17, value18, value1a, value1c;
        u32 byte21, byte22;
    } rows[6] = {
        {{1,7,0,5},{23,7,0,2,2},0, 10,0,0x83,4,16,8,16,2,21},
        {{14,8,0,3},{20,8,0,2,2},0, 20,0,0x83,4,8,4,0,2,7},
        {{28,8,0,3},{24,2,1,2,2},0, 120,0,0x83,4,4,2,0,2,7},
        {{15,3,1,6},{24,4,1,3,2},1, 130,0x383,0,6,4,2,4,1,0},
        {{25,4,1,4},{26,4,1,5,2},16, 150,0x83,0,6,2,1,0,1,0},
        {{25,4,1,4},{26,4,1,5,1},16, 140,0x83,0,6,2,1,0,1,0}
    };
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, optional, count, code, allocation, year, i, row, dst;
    u32 first_error, second_error, line, version;
    u8 mode = (u8)mode_arg;
    if (mode != 0xff && mode != 0 && mode != 1)
        return 0;
    optional = optional_arg;
    if (optional != 0)
        D(optional) = 0;
    count = count_arg;
    W(count) = 3;
    code = code_arg;
    if (mode == 1) {
        if (code != 0)
            W(code) = 0;
        first_error = l + 0x100;
        second_error = l;
        line = 0x112;
    } else {
        W(code) = mode == 0xff ? 0x480 : 0x481;
        first_error = l;
        second_error = l + 0x100;
        line = 0xc6;
    }
    allocation = (u32)cm_fn_00944e46(0x138);
    if (allocation == 0) {
        cm_fn_00944cff(0x00987b60, 0, 0, first_error, second_error);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first_error, second_error, line);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    for (i = 0; i != 3; ++i) {
        row = mode == 1 ? i + 3 : i;
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(allocation, i, rows[row].date1[0], rows[row].date1[1],
            rows[row].date1[2], rows[row].date1[3], year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(allocation, i, rows[row].date2[0], rows[row].date2[1],
            rows[row].date2[2], rows[row].date2[3], rows[row].date2[4],
            year, rows[row].date_last);
        if (mode == 1 && i == 0) {
            cm_fn_00521f10(allocation, 0, 0, 3, 1, 3, 16);
            cm_fn_00521f10(allocation, 0, 1, 3, 2, 3, 16);
            cm_fn_00521f10(allocation, 0, 2, 0xffffffffu, 0xffffffffu, 0xffffffffu, 0);
        }
        dst = allocation + i * 0x68u;
        W(dst + 7) = (u16)rows[row].value7;
        W(dst + 9) = (u16)i;
        W(dst + 0xb) = 0;
        W(dst + 0xd) = (u16)rows[row].valued;
        if (i != 0)
            W(dst + 0xf) = (u16)rows[row].valuef;
        B(dst + 0x17) = (u8)rows[row].byte17;
        W(dst + 0x18) = (u16)rows[row].value18;
        W(dst + 0x1a) = (u16)rows[row].value1a;
        W(dst + 0x1c) = (u16)rows[row].value1c;
        if (i == 0)
            W(dst + 0xf) = (u16)rows[row].valuef;
        W(dst + 0x1e) = 0;
        B(dst + 0x20) = 0;
        B(dst + 0x21) = (u8)rows[row].byte21;
        B(dst + 0x22) = (u8)rows[row].byte22;
        D(dst + 0x5c) = 0;
        D(dst + 0x60) = 0;
        D(dst + 0x64) = 0;
    }
    return allocation;
}
