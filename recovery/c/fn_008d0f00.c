#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x86e pointer to a table with 8-byte entries (flag byte at entry +0, tested with 0x04);
   0x898 and 0x8a0 u32 values passed to 0x00946464 (0x8a0 is read before 0x898). */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern u32 CM_CDECL cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

/* One stack argument (ret 4): a pointer to a u32 index. Returns 0 when the flag
   is clear or when 0x00946464 returns 0 (the entry byte is then decremented by 4);
   otherwise returns the nonzero result of 0x00946464 unchanged. */
u32 CM_THISCALL cm_fn_008d0f00(u32 self, u32 arg1)
{
    /* Scratch slot at the original's local L+0x00; its address is passed to 0x00946464. */
    u32 slot;
    u32 index;
    u32 table;
    u32 field898;
    u32 field8a0;
    u32 result;
    u8 entry;

    index = *(volatile u32 *)arg1;
    table = OBJ_U32(0x86e);
    if ((*(volatile u8 *)(table + index * 8u) & 0x04) == 0) {
        return 0u;
    }

    slot = index;
    field8a0 = OBJ_U32(0x8a0);
    field898 = OBJ_U32(0x898);
    result = cm_fn_00946464((u32)&slot, field898, field8a0, 13, (void *)0x008b4e00u);
    if (result != 0) {
        return result;
    }

    index = *(volatile u32 *)arg1;
    table = OBJ_U32(0x86e);
    entry = *(volatile u8 *)(table + index * 8u);
    *(volatile u8 *)(table + index * 8u) = (u8)(entry + 0xfc);
    return 0u;
}
