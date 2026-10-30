#include "cmre.h"

extern volatile u32 *CM_CDECL cm_fn_00945ae0(u32 path, volatile s8 *mode_text, u32 share_arg);

volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text)
{
    return cm_fn_00945ae0(path, mode_text, 0x40u);
}
