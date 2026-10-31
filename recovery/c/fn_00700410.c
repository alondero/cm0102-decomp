#include "cmre.h"

/* Object field read through ECX: dword at offset 0x8f7a from the object pointer.
   The routine takes no stack arguments and returns the dword in EAX (u32). */
u32 CM_THISCALL cm_fn_00700410(u32 self)
{
    return *(volatile u32 *)(self + 0x8f7au);
}
