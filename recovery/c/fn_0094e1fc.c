/* WIP: incoming ECX seeds local output; case 8 mismatch, plus stack fault addresses. */
#include "cmre.h"
extern s32 CM_CDECL cm_fn_0094c0fe(u32, u32, const char *, s32, void *, s32, u32, s32);

u32 CM_CDECL cm_fn_0094e1fc(u32 value)
{
    if (value > 0xffu) {
        u8 scratch[16];
        u32 high = (value >> 8) & 0xffu;
        scratch[15] = (u8)value;
        scratch[14] = (u8)high;
        if (CM_GLOBAL(volatile u8, 0x00de4521u + high) & 4) {
            u32 code_page = CM_GLOBAL(volatile u32, 0x00de4408);
            u32 locale = CM_GLOBAL(volatile u32, 0x00de4624);
            if (cm_fn_0094c0fe(locale, 0x100, (char *)(scratch + 14), 2,
                               scratch, 2, code_page, 1))
                return ((u32)scratch[0] << 8) + scratch[1];
        }
    } else if ((CM_GLOBAL(volatile u8, 0x00de4521u + value) & 0x10) == 0x10) {
        return CM_GLOBAL(volatile u8, 0x00de4420u + value);
    }
    return value;
}
