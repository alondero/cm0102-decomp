#include "cmre.h"
/* Instruction authority: preferred-image VAs in CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0043bfff..0043c00a loads the actual vtable and calls slot +0x3c
   with ECX=self and four stack arguments (0, L+8, L+4, 0).
   Keep that boundary indirect: substituting a known inventory target would
   not evidence execution of the reported missing original 0043b430 entry.
   This source cannot resolve that inventory/coverage obstacle by itself. */
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_0043bf50(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 p, q, vtable, builder, obj, result, a, b, line;
    u8 *x, *y;
    frame.state = 0xffffffff;
    frame.handler = 0x954aee;
    frame.prev = (u32)CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    scratch[0] = 0; scratch[1] = 3; scratch[2] = 1; scratch[3] = 2;
    p = (u32)cm_fn_00944e46(16);
    if (!p) {
        x = scratch + 0x10; y = scratch + 0x210; line = 0x21b;
        cm_fn_00944cff(0x98b050,0,0,x,y);
    } else {
        q = *(volatile u32 *)(self + 0xb1);
        *(volatile u32 *)p = *(volatile u32 *)q;
        q = *(volatile u32 *)(self + 0xb1);
        *(volatile u32 *)(p+4) = *(volatile u32 *)(q+0xb1);
        q = *(volatile u32 *)(self + 0xb1);
        *(volatile u32 *)(p+8) = *(volatile u32 *)(q+0x3b);
        q = *(volatile u32 *)(self + 0xb1);
        *(volatile u32 *)(p+12) = *(volatile u32 *)(q+0x76);
        vtable = *(volatile u32 *)self;
        builder = ((u32 (CM_THISCALL *)(u32,u32,u32,u32,u32))(*(volatile u32 *)(vtable+0x3c)))(self,0,(u32)(scratch+8),(u32)(scratch+4),0);
        obj = (u32)cm_fn_00944cf1(0xb2);
        *(volatile u32 *)(scratch+12) = obj; /* 0043c01c */
        frame.state = 0;
        if (obj) {
            a = *(volatile u32 *)(scratch+4);
            b = *(volatile u16 *)(self+0x40);
            q = *(volatile u32 *)(self+4);
            result = cm_fn_00522e00(obj,self,4,p,*(volatile u32 *)(scratch+8),q,builder,b,0,1,a,0,0x14,0,1,(u32)scratch);
        } else result = 0;
        q = *(volatile u32 *)(self+12);
        frame.state = 0xffffffff;
        *(volatile u32 *)q = result;
        q = *(volatile u32 *)(self+12);
        obj = *(volatile u32 *)q;
        if (obj) {
            cm_fn_0051c800(obj,0);
            cm_fn_009452ca(builder);
            cm_fn_009452ca(p);
            *(volatile u32 *)(self+0x30) = 0;
            CM_FS(void *, 0) = (void *)frame.prev;
            return;
        }
        x = scratch+0x310; y = scratch+0x110; line = 0x22f;
        cm_fn_00944cff(0x98b050,0,0,x,y);
    }
    q = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,q,x,y,line);
    cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    CM_GLOBAL(u32,0xb67a34) = 0;
    CM_FS(void *, 0) = (void *)frame.prev;
}
