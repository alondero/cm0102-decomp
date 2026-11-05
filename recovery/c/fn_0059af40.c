#include "cmre.h"
/* Preferred-image VAs, base 00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2 / RET8, full EAX. L denotes ESP after SUB 208h, before saves.
   Integer L+0, direction L+4, related error buffers L+8 and L+108h. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *,u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile s16 *)(u32)(a))
#define F(a) CM_GLOBAL(volatile double,a)
#define L(a) (*(volatile u32 *)(scratch+(a)))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059af40(u32 self,u32 match,u32 amount)
{
    u8 scratch[0x208];
    u32 a,c,p,club,account=0,group,factor;
    long double t,lower;
    volatile long long converted;
    if(!match){
        cm_fn_00944cff(0x9c8784,0,0,scratch+0x108,scratch+8);
        a=(u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,a,scratch+0x108,scratch+8,0x10a6);
        cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
        D(0xb67a34)=0;return 0;
    }
    if(!amount)return 0;
    a=D(match+0x1c);
    if(a){
        if(cm_fn_0053b900((u32 *)a))return 0;
        club=D(match+0x1c);
        if(!club){
            cm_fn_00944cff(0x9c8784,0,0,scratch+8,scratch+0x108);
            a=(u32)cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,a,scratch+8,scratch+0x108,0x20b5);
            cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
            D(0xb67a34)=0;
        }else{
            a=cm_fn_00540a50(club,(u32)(scratch+4),1);
            if(a && !L(4))club=a;
            a=D(club);account=a*359u;a=D(self);account+=a;
        }
    }
    if(B(match+0x3c)){
        a=D(match+0x14);
        if(a){c=((u32)(s32)W(a+0x69)+8u)*amount;L(0)=c;goto category;}
    }
    a=D(match+0x14);
    if(!a){
        club=D(match+0x1c);
        c=(u32)((s32)W(club+0x80)/500);
        goto basic_float;
    }
    p=D(0xae2c74);
    a=(u32)cm_fn_005c6710((void *)p,a);
    club=D(match+0x1c);
    if(a){
        c=(u32)((s32)W(club+0x80)/500);
basic_float:
        L(0)=c;t=(long double)(s32)L(0);t*=F(0x968968);
        t*=(long double)(s32)amount;
        /* Registered __ftol, original call 0059b125; EAX consumed. */
        converted=(long long)t;c=(u32)converted;
    }else{
        if(B(club+0x64)==1)p=4;
        else{a=D(match+0x20);p=B(a+0x64)==1?4u:3u;}
        c=((u32)((s32)W(club+0x80)/500)+p)*amount;
    }
    L(0)=c;
    if(account){
        p=D(club+0x57);a=D(match+0x14);
        if(p==a){
            lower=(long double)(s32)L(0);
            t=(long double)CM_GLOBAL(volatile float,account+0x160);
            t*=lower;t=lower-t;
            /* Registered __ftol 0059b19b; lower is discarded at 0059b1a0. */
            converted=(long long)t;c=(u32)converted;L(0)=c;
        }
    }
category:
    club=D(match+0x1c);group=D(club+0x53);
    a=group?B(group+0x7e):3u;
    if((s8)a!=1){
        c=(u32)((s32)c/((s8)a==2?4:10));L(0)=c;
    }
    a=D(club+0x57);
    if(a){p=D(a);a=D(0x9cf570);if(p==a){factor=0x968928;goto scale;}}
    club=D(club);a=D(0x9d0250);
    if(club==a){factor=0x968940;goto scale;}
    if(club==D(0x9d0110)){factor=0x968de8;goto scale;}
    if(club==D(0x9d0248)){factor=0x969040;goto scale;}
    if(club==D(0x9d0244)){factor=0x9689d8;goto scale;}
    if(club!=D(0x9d024c))goto final_group;
    factor=0x968de8;
scale:
    t=(long double)(s32)L(0);t*=F(factor);
    /* Registered __ftol, original call 0059b25f; EAX consumed. */
    converted=(long long)t;c=(u32)converted;L(0)=c;
final_group:
    if(group){
        a=D(group);p=D(0x9cf314);
        if(a==p){
            t=(long double)(s32)L(0);t*=F(0x968978);
            /* Registered __ftol, original call 0059b284; EAX consumed. */
            converted=(long long)t;c=(u32)converted;
        }
    }
    return c;
}
