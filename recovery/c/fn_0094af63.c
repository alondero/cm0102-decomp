#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094b003(u32 mode);
extern u32 CM_CDECL cm_fn_0094af9e(u32 stream);
extern u32 CM_CDECL cm_fn_0094fe2a(u32 index);

u32 CM_CDECL cm_fn_0094af63(volatile u8 *stream)
{
    if (!stream)
        return cm_fn_0094b003(0);
    if (cm_fn_0094af9e((u32)stream))
        return 0xffffffffu;
    if (stream[0xd] & 0x40)
        return 0u - (cm_fn_0094fe2a(*(volatile u32 *)(stream + 0x10)) != 0);
    return 0;
}
