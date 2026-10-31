/* Preferred-image VA 005b50d0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005b50d0(u32 self, u32 flag)
{
    u8 scratch[0x200];
    u32 mask;
    if ((s32)flag <= 0x200) {
        if (flag == 0x200) {
            mask = 0xfffff9ffu;
        } else if ((s32)flag <= 0x20) {
            if (flag == 0x20) {
                mask = 0xffffff8fu;
            } else {
                u32 index = flag - 1;
                if (index > 15) goto error;
                u32 map = B(0x005b5260u + index);
                u32 target = D(0x005b5254u + map * 4);
                /* The pinned, read-only tables contain exactly these targets. */
                switch (target) {
                case 0x005b510a: mask = 0xfffffff0u; break;
                case 0x005b5141: mask = 0xffffff8fu; break;
                case 0x005b51b7: goto error;
                default: __builtin_unreachable();
                }
            }
        } else if (flag == 0x40) {
            mask = 0xffffff8fu;
        } else if (flag == 0x80 || flag == 0x100) {
            mask = 0xfffffe7fu;
        } else {
            goto error;
        }
    } else if ((s32)flag <= 0x4000) {
        if (flag == 0x4000) {
            mask = 0xffff9fffu;
        } else if ((s32)flag <= 0x1000) {
            if (flag == 0x1000 || flag == 0x800) mask = 0xffffe7ffu;
            else if (flag == 0x400) mask = 0xfffff9ffu;
            else goto error;
        } else if (flag == 0x2000) {
            mask = 0xffff9fffu;
        } else {
            goto error;
        }
    } else if (flag == 0x8000 || flag == 0x10000 || flag == 0x20000) {
        mask = 0xfffc7fffu;
    } else {
        goto error;
    }
    u32 value = D(self + 0x584);
    value = (value & mask) | flag;
    D(self + 0x187f) = 0;
    D(self + 0x584) = value;
    return 1;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, scratch + 0x100, scratch);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   scratch + 0x100, scratch, 0x67d);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
