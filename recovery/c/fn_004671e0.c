#include "cmre.h"

/* ECX is the object (thiscall receiver). The routine returns the address
   object + 0x110 in EAX without reading memory. Address arithmetic is unsigned
   32-bit, matching LEA. */
u32 CM_THISCALL cm_fn_004671e0(u32 self)
{
    return self + 0x110u;
}
