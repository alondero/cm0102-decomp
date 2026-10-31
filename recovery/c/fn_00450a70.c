#include "cmre.h"

/* WIP: return width. MOV AX,[ECX] writes only AX; the u16 declaration is refused because
   callers read EAX[31:16], which carries the incoming EAX value that plain C cannot express. */
u16 CM_THISCALL cm_fn_00450a70(volatile u16 *self)
{
    return *self;
}
