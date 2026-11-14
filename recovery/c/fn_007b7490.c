#include "cmre.h"
/* Preferred-image VA 0x007b7490, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CM_THISCALL: ECX receiver, one DWORD stack argument at [entry_esp+4], RET 4.
   The stack formal is the caller-owned argument home. 0x007b7561 stores the
   pointer self+0x194 into [esp+0x30] (entry_esp+4 under four pending pushes)
   and 0x007b75bd reloads that home; EBP keeps the pre-overwrite original
   pointer, so the reloaded value stays a separate object. */
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);
extern u32 CM_THISCALL cm_fn_007aefd0(u32 self, volatile u32 out1,
                                      volatile u32 out2, volatile u32 out3,
                                      volatile u32 out4, volatile u32 out5,
                                      volatile u32 out6);
extern u32 CM_CDECL cm_fn_0053d980(u32 arg_1, u32 arg_2, u32 arg_3);
extern u32 CM_STDCALL cm_fn_007f9190(u32 nation, u32 club);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_007b7490(u32 self, volatile u32 argument_home)
{
    u32 row;      /* L0 at S0+0x10: base + 110*index, or zero */
    u32 *l1;      /* L1 at S0+0x14: pointer self+0x199 (out5) */
    u32 l2;       /* L2 at S0+0x18: DWORD [row+0x39], cached pre-call */
    u32 arg;      /* EBP: original argument pointer cached at 0x007b751a */
    u32 home;     /* EAX: argument home reloaded at 0x007b75bd */
    u32 index, base, value, third, second, first;
    u8 b1, b2;
    s8 cvar;

    /* 0x007b7499: the index is a signed DWORD; -1 or less leaves row zero. */
    index = D(self + 0x180u);
    if ((s32)index <= -1) {
        row = 0u;
    } else {
        /* 0x007b74a4..0x007b74c9: the field reads keep the listing order
           (0x17c, then 0x178, then 0x174, then the shared base pointer). */
        third = D(self + 0x17cu);
        second = D(self + 0x178u);
        first = D(self + 0x174u);
        base = D(0x00ae23bcu);
        /* 0x007b74bf..0x007b74ce: base + 110u*index BYTES, 32-bit wrapping. */
        value = cm_fn_0053e990(base + index * 110u, first, second, third,
                               self + 0x184u);
        if (value == 0u) {
            row = 0u;
        } else {
            /* 0x007b74de: the index and the base are re-read after the call. */
            index = D(self + 0x180u);
            base = D(0x00ae23bcu);
            row = base + index * 110u;
        }
    }

    /* 0x007b7508: BYTE guard first, then the cached row pointer. */
    if (B(self + 0x0au) != 4u) return 0u;
    if (row == 0u) return 0u;

    /* 0x007b751a: cache the original argument pointer in EBP. */
    arg = argument_home;
    if (arg == 0u) return 0u;

    /* 0x007b7526: DWORD [row+0x39] goes to L2 before the null test. */
    l2 = D(row + 0x39u);
    if (l2 == 0u) return 0u;

    /* 0x007b7535..0x007b7542: L1 holds the pointer self+0x199. */
    l1 = (u32 *)(self + 0x199u);
    /* 0x007b7561: store the self+0x194 pointer into the caller-owned home. */
    argument_home = self + 0x194u;
    /* 0x007b7567..0x007b7569: ECX=self, six output pointers in original
       order: +0x190, +0x18c, +0x194, +0x195, +0x199, +0x19d. */
    value = cm_fn_007aefd0(self, self + 0x190u, self + 0x18cu,
                           self + 0x194u, self + 0x195u,
                           (u32)l1, self + 0x19du);

    if (value != 0u) {
        /* 0x007b7576: signed DWORD [self+0x190] compared against -1. */
        value = D(self + 0x190u);
        if ((s32)value > -1) {
            base = D(0x00ae23b4u);
            /* 0x007b7583..0x007b758d: base + 581u*value + 0x53, wrapping. */
            if (D(base + value * 581u + 0x53u) == arg) return 1u;
        }
        /* 0x007b75a2: signed DWORD [self+0x18c] compared against -1. */
        value = D(self + 0x18cu);
        if ((s32)value > -1 && D(arg) == value) return 1u;
        /* 0x007b75bd: reload the home; EBP keeps the original pointer. */
        home = argument_home;
        /* 0x007b75c1: BYTE load through the reloaded home pointer. */
        cvar = (s8)B(home);
        /* 0x007b75c3: signed BYTE comparison against 0xff. */
        if ((s32)cvar > -1 && B(arg + 0x76u) == (u8)cvar) return 1u;
        /* 0x007b75db: DWORD [L1] then the same byte, in listing order. */
        if (D((u32)l1) != 0u && B(arg + 0x76u) == (u8)cvar) return 1u;
        return 0u;
    }

    /* 0x007b7600: L2 is the DWORD [row+0x39] cached before the call. */
    value = D(l2 + 0x53u);
    /* 0x007b7609..0x007b7611: two BYTE equality guards against 2. */
    if (B(value + 0x7fu) == 2u && B(arg + 0x7fu) == 2u) return 1u;
    /* 0x007b7622..0x007b762a: BYTE equalities in listing order. */
    b1 = B(value + 0x76u);
    b2 = B(arg + 0x76u);
    if (b1 == b2) return 1u;
    /* 0x007b763b..0x007b7643: cdecl (row, original pointer, 1). */
    value = cm_fn_0053d980(row, arg, 1u);
    if (value == 0u) return 1u;
    /* 0x007b765e: volatile dead DWORD read before the RET 8 boundary. */
    (void)D(0x00b74448u);
    /* 0x007b7664..0x007b7666: stdcall(arg, L2); ECX is not a receiver. */
    value = cm_fn_007f9190(arg, l2);
    return (value != 0u) ? 1u : 0u;
}
