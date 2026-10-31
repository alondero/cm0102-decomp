#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x86e pointer to a table with 8-byte entries (flag byte at entry +0, tested with 0x20),
   0x93a and 0x93e u32 values passed to 0x00946464 (0x93e is read first). */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern u32 CM_CDECL cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

/* One stack argument (ret 4): a pointer to a u32 value. A null pointer is read
   without a test and faults at address 0, as in the original. */
u32 CM_THISCALL cm_fn_008c7260(u32 self, u32 arg1)
{
    /* Scratch slot at the original's local L+0x00; its address is passed to 0x00946464. */
    u32 slot;
    u32 value;
    u32 table;
    u32 field93e;
    u32 field93a;

    if (arg1 != 0) {
        s32 first = (s32)*(volatile u32 *)arg1;
        if (first >= CM_GLOBAL(volatile s32, 0x00ae2364)) {
            return 0u;
        }
    }

    value = *(volatile u32 *)arg1;
    table = OBJ_U32(0x86e);
    if ((*(volatile u8 *)(table + value * 8u) & 0x20) == 0) {
        return 0u;
    }

    slot = value;
    field93e = OBJ_U32(0x93e);
    field93a = OBJ_U32(0x93a);
    return cm_fn_00946464((u32)&slot, field93a, field93e, 14, (void *)0x008e55c0u);
}
