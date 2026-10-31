#include "cmre.h"

/* Preferred-image VA 0x0072bd50, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The 0x1528-byte probed frame contains adjacent name/message buffers at
   offsets 0x10 and 0x1a0. Ret 0xc proves the three stack arguments. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) (*(volatile s8 *)(u32)(a))

u32 CM_STDCALL cm_fn_0072bd50(u32 source, u32 details, u32 output)
{
    u8 scratch[0x1528];
    u32 slots[8], i, names = (u32)scratch + 0x10u;
    u32 message = (u32)scratch + 0x1a0u;
    u32 base, club = 0, map, target, format, person_offset, eax;
    s32 style;

    for (i = 0; i < 8; ++i)
        slots[i] = (u32)cm_fn_00785830((void *)source, i);
    if (details == 0) {
        cm_fn_0053e140(slots[1], slots[2], slots[3], names, 400, 5, slots[0]);
        if (slots[5] == 0x25u) format = 0x00a17c80u;
        else if ((slots[7] & 1u) != 0) format = 0x00a17c54u;
        else format = 0x00a17c20u;
        cm_fn_0066f4e0(message, format, names);
        goto append;
    }
    base = D(0x00ae23b4u);
    club = base + slots[4] * 581u;
    cm_fn_0053e140(slots[1], slots[2], slots[3], names, 400, 1, slots[0]);
    if (slots[5] > 0x78u) {
        format = 0x00a17cd8u;
        goto simple;
    }
    map = B(0x0072c174u + slots[5]);
    target = D(0x0072c140u + map * 4u);
    switch (target) {
    case 0x0072be5e:
        if (slots[7] == 0) {
            style = S(club + 0x37u);
            cm_fn_0066f4e0(message, 0x00a183c0u, style, style, names, club + 4u);
            goto append;
        }
        if (slots[7] == 1) {
            cm_fn_0066f4e0(message, 0x00a1848cu, 100, names);
            goto append;
        }
        format = slots[7] == 2 ? 0x00a18510u : 0x00a185d0u;
        break;
    case 0x0072bed2: format = 0x00a18308u; break;
    case 0x0072bf26: format = 0x00a18078u; break;
    case 0x0072bf4e: format = 0x00a17f00u; break;
    case 0x0072bf6e: format = 0x00a17d98u; break;
    case 0x0072be4f: format = 0x00a18694u; break;
    case 0x0072bf5a: format = 0x00a17e34u; break;
    case 0x0072bee9: format = 0x00a18228u; break;
    case 0x0072bf00:
        style = S(club + 0x52u);
        cm_fn_0066f4e0(message, 0x00a18130u, style, style, names, club + 0x38u);
        goto append;
    case 0x0072bf3a: format = 0x00a17f8cu; break;
    case 0x0072be21: format = 0x00a18840u; break;
    case 0x0072be38: format = 0x00a1877cu; break;
    case 0x0072bf82: format = 0x00a17cd8u; break;
    default: return ((u32 (CM_CDECL *)(void))target)();
    }
simple:
    cm_fn_0066f4e0(message, format, names);
append:
    cm_fn_004ae660((void *)output, message);
    person_offset = slots[0] * 110u;
    base = D(0x00ae23bcu);
    eax = cm_fn_0053e990(base + person_offset, slots[1], slots[2], slots[3], 0);
    if (eax == 0) return eax;
    base = D(0x00ae23bcu);
    return cm_fn_004ae8a0(output, names, 0x7d7, base + person_offset);
}
