#include "cmre.h"

/* Original cdecl routine with one 16-bit stack argument (si, read as a word).
   Table rows: dword at 0x00b74c4c + (signed word dx) * 0x300. Within a row, the
   element for signed si is at si * 396 + disp, with disp 0xba966 for the first
   and 0xba9a6 for the second family. Object 0x00b74450 is the `this` for every
   callee. Callee 0x007ff010 takes one stack dword and returns a dword in EAX;
   callee 0x007ff270 takes three stack dwords and pops them (thiscall). */

#define OBJ_B74450 0x00b74450u
#define ROW_TABLE 0x00b74c4cu
#define ROW_STRIDE 0x300u
#define ELEM_STRIDE 396u /* 4 * 99: element index is scaled by 99, then by 4 */
#define DISP_A 0x000ba966u
#define DISP_B 0x000ba9a6u
#define RET_FAIL 0xfffffff5u /* -11 */

extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 arg);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2, u32 arg3);

static u32 table_dword(u16 dx, u16 si, u32 disp)
{
    u32 row = CM_GLOBAL(volatile u32, ROW_TABLE + (u32)(s32)(s16)dx * ROW_STRIDE);
    return *(volatile u32 *)(row + (u32)(s32)(s16)si * ELEM_STRIDE + disp);
}

u32 CM_CDECL cm_fn_0088b2f0(u16 arg_si)
{
    u16 dx = CM_GLOBAL(u16, 0x00b77c7a);
    u16 si = arg_si;
    u16 t;
    u32 edi;
    u32 r;
    u32 n;

    /* First lookup: dword from family A, low word tested, 0 falls back to 0xdd6bde. */
    t = (si != 0xffffu) ? (u16)table_dword(dx, si, DISP_A) : (u16)0;
    if (t == 0) {
        t = CM_GLOBAL(u16, 0x00dd6bde);
    }

    if (t == 0x12u) {
        if (si != 0xffffu) {
            edi = table_dword(dx, si, DISP_B);
        } else {
            edi = 0;
        }
        r = cm_fn_007ff010(OBJ_B74450, 4u);
        if (edi != r) {
            if (si != 0xffffu) {
                dx = CM_GLOBAL(u16, 0x00b77c7a);
                /* Second family read, pushed as the middle argument. */
                edi = table_dword(dx, si, DISP_B);
                cm_fn_007ff270(OBJ_B74450, 4u, edi, 0u);
            } else {
                cm_fn_007ff270(OBJ_B74450, 4u, 0u, 0u);
            }
            return RET_FAIL;
        }
        dx = CM_GLOBAL(u16, 0x00b77c7a);
    }

    /* 0x88b3e0: family A again, with the 0x27 check. */
    t = (si != 0xffffu) ? (u16)table_dword(dx, si, DISP_A) : (u16)0;
    if (t == 0) {
        t = CM_GLOBAL(u16, 0x00dd6bde);
    }

    if (t == 0x27u) {
        r = cm_fn_007ff010(OBJ_B74450, 0x13u);
        if ((s8)(u8)r > 0) {
            n = (u32)((s32)(s8)(u8)r - 1);
            cm_fn_007ff270(OBJ_B74450, 0x13u, n, 0u);
            return RET_FAIL;
        }
        dx = CM_GLOBAL(u16, 0x00b77c7a);
    }

    /* 0x88b43e: family A with the 0x28 check. */
    t = (si != 0xffffu) ? (u16)table_dword(dx, si, DISP_A) : (u16)0;
    if (t == 0) {
        t = CM_GLOBAL(u16, 0x00dd6bde);
    }
    if (t != 0x28u) {
        return 0;
    }
    r = cm_fn_007ff010(OBJ_B74450, 0x13u);
    n = (u32)((s32)(s8)(u8)r + 1);
    if ((s32)n >= 4) {
        return 0;
    }
    cm_fn_007ff270(OBJ_B74450, 0x13u, n, 0u);
    return RET_FAIL;
}
