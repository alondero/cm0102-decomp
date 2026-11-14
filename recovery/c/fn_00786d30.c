/* Preferred-image VA00786d30, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   News-slot update; original diagnostics remain explicit call boundaries. */
#include "cmre.h"
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);

u32 CM_THISCALL cm_fn_00786d30(u32 self, volatile u32 person_arg,
                             volatile u32 key_arg, volatile u32 index_arg,
                             volatile u32 value_arg, volatile u32 optional_arg)
{
    u8 scratch[512];
    void *first = scratch, *second = scratch + 256;
    u32 line, resume_after_error = 0;
    u32 person = person_arg;
    u32 key, index, optional, value, row, base, item_index, item;
    if (person == 0) {
        first = scratch + 256;
        second = scratch;
        line = 0x22b1u;
        goto diagnostic;
    }
    key = key_arg;
    if (key == 0) {
        line = 0x22b9u;
        goto diagnostic;
    }
    index = index_arg;
    if ((s32)index < 0 || (s32)index >= 50) {
        line = 0x22c1u;
        goto diagnostic;
    }
    optional = optional_arg;
    value = value_arg;
    if (optional != 0) {
        if (R32(optional + 0xd6u) != key) {
            line = 0x22d0u;
            goto diagnostic;
        }
        /* Native repeats signed BYTE bounds after the full DWORD guard. */
        if ((s8)index < 0 || (s8)index >= 50) {
            line = 0x1e5du;
            resume_after_error = 1;
            goto diagnostic;
        }
        R32(optional + (u32)(s32)(s8)index * 4u + 5u) = value;
    }
calculate:
    row = R32(person);
    base = R32(0x00ae2364u);
    row = row - base + 16u;
    if ((s32)row < 0) {
        line = 0x22dcu;
        goto diagnostic;
    }
    base = R32(self + 8u);
    item_index = R32(base + row * 12u);
    if (item_index == 0xffffffffu)
        return 0;
    base = R32(self);
    item = base + item_index * 223u;
    for (u32 scanned = 0; scanned < 100u; ++scanned, item += 223u) {
        /* Both are native reloads; the optional store may overlap inputs. */
        key = key_arg;
        u32 stored_key = R32(item + 0xd6u);
        if (stored_key == key) {
            index = index_arg;
            R32(item + index * 4u + 5u) = value;
            return 1u;
        }
    }
    line = 0x22f7u;
diagnostic:
    cm_fn_00944cff(0x00a693ecu, 0, 0, first, second);
    u32 version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    R32(0x00b67a34u) = 0;
    if (resume_after_error) {
        resume_after_error = 0;
        goto calculate;
    }
    return 0;
}
