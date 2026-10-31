#include "cmre.h"

/* WIP: return width. MOV AL,[ECX+0x27] writes only AL, but the tool refuses u8 because
   callers read EAX beyond the low byte, which carries the incoming EAX value. */
u8 CM_THISCALL cm_fn_00714d30(volatile u8 *self)
{
    return self[0x27];
}
