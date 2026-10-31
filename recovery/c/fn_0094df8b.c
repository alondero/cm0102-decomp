#include "cmre.h"

/* Globals (preferred-image VAs):
   0x00de441c s32 gate: zero means a direct 0x00951c50 comparison of the two arguments.
   0x00de4408 u32 code page and 0x00de4624 u32 locale, read in that order for each
   0x0094c0fe call.
   0x00de4521 byte class table: bit 0x04 marks a lead byte; bit 0x10 means the byte
   is mapped through the byte table at 0x00de4420.
   0x00de4420 byte map table. */

extern s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags, u32 source, s32 count,
                                   u32 destination, s32 capacity, u32 code_page, s32 option);
extern s32 CM_CDECL cm_fn_00951c50(u32 first, u32 second);

u32 CM_CDECL cm_fn_0094df8b(u32 arg0, u32 arg1)
{
    /* [ebp-4] and [ebp-3]: two-byte conversion buffer. */
    u8 mbuf[2];
    u32 src_a;    /* [ebp-8] */
    u32 src_b;    /* [ebp-0xc] */
    u32 pa;       /* stack slot [ebp+8] */
    u32 pb;       /* stack slot [ebp+0xc] */
    u32 code_page;
    u32 locale;
    u8 c;
    u8 c2;
    u8 cl;
    u16 ax;
    u16 si;
    s32 res;

    if (CM_GLOBAL(volatile s32, 0x00de441c) == 0) {
        return (u32)cm_fn_00951c50(arg0, arg1);
    }

    pa = arg0;
    pb = arg1;
    src_b = arg1 - 1u;
    src_a = arg0 - 1u;

    for (;;) {
        /* First string: 94dfc7..94e04d. */
        c = *(volatile u8 *)pa;
        src_a = src_a + 1u;
        pa = pa + 1u;
        if ((CM_GLOBAL(volatile u8, 0x00de4521u + c) & 0x04u) != 0u) {
            if (*(volatile u8 *)pa != 0u) {
                code_page = CM_GLOBAL(volatile u32, 0x00de4408);
                locale = CM_GLOBAL(volatile u32, 0x00de4624);
                res = cm_fn_0094c0fe(locale, 0x200u, src_a, 2, (u32)&mbuf[0], 2, code_page, 1);
                if (res == 1) {
                    si = (u16)mbuf[0];
                } else if (res == 2) {
                    si = (u16)(((u32)mbuf[0] << 8) + (u32)mbuf[1]);
                } else {
                    return 0x7fffffffu;
                }
                pa = pa + 1u;
                src_a = src_a + 1u;
            } else {
                si = 0u;
            }
        } else {
            cl = CM_GLOBAL(volatile u8, 0x00de4521u + c) & 0x10u;
            if (cl == 0x10u) {
                si = (u16)CM_GLOBAL(volatile u8, 0x00de4420u + c);
            } else {
                si = (u16)c;
            }
        }

        /* Second string: 94e04f..94e0b7. */
        c2 = *(volatile u8 *)pb;
        ax = (u16)c2;
        src_b = src_b + 1u;
        pb = pb + 1u;
        if ((CM_GLOBAL(volatile u8, 0x00de4521u + c2) & 0x04u) != 0u) {
            if (*(volatile u8 *)pb != 0u) {
                code_page = CM_GLOBAL(volatile u32, 0x00de4408);
                locale = CM_GLOBAL(volatile u32, 0x00de4624);
                res = cm_fn_0094c0fe(locale, 0x200u, src_b, 2, (u32)&mbuf[0], 2, code_page, 1);
                if (res == 1) {
                    ax = (u16)mbuf[0];
                } else if (res == 2) {
                    ax = (u16)(((u32)mbuf[0] << 8) + (u32)mbuf[1]);
                } else {
                    return 0x7fffffffu;
                }
                pb = pb + 1u;
                src_b = src_b + 1u;
            } else {
                ax = 0u;
            }
        } else {
            cl = CM_GLOBAL(volatile u8, 0x00de4521u + c2) & 0x10u;
            if (cl == 0x10u) {
                ax = (u16)CM_GLOBAL(volatile u8, 0x00de4420u + c2);
            }
        }

        /* 94e0d2: compare the two values. */
        if (ax != si) {
            return ((u16)ax < (u16)si) ? 1u : 0xffffffffu;
        }
        if (si == 0u) {
            return 0u;
        }
    }
}
