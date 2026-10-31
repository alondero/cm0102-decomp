#include "cmre.h"

/* Thiscall with ECX = object (read before written). One stack argument: key.
   Object field +0x08 is a table T of 30 row pointers (dwords).
   Row fields: +0x08 signed 16-bit count n, +0x04 list pointer L.
   List entries are element pointers Q; the element dword at Q+0x09 is compared
   with key. Scanning order: rows i = 0..29, entries j = 0..n-1 (n cached per row).
   On a hit, returns the dword at L[j] (re-read through the row). Otherwise 0. */
u32 CM_THISCALL cm_fn_00414ac0(u32 self, u32 key)
{
    u32 table;
    u32 i;
    u32 row;
    s32 n;
    s32 j;
    u32 list;
    u32 q;
    u32 row2;
    u32 list2;

    table = *(volatile u32 *)(self + 8);
    i = 0;
    do {
        row = *(volatile u32 *)(table + i * 4u);
        if (row != 0) {
            n = (s32)(s16)*(volatile u16 *)(row + 8);
            j = 0;
            if (n > 0) {
                list = *(volatile u32 *)(row + 4);
                do {
                    q = *(volatile u32 *)(list + (u32)j * 4u);
                    if (*(volatile u32 *)(q + 9) == key) {
                        row2 = *(volatile u32 *)(table + i * 4u);
                        list2 = *(volatile u32 *)(row2 + 4);
                        return *(volatile u32 *)(list2 + (u32)j * 4u);
                    }
                    j = j + 1;
                } while (j < n);
            }
        }
        i = i + 1;
    } while (i < 0x1e);
    return 0;
}
