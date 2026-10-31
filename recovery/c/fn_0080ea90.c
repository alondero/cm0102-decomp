#include "cmre.h"

/* ECX is read before it is written: it is the object pointer (thiscall
   receiver). The routine returns the 64-bit value at object+0 in EDX:EAX,
   low dword first (EAX), then the high dword (EDX) from object+4. */
unsigned long long CM_THISCALL cm_fn_0080ea90(u32 self)
{
    u32 low = *(volatile u32 *)(self + 0u);
    u32 high = *(volatile u32 *)(self + 4u);

    return ((unsigned long long)high << 32) | (unsigned long long)low;
}
