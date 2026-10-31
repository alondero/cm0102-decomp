#include "cmre.h"
/* Preferred-image VA 0x00540a50, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* Global addresses in the exact comparison order. Values are always reloaded:
   the optional direction store can overlap a club or one of these globals. */
static const u32 pair_addresses[][2] = {
    {0x009d012c,0x009d0130}, {0x009d0120,0x009d0134},
    {0x009d0138,0x009d013c}, {0x009d0140,0x009d0144},
    {0x009d0148,0x009d014c}, {0x009d0150,0x009d0154},
    {0x009d0158,0x009d015c}, {0x009d0160,0x009d0164},
    {0x009d0168,0x009d016c}, {0x009d0170,0x009d0174},
    {0x009d0178,0x009d017c}, {0x009d0180,0x009d0184},
    {0x009d0188,0x009d018c}, {0x009d0190,0x009d0194},
    {0x009d0198,0x009d019c}, {0x009d0128,0x009d01a0},
    {0x009d0254,0x009d0258}, {0x009d02e4,0x009d0328},
    {0x009d03ac,0x009d03b0}, {0x009d03b4,0x009d03b8},
    {0x009d03bc,0x009d03c0}, {0x009d03c4,0x009d03c8},
    {0x009d0124,0x009d03d4}, {0x009d03d8,0x009d03dc},
    {0x009d03e0,0x009d03e4}, {0x009d03e8,0x009d03ec},
    {0x009d03f0,0x009d03f4}, {0x009d03f8,0x009d03fc},
    {0x009d0400,0x009d0404}, {0x009d0408,0x009d040c},
    {0x009d0410,0x009d0414}, {0x009d0418,0x009d041c},
    {0x00b63cdc,0x00b63c9c}, {0x00b63cbc,0x00b63ca0},
    {0x00b63ce8,0x00b63cac}, {0x00b63cd8,0x00b63cd0},
    {0x00b63cb4,0x00b63cb0}, {0x00b63cd4,0x00b63ca8},
    {0x00b63ce4,0x00b63cec}, {0x00b63cf0,0x00b63cf8},
    {0x00b63cc0,0x00b63d00}, {0x00b63cc8,0x00b63ccc},
    {0x00b63ce0,0x00b63cb8}, {0x00b63cc4,0x00b63cf4},
    {0x00b63cfc,0x00b63ca4}, {0x009d02a4,0x009d02a8},
    {0x009d02ac,0x009d02b0}, {0x009d02b4,0x009d02b8},
    {0x009d02bc,0x009d02c0}, {0x009d02c4,0x009d02c8},
    {0x009d02cc,0x009d02d0}, {0x009d02d4,0x009d02d8},
    {0x009d02dc,0x009d02e0}, {0x009d0440,0x009d0444},
    {0x009d0448,0x009d044c}, {0x009d0450,0x009d0454},
    {0x009d0458,0x009d045c}, {0x009d0460,0x009d0464},
    {0x009d0468,0x009d046c}, {0x009d0470,0x009d0474},
    {0x009d0478,0x009d047c}, {0x009d0480,0x009d0484},
    {0x009d0488,0x009d048c}, {0x009d0490,0x009d0494},
    {0x009d0498,0x009d049c}, {0x009d04a0,0x009d04a4},
    {0x009d04a8,0x009d04ac}, {0x009d04b0,0x009d04b4},
    {0x009d04b8,0x009d04bc}, {0x009d04c0,0x009d04c4},
    {0x009d04c8,0x009d04cc}, {0x009d04d0,0x009d04d4},
    {0x009d04d8,0x009d04dc}, {0x009d04e0,0x009d04e4},
    {0x009d04e8,0x009d04ec}, {0x009d04f0,0x009d04f4},
    {0x009d04f8,0x009d04fc}, {0x009d0500,0x009d0504},
    {0x009d0508,0x009d050c}, {0x009d0510,0x009d0514},
    {0x009d0518,0x009d051c}, {0x009d0520,0x009d0524}
};
#define PAIR_COUNT (sizeof(pair_addresses) / sizeof(pair_addresses[0]))

static u32 find_id(u32 id, u32 column)
{
    u32 i;
    for (i = 0; i < PAIR_COUNT; ++i) {
        if (id == G(pair_addresses[i][column]))
            return i;
    }
    return (u32)PAIR_COUNT;
}

u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag)
{
    u8 scratch[512];
    u32 nation, tag, id, index, column, mapped, base;
    char *version;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x43c9);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return 0;
    }
    nation = D(club + 0x53u);
    if (nation == 0)
        return 0;
    if (require_flag != 0 && B(club + 0x244u) == 0)
        return 0;
    tag = D(nation);
    if (tag != G(0x009cf488) && tag != G(0x009cf314) && tag != G(0x009cf434))
        return 0;
    id = D(club);
    index = find_id(id, 0);
    if (index < PAIR_COUNT) {
        column = 0;
        if (direction != 0)
            D(direction) = 1;
    } else {
        index = find_id(id, 1);
        if (index == PAIR_COUNT)
            return 0;
        column = 1;
        if (direction != 0)
            D(direction) = 0;
    }
    id = D(club);
    index = find_id(id, column);
    if (index < PAIR_COUNT) {
        mapped = G(pair_addresses[index][column ^ 1u]);
        /* The original tests differ: forward accepts any ID except -1;
           reverse requires a signed nonnegative ID. */
        if ((column == 0 && mapped != 0xffffffffu)
            || (column == 1 && (s32)mapped > -1)) {
            base = G(0x00ae23b4);
            return base + mapped * 581u;
        }
    }
    B(club + 0x244u) = 0;
    return 0;
}
