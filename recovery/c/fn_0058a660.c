#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern char *CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);

u32 CM_CDECL cm_fn_0058a660(void *input, u32 mode, u32 output)
{
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    u8 scratch[0x1900];
    u32 v[8], a, b, i, result;
    s32 x, y, z;
    unsigned long long wide;
    frame.state = 0xffffffff;
    frame.handler = 0x958cbb;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    for (i = 0; i != 8; ++i)
        v[i] = (u32)cm_fn_00785830(input, i);
    a = CM_GLOBAL(volatile u32, 0xae23a8) + v[5] * 290;
    b = CM_GLOBAL(volatile u32, 0xae23b4) + v[0] * 581;
    frame.state = 0;
    cm_fn_006181d0(a, (u32)(scratch + 0x1f4), 0x78, 9);
    wide = cm_fn_0043f460((const int *)&v[7], 6);
    cm_fn_00441c10((u32)scratch, 0x1f4, (u32)wide, (u32)(wide >> 32), 6);
    if (mode) {
        cm_fn_0053e140(v[2], v[3], v[4], (u32)(scratch + 0x3e8), 0x190, 1, v[1]);
        z = *(s8 *)(b + 0x52);
        if (!v[6]) {
            x = *(s8 *)(a + 0x37);
            y = *(s8 *)(b + 0x37);
            cm_fn_0066f4e0((u32)(scratch + 0x578), 0x9c6ee0,
                x, x, y, y, z, z, scratch + 0x1f4, b + 4, scratch, b + 0x38);
        } else {
            x = *(s8 *)(a + 0x37);
            y = *(s8 *)(b + 0x37);
            cm_fn_0066f4e0((u32)(scratch + 0x578), 0x9c6dac,
                y, y, x, x, z, z, b + 4, scratch, scratch + 0x1f4, b + 0x38);
        }
        cm_fn_004ae660((void *)output, (u32)(scratch + 0x578));
        cm_fn_004ae8a0(output, (const char *)(b + 0x38), 0x7d5, b);
    } else {
        z = *(s8 *)(b + 0x52);
        x = *(s8 *)(a + 0x37);
        cm_fn_0066f4e0((u32)(scratch + 0x578), 0x9c6d48,
            x, x, z, z, a + 0x57, b + 0x38);
        cm_fn_004ae660((void *)output, (u32)(scratch + 0x578));
    }
    result = cm_fn_004ae8a0(output, (const char *)(b + 4), 0x7d5, b);
    CM_FS(void *, 0) = frame.prev;
    return result;
}
