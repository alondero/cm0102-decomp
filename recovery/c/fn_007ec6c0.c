/* WIP: fourth argument to 0x007671c0 is `push edi` after `mov di, [P+0xd]` (0x7ec704,
   0x7ec750); its upper 16 bits are the caller's entry EDI, and 0x7671c0 loads the
   full dword ([esp+0x24] at 0x767285) and forwards it. Plain C cannot read entry EDI;
   needs a boundary decision. Test passes only because the harness does not compare it. */
#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (self):
   0x04 pointer to a helper object; its 0x0c (first block) or 0x18 (second
   block) field is the ECX receiver of 0x007671c0.
   0x0a pointer P to a record: P+0x0c byte (compared with 5, then passed as the
   low byte of the third argument), P+0x0d word (low word of the fourth argument).
   Table: the global at 0x00ae23bc points to 0x6e-byte rows; the row for index X
   is at table + 0x39 + X*0x6e and holds a pointer whose first DWORD is passed
   as the second argument (0 when the pointer is null). */

extern u32 CM_THISCALL cm_fn_00768130(u32 self, u32 target, u32 mode_arg);
extern void CM_THISCALL cm_fn_007671c0(u8 *self, u32 a1, u32 a2, u32 a3, u32 a4);

void CM_THISCALL cm_fn_007ec6c0(u32 self)
{
    u32 p;
    u32 global;
    u32 r;
    u32 x;
    u32 row;
    u32 value;
    u32 q;
    u16 word;
    u32 byte_arg;
    u32 obj;
    u32 receiver;

    p = *(volatile u32 *)(self + 0x0a);
    if (*(volatile u8 *)(p + 0x0c) != 5u) {
        /* First block: global at 0x009cf930, ECX still holds P from above. */
        global = CM_GLOBAL(u32, 0x009cf930);
        r = cm_fn_00768130(p, global, 0xffffffffu);
        if (r != 0) {
            x = *(volatile u32 *)r;
            row = *(volatile u32 *)(CM_GLOBAL(u32, 0x00ae23bc) + 0x39u + x * 0x6eu);
            if (row == 0) {
                value = 0;
            } else {
                value = *(volatile u32 *)row;
            }
            q = *(volatile u32 *)(self + 0x0a);
            word = *(volatile u16 *)(q + 0x0d);
            byte_arg = (q & 0xffffff00u) | (u32)*(volatile u8 *)(q + 0x0c);
            obj = *(volatile u32 *)(self + 0x04);
            receiver = *(volatile u32 *)(obj + 0x0c);
            cm_fn_007671c0((u8 *)receiver, x, value, byte_arg, (u32)word);
        }

        /* Second block: global at 0x009cf92c, P re-read into ECX before the call. */
        global = CM_GLOBAL(u32, 0x009cf92c);
        q = *(volatile u32 *)(self + 0x0a);
        r = cm_fn_00768130(q, global, 0xffffffffu);
        if (r != 0) {
            x = *(volatile u32 *)r;
            row = *(volatile u32 *)(CM_GLOBAL(u32, 0x00ae23bc) + 0x39u + x * 0x6eu);
            if (row == 0) {
                value = 0;
            } else {
                value = *(volatile u32 *)row;
            }
            q = *(volatile u32 *)(self + 0x0a);
            word = *(volatile u16 *)(q + 0x0d);
            byte_arg = (q & 0xffffff00u) | (u32)*(volatile u8 *)(q + 0x0c);
            obj = *(volatile u32 *)(self + 0x04);
            receiver = *(volatile u32 *)(obj + 0x18);
            cm_fn_007671c0((u8 *)receiver, x, value, byte_arg, (u32)word);
        }
    }
}
