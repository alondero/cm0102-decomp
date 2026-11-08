/* Preferred-image VA 0x0048ce10, image base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   regparm(3) names the actual entry EAX/EDX/ECX. The first false path
   preserves entry EAX's high24; the first callback also receives entry EDX.
   Each callback receives the original EAX, EDX, ECX and two stack words.
   Its full physical EAX result is retained before changing AL. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))

typedef u32 (CM_STDCALL __attribute__((regparm(3))) *cm_slot_48ce10)
    (u32, u32, u32, u32, u32);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_0048ce10(
    u32 carried_eax, u32 incoming_edx, u32 self)
{
    u32 object = self;
    u32 table, target;

    if ((s32)D(object + 0x1cu) >= 0)
        return carried_eax & 0xffffff00u;

    table = D(object);
    target = D(table + 0x10u);
    carried_eax = ((cm_slot_48ce10)target)(
        table, incoming_edx, object, 1u, 1u);
    if ((u8)carried_eax == 0)
        return carried_eax & 0xffffff00u;

    carried_eax = D(object + 0x20u);
    while ((s32)carried_eax >= 0) {
        table = D(0x00adadfcu);
        object = D(table + carried_eax * 4u);
        table = D(object);
        target = D(table + 0x10u);
        carried_eax = ((cm_slot_48ce10)target)(
            carried_eax, table, object, 1u, 1u);
        if ((u8)carried_eax == 0)
            return carried_eax & 0xffffff00u;
        carried_eax = D(object + 0x20u);
    }
    return (carried_eax & 0xffffff00u) | 1u;
}
