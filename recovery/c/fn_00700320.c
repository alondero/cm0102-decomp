#include "cmre.h"

/* ECX is the object (thiscall receiver). The routine loads the single byte at
   object + 0x8ea7 into AL and returns it; only AL is written, so the return
   width is u8. */
u8 CM_THISCALL cm_fn_00700320(u32 self)
{
    return *(volatile u8 *)(self + 0x8ea7u);
}
