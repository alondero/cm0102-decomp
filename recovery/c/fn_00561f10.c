#include "cmre.h"

/* Preferred-image VA 00561f10, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch238: base L0, 49-byte forward backup L4, diagnostic buffers L38/L138.
   403c80 uses incoming EDX's upper half at 403d56..65. On the unchanged-
   counter path retain EDX from 5e42b0 or the counter with DL replaced by state.
   90d130 takes no arguments: the three live pushes belong to 944c9f. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern void CM_CDECL cm_fn_005e3fd0(u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_005e42b0(u32);
extern void CM_FASTCALL cm_fn_00403c80(u32, u32);
extern u32 CM_CDECL cm_fn_005e2eb0(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_00403ee0(u32, u32);

u32 CM_THISCALL cm_fn_00561f10(u32 self, volatile u16 index_arg)
{
    u8 scratch[0x238];
    u32 l = (u32)scratch, base, object, x, y, left, t, value;
    u32 current, old, width, top, low, high, buffer, edx, a, b, c, d, i;
    u8 state;
    unsigned long long result;
    base = self + (u32)(s32)(s16)index_arg * 0xbf1u;
    D(l) = base;
    object = base + 0xb7e;
    if (object == 0) {
        cm_fn_00944cff(0x009c36ec, 0, 0, l + 0x138, l + 0x38);
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value,
                       l + 0x138, l + 0x38, 0x71f);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    for (i = 0; i < 12; ++i) D(l + 4 + i * 4) = D(object + i * 4);
    B(l + 0x34) = B(object + 0x30);
    B(object + 0x30) = 0;
    value = D(0x00b67a34);
    if (value == 0) goto compare;
    left = D(object);
    x = D(0x00b67a20);
    y = D(0x00b67a14);
    if ((s32)x < (s32)left) goto compare;
    if ((s32)x > (s32)D(object + 8)) goto down;
    if ((s32)y < (s32)D(object + 4)) goto down;
    t = D(object + 0x10) - 1u;
    if ((s32)y > (s32)t) goto down;
    value = D(object + 0x1c) - 1u;
    B(object + 0x30) = 1;
    value &= (u32)((s32)value <= 0) - 1u;
    D(object + 0x1c) = value;
    goto compare;
down:
    if ((s32)x < (s32)left) goto compare;
    if ((s32)x > (s32)D(object + 8)) goto page_up;
    t = D(object + 0x14) + 1u;
    if ((s32)y < (s32)t) goto page_up;
    if ((s32)y > (s32)D(object + 0xc)) goto page_up;
    value = D(object + 0x1c);
    t = D(object + 0x18);
    ++value;
    if ((s32)value >= (s32)t) value = t;
    D(object + 0x1c) = value;
    B(object + 0x30) = 2;
    goto compare;
page_up:
    if ((s32)x < (s32)left) goto compare;
    if ((s32)x > (s32)D(object + 8)) goto page_down;
    if ((s32)y < (s32)D(object + 0x10)) goto page_down;
    t = D(object + 0x24) - 1u;
    if ((s32)y > (s32)t) goto page_down;
    base = D(l);
    t = B(base + 0xbb0);
    value = D(object + 0x1c) - t;
    value &= (u32)((s32)value <= 0) - 1u;
    D(object + 0x1c) = value;
    goto compare;
page_down:
    if ((s32)x < (s32)left) goto compare;
    if ((s32)x > (s32)D(object + 8)) goto thumb;
    t = D(object + 0x28) + 1u;
    if ((s32)y < (s32)t) goto thumb;
    if ((s32)y > (s32)D(object + 0x14)) goto thumb;
    base = D(l);
    t = D(object + 0x1c);
    value = B(base + 0xbb0);
    value += t;
    t = D(object + 0x18);
    if ((s32)value >= (s32)t) value = t;
    D(object + 0x1c) = value;
    goto compare;
thumb:
    if ((s32)x < (s32)left) goto compare;
    if ((s32)x > (s32)D(object + 8)) goto compare;
    if ((s32)y < (s32)D(object + 0x24)) goto compare;
    if ((s32)y > (s32)D(object + 0x28)) goto compare;
    value = index_arg;
    D(self) = 2;
    edx = D(0x00b67a14);
    W(self + 0x12f52f) = (u16)value;
    D(self + 0x12f50d) = edx;
compare:
    current = D(object + 0x1c);
    old = D(l + 0x20);
    if (current == old) goto same_counter;
    width = D(object + 0x20);
    value = D(object + 0x14);
    top = D(object + 0x10);
    value = (value - width - top + 1u) * current;
    value = (u32)((s32)value / (s32)D(object + 0x18));
    low = value + top;
    D(object + 0x24) = low;
    high = low + width - 1u;
    buffer = D(object + 0x2c);
    D(object + 0x28) = high;
    if (buffer != 0) {
        b = D(object + 4); a = D(object);
        cm_fn_005e3fd0(a, b, buffer);
        buffer = D(object + 0x2c);
        cm_fn_005e42b0(buffer);
        D(object + 0x2c) = 0;
    }
    base = D(l);
    cm_fn_00403c80(base + 4, base);
    d = D(object + 0xc); c = D(object + 8);
    b = D(object + 4); a = D(object);
    cm_fn_005e2eb0(a, b, c, d);
    current = D(object + 0x1c);
    cm_fn_00403ee0(base + 4, current - old);
    return 1;
same_counter:
    state = B(object + 0x30);
    if (state == B(l + 0x34)) return 0;
    edx = (current & 0xffffff00u) | state;
    buffer = D(object + 0x2c);
    if (buffer != 0) {
        a = D(object); b = D(object + 4);
        cm_fn_005e3fd0(a, b, buffer);
        buffer = D(object + 0x2c);
        result = cm_fn_005e42b0(buffer);
        edx = (u32)(result >> 32);
        D(object + 0x2c) = 0;
    }
    base = D(l);
    cm_fn_00403c80(base + 4, edx);
    d = D(object + 0xc); c = D(object + 8);
    b = D(object + 4); a = D(object);
    cm_fn_005e2eb0(a, b, c, d);
    return 0;
}
