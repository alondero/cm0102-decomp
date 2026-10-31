#include "cmre.h"

/* Preferred-image VA 0x00542100, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   A null object reports an error and then continues, including null reads.
   String copying scans first, then copies DWORDs and trailing bytes forwards;
   preserve this order and overlap behavior instead of calling memcpy/strcpy. */
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, const char *format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U8_AT(base, offset) (*(volatile u8 *)((base) + (offset)))
#define S8_AT(base, offset) (*(volatile s8 *)((base) + (offset)))

static void copy_name(u32 destination, u32 source)
{
    u32 length = 0, words, bytes;
    while (length != 0xffffffffu) {
        u8 value = U8_AT(source + length, 0);
        ++length;
        if (value == 0) break;
    }
    words = length >> 2;
    while (words != 0) {
        u32 value = U32_AT(source, 0);
        U32_AT(destination, 0) = value;
        source += 4;
        destination += 4;
        --words;
    }
    bytes = length & 3;
    while (bytes != 0) {
        u8 value = U8_AT(source, 0);
        U8_AT(destination, 0) = value;
        ++source;
        ++destination;
        --bytes;
    }
}

u32 CM_CDECL cm_fn_00542100(u32 object, u32 suffix, u32 output, s32 size,
                            s8 mode, u32 brackets)
{
    u8 scratch[512];
    u32 target, index, count, identifier, limit, group, config, table, offset;
    u32 name_offset, gender_offset, format;
    s32 gender;
    u8 selector;
    char *version;

    if (size < 0x47) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                       scratch + 256, scratch, 0x460f);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }
    index = (u32)(s32)mode - 1u;
    if (index > 3) return 1;
    target = CM_GLOBAL(volatile u32, 0x00542724u + index * 4u);
    switch (target) {
    case 0x0054243d: mode = 1; break;
    case 0x0054230c: mode = 2; break;
    case 0x00542197: mode = 3; break;
    case 0x005425b2: mode = 4; break;
    default: return 1;
    }
    name_offset = mode == 3 ? 4u : 0x38u;
    gender_offset = mode == 3 ? 0x37u : 0x52u;
    if (object == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 256);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                       scratch, scratch + 256, 0x349f);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        goto ordinary;
    }
    count = CM_GLOBAL(volatile u32, 0x00ae2350);
    identifier = U32_AT(object, 0);
    limit = CM_GLOBAL(volatile u32, 0x00ae235c);
    if ((s32)identifier < (s32)(limit - count * 2u))
        goto ordinary;
    if (suffix != 0) {
        gender = S8_AT(object, gender_offset);
        format = mode == 4 ? (brackets ? 0x009b76e8u : 0x009b76c8u) :
                            (brackets ? 0x009b7830u : 0x009b7814u);
        goto formatted;
    }
    if ((s32)identifier < (s32)(limit - count)) {
        if (mode != 4) {
            copy_name(output, object + name_offset);
            return 1;
        }
        gender = S8_AT(object, 0x52);
        format = 0x009b7658u;
        goto formatted;
    }
    group = U32_AT(object, 0x53);
    config = CM_GLOBAL(volatile u32, 0x00b74318);
    table = U32_AT(group, 0);
    offset = U32_AT(config, 0x36);
    selector = U8_AT(table + offset, 0);
    gender = S8_AT(object, gender_offset);
    if (mode == 1) format = selector ? 0x009b7750u : 0x009b7734u;
    else if (mode == 2) format = selector ? 0x009b778cu : 0x009b776cu;
    else if (mode == 3) format = selector ? 0x009b77f0u : 0x009b77ccu;
    else format = selector ? 0x009b76a0u : 0x009b7678u;
    goto formatted;

ordinary:
    if (mode == 4) {
        gender = S8_AT(object, 0x52);
        format = suffix ? 0x009b770cu : 0x009b7634u;
    } else if (suffix == 0) {
        copy_name(output, object + name_offset);
        return 1;
    } else {
        gender = S8_AT(object, gender_offset);
        format = mode == 2 ? 0x009b77acu : 0x009b784cu;
    }
formatted:
    cm_fn_0066f4e0(output, (const char *)format, gender, gender, object + name_offset);
    return 1;
}
