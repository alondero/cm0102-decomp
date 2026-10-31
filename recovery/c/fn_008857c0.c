#include "cmre.h"

/* MOV AL,[ECX+0x3f] writes only AL; return width is u8. */
u8 CM_THISCALL cm_fn_008857c0(volatile u8 *self)
{
    return self[0x3f];
}
