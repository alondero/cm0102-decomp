#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0055f270(u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_0077ac00(u32, u32, u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_0077ac30(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff990(u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

/* All addresses below are preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   regparm(3) names entry EAX/EDX/ECX without adding stack arguments.
   007fc69c/007fc6a2 -> 007fc7b2 preserves EAX; 007fc6ae supplies
   EAX on the second early exit (0055f270 writes EAX at 0055f270).
   007fc78a returns raw EDX:EAX: 007ff990 loads EAX from [esp+8],
   captures ECX at 007ff9a1, zeros EDX at 007ff9b4 for argument -1,
   and returns with RET 8 (e.g. 007ffa03/007ffb67/007ffcc2).
   This is register transport, not a semantic 64-bit result.
   007fc78f replaces only DX before PUSH EDX at 007fc798.
   0077ac00 replaces AX at 0077ac00, can return that EAX at
   0077ac21, or consumes ECX and partial EDX at 0077ac13/0077ac1b;
   RET 4 at 0077ac21. Its boundary therefore carries EAX/EDX/ECX.
   0077ac30 similarly needs incoming EAX: 0077ae6d writes only AX
   before PUSH EAX at 0077ae90. ECX is saved at 0077ac45; its three
   stack arguments and RET 0xc are visible at 007fc763-007fc766 and
   0077af17. Keep its full EAX result (0077af0d), not merely AL. */
u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_007fc690(u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u8 scratch[0x200];
    u32 i;
    u32 eax = incoming_eax;
    (void)incoming_edx;
    if (*(volatile u32 *)(self + 0x3840) == 0)
        return eax;
    eax = cm_fn_0055f270(self + 0x132ea5);
    if (*(volatile u32 *)(self + 0x3830) == 0)
        return eax;
    *(volatile u32 *)(self + 0x3830) = 0;
    *(volatile u32 *)(self + 0x3802) = 0;
    if (*(volatile u32 *)(self + 0x3806) == 0) {
        char *text;
        cm_fn_00944cff(0x9c3734, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text,
                       scratch + 0x100, scratch, 0x85);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        CM_GLOBAL(volatile u32, 0xb67a34) = 0;
    } else {
        u32 p = *(volatile u32 *)(self + 0x37fe);
        *(volatile u8 *)p = 4;
        *(volatile u32 *)(self + 0x3802) =
            *(volatile u32 *)(self + 0x3802) + 1;
    }
    {
        u32 len = *(volatile u32 *)(self + 0x3802);
        u32 p = *(volatile u32 *)(self + 0x37fe);
        u32 net = *(volatile u32 *)(self + 0x3840);
        eax = cm_fn_0077ac30(p, len, net, p, len, 0);
    }
    for (i = 0; i < 15; ++i) {
        u32 counter = self + 0x380c + i * 2;
        u32 entry = self + 0xad4 + i * 0x300;
        while (*(volatile s16 *)counter > 0) {
            u32 p = *(volatile u32 *)(entry + 2);
            u32 id;
            u32 net;
            unsigned long long regs = cm_fn_007ff990(self, p, 0xffffffff);
            u32 edx = (u32)(regs >> 32);
            eax = (u32)regs;
            id = (edx & 0xffff0000u) | *(volatile u16 *)entry;
            net = *(volatile u32 *)(self + 0x3840);
            eax = cm_fn_0077ac00(eax, id, net, id);
        }
    }
    return eax;
}
