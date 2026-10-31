#include "cmre.h"

/* Preferred-image VA 0x006b5260, image base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The backward branch at 0x006b53f7 reaches the comparison with eight. */
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_006b7e50(u32);
extern u32 CM_THISCALL cm_fn_006b7f20(u32, u32, u8, u32, u8);
extern u32 CM_THISCALL cm_fn_006b98c0(u32, s8);
extern void CM_THISCALL cm_fn_006b97a0(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern void CM_THISCALL cm_fn_0090d6b0(u32, u32);
extern u32 CM_THISCALL cm_fn_007293f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00729ff0(u32, u32, u32, u32);
extern void CM_STDCALL cm_fn_009463af(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_STDCALL cm_fn_006b5260(u32 report)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x404];
    u32 prev, index, object, slot, allocation, match, receiver, pointer, base;
    u32 result = 0, line;
    u8 mode, count, expected;
    u8 *filename, *extension;
    char *version;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d21a;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    if (!report) {
        filename = scratch + 4;
        extension = scratch + 0x104;
        line = 0x4b8;
        goto error;
    }
    index = (u32)(s32)(s16)W(0x00b77c7a);
    object = D(report + 0x5b);
    slot = object + index * 4u + 0x45;
    if (!D(slot)) {
        D(slot) = 1;
        count = B(object + 0x106);
        ++count;
        B(object + 0x106) = count;
    }
    count = B(object + 0x106);
    expected = B(object + 0x105);
    if (count != expected) goto done;
    allocation = (u32)cm_fn_00944cf1(0xf63b);
    D((u32)scratch) = allocation;
    frame.state = 0;
    match = allocation ? cm_fn_006b7e50(allocation) : 0;
    frame.state = 0xffffffffu;
    D(report + 0x57) = match;
    if (!match) {
        filename = scratch + 0x104;
        extension = scratch + 4;
        line = 0x4d0;
        goto error;
    }
    base = report + 4;
    mode = (u8)cm_fn_006b7f20(match, base, 0, 0, 1);
    if (!mode) {
        mode = B(report + 0x67);
        while (mode != 2 && mode != 8) {
            match = D(report + 0x57);
            mode = (u8)cm_fn_006b98c0(match, -1);
            B(report + 0x67) = mode;
            if (mode == 2 || mode == 8) {
                receiver = D(0x00b67a50);
                D(receiver + 0x25) = D(receiver + 0x25) + 1u;
            }
            mode = B(report + 0x67);
        }
        allocation = (u32)cm_fn_00944cf1(0xd7a);
        D((u32)scratch) = allocation;
        frame.state = 1;
        if (allocation)
            cm_fn_009463af(allocation + 0x11f, 0x625, 2, 0x00453c30, 0x00453c10);
        frame.state = 0xffffffffu;
        D(report + 0x5f) = allocation;
        if (!allocation) {
            filename = scratch + 0x104;
            extension = scratch + 4;
            line = 0x4ea;
            goto error;
        }
        match = D(report + 0x57);
        receiver = D(0x00b6b7c0);
        cm_fn_007293f0(receiver, match, allocation);
        match = D(report + 0x57);
        if (match) {
            cm_fn_006b97a0(match);
            cm_fn_00944c94(match);
        }
        D(report + 0x57) = 0;
        receiver = D(0x00de2f70);
        cm_fn_0090d6b0(receiver, base);
        D(object + 0x119) = 1;
        goto done;
    }
    match = D(report + 0x57);
    pointer = D(match + 0x8f7a);
    if (pointer) B(pointer) = 3;
    allocation = (u32)cm_fn_00944cf1(0xd7a);
    D((u32)scratch) = allocation;
    frame.state = 2;
    if (allocation)
        cm_fn_009463af(allocation + 0x11f, 0x625, 2, 0x00453c30, 0x00453c10);
    frame.state = 0xffffffffu;
    D(report + 0x5f) = allocation;
    if (!allocation) {
        filename = scratch + 0x304;
        extension = scratch + 0x204;
        line = 0x511;
        goto error;
    }
    match = D(report + 0x57);
    receiver = D(0x00b6b7c0);
    cm_fn_00729ff0(receiver, match, allocation, 1);
    result = 1;
    goto done;
error:
    cm_fn_00944cff(0x00a15708, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return result;
}
