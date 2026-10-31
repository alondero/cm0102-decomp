#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (this):
   0x04 base of 12-byte element records, 0x08 signed byte count. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_S8(off) (*(volatile s8 *)(self + (off)))

/* Scratch bytes in the 0x20 bytes that `sub esp, 0x20` reserves. Offsets are
   from that base (B). 0x00 holds the 16-bit key and later the dword stored from
   EDI, 0x04 holds the ECX value, 0x08 is the 8-byte buffer that 0x0054a110
   fills, 0x10 is the record returned by 0x0040a7d0, 0x18 is the out-record
   for 0x0054c8e0. The SEH frame is the volatile struct below (B+0x20). */
#define SCR_U16(off) (*(volatile u16 *)(scr + (off)))
#define SCR_U32(off) (*(volatile u32 *)(scr + (off)))

extern u32 CM_STDCALL cm_fn_008e8c40(u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_0040a7d0(int param_1, u32 param_2, u32 param_3);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
/* 0x0054a110 starts with mov eax, ecx: it returns the buffer it fills. */
extern u32 CM_FASTCALL cm_fn_0054a110(u16 *param_1);
/* The listing reads EAX after this call (it is the out-record pointer), so the
   return is declared as a 32-bit value rather than the printed void. */
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);
/* Callee pops two stack arguments (ret 8, both pushed here); the printed
   prototype has one. */
extern int CM_THISCALL cm_fn_0054cdb0(short *param_1, u32 param_2, u32 param_3);

u32 CM_THISCALL cm_fn_008e8ed0(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scr[0x20];
    s8 count;
    u8 index;
    u32 elem;
    u32 sub;
    u32 rec;
    u32 edi_v;
    u32 ecx_v;
    u32 eax_fail;
    u32 eax_v;
    u32 edx_v;
    u32 ret54;
    u32 buf_ret;
    u16 cx_v;
    u16 dx_v;
    u16 dx_g;
    u16 ax_g;
    int res;
    int result;

    seh.state = -1;
    seh.handler = 0x00964a20u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    buf_ret = cm_fn_0054a110((u16 *)(scr + 0x08));
    count = OBJ_S8(0x08);
    index = 0;
    seh.state = 0;

    if (count > 0) {
        for (;;) {
            elem = OBJ_U32(0x04) + (u32)(s32)(s8)index * 12u;
            if (cm_fn_008e8c40(0x00ae2c90u, elem) != 0) {
                goto found;
            }
            count = OBJ_S8(0x08);
            index = (u8)(index + 1u);
            if (!((s8)index < count)) {
                break;
            }
        }
        /* Loop exhausted: EAX is the last search result (0). With a nonzero
           count the test below fails; with a zero count EAX is the scratch
           address from lea eax,[esp+8]. */
        eax_fail = (count != 0) ? 0u : (u32)(scr);
        goto fail;
    }
    /* Loop skipped: EAX is 0x0054a110's result with the low byte replaced by
       the count; a zero count falls to lea eax,[esp+8]. Callers use only AX
       (bx compares, and `and eax, 0xffff` at 0x8cc87b); the upper half is the
       listing's leftover. */
    eax_fail = (count != 0) ? ((buf_ret & 0xffffff00u) | (u8)count)
                            : (u32)(scr);
    goto fail;

found:
    elem = OBJ_U32(0x04) + (u32)(s32)(s8)index * 12u;

    if (elem == 0) {
        eax_fail = elem;
        goto fail;
    }

    cx_v = CM_GLOBAL(volatile u16, 0x00ae2c92);
    sub = elem + 6u;
    SCR_U16(0x00) = cx_v;
    edx_v = SCR_U32(0x00);

    rec = cm_fn_0040a7d0(sub, (u32)(scr + 0x10), edx_v);
    ecx_v = *(volatile u32 *)(rec + 4);
    edi_v = *(volatile u32 *)(rec + 0);
    SCR_U32(0x04) = ecx_v;

    ((volatile u8 *)&seh.state)[0] = 1;
    SCR_U32(0x00) = edi_v;
    ret54 = cm_fn_0054c8e0((short *)0x00ae2c90u, (u32 *)(scr + 0x18), 1);

    cx_v = *(volatile u16 *)(ret54 + 2);
    dx_v = SCR_U16(0x02);
    if (dx_v > cx_v) {
        res = 1;
    } else if (dx_v < cx_v) {
        res = 0;
    } else {
        res = ((s16)(u16)edi_v > *(volatile s16 *)(ret54)) ? 1 : 0;
    }
    result = res;
    ((volatile u8 *)&seh.state)[0] = 0;

    if (result != 0) {
        /* 0x008e8fd9 */
        dx_g = CM_GLOBAL(volatile u16, 0x00ae2c92);
        SCR_U16(0x00) = dx_g;
        eax_v = SCR_U32(0x00);
        rec = cm_fn_0040a7d0(sub, (u32)(scr + 0x18), eax_v);
        edx_v = (u32)(scr + 0x10);
        ((volatile u8 *)&seh.state)[0] = 2;
    } else {
        /* 0x008e9003 */
        ax_g = CM_GLOBAL(volatile u16, 0x00ae2c92);
        SCR_U16(0x00) = ax_g;
        eax_v = SCR_U32(0x00) + 1u;
        rec = cm_fn_0040a7d0(sub, (u32)(scr + 0x18), eax_v);
        edx_v = (u32)(scr + 0x10);
        ((volatile u8 *)&seh.state)[0] = 3;
    }

    cm_fn_00417c10((u32 *)(scr + 0x08), (u32 *)edx_v, (u16 *)rec);

    {
        u32 g94 = CM_GLOBAL(volatile u32, 0x00ae2c94);
        u32 g90 = CM_GLOBAL(volatile u32, 0x00ae2c90);
        ((volatile u8 *)&seh.state)[0] = 0;
        result = cm_fn_0054cdb0((short *)(scr + 0x08), g90, g94);
    }

    CM_FS(u32, 0) = seh.prev;
    return (u32)result;

fail:
    CM_FS(u32, 0) = seh.prev;
    return (eax_fail & 0xffff0000u) | 0x016du;
}
