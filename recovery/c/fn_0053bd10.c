#include "cmre.h"

/* Preferred-image VA 0x0053bd10, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep partial writes on failure: dispatch 0x0053c043 stores at +0xcf even
   when the later +0xd7 array has no free slot. The +0xd3/+0x19f variants
   instead succeed and change the role byte when that array is full. */
extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *scratch, u16 *date);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern u32 CM_CDECL cm_fn_00600a50(u32 object, u32 arg2, s8 arg3, u32 arg4, u32 arg5);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))
#define U8_AT(base, offset) (*(volatile u8 *)((base) + (offset)))

static u32 first_free(u32 base, u32 capacity)
{
    u32 index = 0;
    while (index < capacity) {
        if (U32_AT(base + index * 4u, 0) == 0)
            return index;
        ++index;
    }
    return capacity;
}

u32 CM_CDECL cm_fn_0053bd10(u32 group, u32 person, s8 role, u32 date,
                            u32 update_person, u32 force_role)
{
    u8 scratch[0x208];
    u32 threshold, value, twice, limit, field, target, index, capacity;
    char *version;
    u8 *first, *second;
    u32 line;

    if (group == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x108, scratch + 8);
        first = scratch + 0x108;
        second = scratch + 8;
        line = 0x35a0;
        goto report;
    }
    if (person == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 8, scratch + 0x108);
        first = scratch + 8;
        second = scratch + 0x108;
        line = 0x35a7;
        goto report;
    }
    threshold = CM_GLOBAL(volatile u32, 0x00ae2364);
    value = U32_AT(person, 0);
    if ((s32)value >= (s32)(threshold - 0x10u) && role != 5)
        return 0;
    if (date != 0) {
        twice = CM_GLOBAL(volatile u32, 0x00ae2350) * 2u;
        limit = CM_GLOBAL(volatile u32, 0x00ae235c) - twice;
        value = U32_AT(group, 0);
        field = (s32)value >= (s32)limit ? 0x29u : 0x3eu;
        cm_fn_00417c10((u32 *)(person + field), (u32 *)scratch, (u16 *)date);
    }
    if (force_role != 0)
        role = role == 11 || role == 12 || role == 13 || role == 15 ? 11 : 0;
    value = U32_AT(person, 0x61);
    if (value != 0 && cm_fn_00600a50(group, 0, 1, 0, 0) == 0) {
        value = U32_AT(person, 0x61);
        if (S16_AT(value, 5) > 0x55) {
            if (role == 6) role = 13;
            else if (role == 8) role = 15;
        }
    }
    if ((u32)(s32)role > 15u)
        return 0;
    target = CM_GLOBAL(volatile u32, 0x0053c14cu + (u32)(s32)role * 4u);
    switch (target) {
    case 0x0053bffe: goto assigned;
    case 0x0053beb0: field = 0xbf; goto single;
    case 0x0053bec9: field = 0xc3; capacity = 3; goto array;
    case 0x0053beff: field = 0xcf; goto single;
    case 0x0053bf18: field = 0xd3; goto single;
    case 0x0053bf31: field = 0x19f; capacity = 5; goto array;
    case 0x0053bf67: field = 0x1b3; capacity = 7; goto array;
    case 0x0053bf9a: field = 0x1cf; capacity = 3; goto array;
    case 0x0053bfcd: field = 0xd7; capacity = 50; goto array;
    case 0x0053c043:
        if (U32_AT(group, 0xcf) != 0) return 0;
        U32_AT(group, 0xcf) = person;
        field = 0xd7; capacity = 50; goto array;
    case 0x0053c078:
        if (U32_AT(group, 0xd3) != 0) return 0;
        U32_AT(group, 0xd3) = person;
        index = first_free(group + 0xd7u, 50);
        if (index < 50) U32_AT(group + index * 4u, 0xd7) = person;
        else role = 6;
        goto assigned;
    case 0x0053c0c5:
        index = first_free(group + 0x19fu, 5);
        if (index >= 5) return 0;
        U32_AT(group + index * 4u, 0x19f) = person;
        index = first_free(group + 0xd7u, 50);
        if (index < 50) U32_AT(group + index * 4u, 0xd7) = person;
        else role = 8;
        goto assigned;
    default: return 0;
    }
single:
    if (U32_AT(group, field) != 0) return 0;
    U32_AT(group, field) = person;
    goto assigned;
array:
    index = first_free(group + field, capacity);
    if (index >= capacity) return 0;
    U32_AT(group + index * 4u, field) = person;
assigned:
    if (update_person != 0) {
        twice = CM_GLOBAL(volatile u32, 0x00ae2350) * 2u;
        limit = CM_GLOBAL(volatile u32, 0x00ae235c) - twice;
        value = U32_AT(group, 0);
        if ((s32)value >= (s32)limit) {
            U32_AT(person, 0x24) = group;
            U8_AT(person, 0x28) = (u8)role;
        } else {
            U32_AT(person, 0x39) = group;
            U8_AT(person, 0x3d) = (u8)role;
        }
    }
    return 1;
report:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
