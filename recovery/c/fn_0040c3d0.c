#include "cmre.h"
#define M(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0051c410(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);

u32 CM_THISCALL cm_fn_0040c3d0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 edi, p, q, t, vtable, builder, obj, result, line, a, b, year;
    frame.state = 0xffffffff;
    frame.handler = 0x9537ee;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    edi = cm_fn_00944e46(0x10);
    if (!edi) {
        cm_fn_00944cff(0x987a40,0,0,scratch+0x20c,scratch+0xc);
        a=(u32)(scratch+0x20c); b=(u32)(scratch+0xc); line=0x3d1;
    } else {
        p=M(self+0xc); q=M(p+4); t=M(q+0xb1); M(edi)=M(t);
        p=M(self+0xc); q=M(p+8); t=M(q+0xb1); M(edi+4)=M(t+0x3b);
        p=M(self+0xc); q=M(p+8); t=M(q+0xb1); M(edi+8)=M(t);
        p=M(self+0xc); q=M(p+4); t=M(q+0xb1); M(edi+12)=M(t+0x3b);
        vtable=M(self);
        /* 0040c435 saves EBX; only four arguments are pushed at
           0040c436/484/485/486 before the thiscall at 0040c48a. */
        builder=((u32 (CM_THISCALL *)(u32,u32,void *,void *,u32))M(vtable+0x3c))
            (self,3,scratch,scratch+4,0);
        obj=cm_fn_00944cf1(0xb2);
        frame.state=0;
        if (obj) {
            t=M((u32)(scratch+4));
            year=(obj & 0xffff0000u) | CM_GLOBAL(volatile u16,self+0x40);
            p=M(self+4);
            q=M((u32)scratch);
            result=cm_fn_00522e00(obj,self,4,edi,q,p,builder,year,3,2,t,0x14,0,0,0,0);
        } else result=0;
        p=M(self+0xc);
        frame.state=0xffffffff;
        M(p+0xc)=result;
        cm_fn_009452ca(builder);
        p=M(self+0xc);
        q=M(p+0xc);
        if (q) {
            cm_fn_0051c410(q,0);
            result=cm_fn_009452ca(edi);
            CM_FS(u32,0)=frame.prev;
            return result;
        }
        cm_fn_00944cff(0x987a40,0,0,scratch+0x30c,scratch+0x10c);
        a=(u32)(scratch+0x30c); b=(u32)(scratch+0x10c); line=0x3e6;
    }
    t=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,t,a,b,line);
    result=cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    M(0xb67a34)=0;
    CM_FS(u32,0)=frame.prev;
    return result;
}
