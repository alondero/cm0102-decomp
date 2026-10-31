#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_5ad780 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_5ad780, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u8 CM_CDECL cm_fn_0048dd70(u32, u32);
extern u8 CM_CDECL cm_fn_0048df40(u32);
extern u32 CM_CDECL cm_fn_005af5b0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_0048dee0(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_FASTCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern void CM_THISCALL cm_fn_005adba0(u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern void CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_00776fc0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA005ad780/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full pointer EAX. Scratch42c: arrayL4, countL18 BYTE,
   outputL14, selfL1c, dateL20, allocationL28, diagnosticsL2c/12c/22c/32c.
   Full549ef0 proves high WORDs of the two partial-register pushes unused;
   full5adba0 proves its fifth physical stack argument is BYTE-only at
   5ade02/5ae2ec. The unwritten upper count bytes are never synthesized.
   Explicit original boundaries; the caller's five helper calls stay distinct. */
u32 CM_THISCALL cm_fn_005ad780(u32 self, u32 fixture,
                              volatile u32 output, volatile u32 extra)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x42c / 4];
    u32 l = (u32)scratch, eax, object, pointer, count, total, receiver;
    u32 filename, extension, line, version, result;
    u8 n, value;
    frame.state = 0xffffffffu;
    frame.handler = 0x009597c9;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(l + 0x1c) = self;
    D(l + 0x14) = 0;
    if (fixture == 0) {
        filename = l + 0x2c; extension = l + 0x12c; line = 0xbdf;
        goto diagnostic;
    }
    pointer = output;
    if (pointer == 0) {
        filename = l + 0x12c; extension = l + 0x2c; line = 0xbe4;
        goto diagnostic;
    }
    D(pointer) = 0;
    eax = D(fixture + 0x14);
    D(l + 4) = 0xffffffffu;
    D(l + 8) = 0xffffffffu;
    D(l + 0xc) = 0xffffffffu;
    W(l + 0x10) = 0xffff;
    B(l + 4) = 0; B(l + 0x12) = 0xff;
    B(l + 5) = 1; B(l + 6) = 2; B(l + 7) = 3;
    B(l + 8) = 4; B(l + 9) = 5; B(l + 0xa) = 6;
    value = cm_fn_0048dd70(eax, fixture);
    B(l + 3) = value;
    B(l + 0xb) = value;
    eax = D(fixture + 0x1c);
    n = 8;
    if (eax != 0) {
        eax = D(eax + 0x57);
        if (eax != 0) {
            value = cm_fn_0048dd70(eax, 0);
            B(l + 0x18) = 9;
            n = B(l + 0x18);
            B(l + 0xc) = value;
        }
    }
    eax = D(fixture + 0x20);
    if (eax != 0) {
        eax = D(eax + 0x57);
        if (eax != 0) {
            value = cm_fn_0048dd70(eax, 0);
            u32 index = (u32)(s32)(s8)n;
            ++n;
            B(l + 4 + index) = value;
        }
    }
    eax = D(fixture + 0x18);
    if (eax != 0) {
        value = cm_fn_0048df40(eax);
        u32 index = (u32)(s32)(s8)n;
        ++n;
        B(l + 4 + index) = value;
    }
    eax = D(fixture + 0x1c);
    eax = cm_fn_005af5b0(eax);
    object = D(fixture + 0x20);
    count = (u32)(s32)(s8)n;
    ++n;
    B(l + 4 + count) = (u8)eax;
    eax = cm_fn_005af5b0(object);
    object = D(fixture);
    count = (u32)(s32)(s8)n;
    ++n;
    B(l + 4 + count) = (u8)eax;
    eax = D(0x009cf7bc);
    B(l + 0x18) = n;
    if (object != eax) {
        value = B(l + 3);
        if (value == 3 || value == 5) extra = 0;
    }
    eax = D(fixture + 0x1c);
    eax = cm_fn_0053b900(eax);
    if (eax != 0) {
        eax = D(fixture + 0x1c);
        eax = cm_fn_0048dee0(eax);
        count = (u32)(s32)(s8)n;
        ++n;
        B(l + 4 + count) = (u8)eax;
        eax = D(fixture + 0x20);
        eax = cm_fn_0048dee0(eax);
        count = (u32)(s32)(s8)n;
        ++n;
        B(l + 4 + count) = (u8)eax;
        B(l + 0x18) = n;
    }
    total = (u32)(s32)(s8)B(l + 0x18);
    u32 end = total - 1u;
    if ((s32)end > 0) {
        u32 index = 0;
        do {
            s8 next = (s8)(u8)((u8)index + 1u);
            s8 limit = (s8)B(l + 0x18);
            if (next < limit) {
                pointer = l + 4 + (u32)(s32)next;
                count = total - (u32)(s32)next;
                do {
                    value = B(l + 4 + index);
                    if ((s8)value > -1 && value == B(pointer)) B(pointer) = 0xff;
                    ++pointer;
                    --count;
                } while (count != 0);
            }
            ++index;
        } while ((s32)index < (s32)end);
    }
    eax = cm_fn_00944cf1(0xe);
    D(l + 0x28) = eax;
    frame.state = 0;
    result = eax != 0 ? cm_fn_0068d020(eax) : 0;
    frame.state = 0xffffffffu;
    if (result == 0) {
        filename = l + 0x32c; extension = l + 0x22c; line = 0xc1d;
        goto diagnostic;
    }
    u16 second = W(fixture + 0x28);
    u16 first = W(fixture + 0x2a);
    cm_fn_00549ef0(l + 0x20, first, second);
    value = B(fixture + 0x3a);
    count = B(l + 0x18);
    frame.state = 1;
    if ((s8)value < 0x17) {
        if (extra != 0) {
            receiver = D(l + 0x1c);
            cm_fn_005adba0(receiver, fixture, l + 0x14, result, l + 4,
                          count, l + 0x20, 0xfffffffeu);
        } else receiver = D(l + 0x1c);
        cm_fn_005adba0(receiver, fixture, l + 0x14, result, l + 4,
                      count, l + 0x20, 0xffffffffu);
    } else receiver = D(l + 0x1c);
    cm_fn_005adba0(receiver, fixture, l + 0x14, result, l + 4, count, l + 0x20, 0);
    cm_fn_005adba0(receiver, fixture, l + 0x14, result, l + 4, count, l + 0x20, 1);
    if (extra != 0)
        cm_fn_005adba0(receiver, fixture, l + 0x14, result, l + 4,
                      count, l + 0x20, 0xfffffffeu);
    eax = D(l + 0x14);
    if (eax == 0) {
        cm_fn_0068d040(result);
        cm_fn_00944c94(result);
        receiver = D(0x00b74318);
        eax = cm_fn_00776fc0(receiver, fixture);
        if (eax == 0) {
            pointer = output;
            D(pointer) = 0x15;
        }
        eax = 0;
    } else {
        pointer = output;
        D(pointer) = eax;
        eax = result;
    }
    goto done;
diagnostic:
    cm_fn_00944cff(0x009cc208, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    eax = 0;
done:
    u32 predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return eax;
}
