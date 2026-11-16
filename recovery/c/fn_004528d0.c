#include "cmre.h"
extern u32 CM_CDECL cm_fn_00452fe0(u32);
extern void CM_CDECL cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_CDECL cm_fn_00945934(u32);

u32 CM_THISCALL cm_fn_004528d0(u32 self, u32 *index, u32 create)
{
    u8 scratch[512];
    u32 entry, n, p, result;
    char *text;
    if (!index) {
        cm_fn_00944cff(0x98b6a0, 0, 0, scratch + 256, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text, scratch + 256, scratch, 0x16b5);
        cm_fn_005e8290((char *)0x9870e0, (char *)0xae24d0, 0);
        CM_GLOBAL(volatile u32, 0xb67a34) = 0;
        return 0;
    }
    n = *(volatile u32 *)index;
    p = *(volatile u32 *)(self + 0x1c);
    entry = p + n * 106;
    n = *(volatile u32 *)(entry + 4);
    if (n == 0xffffffff && create) {
        n = *(volatile u32 *)(self + 0x14);
        p = *(volatile u32 *)(self + 0x18);
        if (p == n) {
            n += 50;
            *(volatile u32 *)(self + 0x14) = n;
            p = *(volatile u32 *)(self + 0x10);
            result = (u32)cm_fn_00945667(p, n * 907);
            *(volatile u32 *)(self + 0x10) = result;
            if (!result) {
                cm_fn_00944cff(0x98b6a0, 0, 0, scratch, scratch + 256);
                text = cm_fn_0090d130();
                cm_fn_00944c9f(0xae24d0, 0x9870e8, text, scratch, scratch + 256, 0x16c9);
                cm_fn_005e8290((char *)0x9870e0, (char *)0xae24d0, 0);
                CM_GLOBAL(volatile u32, 0xb67a34) = 0;
                cm_fn_005cc7d0();
                cm_fn_005a8f60();
                /* VA 00452a27 leaves the pushed word live if 00945934 returns;
                   ordinary C cannot express the resulting shifted epilogue. */
                cm_fn_00945934(0xffffffff);
            }
        }
        n = *(volatile u32 *)(self + 0x18);
        *(volatile u32 *)(entry + 4) = n;
        n = *(volatile u32 *)(self + 0x18);
        *(volatile u32 *)(self + 0x18) = n + 1;
        n = *(volatile u32 *)(entry + 4);
        p = *(volatile u32 *)(self + 0x10);
        cm_fn_00452fe0(p + n * 907);
    }
    n = *(volatile u32 *)(entry + 4);
    if ((s32)n <= -1) return 0;
    p = *(volatile u32 *)(self + 0x10);
    return p + n * 907;
}
