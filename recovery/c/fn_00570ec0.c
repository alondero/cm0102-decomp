#include "cmre.h"

/* Preferred-image VA 0x00570ec0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Row arguments and stores follow each original block. In particular, rows
   3 and 4 retain their non-monotonic WORD store order. */
extern void CM_CDECL cm_fn_00521e60(u32, s16, u32, u32, u32, u32, s32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static const struct {
    u32 day, month, flag, value, position, setting, extra, begin, end;
} rows[5] = {
    {8,10,0, 25,10,0,0, 0, 17},
    {26,10,0, 16,0,1,0, 17, 34},
    {17,0,1, 6,1,1,0, 34, 51},
    {7,1,1, 10,3,1,0, 51, 68},
    {11,3,1, 15,4,1,16384, 68, 84}
};

static const struct { u32 width, offset, value; } stores[84] = {
    {2, 0x7, 0x14u},
    {2, 0x9, 0x0u},
    {2, 0xb, 0x0u},
    {2, 0xd, 0x0u},
    {1, 0x17, 0x4u},
    {2, 0x18, 0x20u},
    {2, 0x1a, 0x10u},
    {2, 0x1c, 0x20u},
    {2, 0xf, 0x2u},
    {2, 0x11, 0x3u},
    {2, 0x1e, 0x0u},
    {1, 0x20, 0x2u},
    {1, 0x21, 0x1u},
    {1, 0x22, 0x0u},
    {4, 0x5c, 0x0u},
    {4, 0x60, 0xffffffffu},
    {4, 0x64, 0x0u},
    {2, 0x6f, 0x1eu},
    {2, 0x71, 0x1u},
    {2, 0x73, 0x0u},
    {2, 0x75, 0x0u},
    {2, 0x77, 0x2u},
    {2, 0x79, 0x3u},
    {1, 0x7f, 0x4u},
    {2, 0x80, 0x10u},
    {2, 0x82, 0x8u},
    {2, 0x84, 0x0u},
    {2, 0x86, 0x0u},
    {1, 0x88, 0x2u},
    {1, 0x89, 0x0u},
    {1, 0x8a, 0x0u},
    {4, 0xc4, 0x0u},
    {4, 0xc8, 0xffffffffu},
    {4, 0xcc, 0x0u},
    {2, 0xd7, 0x28u},
    {2, 0xd9, 0x2u},
    {2, 0xdb, 0x0u},
    {2, 0xdd, 0x0u},
    {2, 0xdf, 0x2u},
    {2, 0xe1, 0x3u},
    {1, 0xe7, 0x4u},
    {2, 0xe8, 0x8u},
    {2, 0xea, 0x4u},
    {2, 0xec, 0x0u},
    {2, 0xee, 0x0u},
    {1, 0xf0, 0x2u},
    {1, 0xf1, 0x0u},
    {1, 0xf2, 0x0u},
    {4, 0x12c, 0x0u},
    {4, 0x130, 0xffffffffu},
    {4, 0x134, 0x0u},
    {2, 0x13f, 0x82u},
    {2, 0x141, 0x3u},
    {2, 0x143, 0x0u},
    {2, 0x145, 0x0u},
    {1, 0x14f, 0x6u},
    {2, 0x150, 0x4u},
    {2, 0x152, 0x2u},
    {2, 0x154, 0x0u},
    {2, 0x147, 0x4u},
    {2, 0x149, 0x3u},
    {2, 0x156, 0x0u},
    {1, 0x158, 0x1u},
    {1, 0x159, 0x2u},
    {1, 0x15a, 0x7u},
    {4, 0x194, 0x0u},
    {4, 0x198, 0xffffffffu},
    {4, 0x19c, 0x0u},
    {2, 0x1af, 0x3u},
    {2, 0x1a7, 0x96u},
    {2, 0x1a9, 0x4u},
    {2, 0x1ab, 0x0u},
    {2, 0x1ad, 0x0u},
    {1, 0x1b7, 0x6u},
    {2, 0x1b8, 0x2u},
    {2, 0x1ba, 0x1u},
    {2, 0x1bc, 0x0u},
    {2, 0x1be, 0x0u},
    {1, 0x1c0, 0x1u},
    {1, 0x1c1, 0x1u},
    {1, 0x1c2, 0x0u},
    {4, 0x1fc, 0x0u},
    {4, 0x200, 0xffffffffu},
    {4, 0x204, 0x0u}
};

u32 CM_THISCALL cm_fn_00570ec0(u32 self, s8 kind, u32 count_out,
                             u32 zero_out, u32 optional_out)
{
    u8 scratch[512];
    u32 result, row, store, address, value;
    s32 year;
    char *version;

    if (kind != -1) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 5;
    W(zero_out) = 0;
    result = (u32)cm_fn_00944e46(0x208);
    if (result == 0) {
        cm_fn_00944cff(0x009c4338u, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                      scratch + 256, scratch, 0xa4);
        cm_fn_005e8290((const char *)0x009870e0u,
                      (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    for (row = 0; row < 5; ++row) {
        year = (s16)W(self + 0x40u);
        cm_fn_00521e60(result, (s16)row, rows[row].day,
                      rows[row].month, rows[row].flag, 6, year);
        year = (s16)W(self + 0x40u);
        cm_fn_00521eb0(result, row, rows[row].value, rows[row].position,
                      rows[row].setting, 5, 1, (u32)year, rows[row].extra);
        for (store = rows[row].begin; store < rows[row].end; ++store) {
            address = result + stores[store].offset;
            value = stores[store].value;
            if (stores[store].width == 1) B(address) = (u8)value;
            else if (stores[store].width == 2) W(address) = (u16)value;
            else D(address) = value;
        }
    }
    return result;
}
