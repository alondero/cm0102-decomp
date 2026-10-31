#include "cmre.h"

/* Plain C model of the original's string walk. The original keeps the source
   length in the second argument slot and the output count in a local; those
   slots are not observable here, so the values live in ordinary locals. */

extern u32 CM_CDECL cm_fn_00947063(s32 value);
extern u32 CM_CDECL cm_fn_00946fd8(const char *source, const char *format, void *out);

void CM_CDECL cm_fn_005e5060(u16 *dest, const char *src)
{
    const u8 *text = (const u8 *)src;
    u32 len = 0;
    u32 index = 0;
    u32 si = 0;
    u32 tmp = 0;
    u8 *out = (u8 *)dest;

    while (text[len] != 0) {
        len++;
    }

    if ((s32)len > 0) {
        do {
            u32 c = text[si];
            u32 ok = 0;

            tmp = c;
            if (c == 0x23u) {
                if ((s32)si < (s32)(len - 5u) && text[si + 5u] == 0x23u) {
                    ok = 1;
                    if (cm_fn_00947063((s32)(s8)text[si + 1u]) == 0) {
                        ok = 0;
                    }
                    if (ok && cm_fn_00947063((s32)(s8)text[si + 2u]) == 0) {
                        ok = 0;
                    }
                    if (ok && cm_fn_00947063((s32)(s8)text[si + 3u]) == 0) {
                        ok = 0;
                    }
                    if (ok && cm_fn_00947063((s32)(s8)text[si + 4u]) == 0) {
                        ok = 0;
                    }
                    if (ok && cm_fn_00946fd8((const char *)(text + si),
                                             (const char *)0x009cda78u,
                                             (void *)&tmp) != 1u) {
                        ok = 0;
                    }
                }
                if (ok) {
                    si = si + 5u;
                }
            }

            *(volatile u16 *)out = (u16)tmp;
            index = index + 1u;
            out = out + 2u;
            si = si + 1u;
        } while ((s32)si < (s32)len);
    }

    *(volatile u16 *)((u8 *)dest + index * 2u) = 0;
}
