#include "cmre.h"

/* Byte offsets used below, from the address named in each access:
   self: 0x04 table base, 0x0c/0x10 element array base (0x0c + 4*c), 0x45 + 20*k
         counter bytes in each 0x6e-byte table row.
   row (table row for *param_2): sel + 0x08 start word, sel + 0x0e.. byte run,
         sel + 4*k + 0x0d count byte, sel = 0x15 when (u8)param_3 == 2.
   element: 0x00 index word, 0x1a, 0x1e, 0x22 byte, 0x24 link word,
         0x28 byte, 0x53 word under link, 0x5e byte, 0x61 word. */
#define REC_U32(addr, off) (*(volatile u32 *)((u32)(addr) + (u32)(off)))
#define REC_U8(addr, off) (*(volatile u8 *)((u32)(addr) + (u32)(off)))

extern u16 cm_fn_005b7d20(u32 value);
extern u32 cm_fn_0053b3d0(u32 value);
extern u32 CM_CDECL cm_fn_005409b0(u32 input);
extern u32 cm_fn_00547d30(u32 a, u32 b, u32 c);
extern u32 CM_THISCALL cm_fn_0055d270(u32 self, u32 a, u32 b, u32 c, u32 d);
extern u32 cm_fn_00600a50(u32 a, u32 b, s8 c, u32 d, u32 e);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 a);
extern void CM_THISCALL cm_fn_0076fd10(u32 self, u32 a, u32 b, u32 c);
extern void CM_THISCALL cm_fn_00776250(u32 self, u32 a, u32 b, u32 c);
extern u8 CM_STDCALL cm_fn_007762f0(u32 value, u8 mode, u32 object);
extern u8 CM_STDCALL cm_fn_007a7eb0(u32 a);
extern u32 CM_THISCALL cm_fn_00456770(u32 self, u32 a, u32 b);

void CM_THISCALL cm_fn_00775f90(u32 self, u32 param_2, u32 param_3, s8 param_4, u32 param_5)
{
    u32 c;
    u32 rec;
    u32 sel;
    u32 sum;
    u32 end;
    u32 first_class;
    u32 idx;
    u32 slot;
    u32 elem;
    u32 n;
    u32 p;
    u32 x;
    u32 r;
    u32 t;
    u8 bVar5;

    first_class = cm_fn_005b7d20(REC_U32(param_5, 0x61));
    rec = REC_U32(self, 0x04) + REC_U32(param_2, 0) * 0x6eu;
    c = ((u8)param_3 == 2);
    sel = c * 0x15u;
    sum = REC_U32(rec, sel + 0x08u);
    if (param_4 > 0) {
        p = rec + sel + 0x0eu;
        n = (u32)(s32)param_4;
        do {
            sum = sum + REC_U8(p, 0);
            p = p + 4u;
            n = n - 1u;
        } while (n != 0u);
    }
    end = sum + REC_U8(rec + sel + (u32)(s32)param_4 * 4u, 0x0d);
    slot = REC_U32(self, 0x0cu + c * 4u) + sum * 4u;
    idx = sum;

    while ((s32)idx < (s32)end) {
        elem = REC_U32(slot, 0);
        if (elem == param_5) goto next;
        if (REC_U32(elem, 0x61) == 0u) goto next;
        x = REC_U32(elem, 0x1a);
        t = cm_fn_0053b3d0(x);
        if (cm_fn_00547d30(elem, t, x) != 0u) goto next;
        if (cm_fn_0062c400(CM_GLOBAL(u32, 0x00b63d28), elem) != 0u) goto next;
        if (CM_GLOBAL(u32, 0x00b7434c) != 0u && cm_fn_007a7eb0(elem) != 0u && (u8)param_3 != 0u) goto next;
        t = REC_U32(elem, 0x24);
        if (t != 0u && REC_U32(t, 0x53) != param_2) goto next;
        if (REC_U32(elem, 0x1a) != param_2) {
            if (t != 0u) goto next;
            if (REC_U8(elem, 0x22) != 0u) goto next;
            if (REC_U8(CM_GLOBAL(u32, 0x00ae2d0c) + REC_U32(elem, 0) * 79u, 0x0c) & 2u) goto next;
            if (REC_U32(elem, 0x1e) != param_2) goto next;
        }
        r = CM_GLOBAL(u32, 0x009cf79c);
        x = CM_GLOBAL(u32, 0x00ae23d0) + r * 0x6bu;
        if (cm_fn_0055d270(CM_GLOBAL(u32, 0x00ae28f0), elem, x, 1u, 0u) != 0u) goto next;
        bVar5 = (u8)cm_fn_005b7d20(REC_U32(elem, 0x61));
        if ((bVar5 & (u8)first_class & 0x7f) == 0u) goto next;
        t = REC_U32(elem, 0x24);
        if (t == 0u) goto LAB_0077620d;
        if (REC_U8(elem, 0x28) != 0x0bu) goto next;
        if (cm_fn_005409b0(t) == 0u) {
            bVar5 = (u8)((REC_U8(elem, 0x5e) >> 3) & 1u);
        } else {
            bVar5 = 2u;
        }
        if (bVar5 != (u8)param_3) {
            r = cm_fn_00456770(rec + (u32)(s32)(s8)bVar5 * 20u + 0x3au,
                               CM_GLOBAL(u32, 0x00ae2c90), CM_GLOBAL(u32, 0x00ae2c94));
            if (r != 0u) {
                if (bVar5 != 0u) goto LAB_f1;
                if (cm_fn_00600a50(REC_U32(elem, 0x24), 0u, 1, 0u, 0u) == 0u) goto LAB_f1;
            }
        }
        goto next;

LAB_f1:
        cm_fn_007762f0(param_2, bVar5, elem);
LAB_0077620d:
        cm_fn_00776250(self, param_2, param_3, elem);
        n = (u32)(s32)(s8)(u8)param_3 * 20u;
        REC_U8(rec, 0x45u + n) = (u8)(REC_U8(rec, 0x45u + n) - 1u);
        cm_fn_0076fd10(self, elem, param_2, param_3);
        return;

next:
        idx = idx + 1u;
        slot = slot + 4u;
    }

    REC_U8(rec, 0x45u + (u32)(s32)(s8)(u8)param_3 * 20u) = 0u;
}
