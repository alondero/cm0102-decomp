#include "cmre.h"

/* Preferred-image VA 0x005428a0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Each format address and optional third argument follows its target block.
   In particular, 0x00542a75 supplies the extra argument only for variant 0,
   while 0x00542d2f supplies it for both variants. */
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 flags);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, const char *format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *filename, void *extension);

u32 CM_CDECL cm_fn_005428a0(s8 kind, u32 output, s16 size, u32 variant)
{
    u8 scratch[512];
    u32 target, format, line;
    int extra = 0;
    u8 *first, *second;
    char *version;

    if (kind < 1 || kind > 31 || output == 0 || size < 100)
        goto invalid;
    *(volatile u8 *)output = 0;
    target = CM_GLOBAL(volatile u32, 0x00543158u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x005428f1: format = variant ? 0x009b7b60u : 0x009b7b50u; break;
    case 0x00542930: format = variant ? 0x009b7b30u : 0x009b7b24u; extra = variant != 0; break;
    case 0x00542974: format = variant ? 0x009b7b04u : 0x009b7af8u; extra = variant != 0; break;
    case 0x005429b8: format = variant ? 0x009b7af4u : 0x009b7ae8u; break;
    case 0x005429f7: format = variant ? 0x009b7ae4u : 0x009b7adcu; break;
    case 0x00542a36: format = variant ? 0x009b7ad8u : 0x009b7ac8u; break;
    case 0x00542a75: format = variant ? 0x009b7ac4u : 0x009b7a98u; extra = variant == 0; break;
    case 0x00542ab9: format = variant ? 0x009b7a94u : 0x009b7a8cu; break;
    case 0x00542af8: format = variant ? 0x009b7a88u : 0x009b7a7cu; break;
    case 0x00542b37: format = variant ? 0x009b7a78u : 0x009b7a6cu; break;
    case 0x00542b76: format = variant ? 0x009b7a68u : 0x009b7a58u; break;
    case 0x00542bb5: format = variant ? 0x009b7a54u : 0x009b7a48u; break;
    case 0x00542bf4: format = variant ? 0x009b7a44u : 0x009b7a3cu; break;
    case 0x00542c33: format = variant ? 0x009b7a38u : 0x009b7a2cu; break;
    case 0x00542c72: format = variant ? 0x009b7a28u : 0x009b7a20u; break;
    case 0x00542cb1: format = variant ? 0x0098ea64u : 0x009b7a14u; break;
    case 0x00542cf0: format = variant ? 0x009b7a10u : 0x009b7a08u; break;
    case 0x00542d2f: format = variant ? 0x009b79d8u : 0x009b79b0u; extra = 1; break;
    case 0x00542d73: format = variant ? 0x009b7984u : 0x009b7974u; extra = variant != 0; break;
    case 0x00542db7: format = variant ? 0x009b7970u : 0x009b7968u; break;
    case 0x00542df6: format = variant ? 0x009b7964u : 0x009b795cu; break;
    case 0x00542e35: format = variant ? 0x009b7958u : 0x009b794cu; break;
    case 0x00542e74: format = variant ? 0x009b792cu : 0x009b7920u; extra = variant != 0; break;
    case 0x00542eb8: format = variant ? 0x009b7900u : 0x009b78f4u; extra = variant != 0; break;
    case 0x00542efc: format = variant ? 0x009b78d4u : 0x009b78c8u; extra = variant != 0; break;
    case 0x00542f40: format = variant ? 0x009b78c4u : 0x009b78bcu; break;
    case 0x00542f7f: format = variant ? 0x009b78b8u : 0x009b78acu; break;
    case 0x00542fbe: format = variant ? 0x009b78a8u : 0x009b789cu; break;
    case 0x00542ffd: format = variant ? 0x009b7898u : 0x009b788cu; break;
    case 0x0054303c: format = variant ? 0x009b7888u : 0x009b787cu; break;
    case 0x0054307b: format = variant ? 0x009b7878u : 0x009b786cu; break;
    default:
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 256, scratch);
        first = scratch + 256; second = scratch; line = 0x4802;
        goto report;
    }
    if (extra)
        cm_fn_0066f4e0(output, (const char *)format, (const char *)0x00ad9c64u);
    else
        cm_fn_0066f4e0(output, (const char *)format);
    return 1;
invalid:
    cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 256);
    first = scratch; second = scratch + 256; line = 0x473d;
report:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
