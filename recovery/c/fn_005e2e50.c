#include "cmre.h"

/* 0x005e2e50: when the global at 0x00aebe0c is zero, query each live
   singleton (0x00ae31a4, then 0x00aebde4) through its vtable: slot +0x60
   selects, slot +0x6c runs on the same object.
   Slot +0x60 (005e2e62/005e2e64, 005e2e80/005e2e82): ECX is loaded with the
   object's vtable pointer, the object itself is pushed, and no caller stack
   cleanup follows, so the callee is thiscall with the vtable value as the
   receiver and the object as the single stack argument.
   Slot +0x6c (005e2e6c-005e2e74, 005e2e8a-005e2e92): the listing sets up no
   ECX for this call (only EAX reload, push and the EDX table load), so the
   receiver is not part of that boundary; only the pushed object is. */
typedef u32 (CM_THISCALL *cm_slot_select_fn)(u32 table, u32 object);
typedef u32 (CM_STDCALL *cm_slot_run_fn)(u32 object);

u32 cm_fn_005e2e50(void)
{
    u32 result = CM_GLOBAL(volatile u32, 0x00aebe0c);
    u32 object;
    u32 table;

    if (result != 0) {
        return result;
    }

    object = CM_GLOBAL(volatile u32, 0x00ae31a4);
    if (object != 0) {
        table = *(volatile u32 *)object;
        result = ((cm_slot_select_fn)*(volatile u32 *)(table + 0x60))(table, object);
        if (result != 0) {
            object = CM_GLOBAL(volatile u32, 0x00ae31a4);
            table = *(volatile u32 *)object;
            result = ((cm_slot_run_fn)*(volatile u32 *)(table + 0x6c))(object);
        }
    }

    object = CM_GLOBAL(volatile u32, 0x00aebde4);
    /* 005e2e77 overwrites EAX; the null branch returns that zero. */
    result = object;
    if (object != 0) {
        table = *(volatile u32 *)object;
        result = ((cm_slot_select_fn)*(volatile u32 *)(table + 0x60))(table, object);
        if (result != 0) {
            object = CM_GLOBAL(volatile u32, 0x00aebde4);
            table = *(volatile u32 *)object;
            result = ((cm_slot_run_fn)*(volatile u32 *)(table + 0x6c))(object);
        }
    }
    return result;
}
