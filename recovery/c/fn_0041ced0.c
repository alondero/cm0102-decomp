#include "cmre.h"

/* Globals (preferred-image VAs):
   0x00989858 s32 gate: zero returns 0 before any other work.
   0x00989898 u32 table base: zero returns 0; rows are 16 bytes, read at base+row*16.
   0x00989896 s16 row count: loop bound.
   0x00989ac8 format string passed to the variadic 0x00944c9f.
   0x00989ad8 empty string substituted for null pointer fields. */

/* Callees that take only the object in ECX, have no stack arguments and return
   with plain `ret`. 0x0054cfb0 returns its result in AL (u8); 0x0054cf70 returns
   its result in AX (u16). The words pushed around these calls (aw, b) are not
   arguments of them: they stay on the stack as trailing varargs of 0x00944c9f. */
extern u8 CM_FASTCALL cm_fn_0054cfb0(u32 self);
extern u16 CM_FASTCALL cm_fn_0054cf70(u32 self);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern s32 CM_CDECL cm_fn_00951c50(u32 first, u32 second);

#define ARG_U16(base, off) (*(volatile u16 *)((base) + (off)))
#define ARG_U32(base, off) (*(volatile u32 *)((base) + (off)))

u32 CM_CDECL cm_fn_0041ced0(u32 arg0, u32 arg1)
{
    /* Scratch frame: frame+8 is the destination buffer for 0x00944c9f. */
    u8 frame[0x1fc];
    u32 found;
    s32 index;
    s32 r;
    s32 ra;
    s32 rb;
    s32 cmp;
    s32 b;
    s32 rc;
    s32 h;
    u32 obj;
    u32 ebx_v;
    u32 esi_v;
    u32 g;
    u32 g2;
    u32 table;
    u32 dest;
    u32 first;
    u32 v;
    u32 e;
    u32 esi;
    s32 idx;
    u16 aw;

    if (CM_GLOBAL(volatile s32, 0x00989858) == 0) {
        return 0u;
    }
    if (CM_GLOBAL(volatile u32, 0x00989898) == 0u) {
        return 0u;
    }

    dest = (u32)&frame[8];

    if (arg0 == 0u) {
        r = cm_fn_0090cfc0((s32)(s16)CM_GLOBAL(volatile s16, 0x00989896));
        found = (u32)r;
    } else {
        found = 0xffffffffu;
        index = 0;
        if ((s16)CM_GLOBAL(volatile s16, 0x00989896) <= 0) {
            return 0u;
        }
        do {
            obj = arg0 + 0x10u;
            ebx_v = ARG_U32(arg0, 0x08);
            if (ebx_v == 0u) {
                ebx_v = 0x00989ad8u;
            }
            esi_v = ARG_U32(arg0, 0x04);
            if (esi_v == 0u) {
                esi_v = 0x00989ad8u;
            }
            aw = ARG_U16(arg0, 0x12);

            ra = cm_fn_0054cfb0(obj);
            b = (s32)(s8)(u8)ra + 1;
            rb = cm_fn_0054cf70(obj);
            rc = (s32)(s16)rb;

            cm_fn_00944c9f(dest, 0x00989ac8u, esi_v, ebx_v, (u32)rc, (u32)b, (u32)aw);

            g = CM_GLOBAL(volatile u32, 0x00989898);
            first = *(volatile u32 *)(g + (u32)(s32)(s16)index * 16u);
            cmp = cm_fn_00951c50(first, dest);
            if (cmp == 0) {
                found = (u32)index;
            }
            index = (s32)((u32)index + 1u);
        } while ((s16)index < CM_GLOBAL(volatile s16, 0x00989896));
    }

    if ((u16)found == 0xffffu) {
        return 0u;
    }

    idx = (s32)(s16)(u16)found;
    if (arg1 != 0u) {
        esi = (u32)idx * 16u;
        g = CM_GLOBAL(volatile u32, 0x00989898);
        v = *(volatile u32 *)(g + esi + 0x0cu);
        if (v == 0u) {
            return 0u;
        }
        h = (s32)*(volatile s16 *)(g + esi + 0x06u);
        r = cm_fn_0090cfc0(h);
        g2 = CM_GLOBAL(volatile u32, 0x00989898);
        e = *(volatile u32 *)(esi + g2 + 0x0cu);
        table = e;
        return *(volatile u32 *)(table + (u32)(s32)(s16)r * 4u);
    }

    esi = (u32)idx * 16u;
    g = CM_GLOBAL(volatile u32, 0x00989898);
    v = *(volatile u32 *)(g + esi + 0x08u);
    if (v == 0u) {
        return 0u;
    }
    h = (s32)*(volatile s16 *)(g + esi + 0x04u);
    r = cm_fn_0090cfc0(h);
    g2 = CM_GLOBAL(volatile u32, 0x00989898);
    e = *(volatile u32 *)(esi + g2 + 0x08u);
    table = e;
    return *(volatile u32 *)(table + (u32)(s32)(s16)r * 4u);
}
