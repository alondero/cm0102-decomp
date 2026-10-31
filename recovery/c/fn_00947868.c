#include "cmre.h"

/* Checks a UNC-style path prefix. The caller passes one string pointer. Returns
   1 when the path starts with two separators, has a non-empty server part
   scanned from offset 3, a non-empty share part, and then either ends or has a
   single trailing separator; otherwise 0. Offsets are bytes from the string
   pointer. */
#define BYTE_AT(base, off) (*(volatile u8 *)((base) + (off)))

extern u32 CM_CDECL cm_fn_009488e0(u32 source);

u32 CM_CDECL cm_fn_00947868(u32 path)
{
    u32 cursor;
    u32 len;
    u8 c;

    len = cm_fn_009488e0(path);
    if (len < 5u) {
        return 0u;
    }
    c = BYTE_AT(path, 0);
    if (c != 0x5c) {
        if (c != 0x2f) {
            return 0u;
        }
    }
    c = BYTE_AT(path, 1);
    if (c != 0x5c) {
        if (c != 0x2f) {
            return 0u;
        }
    }
    c = BYTE_AT(path, 3);
    cursor = path + 3u;
    while (c != 0u && c != 0x5c && c != 0x2f) {
        c = BYTE_AT(cursor, 1);
        cursor = cursor + 1u;
    }
    if (BYTE_AT(cursor, 0) == 0u) {
        return 0u;
    }
    cursor = cursor + 1u;
    if (BYTE_AT(cursor, 0) == 0u) {
        return 0u;
    }
    c = BYTE_AT(cursor, 0);
    while (c != 0u && c != 0x5c && c != 0x2f) {
        cursor = cursor + 1u;
        c = BYTE_AT(cursor, 0);
    }
    if (BYTE_AT(cursor, 0) != 0u) {
        if (BYTE_AT(cursor, 1) != 0u) {
            return 0u;
        }
    }
    return 1u;
}
