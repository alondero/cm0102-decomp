#include "cmre.h"

/* Preferred-image VA 0x005461b0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original returns its final EAX with only AL replaced. Keep the high
   bits from flags or the final formatter, and the raw error-dialog return.
   Scratch buffers are adjacent at offsets 4 and 0x104 within the 0x204 frame. */
extern u32 CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, const char *format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

u32 CM_CDECL cm_fn_005461b0(u32 flags, u32 output, s32 size)
{
    u8 scratch[0x204];
    u32 language = cm_fn_0066f4d0();
    u32 eax, format;
    u8 result = 0, count = 0;
    char *version;

    if (size < 30) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x104, scratch + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                       scratch + 0x104, scratch + 4, 0x5321);
        eax = cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return eax & 0xffffff00u;
    }
    if (flags & 1) {
        count = 1;
        cm_fn_0066f4e0(output, (const char *)0x009b7d5cu);
        if (language == 2) result = 2;
        else if (language == 5 || language == 4) result = 1;
    }
    if (flags & 2) {
        ++count;
        cm_fn_0066f4e0(output, (const char *)0x009b7d54u);
        if (language == 2) result = 2;
        else if (language == 5 || language == 4) result = 1;
    }
    if (flags & 4) {
        ++count;
        if (!(flags & 0x200)) {
            format = 0x009b7d34u;
            if (language == 2) result = 5;
        } else if (!(flags & 0x80) && !(flags & 0x800)) {
            format = 0x009b7d40u;
            if (language == 2) result = 2;
        } else {
            format = 0x009b7d28u;
            if (language == 2) result = 2;
        }
        cm_fn_0066f4e0(output, (const char *)format);
        if (language == 5 || language == 4) result = 1;
    }
    if (flags & 8) {
        ++count;
        if (!(flags & 4) && !(flags & 0x200)) {
            cm_fn_0066f4e0(output, (const char *)0x009b7d14u);
            if (language == 2) result = 5;
            else if (language == 5 || language == 4) result = 1;
        } else if (flags & 0x200) {
            cm_fn_0066f4e0(output, (const char *)0x009b7cfcu);
            if (language == 2) result = 2;
            else if (language == 5 || language == 4) result = 1;
        }
    }
    if (flags & 0x10) {
        ++count;
        cm_fn_0066f4e0(output, (const char *)0x009b7cf0u);
        if (language == 2) result = 2;
        else if (language == 5 || language == 4) result = 1;
    }
    if ((flags & 0x20) && count == 0) {
        count = 1;
        if (flags & 0x200) {
            cm_fn_0066f4e0(output, (const char *)0x009b7cd8u);
            if (language == 2) result = 2;
            else if (language == 5 || language == 4) result = 1;
        } else {
            format = !(flags & 0x80) ? 0x009b7cb4u :
                     (flags & 0x800) ? 0x009b7cd0u : 0x009b7cc4u;
            cm_fn_0066f4e0(output, (const char *)format);
            if (language == 2) result = 5;
            else if (language == 5) result = 1;
            else if (language == 4) result = 4;
        }
    }

    eax = flags;
    if (flags & 0x40) {
        eax &= 0x200;
        if (eax != 0) {
            ++count;
            eax = cm_fn_0066f4e0(output, (const char *)0x009b7cacu);
            if (language == 2) result = 5;
            else if (language == 5) result = 1;
            else if (language == 4) result = 4;
        }
    }
    if ((s8)count > 3) {
        eax = cm_fn_0066f4e0(output, (const char *)0x009b7c9cu);
        if (language == 2) result = 2;
        else if (language == 5 || language == 4) result = 1;
    }
    if (count == 0) {
        eax = cm_fn_0066f4e0(output, (const char *)0x009b7c94u);
        if (language == 2) result = 2;
        else if (language == 5 || language == 4) result = 1;
    }
    return (eax & 0xffffff00u) | result;
}
