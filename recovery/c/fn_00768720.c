#include "cmre.h"
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32, const u8 *, s16);
extern char * CM_CDECL cm_fn_0090d130(void);
extern void CM_THISCALL cm_fn_009302f0(u32, u16 *, int);
extern u16 CM_THISCALL cm_fn_00931d50(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_STDCALL cm_fn_00768720(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g)
{
    u8 scratch[0x254];
    u32 allocation = (u32)cm_fn_00944e46(12);
    u32 i;
    (void)b;
    if (!allocation || !c) {
        u8 *x = scratch + 0x54, *y = scratch + 0x154;
        u32 line;
        char *text;
        if (!allocation) {
            cm_fn_00944cff(0xa5b26c, 0, 0, y, x);
            line = 0x153;
        } else {
            cm_fn_00944cff(0xa5b26c, 0, 0, x, y);
            line = 0x15a;
            { u8 *t = x; x = y; y = t; }
        }
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text, y, x, line);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        CM_GLOBAL(u32, 0xb67a34) = 0;
        return;
    }
    if ((s16)cm_fn_00931d50(c, allocation, 3, d, e, f, g) < 3) {
        cm_fn_009452ca(allocation);
        return;
    }
    *(u16 *)(scratch + 8) = *(u16 *)(a + 2);
    for (i = 0; i < 3; ++i) {
        u32 p = *(u32 *)(allocation + i * 4);
        u32 record = CM_GLOBAL(u32, 0xae23bc) + *(u32 *)p * 0x6e;
        u32 v1, v2, v3, v4, q;
        u8 *out = scratch + 10 + i * 24;
        if (g)
            cm_fn_0068d0d0(g, (const u8 *)&record, 4);
        q = *(u32 *)(record + 0x39);
        v4 = q ? *(u32 *)q : 0xffffffff;
        q = *(u32 *)(record + 4);
        v1 = q ? *(u32 *)(q + 0x33) : 0xffffffff;
        q = *(u32 *)(record + 8);
        v2 = q ? *(u32 *)(q + 0x33) : 0xffffffff;
        q = *(u32 *)(record + 12);
        v3 = q ? *(u32 *)(q + 0x33) : 0xffffffff;
        *(u32 *)(out + 0) = v1;
        *(u32 *)(out + 4) = v2;
        *(u32 *)(out + 8) = v3;
        *(u32 *)(out + 20) = v4;
        *(u32 *)(out + 12) = *(u32 *)record;
    }
    cm_fn_009302f0(b, (u16 *)(scratch + 8), 1);
    cm_fn_009452ca(allocation);
}
