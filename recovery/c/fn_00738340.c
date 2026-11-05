#include "cmre.h"
/* Preferred-image VA00738340/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD5/RET20, actual fullEAX. Original constructor/setters/submission remain
   explicit boundaries. New-C v2 declares DFclear (ADR0004). */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define H(a) CM_GLOBAL(volatile u16,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_CDECL cm_fn_00547790(u32,u32);
extern u32 CM_CDECL cm_fn_005477d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

__attribute__((target("no-sse3")))
u32 CM_STDCALL cm_fn_00738340(u32 a1,u32 a2,u32 a3,u32 a4,u32 a5)
{
    struct { volatile u8 local[0xe4]; volatile u32 prev,handler,state; } frame;
    u32 old=CM_FS(u32,0),l=(u32)frame.local,news=l+4;
    u32 a,b,c,handle,result,index;
    s32 value;
    s16 change;
    u8 slot;
    long double live;
    volatile long long converted;
    frame.state=0xffffffffu;frame.handler=0x95d71b;frame.prev=old;
    CM_FS(u32,0)=(u32)&frame.prev;
    if(!a1)value=0;
    else {a=D(a1);handle=D(0xb67a44);b=D(handle);value=W(a*73u+b+8);}
    D(l)=(u32)(value-5000);
    live=(long double)(s32)D(l);live*=Q(0x96d280);
    converted=(long long)live; /* __ftol CALL00738397, site1. */
    a=(u32)converted;
    D(l)=(u32)(s32)(s16)(u16)(a4-a);
    live=(long double)(s32)D(l);live*=Q(0x969040);
    converted=(long long)live; /* __ftol CALL007383b6, site2. */
    change=(s16)(u16)converted;
    a=D(a1+0xcf);result=cm_fn_00547790(a,a1);
    if(result!=0) {
        D(l)=(u32)(s32)change;live=(long double)(s32)D(l);
        if(change<0)live*=Q(0x968968);else live+=live;
        goto cast3;
    }
    a=D(a1+0xcf);result=cm_fn_005477d0(a,a1);
    if(result!=0) {
        D(l)=(u32)(s32)change;live=(long double)(s32)D(l);
        if(change>0)live*=Q(0x968968);else live+=live;
        goto cast3;
    }
    goto after3;
cast3:
    converted=(long long)live; /* __ftol CALL0073842f, site3. */
    result=(u32)converted;change=(s16)(u16)converted;
after3:
    if(change<-1000) {
        D(l)=(u32)((s32)change+1000);live=(long double)(s32)D(l);
        live*=Q(0x968918);live-=Q(0x968908);
        goto cast4;
    } else if(change>1000) {
        D(l)=(u32)((s32)change-1000);live=(long double)(s32)D(l);
        live*=Q(0x968918);live+=Q(0x968908);
        goto cast4;
    }
    goto after4;
cast4:
    converted=(long long)live; /* __ftol CALL0073847f, site4. */
    result=(u32)converted;change=(s16)(u16)converted;
after4:
    handle=D(0xb67a44);
    if(a1) {
        a=D(a1);b=D(handle);H(a*73u+b+8)=(u16)(H(a*73u+b+8)+(u16)change);
        a=D(a1);b=D(handle);c=a*73u+b+8;
        value=W(c);
        if(value<1)result=1;else if(value>10000)result=10000;else result=(u32)value;
        H(c)=(u16)result;
    }
    if(change>=-500 && change<=500)goto out;
    (void)cm_fn_0077bbe0(news,0xbe0,0);
    frame.state=0;
    (void)cm_fn_00785790(news,0,0);
    (void)cm_fn_00785790(news,1,(u32)(s32)(s16)a3);
    (void)cm_fn_00785790(news,2,(u32)(s32)(s8)a5);
    a=cm_fn_0090cfc0(4);(void)cm_fn_00785790(news,3,a);
    a=D(a1);(void)cm_fn_00785790(news,4,a);
    if(change<-750)a=5;
    else if(change<0)a=4;
    else if(change>750)a=2;
    else a=3;
    (void)cm_fn_00785790(news,5,a);
    a=D(a1+0xcf);b=D(a);(void)cm_fn_00785790(news,6,b);
    a=D(a1+0xcf);b=D(a+4);c=D(b+0x33);(void)cm_fn_00785790(news,7,c);
    a=D(a1+0xcf);b=D(a+8);c=D(b+0x33);(void)cm_fn_00785790(news,8,c);
    a=D(a1+0xcf);b=D(a+12);c=D(b+0x33);(void)cm_fn_00785790(news,9,c);
    if(a2) {
        a=D(a2);(void)cm_fn_00785790(news,10,a);
        a=D(a2+4);b=D(a+0x33);(void)cm_fn_00785790(news,11,b);
        a=D(a2+8);b=D(a+0x33);(void)cm_fn_00785790(news,12,b);
        a=D(a2+12);b=D(a+0x33);(void)cm_fn_00785790(news,13,b);
        if(cm_fn_00547790(a2,a1)!=0)a=6;
        else a=cm_fn_005477d0(a2,a1)!=0?7:8;
        slot=15;(void)cm_fn_00785790(news,14,a);
    } else {slot=11;(void)cm_fn_00785790(news,10,0xffffffffu);}
    index=slot;slot=(u8)(slot+1);U(l)=slot;
    (void)cm_fn_00785790(news,index,0xffffffffu);
    if(a1){a=D(a1);handle=D(0xb67a44);b=D(handle);value=W(a*73u+b+8);}else value=0;
    /* Native passes DWORD L0, whose upper24 are ignored by original setter. */
    index=D(l);(void)cm_fn_00785790(news,index,(u32)value);
    slot=(u8)(slot+1);(void)cm_fn_00785790(news,slot,0);
    handle=D(0xb7431c);result=cm_fn_007861e0(handle,news,a1);
out:
    old=frame.prev;CM_FS(u32,0)=old;return result;
}
