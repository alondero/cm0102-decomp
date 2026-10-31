#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094af9e(u32 stream);

void CM_CDECL cm_fn_0094c0c1(u32 mode, volatile u8 *stream)
{
    u32 reset = mode != 0;
    if (stream[0xd] & 0x10) {
        cm_fn_0094af9e((u32)stream);
        if (reset) {
            stream[0xd] &= 0xee;
            *(volatile u32 *)(stream + 0x18) &= 0;
            *(volatile u32 *)stream &= 0;
            *(volatile u32 *)(stream + 8) &= 0;
        }
    }
}
