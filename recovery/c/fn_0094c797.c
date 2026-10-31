#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094af9e(u32 stream);
extern unsigned long long CM_CDECL cm_fn_0094c827(volatile u8 *stream);
extern unsigned long long CM_CDECL cm_fn_00951183(u32 field, u32 amount_low,
                                                   u32 amount_high, u32 mode);

u32 CM_CDECL cm_fn_0094c797(volatile u8 *stream, unsigned long long amount,
                             u32 mode)
{
    u32 flags = *(volatile u32 *)(stream + 0x0cu);
    if ((flags & 0x83u) == 0 || (mode != 0 && mode != 1 && mode != 2)) {
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x16u;
        return 0xffffffffu;
    }

    *(volatile u32 *)(stream + 0x0cu) = flags & 0xffffffefu;
    if (mode == 1) {
        amount += cm_fn_0094c827(stream);
        mode = 0;
    }

    (void)cm_fn_0094af9e((u32)stream);
    flags = *(volatile u32 *)(stream + 0x0cu);
    if ((flags & 0x80u) != 0) {
        *(volatile u32 *)(stream + 0x0cu) = flags & 0xfffffffcu;
    } else if ((flags & 1u) != 0 && (flags & 8u) != 0 &&
               (flags & 0x400u) == 0) {
        *(volatile u32 *)(stream + 0x18u) = 0x200u;
    }

    unsigned long long result = cm_fn_00951183(
        *(volatile u32 *)(stream + 0x10u), (u32)amount,
        (u32)(amount >> 32), mode);
    return (((u32)result & (u32)(result >> 32)) == 0xffffffffu)
               ? 0xffffffffu
               : 0u;
}
