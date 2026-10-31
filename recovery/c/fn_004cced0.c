#include "cmre.h"

/* Object field access by byte offset from an address (no field names).
   Offsets used: 0x00, 0x04, 0x1c (u8), 0x25 (call receiver base), 0xd7 (base
   of the pointer scan, from the argument slot), 0x02 (u16 at the table pointer
   returned by 0x0054c5b0). The record stride is 80 bytes: rec = 80 * v + *(this). */
#define OBJ_U8(obj, off) (*(volatile u8 *)((u32)(obj) + (off)))
#define OBJ_U16(obj, off) (*(volatile u16 *)((u32)(obj) + (off)))
#define OBJ_U32(obj, off) (*(volatile u32 *)((u32)(obj) + (off)))

/* Scratch frame: 0x20 bytes at the stack base after `sub esp, 0x20`.
   frame[1] (+0x04) is the out-word written by 0x00540a50, frame[2] (+0x08)
   is the saved receiver (ECX), frame[3] (+0x0c) is its result, frame[4]
   (+0x10) and frame[5] (+0x14) hold 0x00ae2c90 and 0x00ae2c94, and frame[6]
   (+0x18) is the buffer passed to 0x0054c5b0. Byte 0x03 is the outer counter
   and is kept in a local. Byte access reads the high word of frame[4]. */
#define FRAME_BYTE(frame, off) (((u8 *)(frame))[(off)])

extern int CM_CDECL cm_fn_00540a50(u32 *param_1, u32 *param_2, int param_3);
extern u16 *CM_THISCALL cm_fn_0054c5b0(void *self, u32 *param_2, int param_3);
extern void CM_THISCALL cm_fn_008e3db0(void *self, u32 *param_1, u32 *param_2, char param_3, int param_4);

u32 CM_THISCALL cm_fn_004cced0(u32 self, u32 arg1, u32 arg2)
{
    u32 frame[8];
    u32 arg1_slot;
    u32 arg2_copy;
    u32 count;
    u8 iter;
    u32 r1;
    u32 ptr_ebp;
    u32 entry;
    u32 entry2;
    u32 g_a;
    u32 key;
    u32 key_x79;
    u32 g_b;
    u32 v;
    u32 this_v;
    u32 rec;
    u32 eax_word;
    u32 ecx_word;
    u32 g_c;
    u32 g_d;
    u32 g_e;
    u16 *tbl;
    u16 cx_w;
    u16 dx_w;

    frame[2] = self;
    arg1_slot = arg1;
    if (arg1_slot == 0) {
        return 0;
    }
    r1 = (u32)cm_fn_00540a50((u32 *)arg1_slot, &frame[1], 1);
    frame[3] = r1;
    if (r1 != 0) {
        if (frame[1] == 0) {
            return r1;
        }
    }

    arg2_copy = arg2;
    iter = 0;
    do {
        /* At this point EAX is r1 on every path into the outer loop. */
        if (iter == 1) {
            arg1_slot = r1;
        }
        if (arg1_slot == 0) {
            goto outer_next;
        }
        ptr_ebp = arg1_slot + 0xd7u;
        count = 0x32u;
        do {
            entry = OBJ_U32(ptr_ebp, 0);
            if (entry == 0) {
                goto inner_next;
            }
            g_a = CM_GLOBAL(volatile u32, 0x00ae2364u);
            key = OBJ_U32(entry, 0);
            if ((s32)key >= (s32)g_a) {
                goto inner_next;
            }
            key_x79 = key * 79u;
            g_b = CM_GLOBAL(volatile u32, 0x00ae2d0cu);
            v = OBJ_U32(key_x79 + g_b, 0);
            if ((s32)v < 0) {
                goto inner_next;
            }
            this_v = frame[2];
            rec = v * 80u + OBJ_U32(this_v, 0);
            if (rec == 0) {
                goto inner_next;
            }
            eax_word = OBJ_U32(rec, 0x04);
            ecx_word = OBJ_U32(arg1_slot, 0);
            if (eax_word != ecx_word) {
                goto inner_next;
            }
            if (OBJ_U8(rec, 0x1c) != 1) {
                goto inner_next;
            }
            g_c = CM_GLOBAL(volatile u32, 0x00ae2c94u);
            g_d = CM_GLOBAL(volatile u32, 0x00ae2c90u);
            frame[5] = g_c;
            frame[4] = g_d;
            tbl = cm_fn_0054c5b0((void *)(rec + 0x25u), &frame[6], 0x2d);
            cx_w = OBJ_U16(tbl, 0x02);
            dx_w = (u16)(FRAME_BYTE(frame, 0x12) | ((u32)FRAME_BYTE(frame, 0x13) << 8));
            if (dx_w < cx_w) {
                goto inner_next;
            }
            if (dx_w > cx_w) {
                goto action2;
            }
            if ((s16)(u16)g_d < (s16)OBJ_U16(tbl, 0x00)) {
                goto inner_next;
            }

action2:
            OBJ_U8(rec, 0x1c) = 2;
            entry2 = OBJ_U32(ptr_ebp, 0);
            g_e = CM_GLOBAL(volatile u32, 0x00de1f54u);
            cm_fn_008e3db0((void *)g_e, (u32 *)entry2, (u32 *)arg2_copy, 0, (int)rec);

inner_next:
            ptr_ebp = ptr_ebp + 4u;
            count = count - 1u;
        } while (count != 0);

outer_next:
        iter = (u8)(iter + 1);
    } while (iter < 2);

    return r1;
}
