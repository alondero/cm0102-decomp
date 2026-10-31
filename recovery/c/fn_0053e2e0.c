#include "cmre.h"

/* Preferred-image VA 0x0053e2e0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All string predicates perform the original full byte scan, including
   repeated scans on fallback paths. Copies scan again before forward DWORD
   and byte transfers. Initials use signed bytes, as MOVSX does. */
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

#define U32_AT(base) (*(volatile u32 *)(base))
#define U8_AT(base) (*(volatile u8 *)(base))
#define S8_AT(base) (*(volatile s8 *)(base))

static u32 scan_count(u32 text)
{
    u32 count = 0;
    while (count != 0xffffffffu) {
        u8 value = U8_AT(text + count);
        ++count;
        if (value == 0) break;
    }
    return count;
}

static int has_text(u32 text)
{
    return text != 0 && scan_count(text) - 1u != 0;
}

static void copy_text(u32 output, u32 text)
{
    u32 count = scan_count(text);
    u32 words = count >> 2, bytes = count & 3;
    while (words != 0) {
        u32 value = U32_AT(text);
        U32_AT(output) = value;
        text += 4; output += 4; --words;
    }
    while (bytes != 0) {
        u8 value = U8_AT(text);
        U8_AT(output) = value;
        ++text; ++output; --bytes;
    }
}

u32 CM_CDECL cm_fn_0053e2e0(u32 first_name, u32 second_name, u32 preferred,
                            u32 output, s16 size, u8 kind)
{
    u8 scratch[512];
    u32 index, target, line;
    u8 *first_buffer, *second_buffer;
    s32 first_initial, second_initial;
    char *version;

    if (output == 0 || size < 0x69) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 256);
        first_buffer = scratch;
        second_buffer = scratch + 256;
        line = 0x3d0a;
        goto report;
    }
    U8_AT(output) = 0;
    index = (u32)kind - 1u;
    if (index > 6) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 256, scratch);
        first_buffer = scratch + 256;
        second_buffer = scratch;
        line = 0x3d78;
        goto report;
    }
    target = CM_GLOBAL(volatile u32, 0x0053e970u + index * 4u);
    switch (target) {
    case 0x0053e32a: kind = 1; break;
    case 0x0053e3fa: kind = 2; break;
    case 0x0053e48f: kind = 3; break;
    case 0x0053e57c: kind = 4; break;
    case 0x0053e66e: kind = 5; break;
    case 0x0053e815: kind = 6; break;
    case 0x0053e713: kind = 7; break;
    default: return 1;
    }

    if (kind <= 4) {
        if (has_text(preferred)) {
            copy_text(output, preferred);
            return 1;
        }
        if (has_text(first_name)) {
            if (second_name == 0) goto first_fallback;
            if (has_text(second_name)) {
                if (kind == 1)
                    cm_fn_00944c9f(output, 0x0098f0b8u, first_name, second_name);
                else if (kind == 2) {
                    first_initial = S8_AT(first_name);
                    cm_fn_00944c9f(output, 0x009b7410u, first_initial, second_name);
                } else if (kind == 3)
                    cm_fn_00944c9f(output, 0x0098f6acu, second_name, first_name);
                else {
                    first_initial = S8_AT(first_name);
                    cm_fn_00944c9f(output, 0x009b7408u, second_name, first_initial);
                }
                return 1;
            }
        }
        if (has_text(second_name)) {
            copy_text(output, second_name);
            return 1;
        }
first_fallback:
        if (has_text(first_name)) copy_text(output, first_name);
        return 1;
    }
    if (kind == 5) {
        if (has_text(preferred)) copy_text(output, preferred);
        else if (has_text(second_name)) copy_text(output, second_name);
        else if (has_text(first_name)) copy_text(output, first_name);
        return 1;
    }
    if (kind == 7) {
        if (has_text(first_name) && has_text(second_name)) {
            cm_fn_00944c9f(output, 0x0098f0b8u, first_name, second_name);
            return 1;
        }
        if (has_text(preferred)) copy_text(output, preferred);
        else if (has_text(second_name)) copy_text(output, second_name);
        else if (has_text(preferred)) copy_text(output, preferred);
        else if (has_text(first_name)) copy_text(output, first_name);
        return 1;
    }

    if (has_text(preferred)) {
        first_initial = S8_AT(preferred);
        goto initial;
    }
    if (has_text(first_name)) {
        if (second_name == 0) goto initial_first;
        if (has_text(second_name)) {
            second_initial = S8_AT(second_name);
            first_initial = S8_AT(first_name);
            cm_fn_00944c9f(output, 0x009b7418u, first_initial, second_initial);
            return 1;
        }
    }
    if (has_text(second_name)) {
        first_initial = S8_AT(second_name);
        goto initial;
    }
initial_first:
    if (!has_text(first_name)) return 1;
    first_initial = S8_AT(first_name);
initial:
    cm_fn_00944c9f(output, 0x0099ec1cu, first_initial);
    return 1;

report:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first_buffer, second_buffer, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
