#include "cmre.h"

/* Preferred-image VAs/base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16. Wrappers forward full EAX. FNSTSW at006d0b15/2d supplies
   AH; MOV AL,[self+8eb5] supplies AL. The high word is the last full EAX
   definition, including RNG returns. Ordinary x87 call entry has empty TOP.
   007160e0 is MOV AL,[ESP+4]; MOV [ECX+2b],AL; RET4, so its upper EAX
   bits survive. Keep this callee as an explicit original boundary. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
extern void CM_THISCALL cm_fn_0062be00(u32 self,u32 key,u32 first,u32 second);
extern void CM_THISCALL cm_fn_007160e0(void *self,u32 value);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006d0980(u32 self,u32 first,u32 second,u32 item,u32 other)
{
    volatile u32 args[2];
    volatile float loss=0.0f;
    volatile long long converted;
    u32 a, square, key, owner, flag, status;
    s32 amount, group;
    s16 count;
    long double live, health, threshold;
    args[0]=first;args[1]=second;
    count=(s16)W(self+0xf639u);
    if(count>3) {
        a=cm_fn_0090cfc0((u32)(s32)count);
        if(a!=0) {
            a=cm_fn_0090cfc0(500);
            if(a!=0)return a;
        }
        if((s16)W(self+0xf639u)>6)return a;
    } else {
        /* 006d09be rereads the same word without any intervening call or
           store. Thus its >6 edge cannot return incoming EAX on this path. */
        (void)W(self+0xf639u);
    }
    if((s8)args[0]>-1) {
        key=D(item+0x69u);
        owner=D(0x00b63d28);
        cm_fn_0062be00(owner,key,(u32)&args[0],(u32)&args[1]);
    }
    a=other;
    D(item+0x55u)=a;
    if(a!=0)W(a+0x31u)=(u16)(W(a+0x31u)-10u);
    amount=(s32)(s8)args[1];
    a=(u32)amount;
    square=a*a;
    if(amount>50) {
        B(item+0x1cu)=1;
        live=(long double)(s32)square;
        health=F(item+0x4du);
        F(item+0x4du)=(float)(health-live);
    } else {
        live=(long double)(s32)square;
        live*=Q(0x00968968);
        converted=(long long)live; /* ADR0006: __ftol006d0a3b. */
        a=cm_fn_0090cfc0((u32)converted);
        loss=(float)(s32)a;
        health=F(item+0x4du);
        F(item+0x4du)=(float)(health-(long double)loss);
        live=(long double)loss;
        live*=F(0x00968f18);
        converted=(long long)live; /* ADR0006: __ftol006d0a66. */
        a=cm_fn_0090cfc0((u32)converted);
        F(item+0x59u)=(float)(s32)a;
        live=F(item+0xd5u);
        live*=F(0x00968f18);
        F(item+0xd5u)=(float)live;
    }
    health=F(item+0x4du);
    threshold=Q(0x009688e8);
    if(!(health>=threshold)) {
        B(self+0x8eb5u)=1;
        B(item+0x1du)=1;
        B(item+0x1cu)=1;
    } else {
        owner=D(item+0x6du);
        health=F(item+0x4du);
        group=(s32)(s8)B(owner+0x2du);
        a=(u32)group*5u;
        live=(long double)(s32)(a*5u);
        live+=Q(0x0096d458);
        if(live>health) {
            cm_fn_007160e0((void *)item,5);
            B(self+0x8eb5u)=1;
            B(item+0x1du)=1;
        } else {
            cm_fn_007160e0((void *)item,3);
            live=loss;
            threshold=F(0x0096d1a8);
            if(live>=threshold) {
                flag=B(item+0x1cu);
                if(flag!=0) {
                    B(self+0x8eb5u)=1;
                    B(item+0x1du)=1;
                }
            }
        }
    }
    health=F(item+0x4du);
    threshold=Q(0x00967890);
    /* FCOMP clears C1. At empty entry TOP, its other AH bits are C0/C2/C3;
       the following FSTP occurs after FNSTSW and cannot alter returned AH. */
    status=health<threshold?0x0100u:health==threshold?0x4000u:
           health>threshold?0u:0x4500u;
    if(!(health>=threshold)) {
        live=Q(0x00967890);
    } else {
        health=F(item+0x4du);
        threshold=Q(0x00968910);
        status=health<threshold?0x0100u:health==threshold?0x4000u:
               health>threshold?0u:0x4500u;
        if(health>threshold)live=Q(0x00968910);
        else live=F(item+0x4du);
    }
    F(item+0x4du)=(float)live;
    flag=B(self+0x8eb5u);
    if(flag!=0)W(self+0xf639u)=(u16)(W(self+0xf639u)+1u);
    return (a&0xffff0000u)|status|flag;
}
