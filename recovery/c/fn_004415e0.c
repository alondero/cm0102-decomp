#include "cmre.h"

/* Global byte at 0x0098b140 (one byte, read-only here). The routine loads it
   into AL and returns; EAX upper bits are not written, so the return is u8. */
#define GLOBAL_U8_0098B140 CM_GLOBAL(u8, 0x0098b140)

u8 CM_CDECL cm_fn_004415e0(void)
{
    return GLOBAL_U8_0098B140;
}
