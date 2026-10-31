#include "cmre.h"

/* Thiscall (ECX is read first and is the receiver), ret 4: one stack argument.
   The routine overwrites that argument slot at 0x007f9731 with the dword at
   [arg + 0x20]; later reads of the slot (0x007f97d1, 0x007f97df) see the new value.
   The original pointer is kept separately (EBX) for every later [ebx + ...] use.

   Argument object fields by byte offset: 0x00 dword, 0x0c dword, 0x10 dword,
   0x14 dword, 0x1c dword, 0x20 dword.
   Receiver fields by byte offset: 0x5c dword (base pointer of a 17-byte record
   table), 0x64 dword (record count, signed).
   Records are 110 bytes apart in the global table at 0x00ae23bc: the record
   index v selects offset 110*v from that global, with fields at 0x24 and 0x39.

   No scratch buffers and no SEH frame in this routine. */

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_THISCALL cm_fn_0055d270(int *param_1, int *param_2, int *param_3, u32 param_4, u32 param_5);
extern u32 CM_THISCALL cm_fn_0062c400(int *param_1, int *param_2);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern int CM_THISCALL cm_fn_007757d0(int *param_1, int param_2, short *param_3, int *param_4, int param_5, int *param_6);
extern u32 CM_THISCALL cm_fn_008c7560(int param_1, int *param_2, int param_3, int param_4);
extern u32 CM_THISCALL cm_fn_008cf5e0(int param_1, u32 param_2, int param_3);

u32 CM_THISCALL cm_fn_007f9720(u32 self, u32 arg1)
{
    u32 obj;
    u32 edi;
    u32 saved_edi;
    u32 k;
    u32 stride;
    u32 count;
    u32 v;
    u32 p;
    u32 t;
    u32 t2;
    u32 rec;
    u32 g;
    u32 r;
    u32 res;
    u32 res2;
    int found;
    int cont;

    obj = arg1;
    v = *(volatile u32 *)(obj + 0x20u);
    edi = *(volatile u32 *)(obj + 0x1cu);
    arg1 = v;                                   /* write to the incoming argument slot */
    saved_edi = edi;

    v = *(volatile u32 *)obj;
    stride = 0u;
    if ((s32)v < 0) {
        return 0u;
    }

    CM_GLOBAL(u32, 0x00b74442u) = CM_GLOBAL(u32, 0x00b7443au);

    r = cm_fn_0068d1b0(0x00b74438);
    if (r != 0u) {
        do {
            v = *(volatile u32 *)r;
            if (*(volatile u32 *)(obj + 0x0cu) == v) {
                return 1u;
            }
            if (*(volatile u32 *)(obj + 0x10u) == v) {
                return 1u;
            }
            r = cm_fn_0068d1b0(0x00b74438);
        } while (r != 0u);
    }

    if (*(volatile u32 *)(self + 0x5cu) == 0u) {
        return 0u;
    }

    count = *(volatile u32 *)(self + 0x64u);
    k = 0u;
    if ((s32)count <= 0) {
        return 0u;
    }

    do {
        p = *(volatile u32 *)(self + 0x5cu);
        v = *(volatile u32 *)(p + stride + 4u);
        if ((s32)v >= 0) {
            g = CM_GLOBAL(u32, 0x00ae23bcu);
            rec = g + v * 110u;
            t = *(volatile u32 *)(rec + 0x39u);

            found = 0;
            if (edi == t) {
                found = 1;
            } else {
                t2 = *(volatile u32 *)(rec + 0x24u);
                if (edi == t2) {
                    found = 1;
                } else if (arg1 == t) {
                    edi = arg1;
                    found = 1;
                } else if (arg1 == t2) {
                    edi = arg1;
                    found = 1;
                }
            }

            if (found) {
                cont = 0;
                res = cm_fn_0053b900((u32 *)edi);
                if (res == 0u) {
                    g = CM_GLOBAL(u32, 0x00de1f54u);
                    res2 = cm_fn_008c7560((int)g, (int *)rec, (int)edi, 0);
                    if (res2 != 0u) {
                        cont = 1;
                    }
                }

                if (!cont) {
                    /* Fallbacks when the first result was nonzero or the second was zero (0x007f9802). */
                    g = CM_GLOBAL(u32, 0x00b63d28u);
                    res = cm_fn_0062c400((int *)g, (int *)rec);
                    if ((u8)res == 2u) {
                        cont = 1;
                    } else {
                        t = *(volatile u32 *)(obj + 0x14u);
                        g = CM_GLOBAL(volatile u32, 0x00ae28f0u);
                        res = cm_fn_0055d270((int *)g, (int *)rec, (int *)t, 1u, 0u);
                        if (res != 0u) {
                            cont = 1;
                        } else {
                            g = CM_GLOBAL(u32, 0x00b74318u);
                            res = (u32)cm_fn_007757d0((int *)g, (int)rec, (short *)0x00ae2c90u, (int *)edi, 0, (int *)0);
                            if (res != 0u) {
                                cont = 1;
                            } else {
                                g = CM_GLOBAL(u32, 0x00de1f54u);
                                res = cm_fn_008cf5e0((int)g, rec, (int)obj);
                                if ((u8)res == 0u) {
                                    return 1u;
                                }
                                cont = 1;
                            }
                        }
                    }
                }
                /* Continue with the saved EDI (0x007f9853). */
                edi = saved_edi;
            }
        }

        k = k + 1u;
        stride = stride + 0x11u;
    } while ((s32)k < (s32)*(volatile u32 *)(self + 0x64u));

    return 0u;
}
