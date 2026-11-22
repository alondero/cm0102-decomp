#include "cmre.h"
extern void cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932e10(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

void CM_THISCALL cm_fn_008e5b30(u32 self, u32 stream)
{
    u8 scratch[0x204];
    u32 p, item, line;
    u8 *a = scratch + 4, *b = scratch + 0x104;
    if (cm_fn_00932e10(self, 0x41, 1, stream) != 1) {
        cm_fn_00944cff(0xa9e318, 0, 0, b, a);
        line = 0xee;
        { u8 *t = a; a = b; b = t; }
        goto error;
    }
    p = *(volatile u32 *)self;
    *(u32 *)scratch = p ? *(u16 *)p : 0;
    if (cm_fn_00932e10((u32)scratch, 2, 1, stream) != 1) {
        line = 0x102;
        goto format_error;
    }
    p = *(volatile u32 *)self;
    if (p) {
        *(u32 *)(p + 10) = *(u32 *)(p + 2);
        item = cm_fn_0068d1b0(*(volatile u32 *)self);
        while (item) {
            if (cm_fn_00932e10(item, 4, 1, stream) != 1) {
                line = 0x110;
                goto format_error;
            }
            item = cm_fn_0068d1b0(*(volatile u32 *)self);
        }
    }
    p = *(volatile u32 *)(self + 4);
    *(u32 *)scratch = p ? (p & 0xffff0000u) | *(u16 *)p : 0;
    if (cm_fn_00932e10((u32)scratch, 2, 1, stream) != 1) {
        line = 0x125;
        goto format_error;
    }
    p = *(volatile u32 *)(self + 4);
    if (!p) return;
    *(u32 *)(p + 10) = *(u32 *)(p + 2);
    item = cm_fn_0068d1b0(*(volatile u32 *)(self + 4));
    while (item) {
        if (cm_fn_00932e10(item, 7, 1, stream) != 1) {
            line = 0x133;
            goto format_error;
        }
        item = cm_fn_0068d1b0(*(volatile u32 *)(self + 4));
    }
    return;
format_error:
    cm_fn_00944cff(0xa9e318, 0, 0, a, b);
error:
    cm_fn_00944c9f(0xae24d0, 0x9870e8, cm_fn_0090d130(), a, b, line);
    cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
    CM_GLOBAL(u32, 0xb67a34) = 0;
}
