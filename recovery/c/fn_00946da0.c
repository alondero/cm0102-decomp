/* Preferred-image VA 0x00946da0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00946db3(u32 path, void *destination, u32 capacity);
u32 CM_CDECL cm_fn_00946da0(void *destination, u32 capacity)
{
    return cm_fn_00946db3(0, destination, capacity);
}
