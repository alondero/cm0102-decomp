#include "cmre.h"

extern u8 *CM_CDECL cm_fn_0094e0f9(u8 *param_1, u32 param_2);
extern u8 *CM_CDECL cm_fn_00951300(u8 *param_1, u32 param_2);
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_009452ca(u32 arg4);
extern u32 CM_CDECL cm_fn_009478d8(const char *param_1, u8 param_2);
extern u32 cm_fn_0094de8d(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
extern u32 *cm_fn_0094c3a0(u32 *param_1, u32 *param_2);
extern u32 *CM_CDECL cm_fn_0094c3b0(u8 *dest, u8 *source);

/* Cdecl with four stack arguments (ret 0). arg2 is a path string s. The routine finds the
   last separator ('\\' or '/', otherwise the first ':'), or allocates a copy with a
   two-byte prefix from a constant when none is found, then finds the first '.' after it.
   With no '.', it builds a new buffer of strlen+5 bytes from the table of constant strings at
   0x00ad954c..0x00ad9540 (walked backwards in steps of 4) and calls 0x0094de8d on the first
   match. With a '.', it calls 0x0094de8d on the block itself. The result is -1 unless one of
   those calls sets it. The temporary blocks are freed in the listing order. */
u32 CM_CDECL cm_fn_0094dd44(u32 arg1, u8 *s, u32 arg3, u32 arg4)
{
    u8 *esi;
    u8 *edi;
    u8 *sl;
    u8 *dot;
    u8 *newbuf;
    u32 bres;
    u32 n;
    u32 p;
    u32 r;

    edi = s;
    esi = cm_fn_0094e0f9(s, 0x5cU);
    sl = cm_fn_0094e0f9(s, 0x2fU);
    if (sl == 0) {
        if (esi == 0) {
            esi = cm_fn_00951300(s, 0x3aU);
            if (esi == 0) {
                n = cm_fn_009488e0((u32)s) + 3U;
                edi = (u8 *)(u32)cm_fn_00944e46(n);
                if (edi == 0) {
                    return 0xffffffffU;
                }
                cm_fn_0094c3a0((u32 *)edi, (u32 *)0x00971664U);
                cm_fn_0094c3b0(edi, s);
                esi = edi + 2;
            }
        }
    } else {
        if (esi == 0 || (u32)sl > (u32)esi) {
            esi = sl;
        }
    }

    bres = 0xffffffffU;
    dot = cm_fn_0094e0f9(esi, 0x2eU);
    if (dot == 0) {
        n = cm_fn_009488e0((u32)edi) + 5U;
        newbuf = (u8 *)(u32)cm_fn_00944e46(n);
        if (newbuf == 0) {
            return 0xffffffffU;
        }
        cm_fn_0094c3a0((u32 *)newbuf, (u32 *)edi);
        esi = (u8 *)(u32)(cm_fn_009488e0((u32)edi) + (u32)newbuf);
        p = 0x00ad954cU;
        for (;;) {
            cm_fn_0094c3a0((u32 *)esi, (u32 *)CM_GLOBAL(volatile u32, p));
            r = cm_fn_009478d8((const char *)newbuf, 0);
            if (r != 0xffffffffU) {
                bres = cm_fn_0094de8d(arg1, (u32)newbuf, arg3, arg4);
                break;
            }
            p = p - 4U;
            if ((s32)p < (s32)0x00ad9540U) {
                break;
            }
        }
        cm_fn_009452ca((u32)newbuf);
    } else {
        r = cm_fn_009478d8((const char *)edi, 0);
        if (r != 0xffffffffU) {
            bres = cm_fn_0094de8d(arg1, (u32)edi, arg3, arg4);
        }
    }
    if (edi != s) {
        cm_fn_009452ca((u32)edi);
    }
    return bres;
}
