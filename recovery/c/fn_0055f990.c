#include "cmre.h"
/* Preferred-image VA 0x0055f990, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Incoming EAX is live on the full-buffer early return. RET0x48 pops eighteen
   stack words; EAX/EDX/ECX precede them as physical register positions. */
extern u32 CM_THISCALL cm_fn_005ee150(u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3)))
    cm_fn_00403200(u32, u32, u32, volatile u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(
    u32 incoming_eax, u32 incoming_edx, u32 self,
    volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
    volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
    volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
    volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
    volatile u32 a16, volatile u32 a17)
{
    u32 eax = incoming_eax;
    u16 count = W(self + 0x12e9a0u), pending;
    u32 old_index, destination, i, saved15, saved17, forwarded[18];
    u32 count_at_forward, target, index_argument, scaled;
    char *version;
    u8 text[0x200];
    (void)incoming_edx;
    if ((s16)count >= 0x4af) {
        D(self + 0x12f549u) = 1;
        return eax | 0xffffu;
    }
    old_index = (u32)(s32)(s16)count;
    W(self + 0x12e9a0u) = (u16)(count + 1u);
    saved15 = a15;
    destination = self + old_index * 396u + 0xba95eu;
    for (i = 0; i != 99u; ++i)
        D(destination + i * 4u) = 0;
    saved17 = a17;
    forwarded[17] = saved17;
    forwarded[16] = a16;
    forwarded[15] = saved15;
    forwarded[14] = a14;
    forwarded[13] = a13;
    forwarded[12] = a12;
    forwarded[11] = a11;
    forwarded[10] = a10;
    forwarded[9] = a9;
    forwarded[8] = a8;
    forwarded[7] = a7;
    forwarded[6] = a6;
    count_at_forward = W(self + 0x12e9a0u);
    forwarded[5] = a5;
    forwarded[4] = a4;
    forwarded[3] = a3;
    forwarded[2] = a2;
    forwarded[1] = a1;
    forwarded[0] = a0;
    target = self + (u32)(s32)(s16)count_at_forward * 396u + 0xba7d2u;
    eax = cm_fn_005ee150(target, self + 4u, self + 0xba95eu,
        count_at_forward - 1u,
        forwarded[0], forwarded[1], forwarded[2], forwarded[3],
        forwarded[4], forwarded[5], forwarded[6], forwarded[7],
        forwarded[8], forwarded[9], forwarded[10], forwarded[11],
        forwarded[12], forwarded[13], forwarded[14], forwarded[15],
        forwarded[16], forwarded[17]);
    if ((u16)saved17 != 0xffffu) {
        index_argument = (u16)(W(self + 0x12e9a0u) - 1u);
        eax = (u32)(s32)(s16)saved17;
        scaled = eax * 3056u;
        target = self + scaled + eax + 4u;
        /* Only the low word of index_argument is consumed by00403200. */
        eax = cm_fn_00403200(eax, scaled, target, index_argument);
        return ((eax & 0xffff0000u) | W(self + 0x12e9a0u)) - 1u;
    }
    count = (u16)(W(self + 0x12e9a0u) - 1u);
    eax = (eax & 0xffff0000u) | count;
    if (count != 0xffffu) {
        pending = W(self + 0x12f4f8u);
        if ((s16)pending >= 0x4b0) {
            cm_fn_00944cff(0x009c36ec, 0, 0, text + 0x100, text);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                          text + 0x100, text, 0x2ea);
            eax = cm_fn_005e8290((const char *)0x009870e0,
                                (const char *)0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        } else {
            W(self + (u32)(s32)(s16)pending * 2u + 0x12eb96u) = count;
            W(self + 0x12f4f8u) = (u16)(W(self + 0x12f4f8u) + 1u);
        }
    }
    return ((eax & 0xffff0000u) | W(self + 0x12e9a0u)) - 1u;
}
