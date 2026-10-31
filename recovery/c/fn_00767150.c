#include "cmre.h"

/* Original callees (explicit boundaries). */
extern void CM_THISCALL cm_fn_007671c0(u8 *self, u32 a1, u32 a2, u32 a3, u32 a4);
extern u32 CM_THISCALL cm_fn_00768130(u32 self, u32 target, u32 mode_arg);

/* Object fields by byte offset:
   self + 0x09: dword pointer. Zero selects the target value 0xffffffff;
                otherwise the dword it points to is the target value.
   obj + 0x0c:  byte.
   obj + 0x0d:  16-bit word. Pushed as the low word of a dword whose upper
                16 bits come from the global at 0x00ae23bc, which is also the
                table base for entries of 110 * p + 0x39 bytes.
   The original has no explicit return value; the declared return is void,
   so the leftover EAX after the callee is not compared. */

void CM_THISCALL cm_fn_00767150(u32 self, u32 obj, u32 mode)
{
    u32 ptr9;
    u32 target;
    u32 result;
    u32 p;
    u32 table_base;
    u32 entry;
    u32 push1;
    u32 push2;
    u32 entry_value;

    ptr9 = *(volatile u32 *)(self + 9u);
    if (ptr9 == 0u) {
        target = 0xffffffffu;
    } else {
        target = *(volatile u32 *)ptr9;
    }
    result = cm_fn_00768130(obj, target, mode);
    if (result == 0u) {
        return;
    }
    p = *(volatile u32 *)result;
    table_base = CM_GLOBAL(volatile u32, 0x00ae23bcu);
    entry = *(volatile u32 *)(table_base + 0x39u + p * 110u);
    push1 = (table_base & 0xffff0000u) | *(volatile u16 *)(obj + 0x0du);
    if (entry != 0u) {
        push2 = (push1 & 0xffffff00u) | *(volatile u8 *)(obj + 0x0cu);
        entry_value = *(volatile u32 *)entry;
        cm_fn_007671c0((u8 *)self, p, entry_value, push2, push1);
    } else {
        cm_fn_007671c0((u8 *)self, p, 0xffffffffu, *(volatile u8 *)(obj + 0x0cu), push1);
    }
}
