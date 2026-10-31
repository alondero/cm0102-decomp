#include "cmre.h"

/* Preferred-image VA 0x008aadb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The output argument is cleared, but formatting targets the original global. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008aadb0(u32 self, volatile u32 index,
    volatile u32 output, volatile u32 length, volatile u32 colour,
    volatile u32 mode)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, dest, colour_ptr, remote, base, record, offset, text;
    u16 colour_value;
    s8 slot = (s8)index, value;
    if (slot < 0 || slot > 19) goto error;
    dest = output;
    if (dest == 0) goto error;
    if ((s32)length < 20) goto error;
    colour_ptr = colour;
    if (colour_ptr == 0) goto error;
    B(dest) = 0;
    colour_value = W(0x00aebdea);
    W(colour_ptr) = colour_value;
    remote = mode;
    if (remote != 0) {
        base = D(self + 0xde5);
        if (base == 0) return 0;
    }
    base = D(self + 4);
    if (base == 0) return 1;
    if (remote != 0) {
        offset = (u32)(s32)(s8)B(self + 0xdea);
        offset = (u32)(s32)slot + offset * 20u;
        record = base + offset * 446u + 0x4796;
    } else {
        record = self + (u32)(s32)slot * 446u + 0x2735;
    }
    if (B(record + 0x19) == 0xfb) return 1;
    value = (s8)B(record + 0x22);
    if (value >= 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a16a68, (u32)(s32)value + 1u);
        colour_value = W(0x00ae3154);
        W(colour_ptr) = colour_value;
        return 1;
    }
    value = (s8)B(record + 0x21);
    if (value >= 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a16a54, (u32)(s32)value + 1u);
        colour_value = W(0x00ae3154);
        W(colour_ptr) = colour_value;
        return 1;
    }
    value = (s8)B(record + 0x20);
    if (value >= 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a16a40, (u32)(s32)value + 1u);
        colour_value = W(0x00ae3154);
        W(colour_ptr) = colour_value;
        return 1;
    }
    value = (s8)B(record + 0x1f);
    if (value >= 0) {
        cm_fn_0066f4e0(0x00de1f64, 0x00a16a2c, (u32)(s32)value + 1u);
        colour_value = W(0x00aebdd4);
        W(colour_ptr) = colour_value;
    }
    return 1;
error:
    cm_fn_00944cff(0x00a8b94c, 0, 0, local + 0x100, local);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x1c9a);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
