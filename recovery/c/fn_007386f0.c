#include "cmre.h"
/* Preferred-image VA007386f0/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD6/RET24, full EAX. New-C v2 entry policy (ADR0004) clears DF.
   Original news constructor/setters/submission remain explicit boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define H(a) CM_GLOBAL(volatile u16,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_CDECL cm_fn_00547810(u32,u32);
extern u32 CM_CDECL cm_fn_00547840(u32,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_007c1950(u32,u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

__attribute__((target("no-sse3")))
u32 CM_STDCALL cm_fn_007386f0(u32 a1,u32 a2,u32 a3,u32 a4,u32 a5,u32 a6)
{
    struct {
        volatile u8 local[0xe8];
        volatile u32 prev,handler,state;
    } frame;
    u32 l=(u32)frame.local,news=l+8,old=CM_FS(u32,0);
    u32 a,b,c,handle,result=0,index;
    s32 value;
    s16 change;
    u8 slot,flag;
    long double live,x,y;
    volatile long long converted;
    frame.state=0xffffffffu;frame.handler=0x95d73b;frame.prev=old;
    CM_FS(u32,0)=(u32)&frame.prev;
    if(!a1)value=0;
    else {a=D(a1);handle=D(0xb67a44);b=D(handle);value=W(a*73u+b+6);}
    value=(value-6000)/20;
    value=(s16)(u16)((u32)value+a4);
    D(l+4)=(u32)value;
    live=(long double)(s32)D(l+4);live*=Q(0x969040);
    converted=(long long)live; /* __ftol CALL00738767, site1. */
    change=(s16)(u16)converted;
    b=D(a1+0xbf);a=D(a1+0xcf);
    if(cm_fn_00547810(a,b)!=0) {
        D(l+4)=(u32)(s32)change;live=(long double)(s32)D(l+4);
        if(change<0)live*=Q(0x968968);else live+=live;
        goto cast2;
    } else {
        a=D(a1+0xbf);b=D(a1+0xcf);
        if(cm_fn_00547840(b,a)!=0) {
            D(l+4)=(u32)(s32)change;live=(long double)(s32)D(l+4);
            if(change>0)live*=Q(0x968968);else live+=live;
            goto cast2;
        }
    }
    goto after2;
cast2:
    converted=(long long)live; /* __ftol CALL007387ec, site2. */
    change=(s16)(u16)converted;
after2:
    if(change<-1000) {
        D(l+4)=(u32)((s32)change+1000);live=(long double)(s32)D(l+4);
        live*=Q(0x968918);live-=Q(0x968908);
        goto cast3;
    } else if(change>1000) {
        D(l+4)=(u32)((s32)change-1000);live=(long double)(s32)D(l+4);
        live*=Q(0x968918);live+=Q(0x968908);
        goto cast3;
    }
    goto after3;
cast3:
    converted=(long long)live; /* __ftol CALL0073883d, site3. */
    change=(s16)(u16)converted;
after3:
    handle=D(0xb67a44);
    if(a1) {
        a=D(a1);b=D(handle);H(a*73u+b+6)=(u16)(H(a*73u+b+6)+(u16)change);
        a=D(a1);b=D(handle);c=a*73u+b+6;
        value=W(c);
        if(value<1)result=1;else if(value>10000)result=10000;else result=(u32)value;
        H(c)=(u16)result;
    }
    if(change>=-500 && change<=500)goto out;
    (void)cm_fn_0077bbe0(news,0xbe0,0);
    frame.state=0;
    (void)cm_fn_00785790(news,0,1);
    (void)cm_fn_00785790(news,1,(u32)(s32)(s16)a3);
    (void)cm_fn_00785790(news,2,(u32)(s32)(s8)a5);
    a=cm_fn_0090cfc0(4);(void)cm_fn_00785790(news,3,a);
    a=D(a1);(void)cm_fn_00785790(news,4,a);
    handle=D(0xb67a44);
    if(change<-750 && (!a1 || (a=D(a1),b=D(handle),W(a*73u+b+6)<4500)))a=5;
    else if(change<0) {
        if(!a1)a=4;
        else {a=D(a1);b=D(handle);a=W(a*73u+b+6)>6250?9:4;}
    } else {
        if(change<=750 || !a1)a=3;
        else {a=D(a1);b=D(handle);a=W(a*73u+b+6)>6250?2:3;}
    }
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
        b=D(a1+0xbf);
        if(cm_fn_00547810(a2,b)!=0)a=6;
        else {b=D(a1+0xbf);a=cm_fn_00547840(a2,b)!=0?7:8;}
        slot=15;(void)cm_fn_00785790(news,14,a);
    } else {slot=11;(void)cm_fn_00785790(news,10,0xffffffffu);}
    a=D(a1+0xbf);index=slot;
    if(a) {
        b=D(a);slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,b);
        a=D(a1+0xbf);index=slot;slot=(u8)(slot+1);b=D(a+4);c=D(b+0x33);(void)cm_fn_00785790(news,index,c);
        a=D(a1+0xbf);index=slot;slot=(u8)(slot+1);b=D(a+8);c=D(b+0x33);(void)cm_fn_00785790(news,index,c);
        a=D(a1+0xbf);index=slot;slot=(u8)(slot+1);b=D(a+12);c=D(b+0x33);
    } else {slot=(u8)(slot+1);c=0xffffffffu;}
    (void)cm_fn_00785790(news,index,c);
    if(a1){a=D(a1);handle=D(0xb67a44);b=D(handle);value=W(a*73u+b+6);}else value=0;
    index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,(u32)value);
    index=slot;
    if(a6) {
        slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,1);
        if(a1){a=D(a1);handle=D(0xb67a44);b=D(handle);value=W(a*73u+b+14);}else value=0;
        index=slot;slot=(u8)(slot+1);
    } else {value=0;slot=(u8)(slot+1);}
    (void)cm_fn_00785790(news,index,(u32)value);
    flag=0;
    if(a2) {
        a=D(a2+0x61);
        if(a) {
            handle=D(0xae1050);a=cm_fn_004cf5a0(handle,a2);
            if(a && B(a2+0x18)>25) {b=U(a+0x4f)&0xf0u;if(b==0x20 || b==0x10)flag=1;}
        }
    }
    index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,flag);
    U(l+3)=0;
    if(a2) {
        a=D(a2+0x61);
        if(a) {
            handle=D(0xb74350);a=cm_fn_007c1950(handle,a);
            handle=D(0xb74350);D(l+4)=(u32)(s32)(s8)a;
            live=(long double)(s32)D(l+4);
            x=(long double)Q(handle+0x18);
            y=(long double)Q(0x9689a0);
            if(x<y || x>y) {
                x=(long double)Q(handle+0x10);x/=(long double)Q(handle+0x18);
            } else x=(long double)Q(0x968998);
            x*=Q(0x9688a0);
            if(live>x)U(l+3)=1;
        }
    }
    a=(u32)(s32)B(l+3);index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,a);
    a=0;if(a2 && B(a2+0x59)>14)a=1;
    index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,a);
    a=0;if(a2 && B(a2+0x5b)>14)a=1;
    index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,a);
    U(l+3)=0;
    if(a2) {a=D(a2+0x42);b=D(a2+0x3e);if(cm_fn_0054cdb0(0xae2c90,b,a)>1500)U(l+3)=1;}
    a=(u32)(s32)B(l+3);index=slot;slot=(u8)(slot+1);(void)cm_fn_00785790(news,index,a);
    U(l+3)=0;
    if(a2) {handle=D(0xae1050);if((s32)cm_fn_004d0c60(handle,a2,a1,0,0xffffffffu,0)>4500000)U(l+3)=1;}
    a=(u32)(s32)B(l+3);index=slot;slot=(u8)(slot+1);U(l+4)=slot;
    (void)cm_fn_00785790(news,index,a);
    a=0;if(a2){b=D(a2+0x61);if(b && W(b+5)>140)a=1;}
    /* Native pushes DWORD L+4, but setter reads only its low BYTE index. */
    index=U(l+4);(void)cm_fn_00785790(news,index,a);
    handle=D(0xb7431c);result=cm_fn_007861e0(handle,news,a1);
out:
    old=frame.prev;CM_FS(u32,0)=old;return result;
}
