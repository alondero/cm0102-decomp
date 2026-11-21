#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u8 CM_THISCALL cm_fn_005fa730(u8 *, s32, s16, s32 *, s32 *, s32);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00945333(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_004022c0(u8 *self)
{
    u8 scratch[0x218];
    u32 p, result, nation, count = 0, i, j, out, idx, eax = 0, edx = 0;
    u32 n;
    s32 k;
    s8 selected;
    u32 line;
    unsigned long long pair;
    u8 *a, *b;
    *(volatile u16 *)(self + 0xf2) = 16;
    p = (u32)cm_fn_00944e46(200);
    *(volatile u32 *)(self + 0xee) = p;
    if (!p) {
        cm_fn_00944cff(0x9870f8, 0, 0, scratch + 0x118, scratch + 0x18);
        a = scratch + 0x118; b = scratch + 0x18; line = 0x349;
        goto error;
    }
    selected = (s8)cm_fn_005fa730((u8 *)CM_GLOBAL(u32, 0xb5e134),
        CM_GLOBAL(u32, 0x9cf7ac), *(u16 *)(self + 0x40),
        (s32 *)(scratch + 0x10), (s32 *)(scratch + 0x14), 1);
    for (i = 0; (s32)i < (s32)selected; i++) {
        nation = CM_GLOBAL(u32, 0xae23a8) + *(u32 *)(scratch + 0x10 + i * 4) * 0x122;
        result = cm_fn_0053b3d0(nation);
        p = *(volatile u32 *)(self + 0xee);
        *(u32 *)(p + count * 4) = result;
        count++;
    }
    nation = CM_GLOBAL(u32, 0xae23a8);
    n = 0;
    if (CM_GLOBAL(s32, 0xae2350) > 0) {
        do {
            p = *(u32 *)(nation + 0x71);
            if (p && *(u32 *)p == CM_GLOBAL(u32, 0x9cfa08) && (s32)count < 50) {
                int found = 0;
                if (selected > 0) {
                    p = *(volatile u32 *)(self + 0xee);
                    for (k = 0; k < selected; k++)
                        if (*(u32 *)(*(u32 *)(p + k * 4) + 0x53) == nation) found = 1;
                }
                if (!found) {
                    result = cm_fn_0053b3d0(nation);
                    p = *(volatile u32 *)(self + 0xee);
                    *(u32 *)(p + count * 4) = result;
                    count++;
                }
            }
            n++;
            nation += 0x122;
        } while ((s32)(s16)n < CM_GLOBAL(s32, 0xae2350));
    }
    if ((s32)count < 16) return 0;
    p = *(volatile u32 *)(self + 0xee);
    eax = (u32)(s32)selected;
    edx = p + eax * 4;
    cm_fn_00945333(edx, count - eax, 4, 0x4b07d0);
    pair = cm_fn_00944e46(64);
    out = (u32)pair;
    if (!out) {
        cm_fn_00944cff(0x9870f8, 0, 0, scratch + 0x18, scratch + 0x118);
        a = scratch + 0x18; b = scratch + 0x118; line = 0x380;
        goto error;
    }
    eax = out;
    edx = (u32)(pair >> 32);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            do {
                eax = (u32)cm_fn_0090cfc0(4);
                idx = (u32)(s32)(s8)eax + j * 4;
                edx = *(volatile u32 *)(self + 0xee);
            } while (!*(u32 *)(edx + idx * 4));
            p = *(volatile u32 *)(self + 0xee);
            *(u32 *)(out + (i * 4 + j) * 4) = *(u32 *)(p + idx * 4);
            p = *(volatile u32 *)(self + 0xee);
            *(u32 *)(p + idx * 4) = 0;
            eax = idx * 4;
            edx = p;
        }
    }
    cm_fn_009452ca(*(volatile u32 *)(self + 0xee));
    *(volatile u32 *)(self + 0xee) = out;
    return 1;
error:
    result = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, result, a, b, line);
    cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
    CM_GLOBAL(u32, 0xb67a34) = 0;
    return 0;
}
