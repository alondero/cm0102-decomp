#include "cmre.h"
/* Preferred-image VA 0x006b5a70, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_006b66a0(u32);
extern u32 CM_THISCALL cm_fn_006b7e50(u32);
extern u32 CM_THISCALL cm_fn_006b7f20(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_0090d2f0(u32, u32, u32, u8);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_FASTCALL cm_fn_006b5a70(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x408];
    u32 prev, index, count, offset, record, base, eax, receiver, club;
    u32 result = 0;
    u8 flag;
    char *version;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d23e;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    index = D(self + 0x2f);
    if (index == 0xfffffffeu) goto done;
    if ((s32)index > -1) {
        base = D(self + 0x19);
        if (D(base + index * 0x69u + 0x57)) {
            cm_fn_00944cff(0x00a15708, 0, 0, scratch + 0x208, scratch + 8);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x208, scratch + 8, 0x68d);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            goto done;
        }
    }
restart:
    index = D(self + 0x2f);
    count = D(self + 0x21);
    ++index;
    if ((s32)index >= (s32)count) goto exhausted;
    offset = index * 0x69u;
    D((u32)scratch) = offset;
    for (;;) {
        record = D(self + 0x19) + offset;
        if (B(record + 0x47) == 0xfe) goto next;
        if (B(self + 0x33) == 1) {
            eax = D(record + 0x63);
            if (!D(eax + 0x10)) goto next;
        }
        flag = B(record + 0x44);
        club = D(record + 0x20);
        if (cm_fn_00600a50(club, 0, flag, 0, 0)) goto next;
        flag = B(record + 0x45);
        club = D(record + 0x24);
        if (cm_fn_00600a50(club, 0, flag, 0, 0)) goto next;
        if (B(record + 0x47) != 0xff) goto next;
        if (D(record + 0x57)) goto next;
        if (D(record + 0x53)) goto next;
        receiver = D(0x00de2f70);
        cm_fn_0090d2f0(receiver, record + 4, 0xfffffffdu, 0);
        eax = (u32)cm_fn_00944cf1(0xf63b);
        D((u32)scratch + 4) = eax;
        frame.state = 0;
        if (eax) eax = cm_fn_006b7e50(eax);
        frame.state = 0xffffffffu;
        D(record + 0x57) = eax;
        if (eax) {
            base = record + 4;
            eax = cm_fn_006b66a0(base);
            receiver = D(record + 0x57);
            cm_fn_006b7f20(receiver, base, eax, 0, 0);
            D(self + 0x2f) = index;
            result = 1;
            goto done;
        }
        cm_fn_00944cff(0x00a15708, 0, 0, scratch + 0x308, scratch + 0x108);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x308, scratch + 0x108, 0x6b2);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
next:
        offset = D((u32)scratch);
        count = D(self + 0x21);
        ++index;
        offset += 0x69;
        D((u32)scratch) = offset;
        if ((s32)index >= (s32)count) break;
    }
exhausted:
    if (B(self + 0x33) == 1) {
        B(self + 0x33) = 2;
        D(self + 0x2f) = 0xffffffffu;
        goto restart;
    }
    D(self + 0x2f) = 0xfffffffeu;
done:
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return result;
}
