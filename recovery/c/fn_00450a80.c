#include "cmre.h"

/* WIP: return width. MOV AX,[ECX+2] writes only AX; the u16 declaration is refused because
   callers read EAX[31:16], which carries the incoming EAX value that plain C cannot express. */
u16 CM_THISCALL cm_fn_00450a80(volatile u8 *self)
{
    return *(volatile u16 *)(self + 2);
}
