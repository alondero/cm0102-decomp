#include "cmre.h"

/* Object flag byte read through ECX at offset 0x4e, masked with 8. The
   listing performs `and eax, 8` after the byte load, so EAX is fully defined
   (zero-extended byte AND 8) and the return is a u32. */
u32 CM_THISCALL cm_fn_008857f0(u32 self)
{
    u32 value = *(volatile u8 *)(self + 0x4eu);
    return value & 8u;
}
