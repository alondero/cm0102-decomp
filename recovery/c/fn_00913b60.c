#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32, u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern void cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_THISCALL cm_fn_006826d0(void *, u32);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_00913b60(u32 self)
{
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    u8 scratch[0x40c];
    u32 clubs, owner, p, vtable, builder, obj, result, n;
    void *prev;
    void *a, *b;
    u32 line;
    prev = CM_FS(void *, 0);
    frame.state = 0xffffffff;
    frame.handler = 0x9657de;
    frame.prev = prev;
    CM_FS(void *, 0) = (void *)&frame;
    clubs = (u32)cm_fn_00944e46(8);
    if (!clubs) {
        a = scratch + 0x20c;
        b = scratch + 0x0c;
        line = 0x552;
    } else {
        n = *(volatile u8 *)(self + 0xee) ? 1 : 2;
        owner = *(volatile u32 *)(self + 0x0c);
        p = *(volatile u32 *)(owner + 0x24);
        p = cm_fn_006826d0((void *)p, n);
        *(volatile u32 *)clubs = *(volatile u32 *)p;
        owner = *(volatile u32 *)(self + 0x0c);
        p = *(volatile u32 *)(owner + 0x28);
        p = cm_fn_006826d0((void *)p, n);
        *(volatile u32 *)(clubs + 4) = *(volatile u32 *)p;
        vtable = *(volatile u32 *)self;
        builder = (*(u32 (CM_THISCALL **)(u32, u32, void *, void *, u32))(vtable + 0x3c))(self, 11, scratch, scratch + 4, 0);
        obj = (u32)cm_fn_00944cf1(0xb2);
        frame.state = 0;
        if (obj) {
            u32 local4 = *(volatile u32 *)(scratch + 4);
            u16 year = *(volatile u16 *)(self + 0x40);
            u32 competition = *(volatile u32 *)(self + 4);
            u32 local0 = *(volatile u32 *)scratch;
            result = cm_fn_00522e00(obj, self, 2, clubs, (s16)local0, competition, builder, year, 11, 4, (u16)local4, 20, 0, 0, 0, 0);
        } else {
            result = 0;
        }
        owner = *(volatile u32 *)(self + 0x0c);
        frame.state = 0xffffffff;
        *(volatile u32 *)(owner + 0x2c) = result;
        cm_fn_009452ca(clubs);
        cm_fn_009452ca(builder);
        owner = *(volatile u32 *)(self + 0x0c);
        p = *(volatile u32 *)(owner + 0x2c);
        if (p) {
            cm_fn_0051c800(p, 0);
            *(volatile u32 *)(self + 0x30) = 11;
            CM_FS(void *, 0) = frame.prev;
            return;
        }
        a = scratch + 0x30c;
        b = scratch + 0x10c;
        line = 0x572;
    }
    cm_fn_00944cff(0xad4680, 0, 0, a, b);
    p = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, p, a, b, line);
    cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
    CM_GLOBAL(u32, 0xb67a34) = 0;
    CM_FS(void *, 0) = frame.prev;
}
