#include "cmre.h"
/* Preferred-image VA 0x0041ccd0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every matching key replaces the selected row; the scan retains the last. */
extern u32 CM_CDECL cm_fn_005456a0(u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern s32 CM_CDECL cm_fn_00951c50(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_CDECL cm_fn_0041ccd0(u32 query, u32 alternate)
{
    u32 selected, index, key, table, row, pointer;
    s32 count;
    if (!D(0x00989858)) return 0;
    if (!D(0x0098988e)) return 0;
    if (!query) {
        count = (s16)W(0x0098988c);
        selected = (u32)cm_fn_0090cfc0(count);
    } else {
        index = 0;
        selected = 0xffffffffu;
        if ((s16)W(0x0098988c) <= 0) return 0;
        do {
            key = cm_fn_005456a0(query);
            table = D(0x0098988e);
            pointer = D(table + (u32)(s32)(s16)index * 16u);
            if (!cm_fn_00951c50(pointer, key)) selected = index;
            ++index;
        } while ((s16)index < (s16)W(0x0098988c));
    }
    if ((u16)selected == 0xffff) return 0;
    row = (u32)(s32)(s16)selected * 16u;
    table = D(0x0098988e);
    if (alternate) {
        if (!D(table + row + 12)) return 0;
        count = (s16)W(table + row + 6);
        index = (u32)(s32)(s16)cm_fn_0090cfc0(count);
        table = D(0x0098988e);
        pointer = D(table + row + 12);
    } else {
        if (!D(table + row + 8)) return 0;
        count = (s16)W(table + row + 4);
        index = (u32)(s32)(s16)cm_fn_0090cfc0(count);
        table = D(0x0098988e);
        pointer = D(table + row + 8);
    }
    return D(pointer + index * 4u);
}
