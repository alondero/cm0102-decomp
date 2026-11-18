#include "cmre.h"
/* Preferred-image VA 0x004ad030, image base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Shutdown contract, from the original instructions:
     004ad136 mov [0xb67a34], edi   clear the error global
     004ad13c call 005cc7d0         shutdown step 1
     004ad141 call 005a8f60         shutdown step 2
     004ad146 push -1               exit status
     004ad148 call 00945934         non-returning exit boundary
     004ad14d shared continuation
   There is no `add esp,4` between 004ad148 and 004ad14d, so the caller never
   balances 004ad146 push -1.  The boundary itself is cdecl and returns only its
   own return address: 00945934 push 0 / push 0 / push [esp+0xc] / call 00945956 /
   00945941 add esp,0xc / 00945944 ret.  It does not remove the caller's -1.
   The reachable path 00945934 -> 00945956(code,0,0) runs to
   009459e7 call dword ptr [0x967164] (ExitProcess), so the exit boundary never
   returns and 004ad14d is unreachable from 004ad148.  Declaring the boundary
   noreturn keeps the plain-C call from emitting a synthetic `add esp,4` and
   falling through with a repaired stack that the original does not have.  The
   shared continuation is reached only through the balancing guards at 004ad0ba
   and 004ad0de, where esp is unchanged and 004ad169 ([esp+0x214]) reads the
   second stack argument.  Binary CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *, const char *, int);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_CDECL cm_fn_00945934(u32) __attribute__((noreturn));

u32 CM_THISCALL cm_fn_004ad030(u32 self, u32 *arg, u32 value)
{
    u8 scratch[512];
    u32 n, p, i, v;
    if (!arg) {
        cm_fn_00944cff(0x99ca94, 0, 0, scratch + 256, scratch);
        p = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, p, scratch + 256, scratch, 0x1902);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        CM_GLOBAL(u32, 0xb67a34) = 0;
        return 0;
    }
    n = *(volatile u32 *)(self + 0x1d);
    if (*(volatile u32 *)(self + 0x21) == n) {
        p = *(volatile u32 *)(self + 0x19);
        n += 0x16;
        *(volatile u32 *)(self + 0x1d) = n;
        p = (u32)cm_fn_00945667(p, n * 45);
        *(volatile u32 *)(self + 0x19) = p;
        if (!p) {
            cm_fn_00944cff(0x99ca94, 0, 0, scratch, scratch + 256);
            p = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0, 0x9870e8, p, scratch, scratch + 256, 0x1910);
            cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
            CM_GLOBAL(u32, 0xb67a34) = 0;
            cm_fn_005cc7d0();
            cm_fn_005a8f60();
            cm_fn_00945934(0xffffffff);
        }
    }
    n = *(volatile u32 *)(self + 0x21);
    p = *(volatile u32 *)(self + 0x19) + n * 45;
    for (i = 0; i < 11; ++i) *(volatile u32 *)(p + i * 4) = 0;
    *(volatile u8 *)(p + 44) = 0;
    v = *(volatile u32 *)arg;
    *(volatile u32 *)p = v;
    *(volatile u32 *)(p + 4) = value;
    *(volatile u8 *)(p + 0x1c) = 0xff;
    n = *(volatile u32 *)(self + 0x21);
    *(volatile u8 *)(self + 0x25) = 0xfe;
    *(volatile u32 *)(self + 0x21) = n + 1;
    return p;
}
