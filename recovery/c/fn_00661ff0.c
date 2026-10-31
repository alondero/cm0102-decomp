#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA00661ff0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Only low input byte FF initializes seven 104-byte
   rows. Each original helper tuple and store order independently checked.
   Scratch200 has error buffersL0/L100; date helpers remain original. */
u32 CM_THISCALL cm_fn_00661ff0(u32 self, u32 mode_arg,
    volatile u32 count_arg, volatile u32 code_arg, volatile u32 optional_arg)
{
    static const struct {
        u32 date1[4], date2[5], date_last;
        u32 value7, byte17, value18, value1a, value1c, value1e, early_f;
    } rows[7] = {
        {{1,10,0,6},{29,10,0,6,1},0, 10,4,64,32,64,0,0},
        {{30,10,0,0},{6,11,0,6,1},0, 20,4,32,16,0,0,1},
        {{7,11,0,0},{13,11,0,6,1},0, 30,4,32,16,16,64,1},
        {{14,11,0,0},{20,11,0,6,1},0, 40,4,16,8,0,0,0},
        {{21,11,0,0},{23,11,0,2,2},0, 120,6,8,4,0,0,0},
        {{24,11,0,3},{27,11,0,6,1},0, 130,6,4,2,0,0,0},
        {{28,11,0,0},{2,0,1,5,1},4, 150,6,2,1,0,0,0}
    };
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, optional, count, code, allocation, year, i, dst, version;
    if ((u8)mode_arg != 0xff)
        return 0;
    optional = optional_arg;
    if (optional != 0)
        D(optional) = 0;
    count = count_arg;
    code = code_arg;
    W(count) = 7;
    W(code) = 0;
    allocation = (u32)cm_fn_00944e46(0x2d8);
    if (allocation == 0) {
        cm_fn_00944cff(0x009eab90, 0, 0, l + 0x100, l);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0xa0);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    for (i = 0; i != 7; ++i) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(allocation, i, rows[i].date1[0], rows[i].date1[1],
            rows[i].date1[2], rows[i].date1[3], year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(allocation, i, rows[i].date2[0], rows[i].date2[1],
            rows[i].date2[2], rows[i].date2[3], rows[i].date2[4], year, rows[i].date_last);
        dst = allocation + i * 0x68u;
        W(dst + 7) = (u16)rows[i].value7;
        W(dst + 9) = (u16)i;
        W(dst + 0xb) = 0;
        W(dst + 0xd) = 0x83;
        if (rows[i].early_f)
            W(dst + 0xf) = 0;
        B(dst + 0x17) = (u8)rows[i].byte17;
        W(dst + 0x18) = (u16)rows[i].value18;
        W(dst + 0x1a) = (u16)rows[i].value1a;
        W(dst + 0x1c) = (u16)rows[i].value1c;
        if (!rows[i].early_f)
            W(dst + 0xf) = 0;
        W(dst + 0x1e) = (u16)rows[i].value1e;
        B(dst + 0x20) = 0;
        B(dst + 0x21) = 1;
        B(dst + 0x22) = 0;
        D(dst + 0x5c) = 0;
        D(dst + 0x60) = 0xffffffffu;
        D(dst + 0x64) = 0;
    }
    return allocation;
}
