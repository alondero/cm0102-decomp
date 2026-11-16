#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u8 CM_THISCALL cm_fn_0062c400(u32, u32);
extern u32 CM_THISCALL cm_fn_0062c590(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008968f0(u32, u32, u32, u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u8 CM_THISCALL cm_fn_008aafd0(u32 self, u32 index, u32 mode)
{
    u8 scratch[512];
    s32 n = (s8)index;
    u32 p, v;
    if (n < 0 || n > 19) {
        cm_fn_00944cff(0xa8b94c, 0, 0, scratch + 256, scratch);
        p = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, p, scratch + 256, scratch, 0x1cf0);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        CM_GLOBAL(u32, 0xb67a34) = 0;
        return 0;
    }
    if (mode && !*(volatile u32 *)(self + 0xde5))
        return 0;
    p = *(volatile u32 *)(self + 4);
    if (p) {
        if (mode)
            p += 0x4796 + (u32)(n + (s32)*(volatile s8 *)(self + 0xdea) * 20) * 0x1be;
        else
            p = self + 0x2735 + (u32)n * 0x1be;
        if (*(volatile u8 *)(p + 0x19) == 0xfb)
            return 0;
        /* 008ab059-008ab062: x87 multiply followed by __ftol.
           Cast uses the compiler's long-double-to-long-long lowering; its
           x87 status, depth and conversion equivalence still require evidence. */
        {
            long double t = (long double)*(volatile float *)(p + 0x4d);
            t *= (long double)CM_GLOBAL(double, 0x967898);
            return (u8)(unsigned long long)(long long)t;
        }
    }
    if (*(volatile u32 *)(self + 0x10) != 0xffffffffu)
        return 0;
    if (mode)
        p = *(volatile u32 *)(self + 0xdf0 + (u32)n * 4);
    else {
        u32 a = *(volatile u32 *)(self + 8);
        u32 b = *(volatile u32 *)self;
        p = cm_fn_008968f0(CM_GLOBAL(u32, 0xde1d08), b, index, a);
    }
    if (!p)
        return 0;
    v = cm_fn_0062c400(CM_GLOBAL(u32, 0xb63d28), p);
    if ((u8)v == 2)
        return 0xff;
    return (u8)cm_fn_0062c590(CM_GLOBAL(u32, 0xb63d28), p, 1);
}
