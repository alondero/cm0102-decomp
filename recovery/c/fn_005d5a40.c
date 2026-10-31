#include "cmre.h"

/* MOV AL,[ECX+0x3e] writes only AL; return width is u8. */
u8 CM_THISCALL cm_fn_005d5a40(volatile u8 *self)
{
    return self[0x3e];
}
