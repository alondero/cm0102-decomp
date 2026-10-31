#include "cmre.h"

/* Stack model: B is esp after the four register pushes (edi, esi, ebp, ebx).
   frame[n] is the original byte at B + 0x10 + n; the 300-byte buffer is
   frame[0x0c] (B+0x1c) and fills the rest of the 0x138-byte local area.
   Stack arguments: a0 at B+0x14c, a1 at B+0x150, a2 at B+0x154. The index
   calls take a0 as ECX; the emit calls (0x4ae660, 0x4ae8a0) take a2. */

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u8 *buffer, u32 code, u32 addr);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_STDCALL cm_fn_004d6ee0(u32 a0, u32 a1, u32 a2)
{
    u8 frame[0x138];
    u32 r0, r2, r3, r4, r5, r6;
    u32 g1, g2, addr;
    s32 b1, b2;

    /* Index 1 is never requested by this routine. */
    r0 = (u32)cm_fn_00785830((void *)a0, 0u);
    r2 = (u32)cm_fn_00785830((void *)a0, 2u);
    r3 = (u32)cm_fn_00785830((void *)a0, 3u);
    r4 = (u32)cm_fn_00785830((void *)a0, 4u);
    r5 = (u32)cm_fn_00785830((void *)a0, 5u);
    r6 = (u32)cm_fn_00785830((void *)a0, 6u);

    if (a1 != 0) {
        cm_fn_0053e140(r3, r4, r5, (u32)(frame + 0x0c), 0x12cu, 1u, r2);
        if ((s32)r6 >= 0) {
            g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
            b1 = (s32)*(volatile s8 *)(g1 + r6 * 0x245u + 0x37u);
            b2 = (s32)*(volatile s8 *)(r0 * 0x245u + g1 + 0x37u);
            cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x009ab2ec),
                           (u32)b2, (u32)b2, (u32)b1, (u32)b1,
                           (u32)(frame + 0x0c),
                           r0 * 0x245u + g1 + 4u,
                           g1 + r6 * 0x245u + 4u);
        } else {
            g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
            b1 = (s32)*(volatile s8 *)(r0 * 0x245u + g1 + 0x37u);
            cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x009ab258),
                           (u32)b1, (u32)b1,
                           (u32)(frame + 0x0c),
                           r0 * 0x245u + g1 + 4u);
        }
        cm_fn_004ae660((void *)a2, (u32)&CM_GLOBAL(u8, 0x00de1f64));
        g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
        addr = r0 * 0x245u + g1;
        cm_fn_004ae8a0((void *)a2, (u8 *)(addr + 4u), 0x7d5u, addr);
        if ((s32)r6 >= 0) {
            g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
            addr = g1 + r6 * 0x245u;
            cm_fn_004ae8a0((void *)a2, (u8 *)(addr + 4u), 0x7d5u, addr);
        }
        g2 = (u32)CM_GLOBAL(s32, 0x00ae23bc);
        if (cm_fn_0053e990(r2 * 0x6eu + g2, r3, r4, r5, 0u) != 0) {
            g2 = (u32)CM_GLOBAL(s32, 0x00ae23bc);
            cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0x0c), 0x7d7u, r2 * 0x6eu + g2);
        }
        return;
    }

    cm_fn_0053e140(r3, r4, r5, (u32)(frame + 0x0c), 0x12cu, 5u, r2);
    if ((s32)r6 >= 0) {
        g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
        b1 = (s32)*(volatile s8 *)(g1 + r6 * 0x245u + 0x52u);
        cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x009ab210),
                       (u32)b1, (u32)b1,
                       (u32)(frame + 0x0c),
                       g1 + r6 * 0x245u + 0x38u);
    } else {
        cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x009ab1e4),
                       (u32)(frame + 0x0c));
    }
    cm_fn_004ae660((void *)a2, (u32)&CM_GLOBAL(u8, 0x00de1f64));
    if ((s32)r6 >= 0) {
        g1 = (u32)CM_GLOBAL(s32, 0x00ae23b4);
        addr = g1 + r6 * 0x245u;
        cm_fn_004ae8a0((void *)a2, (u8 *)(addr + 0x38u), 0x7d5u, addr);
    }
    g2 = (u32)CM_GLOBAL(s32, 0x00ae23bc);
    if (cm_fn_0053e990(r2 * 0x6eu + g2, r3, r4, r5, 0u) != 0) {
        g2 = (u32)CM_GLOBAL(s32, 0x00ae23bc);
        cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0x0c), 0x7d7u, r2 * 0x6eu + g2);
    }
}
