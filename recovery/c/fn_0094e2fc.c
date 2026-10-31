/* Preferred-image VA 0x0094E2FC, image base 0x00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl with three stack arguments (plain RET; the caller pops). Returns
   a signed value in EAX. Argument 2 is a source string and argument 3 a signed
   count; argument 1 is an optional 16-bit output pointer. A NULL source or zero
   count returns 0. An empty source writes one zero word to a non-NULL output and
   returns 0. Otherwise a zero global dword at 0x00DE3048 copies the first byte.
   With that global nonzero, a byte-table entry at 2 * byte + 1 with bit 0x80
   selects a lead-byte path: width global dword 0x00AD91DC (signed) is used as the
   byte count when it is above 1, the count must not be below it, and a converted
   result is returned as the width. Non-lead bytes convert one byte. Failures set
   the last-error global 0x00DE2F8C to 0x2A and return -1. The import is the
   MultiByteToWideChar IAT slot 0x009670AC, read at each call, with code page
   global 0x00DE3058 and flags 9. Table, global and string meanings remain unknown. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

#define CM_FN_0094E2FC_MBTWC_SLOT 0x009670ac

typedef s32 (CM_STDCALL *cm_fn_0094e2fc_mbtwc_t)(u32 code_page, u32 flags,
                                                 const char *multi, s32 multi_bytes,
                                                 u16 *wide, s32 wide_chars);

#define CM_FN_0094E2FC_CONVERT(multi_bytes, dst, src) \
    (CM_GLOBAL(cm_fn_0094e2fc_mbtwc_t, CM_FN_0094E2FC_MBTWC_SLOT))( \
        CM_GLOBAL(u32, 0x00de3058), 9u, (src), (multi_bytes), (dst), \
        ((dst) != NULL ? 1 : 0))

s32 CM_CDECL cm_fn_0094e2fc(u16 *dst, const char *src, s32 count)
{
    u8 first;
    u8 *table;
    s32 width;
    s32 converted;

    if (src == NULL || count == 0) {
        return 0;
    }
    first = (u8)src[0];
    if (first == 0) {
        if (dst != NULL) {
            dst[0] = 0;
        }
        return 0;
    }
    if (CM_GLOBAL(s32, 0x00de3048) == 0) {
        if (dst != NULL) {
            dst[0] = (u16)first;
        }
        return 1;
    }
    table = CM_GLOBAL(u8 *, 0x00ad8fd0);
    if ((table[(u32)first * 2u + 1u] & 0x80u) == 0) {
        converted = CM_FN_0094E2FC_CONVERT(1, dst, src);
        if (converted != 0) {
            return 1;
        }
        goto error;
    }
    width = CM_GLOBAL(s32, 0x00ad91dc);
    if (width > 1) {
        if (count < width) {
            goto error;
        }
        converted = CM_FN_0094E2FC_CONVERT(width, dst, src);
        width = CM_GLOBAL(s32, 0x00ad91dc);
        if (converted != 0) {
            return width;
        }
    }
    if ((u32)count < (u32)width) {
        goto error;
    }
    if (src[1] != 0) {
        return width;
    }
error:
    CM_GLOBAL(u32, 0x00de2f8c) = 0x2au;
    return -1;
}
