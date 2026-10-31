#include "cmre.h"
/* WIP: fails the semantic gate (cov=0.8554): memory mismatches on heap pages 0x3000000/0x3005000/0x3038000 (cases 8, 94, 155). Worker was stopped mid-diagnosis; cause unresolved. */

/* Thiscall with two stack arguments (ret 8). Object fields: 0x00 pointer to an
   int array of socket handles, 0x04 signed count (word), 0x4b6 started flag,
   0x328 WSA data buffer. The chosen slot index is kept in bx; the return value
   is EAX as left by the last step, with the low word replaced by bx on success.
   Frame (local base S): S+0 dword flag (1, then 0x3a98 for the buffer options),
   S+4 linger record, S+8 non-blocking argument, S+0xc sockaddr_in
   (family, port at +2, address at +4). The Winsock imports are stdcall. */

extern u32 CM_FASTCALL cm_fn_008af290(u32 self);
extern s32 CM_STDCALL cm_fn_00944c2e(u32 version, u32 wsadata);
extern s32 CM_STDCALL cm_fn_00944c3a(u32 s);
extern u16 CM_STDCALL cm_fn_00944c64(u32 hostshort);
extern s32 CM_STDCALL cm_fn_00944c7c(u32 s, s32 cmd, u32 argp);
extern s32 CM_STDCALL cm_fn_00944c82(u32 s, u32 name, s32 namelen);
extern s32 CM_STDCALL cm_fn_00944c88(u32 s, s32 level, s32 optname, u32 optval, s32 optlen);
extern u32 CM_STDCALL cm_fn_00944c8e(s32 af, s32 type, s32 protocol);

u32 CM_THISCALL cm_fn_008ae640(u32 self, u32 arg1, u32 arg2)
{
    u8 frame[0x20];
    u32 esi;
    u32 eax;
    u32 bx;
    u32 edi;
    u32 array;
    u32 sock;
    u32 cx;
    u32 i;
    u32 ecx;

    esi = self;
    bx = 0xffffffffu;
    edi = 1u;
    *(volatile u32 *)(frame + 0u) = 1u;

    eax = *(volatile u32 *)esi;
    if (eax == 0u) {
        return eax | 0xffffu;
    }

    eax = *(volatile u32 *)(esi + 0x4b6u);
    if (eax == 0u) {
        eax = (u32)cm_fn_00944c2e(0x101u, esi + 0x328u);
        if (eax != 0u) {
            return eax | 0xffffu;
        }
        *(volatile u32 *)(esi + 0x4b6u) = edi;
        eax = cm_fn_008af290(esi);
        if (eax == 0u) {
            return eax | 0xffffu;
        }
    }

    cx = *(volatile u16 *)(esi + 4u);
    eax = 0u;
    if ((s16)cx <= 0) {
        return eax | 0xffffu;
    }

    if ((u16)bx == 0xffffu) {
        i = 0u;
        do {
            array = *(volatile u32 *)esi;
            if (*(volatile u32 *)(array + (u32)(s32)(s16)eax * 4u) == 0xffffffffu) {
                bx = eax;
            }
            eax = eax + 1u;
        } while ((s16)eax < (s16)cx);
        if ((u16)bx == 0xffffu) {
            return eax | 0xffffu;
        }
        (void)i;
    }

    edi = (u32)(s32)(s16)bx;
    sock = cm_fn_00944c8e(2, 1, 0);
    eax = sock;
    array = *(volatile u32 *)esi;
    *(volatile u32 *)(array + edi * 4u) = eax;
    array = *(volatile u32 *)esi;
    eax = *(volatile u32 *)(array + edi * 4u);
    if (eax == 0xffffffffu) {
        *(volatile u32 *)(array + edi * 4u) = eax;
        return eax;
    }

    eax = (u32)cm_fn_00944c88(eax, 0xffff, 4, (u32)frame, 4);
    if (eax == 0xffffffffu) {
        array = *(volatile u32 *)esi;
        *(volatile u32 *)(array + edi * 4u) = eax;
        return eax;
    }

    eax = arg2;
    *(volatile u16 *)(frame + 0xcu) = 2;
    ecx = *(volatile u32 *)(eax + 4u);
    *(volatile u32 *)(frame + 0x10u) = ecx;
    {
        u16 port = cm_fn_00944c64(arg1);
        ecx = *(volatile u32 *)esi;
        *(volatile u16 *)(frame + 0xeu) = port;
        eax = (u32)port;
        (void)ecx;
    }
    ecx = *(volatile u32 *)esi;
    eax = (u32)cm_fn_00944c82(*(volatile u32 *)(ecx + edi * 4u),
                              (u32)(frame + 0xcu), 0x10);
    if (eax == 0xffffffffu) {
        cm_fn_00944c3a(edi);
        eax = *(volatile u32 *)esi;
        *(volatile u32 *)(eax + edi * 4u) = 0xffffffffu;
        return eax | 0xffffu;
    }

    ecx = *(volatile u32 *)esi;
    *(volatile u32 *)(frame + 8u) = 1u;
    eax = (u32)cm_fn_00944c7c(*(volatile u32 *)(ecx + edi * 4u), (s32)0x8004667eu,
                              (u32)(frame + 8u));

    ecx = *(volatile u32 *)esi;
    *(volatile u16 *)(frame + 4u) = 0;
    *(volatile u16 *)(frame + 6u) = 0;
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x80,
                              (u32)(frame + 4u), 4);

    ecx = *(volatile u32 *)esi;
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 8,
                              (u32)frame, 4);

    ecx = *(volatile u32 *)esi;
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 1,
                              (u32)frame, 4);

    ecx = *(volatile u32 *)esi;
    *(volatile u32 *)frame = 0x3a98u;
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x1001,
                              (u32)frame, 4);

    ecx = *(volatile u32 *)esi;
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x1002,
                              (u32)frame, 4);

    return (eax & 0xffff0000u) | (bx & 0xffffu);
}
