#include "cmre.h"

/* Object fields by byte offset:
   self (ECX on entry): +0x04 pointer, +0x08 byte limit, +0x0c..+0x0f via the
   table at +0x13 byte copy; +0x13 byte copied into a frame slot; +0x12 and
   +0x13 frame bytes are counters/flags (see slot12/slot13 below).
   Argument 2 object: +0x00 DWORD tag compared with a global, +0x53 pointer to a
   pointer, +0x57 pointer whose first DWORD is compared with a second global.
   Argument 1 object: +0x04 record pointer, +0x08 DWORD tag, +0x0c DWORD index.
   Element table at arg2 + 0xd7 (4-byte pointers, up to 50 entries).
   Globals: 0x009cf4dc and 0x009cf8d8 are DWORD tags; 0x00ae23bc is the base for
   records of stride 110 * index. */

extern u32 CM_CDECL cm_fn_0053d980(u32 arg_1, u32 arg_2, u32 arg_3);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 param_1);
extern u32 CM_STDCALL cm_fn_008e8c40(u32 arg1, u32 arg2);

#define FIELD_U8(base, off) (*(volatile u8 *)((base) + (u32)(off)))
#define FIELD_U16(base, off) (*(volatile u16 *)((base) + (u32)(off)))
#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (u32)(off)))

u32 CM_THISCALL cm_fn_008e8800(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 this_ptr;
    u32 ebp_obj;
    u32 esi_obj;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 p53;
    u32 v;
    u32 p;
    u32 q;
    u32 r;
    u32 g;
    u32 table_ptr;
    u32 element;
    u8 bl;
    u8 slot12;
    u8 slot13;
    u8 slot20;
    u8 al;

    this_ptr = self;
    edx = CM_GLOBAL(volatile u32, 0x009cf4dc);
    bl = 0;
    al = FIELD_U8(this_ptr, 0x13);
    slot13 = al;
    eax = FIELD_U32(arg2, 0x53);
    slot12 = bl;
    ecx = FIELD_U32(eax, 0);
    if (ecx == edx) {
        ecx = FIELD_U32(arg2, 0x57);
        if (ecx == 0) {
            slot13 = 0;
        } else {
            edx = FIELD_U32(ecx, 0);
            ecx = CM_GLOBAL(volatile u32, 0x009cf8d8);
            if (edx != ecx) {
                slot13 = 0;
            }
        }
    }

    ebp_obj = arg1;
    esi_obj = arg2;
    ecx = FIELD_U32(esi_obj, 0);
    if (ecx != FIELD_U32(ebp_obj, 8)) {
        goto block_c;
    }

    /* Block B: first test call, then the record walk. */
    p53 = eax;
    v = FIELD_U32(ebp_obj, 0xc);
    g = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053d980(g + 110u * v, p53, 1u);
    if (r != 0) {
        slot12 = 1;
        bl = slot12;
    }

    p = FIELD_U32(ebp_obj, 4);
    if (p == 0) {
        goto after_walk;
    }
    FIELD_U32(p, 0xa) = FIELD_U32(p, 2);
    q = cm_fn_0068d1b0(FIELD_U32(ebp_obj, 4));
    if (q == 0) {
        goto after_walk;
    }
block_b_loop:
    v = FIELD_U32(q, 0);
    p53 = FIELD_U32(esi_obj, 0x53);
    g = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053d980(g + 110u * v, p53, 1u);
    if (r != 0) {
        goto success_tail;
    }
    q = cm_fn_0068d1b0(FIELD_U32(ebp_obj, 4));
    if (q != 0) {
        goto block_b_loop;
    }
    goto after_walk;

block_c:
    p = FIELD_U32(ebp_obj, 4);
    if (p == 0) {
        goto success_tail;
    }
    FIELD_U32(p, 0xa) = FIELD_U32(p, 2);
    q = cm_fn_0068d1b0(FIELD_U32(ebp_obj, 4));
    if (q == 0) {
        goto success_tail;
    }
block_c_loop:
    v = FIELD_U32(q, 0);
    p53 = FIELD_U32(esi_obj, 0x53);
    g = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053d980(g + 110u * v, p53, 1u);
    if (r != 0) {
        bl = bl + 1u;
    }
    q = cm_fn_0068d1b0(FIELD_U32(ebp_obj, 4));
    if (q != 0) {
        goto block_c_loop;
    }
    slot12 = bl;
    if (bl == 0) {
        goto success_tail;
    }
    p53 = FIELD_U32(esi_obj, 0x53);
    v = FIELD_U32(ebp_obj, 0xc);
    g = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053d980(g + 110u * v, p53, 1u);
    if (r == 0) {
        goto after_walk;
    }
    bl = bl - 1u;
    slot12 = bl;
    goto after_walk;

after_walk:
    /* 0x008e896d: bl is the signed count of pending elements. */
    if ((s8)bl <= 0) {
        goto success_tail;
    }
    bl = (u8)(0u - (u32)bl);
    slot20 = 0;
    table_ptr = esi_obj + 0xd7u;
    do {
        element = FIELD_U32(table_ptr, 0);
        if (element != 0) {
            p53 = FIELD_U32(esi_obj, 0x53);
            r = cm_fn_0053d980(element, p53, 1u);
            if (r != 0) {
                slot12 = (u8)(slot12 + 1u);
                goto compare_counts;
            }
        }
        bl = bl + 1u;
    compare_counts:
        al = slot13;
        if ((s8)slot12 > (s8)al) {
            goto mismatch_return;
        }
        ecx = (u32)(s32)(s8)al;
        edx = 0x32u - ecx;
        ecx = (u32)(s32)(s8)bl;
        if ((s32)ecx > (s32)edx) {
            goto compare_slot12;
        }
        al = slot20;
        table_ptr = table_ptr + 4u;
        al = (u8)(al + 1u);
        slot20 = al;
    } while ((s8)al < 0x32);
    al = slot13;

compare_slot12:
    if ((s8)slot12 <= (s8)al) {
        goto element_loop_setup;
    }

mismatch_return:
    p = arg4;
    if (p != 0) {
        FIELD_U16(p, 0) = 0x14;
    }
    return 0;

element_loop_setup:
    this_ptr = self;

success_tail:
    edx = FIELD_U32(esi_obj, 0);
    eax = FIELD_U32(ebp_obj, 8);
    if (edx != eax) {
        return 1;
    }
    al = FIELD_U8(this_ptr, 8);
    bl = 0;
    if ((s8)al <= 0) {
        goto pair_end;
    }
    esi_obj = arg3;
    do {
        ecx = FIELD_U32(this_ptr, 4);
        eax = (u32)(s32)(s8)bl;
        eax = eax * 3u;
        element = ecx + eax * 4u;
        r = cm_fn_008e8c40(esi_obj, element);
        if (r != 0) {
            return 1;
        }
        al = FIELD_U8(this_ptr, 8);
        bl = bl + 1u;
    } while ((s8)bl < (s8)al);

pair_end:
    if (al == 0) {
        return 1;
    }
    p = arg4;
    if (p != 0) {
        FIELD_U16(p, 0) = 9;
    }
    return 0;
}
