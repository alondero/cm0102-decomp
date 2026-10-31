#include "cmre.h"

/* Stack model: B is esp after the four register pushes (edi, esi, ebp, ebx).
   frame[n] is the original byte at B + 0x10 + n. The 200-byte buffer is at
   frame[0x0c] (B+0x1c) and the 300-byte buffer at frame[0xd4] (B+0xe4); they
   fill the 0x200-byte local area. Stack arguments: a0 at B+0x214, a1 at
   B+0x218, a2 at B+0x21c. The index calls take a0 as ECX; the emit calls
   (0x4ae660, 0x4ae8a0) take a2 as ECX. */

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u8 *buffer, u32 code, u32 addr);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 cm_fn_00542100(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_STDCALL cm_fn_0076fa40(u32 a0, u32 a1, u32 a2)
{
    u8 frame[0x200];
    u32 r0, r1, r2, r3, r4, r5;
    u8 r6;
    s32 b;

    r0 = (u32)cm_fn_00785830((void *)a0, 0u);
    r1 = (u32)cm_fn_00785830((void *)a0, 1u);
    r2 = (u32)cm_fn_00785830((void *)a0, 2u);
    r3 = (u32)cm_fn_00785830((void *)a0, 3u);
    r4 = (u32)cm_fn_00785830((void *)a0, 4u);
    if (a1 == 0) {
        r5 = (u32)cm_fn_00785830((void *)a0, 5u);
        cm_fn_0053e140(r1, r2, r3, (u32)(frame + 0xd4), 0x12cu, 5u, r0);
        r4 = r4 * 0x245u;
        b = (s32)*(volatile s8 *)(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x52u);
        cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x00a5c73c),
                       (u32)b, (u32)b, (u32)(frame + 0xd4),
                       r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x38u);
        cm_fn_004ae660((void *)a2, (u32)&CM_GLOBAL(u8, 0x00de1f64));
        if (cm_fn_0053e990(r0 * 0x6eu + (u32)CM_GLOBAL(s32, 0x00ae23bc), r1, r2, r3, 0u) != 0) {
            cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0xd4), 0x7d7u,
                           r0 * 0x6eu + (u32)CM_GLOBAL(s32, 0x00ae23bc));
        }
        if (r5 == 0) {
            cm_fn_004ae8a0((void *)a2,
                           (u8 *)(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x38u),
                           0x7d5u, r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4));
        } else {
            cm_fn_004ae8a0((void *)a2,
                           (u8 *)(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x38u),
                           0x7d6u, r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4));
        }
        return;
    }

    r5 = (u32)cm_fn_00785830((void *)a0, 5u);
    r6 = (u8)cm_fn_00785830((void *)a0, 6u);
    r4 = r4 * 0x245u;
    cm_fn_00542100(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4), r5, (u32)(frame + 0x0c), 0xc8u, 2u, 1u);
    cm_fn_0053e140(r1, r2, r3, (u32)(frame + 0xd4), 0x12cu, 1u, r0);
    if (r6 == 0) {
        b = (s32)*(volatile s8 *)(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x52u);
        cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x00a5c80c),
                       (u32)b, (u32)b, (u32)(frame + 0xd4), (u32)(frame + 0x0c));
    } else {
        b = (s32)*(volatile s8 *)(r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4) + 0x52u);
        cm_fn_0066f4e0((u32)&CM_GLOBAL(u8, 0x00de1f64), (u32)&CM_GLOBAL(u8, 0x00a5c790),
                       (u32)b, (u32)b, (u32)(frame + 0xd4), (u32)(frame + 0x0c));
    }
    cm_fn_004ae660((void *)a2, (u32)&CM_GLOBAL(u8, 0x00de1f64));
    if (cm_fn_0053e990(r0 * 0x6eu + (u32)CM_GLOBAL(s32, 0x00ae23bc), r1, r2, r3, 0u) != 0) {
        cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0xd4), 0x7d7u,
                       r0 * 0x6eu + (u32)CM_GLOBAL(s32, 0x00ae23bc));
    }
    if (r5 == 0) {
        cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0x0c), 0x7d5u,
                       r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4));
    } else {
        cm_fn_004ae8a0((void *)a2, (u8 *)(frame + 0x0c), 0x7d6u,
                       r4 + (u32)CM_GLOBAL(s32, 0x00ae23b4));
    }
}
