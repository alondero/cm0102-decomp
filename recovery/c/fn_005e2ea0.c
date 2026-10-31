#include "cmre.h"

/* Global dword at 0x00ae319c, loaded whole into EAX and returned. */
#define GLOBAL_U32_00AE319C CM_GLOBAL(u32, 0x00ae319c)

u32 CM_CDECL cm_fn_005e2ea0(void)
{
    return GLOBAL_U32_00AE319C;
}
