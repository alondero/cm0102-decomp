#include "cmre.h"
extern u32 CM_CDECL cm_fn_00945667(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_00945934(u32);

void CM_THISCALL cm_fn_007a8510(u32 self, u32 source)
{
    u8 scratch[512];
    u32 n, p, v, x;
    n = *(volatile u32 *)(self + 12);
    if (*(volatile u32 *)(self + 8) == n - 1) {
        p = cm_fn_00945667(*(volatile u32 *)(self + 4), (n * 13 + 0xcb2) * 2);
        n = *(volatile u32 *)(self + 12) + 250;
        *(volatile u32 *)(self + 4) = p;
        *(volatile u32 *)(self + 12) = n;
        if (!p) {
            char *s;
            cm_fn_00944cff(0xa6bf9c, 0, 0, scratch + 256, scratch);
            s = cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0, 0x9870e8, s, scratch + 256, scratch, 0xa53);
            cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
            CM_GLOBAL(volatile u32, 0xb67a34) = 0;
            cm_fn_005cc7d0();
            cm_fn_005a8f60();
            /* UNRESOLVED fidelity blocker, not a recovered continuation.
               CM0102 3.9.68 SHA-256:
               0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
               All addresses below are preferred-image VAs.
               007a85c7 pushes -1; 007a85c9 calls 00945934 without
               caller cleanup. The target's 00945941 add esp,12
               removes its own outgoing arguments; 00945944 is plain RET.
               If it returns, 007a85d9 therefore reads the entry return
               address, not source. At 007a867e POP ESI consumes scratch,
               and 007a8685 RET 4 consumes the displaced saved-ESI slot.
               An ordinary C call and epilogue cannot represent this.
               Neither a stdcall declaration nor noreturn is justified.
               Requires an authorized ABI boundary/continuation solution;
               a passing semantic gate does not resolve this blocker. */
            cm_fn_00945934(0xffffffff);
        }
    }
#define COPY(off, type) do { \
    n = *(volatile u32 *)(self + 8); \
    v = *(volatile type *)(source + (off)); \
    p = *(volatile u32 *)(self + 4); \
    *(volatile type *)(p + n * 26 + (off)) = (type)v; \
} while (0)
    COPY(0, u32);
    COPY(4, u32);
    COPY(0x18, u8);
    COPY(8, u16);
    COPY(10, u16);
    COPY(0x10, u16);
    COPY(0x12, u16);
    COPY(0x19, u8);
#undef COPY
    x = *(volatile u32 *)(self + 8);
    *(volatile u32 *)(self + 8) = x + 1;
}
