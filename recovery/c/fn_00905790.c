/* Preferred-image VA 00905790, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain incidental full EAX on all paths, including diagnostic return. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00905790(u32 self, u32 id, volatile u32 second, volatile u32 third)
{
    u8 work[0x200];
    u32 table, pointer, value, target, index, row, text, result;
    u8 selected = 0xffu;
    table = D(0x00ae23d0u);
    pointer = D(table + id * 107u + 0x59u);
    if (pointer == 0)
        return 0;
    value = D(pointer);
    target = D(0x009cfa10u);
    if (value != target)
        return value;
    for (index = 0; index < 50u; ++index) {
        value = D(self + index * 12u + 0x440u);
        if (value == id) {
            selected = (u8)index;
            break;
        }
        if (selected == 0xffu && value == 0xffffffffu)
            selected = (u8)index;
    }
    if (selected == 0xffu) {
        cm_fn_00944cff(0x00aa1a1cu, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x6b2u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    row = self + (u32)(s32)(s8)selected * 12u;
    D(row + 0x440u) = id;
    result = second;
    D(row + 0x444u) = result;
    value = third;
    D(row + 0x448u) = value;
    return result;
}
