#include "cmre.h"

extern s32 CM_THISCALL cm_fn_005a0590(u32 self, u32 element);
extern void CM_THISCALL cm_fn_005987c0(u32 self, u32 arg);
extern void CM_THISCALL cm_fn_0048ea40(void *self, u32 element, u32 arg);

/* Thiscall (ECX = self), two stack arguments named by their entry offsets
   (arg_04 = [entry+4], arg_08 = [entry+8]), `ret 8`, no return value.
   Field offsets from self: 0x3e is a signed 16-bit count, 0xb1 is a pointer
   to a table of 59-byte entries (each entry holds one u32 element pointer).
   The loop index is a 16-bit value (BX) read as signed (MOVSX) for the
   0x3b offset. The global at 0x00ae2a38 gates the whole loop and is the
   ECX object for 0x5a0590.

   Stack lifetime (from the listing): arg_04 is pushed before the element for
   0x5a0590 and stays on the stack until 0x5987c0 consumes it; the element
   is pushed on top and popped by 0x5a0590 (thiscall, callee-cleaned). The
   second element read happens after 0x5987c0 and is pushed with arg_04 for
   0x48ea40, which also pops its arguments (ret 8). Table and element values
   are re-read through volatile accesses at each use, as in the listing. */
void CM_THISCALL cm_fn_00683010(void *self, u32 arg_04, u32 arg_08)
{
    volatile u8 *base = (volatile u8 *)self;
    u16 index;
    s16 i;
    u32 off;
    u32 table;
    u32 elem;
    u32 ret;
    int matched;

    if (CM_GLOBAL(s32, 0x00ae2a38) == 0)
        return;
    index = 0;
    if (*(volatile s16 *)(base + 0x3e) <= (s16)index)
        return;

    do {
        i = (s16)index;
        off = (u32)((s32)i * 59);
        matched = 1;
        if (arg_08 != 0) {
            table = *(volatile u32 *)(base + 0xb1);
            elem = *(volatile u32 *)(table + off);
            matched = (*(volatile u32 *)(elem + 0x5b) == arg_08);
        }
        if (matched) {
            table = *(volatile u32 *)(base + 0xb1);
            elem = *(volatile u32 *)(table + off);
            ret = (u32)cm_fn_005a0590(CM_GLOBAL(u32, 0x00ae2a38), elem);
            cm_fn_005987c0(ret, arg_04);
            table = *(volatile u32 *)(base + 0xb1);
            elem = *(volatile u32 *)(table + off);
            cm_fn_0048ea40(self, elem, arg_04);
        }
        index++;
    } while ((s16)index < *(volatile s16 *)(base + 0x3e));
}
