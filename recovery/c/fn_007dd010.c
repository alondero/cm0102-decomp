#include "cmre.h"

/* Preferred-image VA 007dd010, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch40: count BYTE L2/kind L3, dates Lc/L4/L14, temps L1c/L24,
   notification L2c (date L30, flags L3f). Keep partial EAX returns and the
   BYTE rewrite of the second formal distinct from the saved payload pointer.
   Flags L3f are not initialized: callees observe only bits 0..2, and 556940
   supplies bit0 before copying/sending; high five bits never escape. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
#define AL(v) (eax = (eax & 0xffffff00u) | (u8)(v))
#define AX(v) (eax = (eax & 0xffff0000u) | (u16)(v))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32, u16, u8, u16, u32);
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007e82e0(u32, u32, volatile u32);
extern u32 CM_THISCALL cm_fn_00556940(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_00557280(u32, u32, u32);

u32 CM_THISCALL cm_fn_007dd010(u32 self, u32 input,
                              volatile u32 payload_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40];
    u32 l = (u32)scratch, eax, payload, object, first, second, date;
    u32 threshold, key, owner, person, base, compare_date;
    u16 year, other_year, day;
    u8 count, previous, current, flags, mode;
    frame.state = 0xffffffffu; frame.handler = 0x0095ffa0;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    eax = frame.prev;
    AL(B(input + 0x2c));
    B(l + 2) = 0;
    B(l + 3) = 0xff;
    if ((u8)eax == 0) {
        AL(B(input + 0x2d));
        if ((u8)eax == 0) goto done;
    }
    payload = payload_arg;
    second = W(payload + 0x28);
    first = W(payload + 0x2a);
    cm_fn_00549ef0((void *)(l + 0xc), first, second);
    year = (u16)(W(payload + 0x2c) + 1u);
    frame.state = 0;
    cm_fn_00549f70(l + 4, 1, 3, year, 0xffffffffu);
    *(volatile u8 *)&frame.state = 1;
    count = 0;
    do {
        eax = cm_fn_0054a3f0(l + 4);
        if ((u16)eax == 6) ++count;
        date = cm_fn_0054c5b0(l + 4, l + 0x1c, 1);
        *(volatile u8 *)&frame.state = 2;
        cm_fn_00417c10(l + 4, l + 0x24, date);
        *(volatile u8 *)&frame.state = 1;
    } while ((s8)count < 2);
    year = (u16)(W(payload + 0x2c) + 1u);
    cm_fn_00549f70(l + 0x14, 1, 2, year, 0xffffffffu);
    key = D(input);
    *(volatile u8 *)&frame.state = 3;
    eax = cm_fn_007e82e0(self, key, 1);
    count = B(input + 0x2c);
    object = eax;
    if (count == 0) goto no_counter_event;
    previous = B(object + 4);
    if (previous < 5) AL(5);
    else if (previous < 10) AL(10);
    else if (previous < 15) AL(15);
    else eax = (previous % 5u + 1u) * 5u;
    threshold = (u32)(s32)(s8)(u8)eax;
    current = (u8)(previous + count);
    B(object + 4) = current;
    if ((s32)(u32)current < (s32)threshold) goto no_counter_event;
    if ((u8)eax == 5) compare_date = l + 0x14;
    else if ((u8)eax == 10) compare_date = l + 4;
    else {
        if ((s8)(u8)eax <= 10) goto no_counter_event;
        goto counter_event;
    }
    year = W(compare_date + 2);
    AX(year);
    other_year = W(l + 0xe);
    if (year > other_year) goto counter_event;
    if (year < other_year) goto no_counter_event;
    day = W(compare_date);
    eax = 0;
    eax = (s16)day > (s16)W(l + 0xc);
    if (eax == 0) goto no_counter_event;
counter_event:
    B(l + 3) = 1;
    count = (u8)((s32)threshold / 5);
    /* The magic signed division leaves EAX as the quotient's sign bit. */
    eax = 0;
    *(volatile u8 *)&payload_arg = current;
    goto additional_event;
no_counter_event:
    count = B(l + 2);
additional_event:
    AL(B(input + 0x2d));
    if ((u8)eax != 0) {
        AL(B(input + 0x2c));
        if ((u8)eax == 0) AL(B(input + 0x2e));
        else if ((u8)eax == 1) AL(B(input + 0x2f));
        else AL(0xff);
        ++count;
        if ((u8)eax != 0xff) {
            if ((u8)eax == 12) count = (u8)(count + 3);
            else if ((s8)(u8)eax >= 9) count = (u8)(count + 2);
        }
        AL(B(object + 5));
        B(l + 3) = 2;
        AL((u8)eax + 1u);
        *(volatile u8 *)&payload_arg = 1;
        B(object + 5) = (u8)eax;
        AL((u8)eax - 1u);
        count = (u8)(count + (u8)eax);
    }
    if (count == 0) goto done;
    cm_fn_0054a110(l + 0x30);
    *(volatile u8 *)&frame.state = 4;
    B(l + 0x2c) = count;
    W(l + 0x2e) = 0xffff;
    B(l + 0x2d) = 0;
    date = cm_fn_0054c5b0(0x00ae2c90, l + 0x24, 14);
    *(volatile u8 *)&frame.state = 5;
    cm_fn_00417c10(l + 0x30, l + 0x1c, date);
    mode = B(l + 3);
    current = B(input + 0x48);
    B(l + 0x3c) = mode;
    flags = B(l + 0x3f);
    mode = *(volatile u8 *)&payload_arg;
    flags &= 0xfd;
    W(l + 0x3d) = (u16)(s16)(s8)mode;
    key = D(input);
    flags = (u8)((flags & 0xfb) | (current == 0 ? 4 : 0));
    *(volatile u8 *)&frame.state = 4;
    owner = D(0x00ae28f0);
    B(l + 0x38) = 9; B(l + 0x39) = 0xff;
    B(l + 0x3a) = 0xff; B(l + 0x3b) = 0xff;
    B(l + 0x3f) = flags;
    cm_fn_00556940(owner, key, l + 0x2c);
    key = D(input);
    base = D(0x00ae23bc);
    person = base + key * 0x6eu;
    (void)D(0x00ae28f0);
    eax = cm_fn_00557280(person, l + 0x2c, payload);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
