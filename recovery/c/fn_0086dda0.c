#include "cmre.h"

/* This object (ECX) fields by byte offset: 0x08 pointer to element table base,
   0x14 u16 key, 0x16 u16 second key. Record (stack argument) fields: 0x00 u16,
   0x02 u16. Element table: stride 0xd2; element field 0x86 (index, -1 = empty)
   and field 0xb6 (value, cleared after each element). */

extern u8 CM_FASTCALL cm_fn_0054cfb0(u32 record);
/* ECX is loaded with the global at 0x00b5ded8 before the call; the callee does
   not read it, but the object is still passed as the thiscall first argument. */
extern u32 CM_THISCALL cm_fn_005ef270(u32 ecx_value, u32 pointer, u32 value);

void CM_THISCALL cm_fn_0086dda0(u32 self, u32 rec)
{
    u8 key_byte;
    u16 key16;
    int matched;
    s32 count;
    u32 base;
    u32 p;
    s32 k;
    u32 elem;
    u32 index;
    u32 ecx_value;
    u32 g2;
    u32 g3;
    u32 pointer;

    key_byte = cm_fn_0054cfb0(rec);
    key16 = *(volatile u16 *)(self + 0x14u);
    matched = 0;
    if ((s32)(s8)key_byte == (s32)(u32)key16) {
        u16 rec_word = *(volatile u16 *)(rec + 2u);
        u16 field_word = *(volatile u16 *)(self + 0x16u);
        matched = (rec_word == field_word);
    }

    if (!matched) {
        count = (s32)(CM_GLOBAL(volatile u32, 0x00ae2368u) + 0x10u);
        base = *(volatile u32 *)(self + 8u);
        for (k = 0; k < count; k++) {
            p = base + 0xceu + (u32)k * 0xd2u;
            *(volatile u32 *)(p - 4u) = 0;
            *(volatile u32 *)p = 0;
        }

        key_byte = cm_fn_0054cfb0(rec);
        *(volatile u16 *)(self + 0x14u) = (u16)(s16)(s8)key_byte;
        *(volatile u16 *)(self + 0x16u) = *(volatile u16 *)(rec + 2u);
    }

    if (*(volatile u16 *)rec == 0) {
        count = (s32)(CM_GLOBAL(volatile u32, 0x00ae2368u) + 0x10u);
        base = *(volatile u32 *)(self + 8u);
        if (count > 0) {
            elem = base + 0xb6u;
            k = 0;
            do {
                index = *(volatile u32 *)(elem - 0x30u);
                if (index != 0xffffffffu) {
                    ecx_value = *(volatile u32 *)elem;
                    g2 = CM_GLOBAL(volatile u32, 0x00ae23bcu);
                    g3 = CM_GLOBAL(volatile u32, 0x00b5ded8u);
                    pointer = g2 + index * 110u;
                    cm_fn_005ef270(g3, pointer, ecx_value);
                }
                *(volatile u32 *)elem = 0;
                k = k + 1;
                elem = elem + 0xd2u;
            } while (k < (s32)(CM_GLOBAL(volatile u32, 0x00ae2368u) + 0x10u));
        }
    }
}
