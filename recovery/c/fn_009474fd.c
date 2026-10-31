#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094e0f9(u32 path, u32 extension);
extern s32 CM_CDECL cm_fn_0094df8b(u32 path, u32 suffix);

u32 CM_CDECL cm_fn_009474fd(u8 flags, const u8 *source)
{
    const u8 *path = source;
    if (*(volatile u8 *)(source + 1u) == 0x3au)
        path = source + 2u;

    u8 ch = *(volatile u8 *)path;
    u32 result;
    if (((ch == 0x5cu || ch == 0x2fu) &&
         *(volatile u8 *)(path + 1u) == 0) || (flags & 0x10u) != 0) {
        result = 0x4040u;
    } else if (ch != 0) {
        result = 0x8000u;
    } else {
        result = 0x4040u;
    }

    u8 low = (u8)~flags;
    low &= 1u;
    low |= 2u;
    result |= (u32)low << 7;

    u32 normalized = cm_fn_0094e0f9((u32)source, 0x2eu);
    if (normalized != 0) {
        if (cm_fn_0094df8b(normalized, 0x00971434u) == 0 ||
            cm_fn_0094df8b(normalized, 0x0097142cu) == 0 ||
            cm_fn_0094df8b(normalized, 0x00971424u) == 0 ||
            cm_fn_0094df8b(normalized, 0x0097141cu) == 0)
            result |= 0x40u;
    }

    u32 bits = result;
    result |= (bits >> 3) & 0x38u;
    bits = result;
    result |= (bits >> 6) & 7u;
    return result;
}
