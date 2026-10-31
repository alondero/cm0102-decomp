#include "cmre.h"

/* Global dword at 0x009cd804, loaded whole into EAX and returned. */
#define GLOBAL_U32_009CD804 CM_GLOBAL(u32, 0x009cd804)

u32 CM_CDECL cm_fn_005e8160(void)
{
    return GLOBAL_U32_009CD804;
}
