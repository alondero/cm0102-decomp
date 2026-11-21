#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,const char *,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);

void CM_STDCALL cm_fn_008dba80(u32 obj, u32 mode, u32 receiver)
{
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    u8 scratch[0x1e0];
    u32 a,b,c,d,date,x,y=0,z=0,kind=0,base,p,q,person;
    s32 sx,sy;
    unsigned long long v;
    frame.state=~0u;
    frame.handler=0x964546;
    frame.prev=CM_FS(void *,0);
    CM_FS(void *,0)=(void *)&frame;
    a=(u32)cm_fn_00785830((void *)obj,0);
    b=(u32)cm_fn_00785830((void *)obj,1);
    c=(u32)cm_fn_00785830((void *)obj,2);
    d=(u32)cm_fn_00785830((void *)obj,3);
    date=(u32)cm_fn_00785830((void *)obj,4);
    x=(u32)cm_fn_00785830((void *)obj,6);
    if(mode) {
        kind=(u32)cm_fn_00785830((void *)obj,5);
        y=(u8)cm_fn_00785830((void *)obj,7);
        z=(u32)cm_fn_00785830((void *)obj,8);
    }
    cm_fn_0053e140(b,c,d,(u32)(scratch+0x50),400,mode?1:5,a);
    *(u32 *)(scratch+(mode?0x18:0))=date;
    frame.state=mode?0:1;
    v=cm_fn_0043f460((const int *)(scratch+(mode?0x18:0)),6);
    cm_fn_00441c10((u32)(scratch+0x1c),0x32,(u32)v,(u32)(v>>32),6);
    base=CM_GLOBAL(volatile u32,0xae23b4);
    x*=0x245;
    if(mode) {
        z*=0x245;
        p=base+z+0x38;
        sx=*(s8 *)(base+z+0x52);
        sy=*(s8 *)(base+x+0x52);
        q=base+x+0x38;
        if(kind==1)
            cm_fn_0066f4e0(0xde1f64,y==2?0xa977b4:0xa97564,sx,sx,sy,sy,100,sx,sx,p,q,(u32)(scratch+0x1c),(u32)(scratch+0x50),p);
        else
            cm_fn_0066f4e0(0xde1f64,y==2?0xa97690:0xa9742c,sx,sx,sy,sy,100,sx,sx,p,q,(u32)(scratch+0x1c),(u32)(scratch+0x50),kind,p);
    } else {
        sx=*(s8 *)(base+x+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa973b4,sx,sx,100,base+x+0x38,(u32)(scratch+0x1c),(u32)(scratch+0x50));
    }
    cm_fn_004ae660((void *)receiver,0xde1f64);
    if(mode) {
        base=CM_GLOBAL(volatile u32,0xae23b4);
        p=base+z;
        cm_fn_004ae8a0(receiver,(const char *)(p+0x38),0x7d5,p);
    }
    base=CM_GLOBAL(volatile u32,0xae23b4);
    p=base+x;
    cm_fn_004ae8a0(receiver,(const char *)(p+0x38),0x7d5,p);
    person=a*0x6e + CM_GLOBAL(volatile u32,0xae23bc);
    if(cm_fn_0053e990(person,b,c,d,0)) {
        person=a*0x6e + CM_GLOBAL(volatile u32,0xae23bc);
        cm_fn_004ae8a0(receiver,(const char *)(scratch+0x50),0x7d7,person);
    }
    CM_FS(void *,0)=frame.prev;
}
