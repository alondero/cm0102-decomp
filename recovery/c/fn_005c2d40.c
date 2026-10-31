#include "cmre.h"
/* Preferred-image VA005c2d40, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/RET8/fullEAX from final original destructor-array boundary.
   SEH959e4e/scratch70: tableL0/IDsL4,L8/countLc/keyptrL10/allocationL14,
   datesL18,L20,L28,L30,L38,L40,L48..57; fully initialized22B keyL58.
   549ef0 ignores upper argument halves; retain its real EDX return when
   forming the later partial-DX day argument. Keep partial-AX date arithmetic. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_STDCALL cm_fn_009463af(u32, u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_009462bb(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054cab0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);

u32 CM_STDCALL cm_fn_005c2d40(volatile u32 base_arg, volatile u32 count_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x70];
    u32 l = (u32)scratch, table, p, q, object, info, id, map, first, second;
    u32 a, b, eax, edx, difference, minimum, base, count, position, last, expected;
    unsigned long long pair;
    frame.state = 0xffffffffu; frame.handler = 0x00959e4e;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_009463af(l + 0x48, 8, 2, 0x0054a110, 0x005a53c0);
    table = 0x00b63d60;
    frame.state = 0;
    D(l) = table; D(l + 0xc) = 35;
    do {
        p = D(table);
        if (p == 0) goto next_table;
        if (B(p + 0x11c) == 0) goto next_table;
        p = D(table + 0x26);
        if (p != 0) {
            id = D(p); map = G(0x00adadfc); object = D(map + id * 4u);
            if (object != 0 && B(object + 0x43) == 1) {
                info = D(object + 0xa3);
                second = (u32)(s32)(s8)B(info + 5);
                first = W(info + 3);
                pair = cm_fn_00549ef0(l + 0x20, first, second);
                cm_fn_00417c10(l + 0x48, l + 0x28, (u32)pair);
                info = D(object + 0xa3);
                if (B(info + 0x21) == 2) {
                    second = (u32)(s32)(s8)B(info + 5);
                    first = W(info + 3);
                    pair = cm_fn_00549ef0(l + 0x30, first, second);
                    info = D(object + 0xa3);
                    B((u32)&frame.state) = 1;
                    edx = ((u32)(pair >> 32) & 0xffff0000u) |
                          (u16)(s16)(s8)B(info + 0x22);
                    eax = cm_fn_0054c5b0((u32)pair, l + 0x38, edx);
                    cm_fn_00417c10(l + 0x48, l + 0x40, eax);
                    B((u32)&frame.state) = 0;
                }
                object = D(object + 0xa7);
                p = D(object);
                if (p != 0) D(l + 4) = D(p);
                else D(l + 4) = 0xffffffffu;
                p = D(object + 6);
                if (p != 0) D(l + 8) = D(p);
                else D(l + 8) = 0xffffffffu;
                goto date_and_search;
            }
        }
        D(l + 4) = 0xffffffffu; D(l + 8) = 0xffffffffu;
date_and_search:
        eax = cm_fn_0054cab0(table + 0x2a, l + 0x18, 1);
        a = W(l + 0x1a); edx = G(0x00ae2c90);
        a = (u16)(a - W(0x00ae2c92));
        B((u32)&frame.state) = 2;
        eax = (eax & 0xffff0000u) | a;
        b = D(l + 0x18);
        difference = eax * 365u - edx + b;
        minimum = difference - 18u;
        if ((s16)(u16)minimum < 3) minimum = 3;
        p = D(table); base = base_arg;
        D(l + 0x58) = 0;
        B(l + 0x5c) = 0; B(l + 0x5d) = 0xff;
        a = D(p); D(l + 0x5e) = a;
        count = count_arg;
        D(l + 0x62) = 0; D(l + 0x66) = 0; D(l + 0x6a) = 0;
        D(l + 0x10) = l + 0x58;
        position = cm_fn_00946464(l + 0x10, base, count, 4, 0x005c5290);
        if (position != 0) {
            if (position > base) {
                p = D(l); p = D(p); expected = D(p);
                do {
                    p = D(position - 4); position -= 4;
                    if (D(p + 6) != expected) { position += 4; break; }
                } while (position > base);
            }
            count = count_arg;
            last = base + count * 4u - 4u;
            if (position < last) {
                do {
                    p = D(position); id = D(p); map = G(0x00ae23b4);
                    a = D(map + id * 581u + 0x53);
                    q = D(l);
                    if (a != D(q)) break;
                    if (id == D(l + 4) || id == D(l + 8)) {
                        eax = (u32)cm_fn_00944cf1(14);
                        D(l + 0x14) = eax;
                        B((u32)&frame.state) = 3;
                        if (eax != 0) eax = cm_fn_0068d020(eax);
                        p = D(position);
                        B((u32)&frame.state) = 2;
                        D(p + 0x12) = eax;
                    }
                    p = D(position); position += 4;
                    W(p + 0xe) = (u16)minimum;
                    p = D(position - 4); W(p + 0x10) = (u16)difference;
                } while (position < last);
            }
        }
        table = D(l);
        B((u32)&frame.state) = 0;
next_table:
        count = D(l + 0xc); table += 0x48; --count;
        D(l) = table; D(l + 0xc) = count;
    } while (count != 0);
    frame.state = 0xffffffffu;
    eax = cm_fn_009462bb(l + 0x48, 8, 2, 0x005a53c0);
    a = frame.prev;
    CM_FS(volatile u32, 0) = a;
    return eax;
}
