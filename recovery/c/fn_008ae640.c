#include "cmre.h"

/* Thiscall, ret 8 (two stack arguments). Object (ECX): 0x00 pointer to the
   socket-handle array, 0x04 signed 16-bit slot count, 0x4b6 started flag,
   0x328 WSAStartup data buffer. The selected slot index lives in BX; the
   return value is EAX as left by the last call with the low word replaced by
   BX on the success path (008ae816 "mov ax, bx").
   Frame (local base S, from "sub esp, 0x1c" at 008ae640): S+0 dword option
   value (1, then 0x3a98), S+4 linger record, S+8 non-blocking argument,
   S+0xc sockaddr_in (family at +0, port at +2, address at +4). The frame is
   established with three saved registers plus pushes, so the incoming stack
   arguments sit at [esp+0x2c] (arg1) and [esp+0x30] (arg2) once the calls
   return to the prologue stack depth of 008ae720/008ae724. The Winsock
   imports are stdcall (callee pops their arguments). */

extern u32 CM_THISCALL cm_fn_008af290(u32 self);
extern s32 CM_STDCALL cm_fn_00944c2e(u32 version, u32 wsadata);
extern s32 CM_STDCALL cm_fn_00944c3a(u32 s);
extern u16 CM_STDCALL cm_fn_00944c64(u32 hostshort);
extern s32 CM_STDCALL cm_fn_00944c7c(u32 s, s32 cmd, u32 argp);
extern s32 CM_STDCALL cm_fn_00944c82(u32 s, u32 name, s32 namelen);
extern s32 CM_STDCALL cm_fn_00944c88(u32 s, s32 level, s32 optname, u32 optval, s32 optlen);
extern u32 CM_STDCALL cm_fn_00944c8e(s32 af, s32 type, s32 protocol);

u32 CM_THISCALL cm_fn_008ae640(u32 self, u32 arg1, u32 arg2)
{
    u8 frame[0x1c];
    u32 esi;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 bx;
    u32 edi;
    u32 array;

    esi = self;
    bx = 0xffffffffu;                                    /* 008ae64d */
    edi = 1u;                                            /* 008ae648 */
    eax = *(volatile u32 *)esi;                          /* 008ae650 */
    *(volatile u32 *)(frame + 0u) = edi;                 /* 008ae652 */
    if (eax == 0u) {
        return eax | 0xffffu;                            /* 008ae658 -> 008ae832 */
    }

    eax = *(volatile u32 *)(esi + 0x4b6u);               /* 008ae65e */
    if (eax == 0u) {                                     /* 008ae666 */
        eax = (u32)cm_fn_00944c2e(0x101u, esi + 0x328u); /* 008ae66e/674 */
        if (eax != 0u) {
            return eax | 0xffffu;                        /* 008ae67b -> 008ae832 */
        }
        *(volatile u32 *)(esi + 0x4b6u) = edi;           /* 008ae683 */
        eax = cm_fn_008af290(esi);                       /* 008ae689 (ECX = esi) */
        if (eax == 0u) {
            return eax | 0xffffu;                        /* 008ae690 -> 008ae832 */
        }
    }

    ecx = *(volatile u16 *)(esi + 4u);                   /* 008ae696 (CX) */
    eax = 0u;                                            /* 008ae69a */
    if ((s16)ecx <= 0) {                                 /* 008ae69c */
        return eax | 0xffffu;                            /* 008ae69f -> 008ae832 */
    }

    /* 008ae6a5: the scan leaves on the first iteration whose top check sees a
       chosen slot (BX no longer -1), not only when the count is exhausted. */
    for (;;) {
        if ((u16)bx != 0xffffu) {
            break;                                       /* 008ae6a5/6a9 */
        }
        array = *(volatile u32 *)esi;                    /* 008ae6ab */
        if (*(volatile u32 *)(array + (u32)(s32)(s16)eax * 4u) == 0xffffffffu) {
            bx = eax;                                    /* 008ae6b6 */
        }
        eax = eax + 1u;                                  /* 008ae6b8 */
        if (!((s16)eax < (s16)ecx)) {                    /* 008ae6b9/6bc */
            if ((u16)bx == 0xffffu) {
                return eax | 0xffffu;                    /* 008ae6be/6c2 -> 008ae832 */
            }
            break;
        }
    }

    edi = (u32)(s32)(s16)bx;                             /* 008ae6ce movsx edi, bx */
    array = cm_fn_00944c8e(2, 1, 0);                     /* 008ae6c8/6cc/6d1 */
    *(volatile u32 *)((*(volatile u32 *)esi) + edi * 4u) = array; /* 008ae6d6/6d8 */
    array = *(volatile u32 *)esi;                        /* 008ae6db */
    eax = *(volatile u32 *)(array + edi * 4u);           /* 008ae6dd */
    if (eax == 0xffffffffu) {                            /* 008ae6e3 */
        *(volatile u32 *)(array + edi * 4u) = eax;       /* 008ae6e9 (ECX = array+edi*4) */
        return eax;                                      /* 008ae6ec or ax, ax */
    }

    eax = (u32)cm_fn_00944c88(eax, 0xffff, 4, (u32)frame, 4); /* 008ae6f6..705 */
    if (eax == 0xffffffffu) {                            /* 008ae70a */
        array = *(volatile u32 *)esi;                    /* 008ae70f */
        *(volatile u32 *)(array + edi * 4u) = eax;       /* 008ae711 */
        return eax;                                      /* 008ae716 or ax, ax */
    }

    eax = arg2;                                          /* 008ae720 */
    edx = arg1;                                          /* 008ae724 */
    *(volatile u16 *)(frame + 0xcu) = 2;                 /* 008ae729 sin_family */
    ecx = *(volatile u32 *)(eax + 4u);                   /* 008ae730 */
    *(volatile u32 *)(frame + 0x10u) = ecx;              /* 008ae733 sin_addr */
    edx = (u32)cm_fn_00944c64(edx);                      /* 008ae737 htons(arg1) */
    ecx = *(volatile u32 *)esi;                          /* 008ae73c */
    *(volatile u16 *)(frame + 0xeu) = (u16)edx;          /* 008ae73e sin_port */

    eax = (u32)cm_fn_00944c82(*(volatile u32 *)(ecx + edi * 4u),
                              (u32)(frame + 0xcu), 0x10); /* 008ae743..74e */
    if (eax == 0xffffffffu) {                            /* 008ae753 */
        cm_fn_00944c3a(edi);                             /* 008ae821 pushes the index */
        eax = *(volatile u32 *)esi;                      /* 008ae827 */
        *(volatile u32 *)(eax + edi * 4u) = 0xffffffffu; /* 008ae829 */
        return eax | 0xffffu;                            /* 008ae832 */
    }

    ecx = *(volatile u32 *)esi;                          /* 008ae75c */
    *(volatile u32 *)(frame + 8u) = 1u;                  /* 008ae762 */
    eax = (u32)cm_fn_00944c7c(*(volatile u32 *)(ecx + edi * 4u), (s32)0x8004667eu,
                              (u32)(frame + 8u));        /* 008ae774 ioctlsocket */

    ecx = *(volatile u32 *)esi;                          /* 008ae779 */
    *(volatile u16 *)(frame + 4u) = 0;                   /* 008ae781 */
    *(volatile u16 *)(frame + 6u) = 0;                   /* 008ae788 */
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x80,
                              (u32)(frame + 4u), 4);     /* 008ae79e SO_LINGER 0 */

    ecx = *(volatile u32 *)esi;                          /* 008ae7a3 */
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 8,
                              (u32)frame, 4);            /* 008ae7b7 */

    ecx = *(volatile u32 *)esi;                          /* 008ae7bc */
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 1,
                              (u32)frame, 4);            /* 008ae7d0 */

    ecx = *(volatile u32 *)esi;                          /* 008ae7d5 */
    *(volatile u32 *)frame = 0x3a98u;                    /* 008ae7dd */
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x1001,
                              (u32)frame, 4);            /* 008ae7f4 SO_RCVTIMEO */

    ecx = *(volatile u32 *)esi;                          /* 008ae7f9 */
    eax = (u32)cm_fn_00944c88(*(volatile u32 *)(ecx + edi * 4u), 0xffff, 0x1002,
                              (u32)frame, 4);            /* 008ae810 SO_SNDTIMEO */

    return (eax & 0xffff0000u) | (bx & 0xffffu);         /* 008ae815/816 */
}
