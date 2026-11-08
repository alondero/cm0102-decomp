/* Preferred-image VA 0x007f8ee0, image base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8; the sole indexed caller does not consume a result.
   Original scratch: L0 temporary date, L8 search key, L10 date object.
   qsort's incidental EDX:EAX is ignored. Its ordinary four stack arguments
   remain a native boundary; its accepted register-transport C declaration
   requires a separate composition binding. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern void CM_THISCALL cm_fn_0054a110(u16 *self);
extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *temporary, u16 *source);
extern u32 CM_CDECL cm_fn_00946464(u32 key, u32 base, u32 count, u32 width, u32 compare);
extern unsigned long long CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 width, u32 compare);

void CM_THISCALL cm_fn_007f8ee0(u32 self, u32 first, u32 second)
{
    u8 scratch[0x1c];
    u32 l = (u32)scratch;
    u32 value, count, base, found, index, offset;
    cm_fn_0054a110((u16 *)(l + 0x10u));
    if (second == 0u) {
        value = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        value = D(value * 0x300u + 0x00b74c26u);
        if (value == 0u) return;
        second = D(value + 0x39u);
        if (second == 0u) return;
    }
    if (first == 0u) return;
    value = D(first);
    D(l + 0xcu) = value;
    if (second != 0u)
        D(l + 8u) = D(second);
    else
        D(l + 8u) = 0xffffffffu;
    count = D(self + 0x64u);
    base = D(self + 0x5cu);
    found = cm_fn_00946464(l + 8u, base, count, 0x11u, 0x007fc270u);
    if (found == 0u) return;
    base = D(self + 0x5cu);
    index = (u32)(((unsigned long long)0xf0f0f0f1u * (found - base)) >> 36);
    count = D(self + 0x64u);
    if ((s32)index < (s32)(count - 1u)) {
        offset = index * 0x11u;
        do {
            base = D(self + 0x5cu);
            value = D(offset + base + 0x11u);
            D(offset + base) = value;
            value = D(offset + base + 0x15u);
            D(offset + base + 4u) = value;
            cm_fn_00417c10((u32 *)(offset + base + 8u),
                          (u32 *)l, (u16 *)(offset + base + 0x19u));
            value = B(offset + base + 0x21u);
            index += 1u;
            B(offset + base + 0x10u) = (u8)value;
            count = D(self + 0x64u);
            offset += 0x11u;
        } while ((s32)index < (s32)(count - 1u));
    }
    count = D(self + 0x64u) - 1u;
    D(self + 0x64u) = count;
    if (count != 0u) {
        base = D(self + 0x5cu);
        (void)cm_fn_00945333(base, count, 0x11u, 0x007fc270u);
    }
}
