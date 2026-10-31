#include "cmre.h"

/* Preferred-image VA 008aa550, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch2d4: kind BYTE L0 (original DWORD read retained), pass L4,
   saved person L8, 200-byte text Lc, diagnostics Ld4/L1d4.
   5b2ae0 observes kind BYTE and supplies AX; 5b7580 observes only bits0..11,
   so the original EAX upper half cannot escape this nested call boundary.
   Recursive call remains an explicit original boundary. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_0053dba0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u16 CM_THISCALL cm_fn_005b2ae0(u32, u8, u32);
extern u32 CM_CDECL cm_fn_005b7580(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008aa9d0(u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_008ab480(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090c1d0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008aa550(u32 self, volatile u32 index_arg,
                              volatile u32 output_arg, volatile u32 size_arg,
                              volatile u32 disable_arg, volatile u32 kind_arg,
                              volatile u32 alternate_arg)
{
    u8 scratch[0x2d4];
    u32 l = (u32)scratch, index, output, alternate, value, tag, record, lead;
    u32 person, field, kind, size, a, b, c, pool, other, pass;
    u8 slot;
    index = index_arg;
    if ((s8)(u8)index < 0 || (s8)(u8)index > 19) goto diagnostic;
    output = output_arg;
    if (output == 0) goto diagnostic;
    if ((s32)size_arg < 100) goto diagnostic;
    alternate = alternate_arg;
    B(output) = 0;
    if (alternate != 0 && D(self + 0xde5) == 0) return 0;
    person = cm_fn_008ab480(self, index, alternate);
    field = D(self + 0x10);
    D(l + 8) = person;
    if ((s32)field <= -1) {
        if (person == 0) goto fallback;
        kind = kind_arg;
        size = size_arg;
        cm_fn_0053dba0(person, output, size, kind);
        goto check_names;
    }
    if (alternate != 0) lead = (u32)(s32)(s8)B(self + 0xdea);
    else lead = (u32)(s32)(s8)B(self + 0xde9);
    record = self + lead * 0x625u + (u32)(s32)(s8)(u8)index * 0x49u + 0x1a8;
    tag = B(record + 0x1d);
    if (tag == 0xfb) goto fallback;
    person = D(record);
    kind = kind_arg;
    size = size_arg;
    c = D(record + 0x10);
    b = D(record + 0xc);
    a = D(record + 8);
    cm_fn_0053e140(a, b, c, output, size, kind, person);
    alternate = alternate_arg;
check_names:
    if ((u8)kind != 5) return 1;
    if (alternate != 0) pool = D(self + 0xde5);
    else pool = D(self);
    D(l + 4) = 0;
    for (;;) {
        slot = 0;
        do {
            other = D(pool + (u32)(s32)(s8)slot * 4u + 0xd7);
            if (other != 0 && other != D(l + 8)) {
                cm_fn_0053dba0(other, l + 0xc, 200, 5);
                value = cm_fn_0090c1d0(l + 0xc, output, 1, 0);
                if (value == 0) {
                    alternate = alternate_arg;
                    a = disable_arg;
                    size = size_arg;
                    index = index_arg;
                    return ((u32 (CM_THISCALL *)(u32, u32, u32, u32,
                                                u32, u32, u32))0x008aa550)
                        (self, index, output, size, a, 2, alternate);
                }
            }
            ++slot;
        } while ((s8)slot < 50);
        pool = cm_fn_00540a50(pool, 0, 1);
        if (pool == 0) return 1;
        pass = D(l + 4);
        if (pass != 0) return 1;
        D(l + 4) = 1;
    }
fallback:
    if (disable_arg != 0) return 1;
    if ((s32)field >= 0) alternate = alternate_arg;
    value = cm_fn_008aa9d0(self, (u8)index, l, alternate);
    if (value == 0) return 1;
    size = size_arg;
    value = D(l);
    value = cm_fn_005b2ae0(self + 0xe44, (u8)value, 0);
    return cm_fn_005b7580(value, output, size, 1);
diagnostic:
    cm_fn_00944cff(0x00a8b94c, 0, 0, l + 0x1d4, l + 0xd4);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value,
                   l + 0x1d4, l + 0xd4, 0x1af7);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
