/* Preferred-image VA 0x007a50f0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Five cdecl stack slots; full EAX is a record pointer or zero.
   Name argument order follows +4, +0xc, +8 record fields. Callees remain native.
   Volatile DWORD reloads retain helper-visible aliases and original fault order. */
#include "cmre.h"

extern int *CM_CDECL cm_fn_00545880(int *record);
extern s32 CM_CDECL cm_fn_00946fa0(u32 first, u32 second, u32 count);

u32 CM_CDECL cm_fn_007a50f0(u32 first, u32 third, u32 second, u32 country, u32 mode)
{
    u32 count, index, offset, nation, person, slot, candidate;
    u32 name;

    if (country == 0u)
        goto global_people;
    if (CM_GLOBAL(volatile u8, country) == 0u)
        goto global_people;

    count = CM_GLOBAL(volatile u32, 0x00ae235c);
    index = 0u;
    if ((s32)count <= 0)
        return 0u;
    offset = 0u;
    for (;;) {
        nation = CM_GLOBAL(volatile u32, 0x00ae23b4);
        name = (u32)cm_fn_00545880((int *)(nation + offset));
        if (cm_fn_00946fa0(name, country, 0x33u) == 0)
            break;
        count = CM_GLOBAL(volatile u32, 0x00ae235c);
        index += 1u;
        offset += 0x245u;
        if ((s32)index >= (s32)count)
            return 0u;
    }
    nation = CM_GLOBAL(volatile u32, 0x00ae23b4);
    nation += index * 0x245u;
    if (nation == 0u)
        return 0u;
    if ((u8)mode == 1u) {
        index = 0u;
        slot = nation + 0xd7u;
        do {
            candidate = CM_GLOBAL(volatile u32, slot);
            if (candidate == 0u)
                goto next_slot;
            if (CM_GLOBAL(volatile u8, first) != 0u) {
                name = CM_GLOBAL(volatile u32, candidate + 4u);
                if (cm_fn_00946fa0(name, first, 0x1au) != 0)
                    goto next_slot;
            }
            if (CM_GLOBAL(volatile u8, second) != 0u) {
                candidate = CM_GLOBAL(volatile u32, slot);
                name = CM_GLOBAL(volatile u32, candidate + 8u);
                if (cm_fn_00946fa0(name, second, 0x1au) != 0)
                    goto next_slot;
            }
            if (CM_GLOBAL(volatile u8, third) != 0u) {
                candidate = CM_GLOBAL(volatile u32, slot);
                name = CM_GLOBAL(volatile u32, candidate + 0xcu);
                if (cm_fn_00946fa0(name, third, 0x1au) != 0)
                    goto next_slot;
            }
            return CM_GLOBAL(volatile u32, nation + index * 4u + 0xd7u);
next_slot:
            index += 1u;
            slot += 4u;
        } while ((s32)index < 0x32);
    }
    candidate = CM_GLOBAL(volatile u32, nation + 0xcfu);
    if (candidate == 0u)
        return 0u;
    if (CM_GLOBAL(volatile u8, first) != 0u) {
        name = CM_GLOBAL(volatile u32, candidate + 4u);
        if (cm_fn_00946fa0(name, first, 0x1au) != 0)
            return 0u;
    }
    if (CM_GLOBAL(volatile u8, second) != 0u) {
        candidate = CM_GLOBAL(volatile u32, nation + 0xcfu);
        name = CM_GLOBAL(volatile u32, candidate + 8u);
        if (cm_fn_00946fa0(name, second, 0x1au) != 0)
            return 0u;
    }
    if (CM_GLOBAL(volatile u8, third) != 0u) {
        candidate = CM_GLOBAL(volatile u32, nation + 0xcfu);
        name = CM_GLOBAL(volatile u32, candidate + 0xcu);
        if (cm_fn_00946fa0(name, third, 0x1au) != 0)
            return 0u;
    }
    return CM_GLOBAL(volatile u32, nation + 0xcfu);

global_people:
    count = CM_GLOBAL(volatile u32, 0x00ae2364);
    person = CM_GLOBAL(volatile u32, 0x00ae23bc);
    index = 0u;
    if ((s32)count <= 0)
        return 0u;
    for (;;) {
        if (CM_GLOBAL(volatile u8, first) != 0u) {
            name = CM_GLOBAL(volatile u32, person + 4u);
            if (name != 0u && cm_fn_00946fa0(name, first, 0x1au) != 0)
                goto next_person;
        }
        if (CM_GLOBAL(volatile u8, second) != 0u) {
            name = CM_GLOBAL(volatile u32, person + 8u);
            if (name != 0u && cm_fn_00946fa0(name, second, 0x1au) != 0)
                goto next_person;
        }
        if (CM_GLOBAL(volatile u8, third) != 0u) {
            name = CM_GLOBAL(volatile u32, person + 0xcu);
            if (name != 0u && cm_fn_00946fa0(name, third, 0x1au) != 0)
                goto next_person;
        }
        return person;
next_person:
        count = CM_GLOBAL(volatile u32, 0x00ae2364);
        index += 1u;
        person += 0x6eu;
        if ((s32)index >= (s32)count)
            return 0u;
    }
}
