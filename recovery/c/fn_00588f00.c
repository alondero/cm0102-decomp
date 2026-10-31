#include "cmre.h"

/* Preferred-image VA 0x00588f00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl with three stack arguments, as the caller's add esp,0xc confirms.
   Name buffers retain their original 400-byte adjacency. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_CDECL cm_fn_00588f00(u32 source, u32 details, u32 output)
{
    u8 scratch[0x338];
    u32 slots[10], i, target, format, first_buffer, second_buffer;
    u32 a_buffer = (u32)scratch + 0x1a8u;
    u32 b_buffer = (u32)scratch + 0x18u;
    u32 base, a_offset, b_offset, eax;

    for (i = 0; i < 10; ++i)
        slots[i] = (u32)cm_fn_00785830((void *)source, i);
    if (details == 0) {
        /* The original signed-remainder sequence tests evenness of a wrapped
           DWORD sum. Its zero/nonzero result is exactly the low-bit test. */
        if (((slots[8] + slots[0]) & 1u) == 0) {
            cm_fn_0053e140(slots[5], slots[6], slots[7], b_buffer, 400, 5, slots[4]);
            cm_fn_0066f4e0(0x00de1f64u, 0x009c5e70u, b_buffer);
            cm_fn_004ae660((void *)output, 0x00de1f64u);
            base = D(0x00ae23bcu);
            b_offset = slots[4] * 110u;
            eax = cm_fn_0053e990(base + b_offset, slots[5], slots[6], slots[7], 0);
            if (eax == 0) return eax;
            base = D(0x00ae23bcu);
            return cm_fn_004ae8a0(output, b_buffer, 0x7da, base + b_offset);
        }
        cm_fn_0053e140(slots[1], slots[2], slots[3], a_buffer, 400, 5, slots[0]);
        cm_fn_0066f4e0(0x00de1f64u, 0x009c5e44u, a_buffer);
        cm_fn_004ae660((void *)output, 0x00de1f64u);
        base = D(0x00ae23bcu);
        a_offset = slots[0] * 110u;
        eax = cm_fn_0053e990(base + a_offset, slots[1], slots[2], slots[3], 0);
        if (eax == 0) return eax;
        base = D(0x00ae23bcu);
        return cm_fn_004ae8a0(output, a_buffer, 0x7da, base + a_offset);
    }
    cm_fn_0053e140(slots[5], slots[6], slots[7], b_buffer, 400, 1, slots[4]);
    cm_fn_0053e140(slots[1], slots[2], slots[3], a_buffer, 400, 1, slots[0]);
    first_buffer = b_buffer;
    second_buffer = a_buffer;
    if (slots[9] == 999) {
        if (slots[8] == 0) {
            format = 0x009c5ea8u;
        } else if (slots[8] == 1) {
            first_buffer = a_buffer;
            second_buffer = b_buffer;
            format = 0x009c5f64u;
        } else {
            cm_fn_0066f4e0(0x00de1f64u, 0x009c601cu, b_buffer, a_buffer);
            goto append;
        }
    } else if (slots[8] <= 4) {
        target = D(0x005892ecu + slots[8] * 4u);
        switch (target) {
        case 0x00588ffa: format = 0x009c6454u; break;
        case 0x00589013: format = 0x009c63b0u; break;
        case 0x0058902c: format = 0x009c62e0u; break;
        case 0x00589045: format = 0x009c6234u; break;
        case 0x0058905b: format = 0x009c6154u; break;
        default: return ((u32 (CM_CDECL *)(void))target)();
        }
    } else {
        format = 0x009c60d4u;
    }
    cm_fn_0066f4e0(0x00de1f64u, format, 100, first_buffer, second_buffer);
append:
    cm_fn_004ae660((void *)output, 0x00de1f64u);
    base = D(0x00ae23bcu);
    a_offset = slots[0] * 110u;
    eax = cm_fn_0053e990(base + a_offset, slots[1], slots[2], slots[3], 0);
    if (eax != 0) {
        base = D(0x00ae23bcu);
        cm_fn_004ae8a0(output, a_buffer, 0x7da, base + a_offset);
    }
    base = D(0x00ae23bcu);
    b_offset = slots[4] * 110u;
    eax = cm_fn_0053e990(base + b_offset, slots[5], slots[6], slots[7], 0);
    if (eax == 0) return eax;
    base = D(0x00ae23bcu);
    return cm_fn_004ae8a0(output, b_buffer, 0x7da, base + b_offset);
}
