/* Preferred-image VA 008cb6d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep full incidental EAX, including scan-end pointer and rejected role index. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053bd10(u32 group, u32 person, s8 role, u32 date, u32 update, u32 force);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_008cb6d0(u32 group, volatile u32 role)
{
    u8 work[0x200];
    u32 index, target, count, person, row, value, nation, expected, raw_role, text, result;
    u8 kind;
    if (group == 0) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x274eu);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    kind = *(volatile u8 *)&role;
    index = (u32)(s32)(s8)kind - 6u;
    if (index > 9u)
        return index;
    value = B(0x008cb7f8u + index);
    target = D(0x008cb7e8u + value * 4u);
    switch (target) {
    case 0x008cb76fu:
        kind = 8;
        *(volatile u8 *)&role = kind;
        break;
    case 0x008cb773u:
        kind = 6;
        *(volatile u8 *)&role = kind;
        break;
    case 0x008cb77cu:
        break;
    case 0x008cb7dau:
        return index;
    default:
        __builtin_unreachable();
    }
    count = D(0x00ae2364u);
    person = D(0x00ae23bcu);
    row = 0;
    if ((s32)count <= 0)
        return person;
    do {
        value = D(person + 0x39u);
        if (value == 0 && B(person + 0x3du) == kind) {
            value = D(person + 0x69u);
            if (value != 0) {
                nation = D(person + 0x1au);
                expected = D(group + 0x53u);
                if (nation == expected) {
                    raw_role = role;
                    return cm_fn_0053bd10(group, person, (s8)raw_role, 0x00ae2c90u, 1, 0);
                }
            }
        }
        row += 1u;
        person += 0x6eu;
    } while ((s32)row < (s32)count);
    return person;
}
