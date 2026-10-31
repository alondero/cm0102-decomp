/* Preferred-image VA 00776250, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);
extern u32 CM_THISCALL cm_fn_00417c10(u32 self, void *output, u32 source);
extern u32 CM_CDECL cm_fn_00540980(u32 person, u32 flags);

u32 CM_STDCALL cm_fn_00776250(u32 nation, u32 kind, u32 person)
{
    u8 scratch[8];
    u8 mode = (u8)kind;
    u32 country, index, existing;
    if (mode == 0 || mode == 1)
        country = cm_fn_0053b3d0(nation);
    else
        country = cm_fn_0053b4f0(nation);
    kind = (kind & 0xffffff00u) | ((mode == 0 || mode == 2) ? 4u : 8u);
    index = 0;
    for (;;) {
        existing = D(country + index * 4u + 0xd7u);
        if (existing == 0) break;
        ++index;
        if (index == 0x32u) return country;
    }
    D(country + index * 4u + 0xd7u) = person;
    D(person + 0x24u) = country;
    (void)cm_fn_00417c10(person + 0x29u, scratch, 0x00ae2c90u);
    CM_GLOBAL(volatile u8, person + 0x28u) = 0xb;
    return cm_fn_00540980(person, kind);
}
