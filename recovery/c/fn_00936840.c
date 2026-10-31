#include "cmre.h"

/* Preferred-image VA 0x00936840, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The allocator context is cached once; each released pointer is reloaded
   after the previous call. The signature guards short-circuit in listing order. */
extern void CM_CDECL cm_fn_009445d0(u32 context, u32 allocation);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))

u32 CM_CDECL cm_fn_00936840(u32 object)
{
    u32 allocation, context;
    if (object == 0)
        return 0;
    if (U32_AT(object, 0) != 0xa5a0f5a5u)
        return 0xffffffffu;
    if (U32_AT(object, 0x44c) != 0x0fa55af0u)
        return 0xffffffffu;

    allocation = U32_AT(object, 0x3d0);
    context = U32_AT(object, 0x444);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x3c8);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x1c);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x28);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x2c);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x34);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x40);
    cm_fn_009445d0(context, allocation);
    allocation = U32_AT(object, 0x58);
    cm_fn_009445d0(context, allocation);
    cm_fn_009445d0(context, object);
    return 0;
}
