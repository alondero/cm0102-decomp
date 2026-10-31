#include "cmre.h"

/* Preferred-image VA 005c71f0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch410: count L0, saved self L4, date output L8, diagnostics L10/L110
   and L210/L310. Capture EDX:EAX across original boundaries to forward the
   real EDX residue into 68d0d0; the root returns only its EAX result. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_STDCALL cm_fn_009463af(u32, u32, u32, u32, u32);
extern void CM_STDCALL cm_fn_009462bb(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *, u32, const u8 *, s16);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_005c71f0(u32 self, volatile u32 first_arg,
                              volatile u32 second_arg, volatile s8 count_arg,
                              volatile u32 dates_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 l = (u32)scratch, object, eax, edx, first, second, dates, cursor, result;
    u32 filename, extension, line, version;
    s8 count;
    unsigned long long pair;
    frame.state = 0xffffffffu; frame.handler = 0x0095a18c;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D(l + 4) = self;
    pair = cm_fn_00944cf1(0x107);
    object = (u32)pair;
    D(l) = object;
    frame.state = 0;
    if (object != 0) {
        pair = cm_fn_009463af(object + 9, 0x19, 10, 0x00645800, 0x005a53c0);
    }
    frame.state = 0xffffffffu;
    if (object == 0) {
        filename = l + 0x110; extension = l + 0x10; line = 0xed0;
        goto allocation_error;
    }
    count = count_arg;
    second = second_arg;
    first = first_arg;
    edx = second;
    D(object) = first;
    D(object + 4) = second;
    B(object + 8) = (u8)count;
    D(object + 0x103) = 0;
    if (count > 0) {
        dates = dates_arg;
        cursor = object + 0x11;
        D(l) = (u32)(s32)count;
        do {
            pair = cm_fn_00417c10(cursor - 8, l + 8, dates);
            edx = (u32)(pair >> 32);
            eax = D(l);
            D(cursor) = 0;
            D(cursor + 4) = first;
            D(cursor + 8) = 0;
            D(cursor + 0xc) = first;
            B(cursor + 0x10) = 1;
            dates += 8;
            cursor += 0x19;
            --eax;
            D(l) = eax;
        } while (eax != 0);
        self = D(l + 4);
    }
    eax = D(self + 0x34cc);
    if (eax == 0) {
        pair = cm_fn_00944cf1(0xe);
        eax = (u32)pair;
        edx = (u32)(pair >> 32);
        D(l + 4) = eax;
        frame.state = 1;
        if (eax != 0) eax = cm_fn_0068d020(eax);
        frame.state = 0xffffffffu;
        D(self + 0x34cc) = eax;
        if (eax == 0) {
            filename = l + 0x10; extension = l + 0x110; line = 0xeeb;
            goto append_error;
        }
    }
    eax = D(self + 0x34cc);
    result = (u32)cm_fn_0068d0d0((volatile u8 *)eax, edx, (const u8 *)object, 0x107);
    if (result == 0) {
        filename = l + 0x310; extension = l + 0x210; line = 0xef5;
        goto append_error;
    }
    cm_fn_009462bb(object + 9, 0x19, 10, 0x005a53c0);
    cm_fn_00944c94((void *)object);
    eax = result;
    goto done;

allocation_error:
    cm_fn_00944cff(0x009cd0a0, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    eax = 0;
    goto done;
append_error:
    cm_fn_00944cff(0x009cd0a0, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    cm_fn_009462bb(object + 9, 0x19, 10, 0x005a53c0);
    cm_fn_00944c94((void *)object);
    eax = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
