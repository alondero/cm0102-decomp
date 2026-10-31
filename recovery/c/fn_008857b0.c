#include "cmre.h"

/* MOV AL,[ECX+0x44] writes only AL; return width is u8. */
u8 CM_THISCALL cm_fn_008857b0(volatile u8 *self)
{
    return self[0x44];
}
