#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry ECX saved at 006b5cfc, ret 4 at 006b5efc.
   006b5dab/5db0 and 006b5d8e/5dce supply ECX and one mode push.
   006b5e11..5e34: five iterator pushes, no caller cleanup.
   006b5e6d..5e99: three pushes survive 0090d130 for the formatter.
   006b5ea6..5eda explicitly load each thiscall receiver. */
extern u32 CM_THISCALL cm_fn_006b98c0(u32 self, s8 mode);
extern void CM_THISCALL cm_fn_006b97a0(void *self);
extern u32 CM_THISCALL cm_fn_007293f0(u32 self, u32 match, u32 report);
extern u32 CM_THISCALL cm_fn_0090d6b0(u32 self, u32 record);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *p);
extern void CM_STDCALL cm_fn_009463af(u32 p, u32 size, u32 count,
                                     u32 ctor, u32 dtor);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_THISCALL cm_fn_006b5ce0(u32 self, u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[512];
    u32 counter, flags, index, record, match, report, result = 0;
    u8 status;
    char *text;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d25e;
    CM_FS(u32, 0) = (u32)&frame;
    counter = D(0x00b67a54) + 1u;
    D(0x00b67a54) = counter;
    if ((s32)counter > 100) {
        counter = 0;
        D(0x00b67a54) = 0;
    }
    if (!mode) {
        flags = D(D(0x00ae3138) + 4u);
        if ((flags & 0x40000u) && (s32)counter % 3 > 0)
            goto done;
        if ((flags & 0x80000u) && (s32)counter % 6 > 0)
            goto done;
    }
    index = D(self + 0x2f);
    if ((s32)index < 0) goto done;
    record = index * 105u + D(self + 0x19);
    match = D(record + 0x57);
    if (!match) goto done;
    if (mode) {
        status = B(record + 0x67);
        while (status != 2 && status != 8) {
            match = D(record + 0x57);
            status = (u8)cm_fn_006b98c0(match, -1);
            B(record + 0x67) = status;
            if (status == 2 || status == 8)
                D(self + 0x25) = D(self + 0x25) + 1u;
            status = B(record + 0x67);
        }
    } else {
        status = (u8)cm_fn_006b98c0(match, 1);
        B(record + 0x67) = status;
        if (status == 2 || status == 8)
            D(self + 0x25) = D(self + 0x25) + 1u;
    }
    status = B(record + 0x67);
    if (status != 2 && status != 8) goto done;
    report = (u32)cm_fn_00944cf1(0xd7a);
    frame.state = 0;
    if (report)
        cm_fn_009463af(report + 0x11f, 0x625, 2, 0x00453c30, 0x00453c10);
    frame.state = 0xffffffffu;
    D(record + 0x5f) = report;
    if (!report) {
        cm_fn_00944cff(0x00a15708, 0, 0, scratch + 256, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      scratch + 256, scratch, 0x713);
        cm_fn_005e8290((const char *)0x009870e0,
                      (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    match = D(record + 0x57);
    cm_fn_007293f0(D(0x00b6b7c0), match, report);
    match = D(record + 0x57);
    if (match) {
        cm_fn_006b97a0((void *)match);
        cm_fn_00944c94((void *)match);
    }
    D(record + 0x57) = 0;
    cm_fn_0090d6b0(D(0x00de2f70), record + 4);
    result = 1;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
