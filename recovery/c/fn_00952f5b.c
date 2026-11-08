/* Preferred-image VA 0x00952f5b, image base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The seven indexed callers observe the full EAX result. Original length,
   allocation and copy routines remain explicit execution boundaries. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_0094c3a0(u32 destination, u32 source);

u32 CM_CDECL cm_fn_00952f5b(u32 source)
{
    u32 length, allocated;

    if (source == 0)
        return 0;
    length = cm_fn_009488e0(source);
    allocated = (u32)cm_fn_00944e46(length + 1u);
    if (allocated == 0)
        return 0;
    return cm_fn_0094c3a0(allocated, source);
}
