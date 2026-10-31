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

/* Preferred-image VA007c8040/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. All seven original tuples independently checked;
   row5 date1 argument6 is the full EAX=4 established at007C8377, retained
   through the intervening stores. Scratch200 error buffersL0/L100.
   Signed WORD years reload before each original date call. */
u32 CM_THISCALL cm_fn_007c8040(u32 self, u32 mode_arg,
    volatile u32 count_arg, volatile u32 code_arg, volatile u32 optional_arg)
{
    static const struct {
        u32 date1[4], date2[5];
        u32 value7, valueb, valued, valuef, byte17, value18, value1a, value1c, value1e;
        u32 byte21, byte22, value60;
    } rows[7] = {
        {{6,7,0,4},{9,7,0,2,2},10,0,0x403,0,4,8,4,8,0,1,0,0},
        {{11,7,0,4},{13,8,0,2,2},20,0,3,0,4,32,16,28,8,1,0,0},
        {{15,8,0,4},{23,8,0,5,1},30,8,0x403,0,4,32,16,16,36,1,0,0},
        {{29,8,0,4},{8,10,0,2,2},40,0,0,3,4,16,8,0,0,2,14,0},
        {{3,2,1,4},{14,2,1,2,2},120,0,0,3,4,8,4,0,0,2,14,0},
        {{1,3,1,4},{11,3,1,2,2},130,0,0,3,6,4,2,0,0,2,7,0},
        {{19,3,1,3},{16,4,1,1,2},150,0,0,3,6,2,1,0,0,2,7,500000}
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
        cm_fn_00944cff(0x00a78564, 0, 0, l + 0x100, l);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0x9d);
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
            rows[i].date2[2], rows[i].date2[3], rows[i].date2[4], year, 0);
        dst = allocation + i * 0x68u;
        W(dst + 7) = (u16)rows[i].value7;
        W(dst + 9) = (u16)i;
        W(dst + 0xb) = (u16)rows[i].valueb;
        W(dst + 0xd) = (u16)rows[i].valued;
        if (i >= 2)
            W(dst + 0xf) = (u16)rows[i].valuef;
        B(dst + 0x17) = (u8)rows[i].byte17;
        W(dst + 0x18) = (u16)rows[i].value18;
        W(dst + 0x1a) = (u16)rows[i].value1a;
        W(dst + 0x1c) = (u16)rows[i].value1c;
        if (i < 2)
            W(dst + 0xf) = (u16)rows[i].valuef;
        W(dst + 0x1e) = (u16)rows[i].value1e;
        B(dst + 0x20) = 0;
        B(dst + 0x21) = (u8)rows[i].byte21;
        B(dst + 0x22) = (u8)rows[i].byte22;
        D(dst + 0x5c) = 0;
        D(dst + 0x60) = rows[i].value60;
        D(dst + 0x64) = 0;
    }
    return allocation;
}
