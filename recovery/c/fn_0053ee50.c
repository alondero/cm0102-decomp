/* Preferred-image VA 0x0053ee50, base 0x00400000. CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, const char *, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) (*(volatile s8 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

/* SCASB counts include the terminator. Copies retain the original forward
   DWORD accesses followed by byte accesses, including when ranges overlap. */
static u32 scan(u32 address)
{
    u32 remaining = 0xffffffffu;
    u8 value;
    do {
        value = B(address);
        ++address;
        --remaining;
    } while (value != 0 && remaining != 0);
    return ~remaining;
}

static void copy_count(u32 destination, u32 source, u32 count)
{
    u32 n = count >> 2, value;
    while (n != 0) {
        value = D(source);
        D(destination) = value;
        source += 4u;
        destination += 4u;
        --n;
    }
    n = count & 3u;
    while (n != 0) {
        B(destination) = B(source);
        ++source;
        ++destination;
        --n;
    }
}

static void copy_text(u32 destination, u32 source)
{
    u32 count = scan(source);
    copy_count(destination, source, count);
}

static void append(u32 destination, u32 source)
{
    u32 count = scan(source);
    u32 end = destination + scan(destination) - 1u;
    copy_count(end, source, count);
}

static void emit_primary(u32 text, u8 mode, u32 long_format,
                         u32 short_format, u32 fallback, int short_extra)
{
    if (mode == 1) {
        if (scan(text) - 1u != 0)
            append(text, 0x009b7630);
        cm_fn_0066f4e0(0x00de1f64, (const char *)long_format,
                      (const char *)0x00ad9c64);
        append(text, 0x00de1f64);
    } else if (mode == 2 || mode == 3) {
        if (scan(text) - 1u != 0)
            append(text, 0x009b7630);
        if (short_extra)
            cm_fn_0066f4e0(0x00de1f64, (const char *)short_format,
                          (const char *)0x00ad9c64);
        else
            cm_fn_0066f4e0(0x00de1f64, (const char *)short_format);
        append(text, 0x00de1f64);
    } else if (scan(text) - 1u == 0) {
        append(text, fallback);
    }
}

static void emit_secondary(u32 text, u8 mode, u32 long_format,
                           u32 short_format, u32 fallback)
{
    if (mode == 1) {
        cm_fn_0066f4e0(0x00de1f64, (const char *)long_format,
                      (const char *)0x00ad9c64);
        append(text, 0x00de1f64);
    } else if (mode == 2) {
        if (scan(text) - 1u != 0)
            append(text, 0x009b7630);
        cm_fn_0066f4e0(0x00de1f64, (const char *)short_format);
        append(text, 0x00de1f64);
    } else if (mode == 4 && scan(text) - 1u == 0) {
        append(text, fallback);
    }
}

u32 CM_CDECL cm_fn_0053ee50(u32 roles, u32 output, s16 size, u8 mode)
{
    u8 scratch[0x11a4];
    u32 primary = (u32)(scratch + 4), secondary = (u32)(scratch + 0x9d4);
    u8 *first, *second;
    u32 line, threshold;
    u8 counter;
    char *version;

    if (output == 0) {
        first = scratch + 0x7d4; second = scratch + 0x8d4; line = 0x3e54;
        goto error;
    }
    B(output) = 0;
    if (roles == 0) {
        first = scratch + 0x8d4; second = scratch + 0x7d4; line = 0x3e5f;
        goto error;
    }
    if (size < 30) {
        first = scratch + 0x8d4; second = scratch + 0x7d4; line = 0x3e67;
        goto error;
    }
    scratch[0] = 15;
    for (;;) {
        threshold = scratch[0];
        copy_text(primary, 0x00ad9c64);
        if ((s32)S(roles + 0x0fu) >= (s32)threshold)
            emit_primary(primary, mode, 0x009b7610, 0x009b7600, 0x009b760c, 0);
        if ((s32)S(roles + 0x10u) >= (s32)threshold)
            emit_primary(primary, mode, 0x009b75e4, 0x009b75dc, 0x0098b3f0, 0);
        if ((s32)S(roles + 0x11u) >= (s32)threshold)
            emit_primary(primary, mode, 0x009b75c0, 0x009b75b0, 0x009b75bc, 0);
        if ((s32)S(roles + 0x12u) >= (s32)threshold) {
            emit_primary(primary, mode, 0x009b7588, 0x009b756c, 0x009b7584, 0);
        } else if ((s32)S(roles + 0x14u) >= (s32)threshold
                   && (s32)S(roles + 0x13u) >= (s32)threshold) {
            emit_primary(primary, mode, 0x009b7544, 0x009b7528, 0x009b7540, 0);
        } else if ((s32)S(roles + 0x13u) >= (s32)threshold) {
            emit_primary(primary, mode, 0x009b7508, 0x009b74f8, 0x009b7504, 0);
        }
        if ((s32)S(roles + 0x15u) >= (s32)threshold) {
            if ((s32)S(roles + 0x14u) >= (s32)threshold
                || (s32)S(roles + 0x17u) >= (s32)threshold
                || (s32)S(roles + 0x18u) >= (s32)threshold
                || (s32)S(roles + 0x1au) >= (s32)threshold)
                emit_primary(primary, mode, 0x009b74b4, 0x009b7484, 0x009b74b0, 1);
            else
                emit_primary(primary, mode, 0x009b74dc, 0x009b74d0, 0x009b74d8, 0);
        }
        if ((s32)S(roles + 0x0fu) >= (s32)threshold) {
            copy_text(output, primary);
            return 1;
        }
        --scratch[0];
        if (scan(primary) - 1u != 0 || scratch[0] < 10)
            break;
    }
    if (B(primary) == 0) {
        if (S(roles + 0x14u) >= 15)
            B(roles + 0x13u) = 15;
        if (mode == 1) {
            cm_fn_0066f4e0(0x00de1f64, (const char *)0x009b7544,
                          (const char *)0x00ad9c64);
            append(primary, 0x00de1f64);
        } else if (mode == 2 || mode == 3) {
            if (scan(primary) - 1u != 0)
                append(primary, 0x009b7630);
            cm_fn_0066f4e0(0x00de1f64, (const char *)0x009b7528);
            append(primary, 0x00de1f64);
        } else if (scan(primary) - 1u == 0) {
            cm_fn_0066f4e0(0x00de1f64, (const char *)0x009b7544,
                          (const char *)0x00ad9c64);
            append(primary, 0x00de1f64);
        }
    }
    counter = 15;
    do {
        copy_text(secondary, 0x00ad9c64);
        if (S(roles + 0x17u) >= 15)
            emit_secondary(secondary, mode, 0x009b746c, 0x009b7464, 0x009b760c);
        if (S(roles + 0x18u) >= 15)
            emit_secondary(secondary, mode, 0x009b744c, 0x009b7444, 0x0098b3f0);
        if (S(roles + 0x19u) >= 15)
            emit_secondary(secondary, mode, 0x009b7428, 0x009b7420, 0x009b75bc);
        --counter;
        if (scan(secondary) - 1u != 0)
            break;
    } while (counter >= 10);
    if (mode == 2 && scan(secondary) - 1u != 0)
        cm_fn_00944c9f(output, 0x009a3d8c, (const char *)primary, (const char *)secondary);
    else if (scan(secondary) - 1u != 0)
        cm_fn_00944c9f(output, 0x0098f0b8, (const char *)primary, (const char *)secondary);
    else
        copy_text(output, primary);
    return 1;

error:
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
