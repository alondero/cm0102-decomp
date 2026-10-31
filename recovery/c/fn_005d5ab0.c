#include "cmre.h"

/* ECX is the object (thiscall receiver). The routine loads the dword at
   object + 0x856 into EAX and returns it. */
u32 CM_THISCALL cm_fn_005d5ab0(u32 self)
{
    return *(volatile u32 *)(self + 0x856u);
}
