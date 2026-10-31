#include "cmre.h"

extern const volatile u8 *CM_CDECL cm_fn_0094b258(
    const volatile u8 *directory, const volatile u8 *end);

void CM_CDECL cm_fn_00945b13(volatile u8 *output,
                              const volatile u8 *drive,
                              const volatile u8 *directory,
                              const volatile u8 *name,
                              const volatile u8 *extension)
{
    if (drive != 0) {
        u8 drive_byte = *drive;
        if (drive_byte != 0) {
            *output++ = drive_byte;
            *output++ = 0x3au;
        }
    }

    if (directory != 0) {
        const volatile u8 *cursor = directory;
        u8 ch = *cursor;
        if (ch != 0) {
            do {
                *output++ = ch;
                ch = *(cursor + 1);
                ++cursor;
            } while (ch != 0);

            const volatile u8 *tail = cm_fn_0094b258(directory, cursor);
            u8 tail_byte = *tail;
            if (tail_byte != 0x2fu && tail_byte != 0x5cu)
                *output++ = 0x5cu;
        }
    }

    if (name != 0) {
        const volatile u8 *cursor = name;
        u8 ch = *cursor;
        while (ch != 0) {
            *output++ = ch;
            ++cursor;
            ch = *cursor;
        }
    }

    if (extension == 0) {
        u8 old = *output;
        *output = old & 0u;
    } else {
        u8 first = *extension;
        if (first != 0 && first != 0x2eu)
            *output++ = 0x2eu;
        for (;;) {
            u8 ch = *extension;
            *output++ = ch;
            if (ch == 0)
                break;
            ++extension;
        }
    }
}
