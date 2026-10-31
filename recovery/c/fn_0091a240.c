#include "cmre.h"

/* Preferred-image VA 0091a240, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch224: index L0/offset L4/list L8/target Lc/allocated object L10,
   dates L14/L1c, diagnostics L24/L124. The original next index is offset+1,
   not the previous index+1. Retain the forward 14 DWORD/WORD/BYTE copy. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(void *, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0088c6d0(u32, u16, u32, u32, u32, u32, u32,
                                     u32, u8, u8, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0091a240(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x224];
    u32 l = (u32)scratch, base, first, second, index, part, eax, receiver;
    u32 list, object, slot, value, expected, count, outer_list, selector;
    u32 bytes, filter, ids, src, dst, offset, target, i, version;
    u8 flag_a, flag_b;
    s8 bound;
    frame.state = 0xffffffffu;
    frame.handler = 0x00965a54;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    base = D(self + 0xba);
    second = W(base + 2);
    first = W(base);
    second = (u16)(second + W(self + 0x40));
    cm_fn_00549ef0((void *)(l + 0x14), first, second);
    index = (u32)(s32)(s16)W(self + 0xa9);
    base = D(self + 0xba);
    part = index * 65u;
    frame.state = 0;
    second = W(part + base - 0x3f);
    first = W(part + base - 0x41);
    second = (u16)(second + W(self + 0x40));
    cm_fn_00549ef0((void *)(l + 0x1c), first, second);
    bound = (s8)B(self + 0xff);
    *(volatile u8 *)&frame.state = 1;
    D(l) = 0xffffffffu;
    if (bound < -1) goto final_sort;
    D(l + 4) = 0;
    eax = 0xffffffffu;
    for (;;) {
        if ((s32)eax > -1) {
            base = D(self + 0xc);
            receiver = D(base + eax * 4u);
        } else receiver = self;
        cm_fn_006827d0(receiver, 0);
        base = cm_fn_006826d0((void *)receiver, 1);
        value = D(base);
        D(l + 0xc) = value;
        eax = (u32)cm_fn_00944e46(0x10);
        D(l + 8) = eax;
        if (eax == 0) goto allocation_error;
        list = eax;
        for (slot = 0; slot < 4; ++slot) {
            base = cm_fn_006826d0((void *)receiver, slot);
            base = D(base);
            value = D(base);
            D(list + slot * 4u) = value;
        }
        eax = (u32)cm_fn_00944cf1(0xee);
        D(l + 0x10) = eax;
        *(volatile u8 *)&frame.state = 2;
        if (eax != 0) {
            count = (u32)(s32)(s16)W(receiver + 0x3e);
            outer_list = D(self + 0x10);
            selector = (count - 1u) * count * 2u;
            flag_b = B(self + 0xc3);
            bytes = self + 0xc5;
            flag_a = B(self + 0xc2);
            base = D(self + 4);
            filter = D(base);
            ids = D(l + 8);
            object = cm_fn_0088c6d0(eax, 4, ids, 0xffffffffu, 0xffffffffu,
                                     l + 0x14, l + 0x1c, filter, flag_a,
                                     flag_b, bytes, selector, outer_list);
        } else object = 0;
        *(volatile u8 *)&frame.state = 1;
        for (slot = 0; slot < 4; ++slot) {
            base = cm_fn_006826d0((void *)object, slot);
            expected = D(l + 0xc);
            value = D(base);
            if (value == expected) {
                index = (u32)(s32)(s8)B(self + 0x100);
                src = D(object + 0xb1);
                base = D(self + 0xc);
                src += slot * 59u;
                base = D(base + index * 4u);
                offset = D(l + 4);
                dst = D(base + 0xb1) + offset;
                for (i = 0; i < 14; ++i) {
                    value = D(src); D(dst) = value;
                    src += 4; dst += 4;
                }
                value = W(src); W(dst) = (u16)value;
                src += 2; dst += 2;
                value = B(src); B(dst) = (u8)value;
                index = (u32)(s32)(s8)B(self + 0x100);
                base = D(self + 0xc);
                base = D(base + index * 4u);
                dst = D(base + 0xb1);
                D(dst + offset + 9) = 0;
                base = D(self + 0xc);
                index = (u32)(s32)(s8)B(self + 0x100);
                offset += 0x3b;
                base = D(base + index * 4u);
                D(l + 4) = offset;
                dst = D(base + 0xb1);
                D(dst + offset - 0x36) = 0;
                break;
            }
        }
        if (object != 0) {
            base = D(object);
            target = D(base);
            ((u32 (CM_THISCALL *)(u32, u32))target)(object, 1);
        }
        list = D(l + 8);
        cm_fn_009452ca(list);
        eax = D(l + 4);
        bound = (s8)B(self + 0xff);
        ++eax;
        D(l) = eax;
        if ((s32)eax > (s32)bound) break;
        eax = D(l);
    }
final_sort:
    index = (u32)(s32)(s8)B(self + 0x100);
    base = D(self + 0xc);
    receiver = D(base + index * 4u);
    eax = cm_fn_006827d0(receiver, 0);
    goto done;
allocation_error:
    cm_fn_00944cff(0x00ad4a6c, 0, 0, (void *)(l + 0x124), (void *)(l + 0x24));
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x124, l + 0x24, 0x57c);
    eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
