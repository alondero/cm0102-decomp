/* WIP: import-event mismatch (cases 1 and 3, after a call to 0x005e5df0; case 4 memory on
   page 0xb67000). Suspected cause, not proven: the listing loads 16-bit globals into CX and
   AX, so the upper halves of the pushed dwords are the entry ECX (and EAX left by the callees).
   Plain cdecl C cannot read the entry ECX, so those upper bits are zero here. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0055f2c0(int param_1);
extern void cm_fn_005e2eb0(int param_1, int param_2, int param_3, s32 param_4);
extern void cm_fn_005e3b50(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5, u32 param_6);
extern void cm_fn_005e5df0(int param_1, int param_2, int param_3, int param_4, u32 param_5, u32 param_6, u32 param_7);
extern void cm_fn_005e6d80(int param_1, int param_2, int param_3, int param_4, u32 param_5, u32 param_6, u32 param_7, char *param_8, int param_9);
extern void cm_fn_0068d520(void);

/* Cdecl with three stack arguments (ret 0) returning 1, or 0 on the early exits. The
   routine returns 0 when the global 0x00aebe0c is nonzero or the first argument is null.
   Otherwise it issues a fixed sequence of layout calls. The 16-bit globals 0x00ae317e,
   0x00ae3184 and 0x00aebe04 are passed as words. When the third argument is positive and
   the second is at most it, one call uses the quotient (496 * arg2) / arg3 (signed). The
   second argument of zero selects a fastcall to 0x0055f2c0 with a table entry. */
u32 CM_CDECL cm_fn_005e76f0(u32 param_1, s32 param_2, s32 param_3)
{
    u32 w1;
    u32 w2;
    u32 w3;
    u32 tbl;
    s32 quotient;

    if (CM_GLOBAL(volatile u32, 0x00aebe0c) != 0) {
        return 0;
    }
    cm_fn_0068d520();
    if (param_1 == 0) {
        return 0;
    }
    if (param_2 == 0) {
        tbl = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a) * 768U + 0x00b74c4cU;
        cm_fn_0055f2c0((int)CM_GLOBAL(volatile u32, tbl));
    }

    w1 = (u32)CM_GLOBAL(volatile u16, 0x00ae317e);
    cm_fn_005e5df0(0x64, 0x22b, 0x316, 0x24e, 0x30U, w1, 0U);
    if (param_3 > 0 && param_2 <= param_3) {
        w2 = (u32)CM_GLOBAL(volatile u16, 0x00ae3184);
        cm_fn_005e6d80(0x78, 0x235, 0xfa, 0x244, 0xcU, 1U, w2, (char *)param_1, -1);
        w1 = (u32)CM_GLOBAL(volatile u16, 0x00ae317e);
        cm_fn_005e5df0(0x10e, 0x235, 0x302, 0x244, 0x60U, w1, 0U);
        w3 = (u32)CM_GLOBAL(volatile u16, 0x00aebe04);
        quotient = (s32)(496U * (u32)param_2) / param_3;
        cm_fn_005e3b50(0x110U, 0x237U, (u32)quotient + 0x110U, 0x242U, 4U, w3);
    } else {
        w2 = (u32)CM_GLOBAL(volatile u16, 0x00ae3184);
        cm_fn_005e6d80(0x78, 0x235, 0x302, 0x244, 0xcU, 1U, w2, (char *)param_1, -1);
    }
    cm_fn_005e2eb0(0x64, 0x22b, 0x316, 0x24e);
    return 1;
}
