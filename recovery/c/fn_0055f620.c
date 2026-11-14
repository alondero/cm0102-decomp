/* fn_0055f620 — preferred-image VA 0x0055f620 (display.cpp), CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Frame: 0x200 bytes; two 256-byte buffers at esp+0x0c and esp+0x10c.
   EAX on return = (last callee EAX & 0xffff0000) | low half word. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))

extern u32 CM_FASTCALL cm_fn_00560490(u32 *param_1, int param_2);
extern u32 CM_FASTCALL cm_fn_00560c30(u32 *param_1);
extern u32 CM_FASTCALL cm_fn_00561050(u32 *param_1);
extern u32 CM_FASTCALL cm_fn_00561210(u32 *param_1);
extern u32 CM_THISCALL cm_fn_00569610(u32 self);
extern u32 CM_CDECL cm_fn_005e8170(u32 mode);
extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

typedef char *(*cm_three_args_t)(void *, void *, u32);

u32 CM_FASTCALL cm_fn_0055f620(u32 *param_1)
{
    u32 self = (u32)param_1;
    u8 scratch[0x210];
    u8 *buf1 = scratch + 0x0c;   /* lea ecx,[esp+0x0c]  */
    u8 *buf2 = scratch + 0x10c;  /* lea edx,[esp+0x10c] */
    u32 value;
    u32 half;
    u32 low;

    D(0x009c36e8u) = 0;

    if (W(self + 0x12f4ffu) != 0xffffu) {
        u8 tag = B(self + 0x12f4feu);
        if (tag != 0 && B(0x00b67a24u) == tag) {
            half = cm_fn_005e8170(0);
            B(0x00b67a24u) = 0;
            return (half & 0xffff0000u) | W(self + 0x12f4ffu);
        }
    }

    if (D(0x00b67a0cu) != 0) {
        if (W(self + 0x12f501u) != 0xffffu) {
            half = cm_fn_005e8170(0);
            D(0x00b67a0cu) = 0;
            value = W(self + 0x12f501u);
            W(self + 0x12f501u) = 0xffffu;
            return (half & 0xffff0000u) | value;
        }
        D(0x00b67a0cu) = 0;
    }

    if (D(0x00b67a1cu) != 0) {
        if (W(self + 0x12f503u) != 0xffffu) {
            half = cm_fn_005e8170(0);
            D(0x00b67a1cu) = 0;
            value = W(self + 0x12f503u);
            W(self + 0x12f503u) = 0xffffu;
            return (half & 0xffff0000u) | value;
        }
        D(0x00b67a1cu) = 0;
    }

    cm_fn_00569610(self);

    value = D(self);
    switch (value) {
    case 0:
        low = cm_fn_00560490((u32 *)self, 0);
        break;
    case 1:
        low = cm_fn_00560c30((u32 *)self);
        break;
    case 2:
        low = cm_fn_00561050((u32 *)self);
        break;
    case 3:
        low = cm_fn_00561210((u32 *)self);
        break;
    default:
        cm_fn_00944cff(0x009c36ecu, 0, 0, (void *)buf2, (void *)buf1);
        value = (u32)cm_fn_0090d130((void *)buf2, (void *)buf1, 0x201u);
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, value,
                       (u32)(unsigned long)buf2, (u32)(unsigned long)buf1, 0x201u);
        half = cm_fn_005e8290((const char *)0x009870e0u,
                              (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return half & 0xffff0000u;
    }

    half = cm_fn_005e8170(D(0x009c36e8u));
    return (half & 0xffff0000u) | (low & 0xffffu);
}
