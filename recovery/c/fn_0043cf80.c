#include "cmre.h"
/* Original evidence: preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0043d02f..0043d03a dispatches [vtable+3c] with ECX=self and
   four stack arguments (0, L+8, L+4, 0). Keep the actual target:
   no substitution for 0043c5a0 is justified. Its absent inventory
   coverage remains an external gate-evidence obstacle, not C logic. */
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern void cm_fn_005e8290(const char *,const char *,int);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
typedef u32 (CM_THISCALL *slot_fn)(u32,u32,u32,u32,u32);
void CM_THISCALL cm_fn_0043cf80(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 p, q, vt, target, builder, obj, value, line;
    void *a, *b;
    frame.state=0xffffffff;
    frame.handler=0x954b5e;
    frame.prev=(u32)CM_FS(void *,0);
    CM_FS(void *,0)=(void *)&frame;
    scratch[0]=0; scratch[1]=3; scratch[2]=1; scratch[3]=2;
    p=(u32)cm_fn_00944e46(16);
    if (!p) {
        cm_fn_00944cff(0x98b090,0,0,scratch+0x10,scratch+0x210);
        a=scratch+0x10; b=scratch+0x210; line=0x20d;
    } else {
        q=CM_GLOBAL(volatile u32,self+0xb1);
        CM_GLOBAL(volatile u32,p)=CM_GLOBAL(volatile u32,q);
        q=CM_GLOBAL(volatile u32,self+0xb1);
        CM_GLOBAL(volatile u32,p+4)=CM_GLOBAL(volatile u32,q+0xb1);
        q=CM_GLOBAL(volatile u32,self+0xb1);
        CM_GLOBAL(volatile u32,p+8)=CM_GLOBAL(volatile u32,q+0x3b);
        q=CM_GLOBAL(volatile u32,self+0xb1);
        CM_GLOBAL(volatile u32,p+12)=CM_GLOBAL(volatile u32,q+0x76);
        vt=CM_GLOBAL(volatile u32,self);
        target=CM_GLOBAL(volatile u32,vt+0x3c);
        builder=((slot_fn)target)(self,0,(u32)(scratch+8),(u32)(scratch+4),0);
        obj=(u32)cm_fn_00944cf1(0xb2);
        CM_GLOBAL(volatile u32,(u32)(scratch+12))=obj;
        frame.state=0;
        if (obj) {
            value=CM_GLOBAL(volatile u32,(u32)(scratch+4));
            q=CM_GLOBAL(volatile u16,self+0x40);
            vt=CM_GLOBAL(volatile u32,self+4);
            obj=cm_fn_00522e00(obj,self,4,p,CM_GLOBAL(volatile u32,(u32)(scratch+8)),vt,builder,(u16)q,0,1,(u16)value,0x14,0,1,0,(u32)scratch);
        }
        q=CM_GLOBAL(volatile u32,self+0xc);
        frame.state=0xffffffff;
        CM_GLOBAL(volatile u32,q)=obj;
        q=CM_GLOBAL(volatile u32,self+0xc);
        obj=CM_GLOBAL(volatile u32,q);
        if (obj) {
            cm_fn_0051c800(obj,0);
            cm_fn_009452ca(builder);
            cm_fn_009452ca(p);
            CM_GLOBAL(volatile u32,self+0x30)=0;
            CM_FS(void *,0)=(void *)frame.prev;
            return;
        }
        cm_fn_00944cff(0x98b090,0,0,scratch+0x310,scratch+0x110);
        a=scratch+0x310; b=scratch+0x110; line=0x221;
    }
    value=(u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,value,a,b,line);
    cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
    CM_GLOBAL(volatile u32,0xb67a34)=0;
    CM_FS(void *,0)=(void *)frame.prev;
}
