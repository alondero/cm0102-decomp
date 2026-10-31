#include "cmre.h"

extern s32 CM_CDECL cm_fn_0053b3d0(s32 *param_1);
extern u32 CM_CDECL cm_fn_004afd80(u32 param_1, s8 param_2);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);

/* True when the first `count` entries of the array contain `value`. */
static int cm_array_contains(const u32 *arr, s16 count, u32 value)
{
    s16 k;

    for (k = 0; k < count; k++) {
        if (arr[k] == value) {
            return 1;
        }
    }
    return 0;
}

/* Thiscall with three stack arguments (ret 0xc). ECX is the receiver; its word at +0x40 is
   compared with 0x07d2. arg1 selects the branch (-1 or not). arg2 is an array of dwords and
   arg3's low word is its signed count. The routine keeps eight dwords of globals in a local
   table, the slots used as indexes by the callee 0x0090cfc0 or the loop counter. All
   globals are read in listing order. The globals are read as volatile so their order
   relative to the callees is kept. */
s32 CM_THISCALL cm_fn_0058ccb0(u8 *param_1, s32 arg1, u32 *arg2, u32 arg3)
{
    u32 slot[8];
    s16 count;
    u32 g_e;
    u32 gf;
    u32 gi;
    s32 x;
    s32 r;
    s32 v;
    s32 word40;
    u32 q;
    int bl;

    slot[0] = CM_GLOBAL(volatile u32, 0x009cf260);
    slot[1] = CM_GLOBAL(volatile u32, 0x009cf360);
    slot[2] = CM_GLOBAL(volatile u32, 0x009cf488);
    slot[5] = CM_GLOBAL(volatile u32, 0x009cf224);
    slot[6] = CM_GLOBAL(volatile u32, 0x009cf300);
    slot[7] = CM_GLOBAL(volatile u32, 0x009cf314);
    slot[3] = CM_GLOBAL(volatile u32, 0x009cf218);
    g_e = CM_GLOBAL(volatile u32, 0x009cf4f8);
    slot[4] = g_e;
    count = (s16)arg3;

    if (arg1 == -1) {
        gf = CM_GLOBAL(volatile u32, 0x009cf280);
        word40 = (s16)CM_GLOBAL(volatile u16, (u32)param_1 + 0x40);
        slot[0] = g_e;
        slot[2] = CM_GLOBAL(volatile u32, 0x009cf3d0);
        slot[1] = gf;
        slot[3] = CM_GLOBAL(volatile u32, 0x009cf368);

        if (word40 < 0x7d2) {
            gi = CM_GLOBAL(volatile u32, 0x00ae23a8);
            x = cm_fn_0053b3d0((s32 *)(gi + 290U * gf));
            if (count <= 0) {
                return x;
            }
            if (!cm_array_contains(arg2, count, (u32)x)) {
                return x;
            }
        }
        for (;;) {
            r = cm_fn_0090cfc0(4);
            /* The original indexes the frame with the signed remainder; a negative value reads the saved-register area below slot[0], which this C does not model. */
            v = (s32)slot[(s32)r];
            gi = CM_GLOBAL(volatile u32, 0x00ae23a8);
            x = cm_fn_0053b3d0((s32 *)(gi + 290U * (u32)v));
            if (count <= 0) {
                return x;
            }
            if (!cm_array_contains(arg2, count, (u32)x)) {
                return x;
            }
        }
    }

    q = (u32)(107 * arg1) + CM_GLOBAL(volatile u32, 0x00ae23d0);
    x = (s32)cm_fn_004afd80(q, 0);
    if (count > 0 && cm_array_contains(arg2, count, (u32)x)) {
        x = (s32)cm_fn_004afd80(q, 1);
        if (count > 0 && cm_array_contains(arg2, count, (u32)x)) {
            x = (s32)cm_fn_004afd80((u32)(107 * arg1) + CM_GLOBAL(volatile u32, 0x00ae23d0), 2);
        }
    }

    bl = 0;
    while (x == 0 && bl < 8) {
        v = (s32)slot[bl];
        gi = CM_GLOBAL(volatile u32, 0x00ae23a8);
        x = cm_fn_0053b3d0((s32 *)(gi + 290U * (u32)v));
        if (count > 0 && cm_array_contains(arg2, count, (u32)x)) {
            x = 0;
        }
        bl++;
    }
    return x;
}
