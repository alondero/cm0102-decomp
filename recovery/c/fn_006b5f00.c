#include "cmre.h"
/* Preferred-image VA 0x006b5f00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both iterations deliberately reload the same club field, self+0x20. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_004a4850(u32, u32);
extern void CM_STDCALL cm_fn_00924050(u32, s8);
extern void CM_THISCALL cm_fn_005ab510(u32, u32);
extern u8 CM_THISCALL cm_fn_005aa680(u32, u32, u32);
extern void CM_CDECL cm_fn_006b6cc0(u32, u32);
extern void CM_CDECL cm_fn_006b74a0(u32);
extern void CM_THISCALL cm_fn_004ca6d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0055ca00(u32, u32, u32);
extern void CM_THISCALL cm_fn_0076b600(u32, u32, u32);
extern void CM_THISCALL cm_fn_0062c080(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007c0d70(u32, u32, u32);
extern void CM_THISCALL cm_fn_008c39f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00444930(u32, u32, u32);
extern void CM_THISCALL cm_fn_00868830(u32, u32, u32);
extern void CM_THISCALL cm_fn_004134c0(u32, u32, u32);
extern void CM_THISCALL cm_fn_006951e0(u32, u32, u32);
extern void CM_THISCALL cm_fn_0059a5d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007f8c00(u32, u32, u32);
extern void CM_CDECL cm_fn_006b79a0(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
typedef void (CM_THISCALL *callback)(u32, u32, u32);

u32 CM_STDCALL cm_fn_006b5f00(u32 report)
{
    u8 scratch[0x258];
    u32 work = (u32)scratch;
    u32 source, destination, base, packet, club, eax, ecx, edx, table, target;
    u32 i, index, receiver, left, right, flag;
    u16 flags;
    char *version;
    B(work) = 0;
    B(work + 1) = 0;
    if (!report) {
        cm_fn_00944cff(0x00a15708, 0, 0, scratch + 0x158, scratch + 0x58);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x158, scratch + 0x58, 0x73f);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    destination = D(report);
    source = report + 4;
    for (i = 0; i < 19; ++i) {
        eax = D(source);
        D(destination) = eax;
        source += 4;
        destination += 4;
    }
    flags = W(source);
    W(destination) = flags;
    eax = B(source + 2);
    B(destination + 2) = (u8)eax;
    base = D(report);
    packet = D(report + 0x5f);
    eax = D(base + 0x14);
    if (eax) {
        for (i = 0; i < 2; ++i) {
            club = D(base + 0x20);
            if (!club) continue;
            if (!cm_fn_00600a50(club, 0, 1, 0, 0)) continue;
            eax = D(club + 0x57);
            if (!eax) continue;
            ecx = D(eax);
            eax = D(base);
            if (ecx != eax) continue;
            edx = D(base + 0x14);
            eax = D(edx);
            table = D(0x00adadfc);
            eax = D(table + eax * 4u);
            if (!eax) continue;
            if (!D(eax + 8)) continue;
            ecx = D(table + ecx * 4u);
            receiver = D(ecx + 8);
            eax = cm_fn_004a4850(receiver, club);
            if (!eax) continue;
            ecx = D(eax + 0x2d);
            if ((s32)ecx > 1) B(work + i) = (u8)ecx;
        }
        if (B(report + 0x47) == 0xfd) {
            source = report + 4;
            destination = work + 8;
            for (i = 0; i < 19; ++i) {
                eax = D(source);
                D(destination) = eax;
                source += 4;
                destination += 4;
            }
            flags = W(source);
            W(destination) = flags;
            receiver = D(0x00de2f74);
            eax = B(source + 2);
            B(destination + 2) = (u8)eax;
            B(work + 0x4b) = 0xff;
            B(work + 0x4c) = 0xff;
            B(work + 0x4d) = 0xff;
            B(work + 0x4e) = 0xff;
            B(work + 0x51) = 0xff;
            B(work + 0x52) = 0xff;
            B(work + 0x50) = 0xff;
            B(work + 0x4f) = 0xff;
            flags = W(base + 0x4d);
            W(work + 0x55) = W(work + 0x55) & 0xcfffu;
            (void)receiver;
            cm_fn_00924050(report + 4, (s8)((flags >> 12) & 3));
            receiver = D(0x00ae2a58);
            cm_fn_005ab510(receiver, work + 8);
            receiver = D(0x00ae2a58);
            cm_fn_005aa680(receiver, work + 8, 1);
        } else {
            index = D(base);
            table = D(0x00adadfc);
            receiver = D(table + index * 4u);
            table = D(receiver);
            target = D(table + 4);
            ((callback)target)(receiver, base, packet);
        }
    }
    D(0x00a801bc) = 1;
    cm_fn_006b6cc0(base, packet);
    cm_fn_006b74a0(base);
    receiver = D(0x00ae1050);
    cm_fn_004ca6d0(receiver, base, packet);
    receiver = D(0x00ae28f0);
    cm_fn_0055ca00(receiver, base, packet);
    if (B(report + 0x47) != 0xfd) {
        receiver = D(0x00b74318);
        cm_fn_0076b600(receiver, base, packet);
    }
    receiver = D(0x00b63d28);
    cm_fn_0062c080(receiver, base, packet);
    if (B(report + 0x47) == 0xfd) return 1;
    eax = D(base);
    ecx = D(0x009cf684);
    if (eax != ecx) {
        receiver = D(0x00b74434);
        table = D(receiver);
        target = D(table + 4);
        ((callback)target)(receiver, base, packet);
    }
    receiver = D(0x00b74350);
    cm_fn_007c0d70(receiver, base, packet);
    receiver = D(0x00de1f54);
    cm_fn_008c39f0(receiver, base, packet);
    receiver = D(0x00ad9dac);
    cm_fn_00444930(receiver, base, packet);
    receiver = D(0x00dd7f00);
    cm_fn_00868830(receiver, base, packet);
    receiver = D(0x00ad9c60);
    cm_fn_004134c0(receiver, base, packet);
    receiver = D(0x00b67a44);
    cm_fn_006951e0(receiver, base, packet);
    receiver = D(0x00ae2a38);
    cm_fn_0059a5d0(receiver, base, packet);
    receiver = D(0x00b74448);
    cm_fn_007f8c00(receiver, base, packet);
    flag = D(work);
    right = D(base + 0x20);
    left = D(base + 0x1c);
    cm_fn_006b79a0(left, right, base, flag, packet);
    flag = D(work + 1);
    left = D(base + 0x1c);
    right = D(base + 0x20);
    cm_fn_006b79a0(right, left, base, flag, packet);
    return 1;
}
