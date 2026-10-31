#include "cmre.h"

/* Preferred-image VA 0x00543b10, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain the guarded DWORD dispatch read and the two adjacent 256-byte buffers.
   Arguments pushed before 0x0090d130 belong to the later formatter call;
   0x0090d130 itself takes no stack arguments. */
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, const char *format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

u32 CM_CDECL cm_fn_00543b10(s8 kind, u32 output, s16 size, u32 variant)
{
    u8 scratch[512];
    u32 target, format;
    u32 line;
    u8 *first, *second;
    char *version;

    if (kind < 1 || kind > 12 || output == 0 || size < 100)
        goto invalid;
    *(volatile u8 *)output = 0;
    target = CM_GLOBAL(volatile u32, 0x00543ef8u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x00543b61: format = variant ? 0x009b7c90u : 0x009b7c80u; break;
    case 0x00543ba0: format = variant ? 0x009b7c7cu : 0x009b7c64u; break;
    case 0x00543bdf: format = variant ? 0x009b7c60u : 0x009b7c44u; break;
    case 0x00543c1e: format = variant ? 0x009b7a68u : 0x009b7a58u; break;
    case 0x00543c5d: format = variant ? 0x009b7c40u : 0x009b7c28u; break;
    case 0x00543c9c: format = variant ? 0x009b7c24u : 0x009b7c08u; break;
    case 0x00543cdb: format = variant ? 0x009b7c04u : 0x009b7bf0u; break;
    case 0x00543d1a:
        if (variant != 0) {
            cm_fn_0066f4e0(output, (const char *)0x009b7bccu, (const char *)0x00ad9c64u);
            return 1;
        }
        format = 0x009b7bbcu;
        break;
    case 0x00543d5e: format = variant ? 0x009b7bb8u : 0x009b7bacu; break;
    case 0x00543d9d: format = variant ? 0x009b7ba8u : 0x009b7b98u; break;
    case 0x00543ddc: format = variant ? 0x009b7b94u : 0x009b7b80u; break;
    case 0x00543e1b: format = variant ? 0x009b7b7cu : 0x009b7b64u; break;
    default:
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 256, scratch);
        first = scratch + 256;
        second = scratch;
        line = 0x4a27;
        goto report;
    }
    cm_fn_0066f4e0(output, (const char *)format);
    return 1;

invalid:
    cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 256);
    first = scratch;
    second = scratch + 256;
    line = 0x49b8;
report:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
