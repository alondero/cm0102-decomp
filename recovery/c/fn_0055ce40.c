#include "cmre.h"

/* Preferred-image VA 0x0055ce40, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EAX is retained across AL-only return paths; the type map and target
   table remain explicit pinned reads. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0055ce40(u32 self, volatile u32 source,
    volatile u32 output, volatile u32 length, volatile u32 mode,
    volatile u32 extra)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, result = length, address, text, format;
    u32 all_four = 1, first_two = 1, first_three = 1;
    u32 table, object, target, dest, capacity, last, mapped;
    u8 selected = 0xff, value, kind;
    if ((s32)result < 0x46) {
        cm_fn_00944cff(0x009b8f18, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x1475);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return (result & 0xffffff00u) | 0xff;
    }
    address = source;
    value = B(address);
    result = (result & 0xffffff00u) | value;
    if (value != 0xff) selected = value;
    else first_three = first_two = all_four = 0;
    value = B(address + 1);
    result = (result & 0xffffff00u) | value;
    if (value != 0xff) {
        if (selected == 0xff) selected = value;
    } else first_three = first_two = all_four = 0;
    value = B(address + 2);
    result = (result & 0xffffff00u) | value;
    if (value != 0xff) {
        if (selected == 0xff) selected = value;
        first_two = 0;
    } else first_three = all_four = 0;
    value = B(address + 3);
    result = (result & 0xffffff00u) | value;
    if (value != 0xff) {
        if (mode != 0) {
            dest = output;
            result = cm_fn_0066f4e0(dest, 0x009c3478);
            return (result & 0xffffff00u) | 0xff;
        }
        if (selected == 0xff) selected = value;
        if (all_four == 0) goto virtual_call;
        format = 0x009c346c;
    } else if (first_two != 0) {
        format = first_three != 0 ? 0x009c3460 : 0x009c3454;
    } else if (first_three != 0) {
        format = 0x009c3460;
    } else {
        goto virtual_call;
    }
    dest = output;
    result = cm_fn_0066f4e0(dest, format);
    goto classify;
virtual_call:
    table = D(self);
    object = D(table + (u32)(s32)(s8)selected * 4u);
    last = extra;
    capacity = length;
    table = D(object);
    dest = output;
    target = D(table + 0x10);
    result = ((u32 (CM_THISCALL *)(u32, u32, u32, u32))target)
        (object, dest, capacity, last);
classify:
    table = D(self);
    object = D(table + (u32)(s32)(s8)selected * 4u);
    kind = B(object + 4);
    result = (result & 0xffffff00u) | kind;
    if (kind == 1) return (result & 0xffffff00u) | 9;
    if (kind == 0) {
        value = B(object + 0x20);
        return (u8)(value + 1u) != 0 ? 8 : 0;
    }
    if (kind == 3) return (result & 0xffffff00u) | 1;
    result = (u32)(s32)(s8)B(object + 5) - 1u;
    if (result > 0x28) return (result & 0xffffff00u) | 0xff;
    mapped = B(0x0055d074 + result);
    target = D(0x0055d060 + mapped * 4u);
    switch (target) {
    case 0x0055d028: return (result & 0xffffff00u) | 2;
    case 0x0055d036: return (result & 0xffffff00u) | 6;
    case 0x0055d044: return (result & 0xffffff00u) | 7;
    case 0x0055d052: return (result & 0xffffff00u) | 3;
    default: return (result & 0xffffff00u) | 0xff;
    }
}
