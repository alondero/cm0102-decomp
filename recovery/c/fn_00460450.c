/* Preferred-image VA 00460450, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00545880(u32 club);
extern u32 CM_CDECL cm_fn_0090bde0(u32 directory, u32 name);
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);

static u32 scan_count(u32 pointer)
{
    u32 remaining = 0xffffffffu;
    u8 value;
    do {
        value = B(pointer);
        ++pointer;
        --remaining;
    } while (value != 0 && remaining != 0);
    return ~remaining;
}

u32 CM_CDECL cm_fn_00460450(u32 club, u32 destination)
{
    u32 original = club, pointer, id, expected, mapped, name;
    u32 index = 0, offset = 0, table, left, right, equal, path, stream;
    u32 count, i, value;
    u8 a, b;
    if (club == 0)
        return 0;
    pointer = D(club + 0x53u);
    if (pointer == 0)
        return 0;
    id = D(pointer);
    expected = D(0x009cf488u);
    if (id != expected)
        return 0;
    mapped = cm_fn_00540a50(original, (u32)&club, 1);
    if (club != 0)
        mapped = original;
    name = cm_fn_00545880(mapped);
    if (name == 0)
        return 0;
    pointer = D(0x0098dedcu);
    if (scan_count(pointer) - 1u == 0)
        return 0;
    for (;;) {
        table = 0x0098dedcu + offset;
        left = D(table);
        right = name;
        for (;;) {
            a = B(left);
            b = B(right);
            if (a != b) { equal = 0; break; }
            if (a == 0) { equal = 1; break; }
            a = B(left + 1u);
            b = B(right + 1u);
            if (a != b) { equal = 0; break; }
            left += 2u;
            right += 2u;
            if (a == 0) { equal = 1; break; }
        }
        if (equal != 0) {
            pointer = D(0x0098dee0u + offset);
            path = cm_fn_0090bde0(0x009899e0u, pointer);
            if (path != 0)
                break;
        }
        ++index;
        offset = (u32)(s32)(s16)index * 8u;
        pointer = D(0x0098dedcu + offset);
        if (scan_count(pointer) - 1u == 0)
            return 0;
    }
    stream = cm_fn_00945b00(path, 0x0098f928u);
    if (stream == 0)
        return 0;
    count = scan_count(path);
    for (i = 0; i < (count >> 2); ++i) {
        value = D(path);
        path += 4u;
        D(destination) = value;
        destination += 4u;
    }
    for (i = 0; i < (count & 3u); ++i) {
        a = B(path);
        ++path;
        B(destination) = a;
        ++destination;
    }
    cm_fn_00945a09(stream);
    return 1;
}
