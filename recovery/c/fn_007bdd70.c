#include "cmre.h"
/* Preferred-image VAs, base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl3/RET0. All20indexed callers consume AL only. */
extern s16 CM_CDECL cm_fn_005407e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u8 CM_CDECL cm_fn_00544410(u32,s8);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define F(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u8 CM_CDECL cm_fn_007bdd70(u32 first,u32 second,u32 mode)
{
    u32 a,b,c,threshold,magnitude,sum;
    s32 high,value,remainder;
    s16 rating;
    long double live;
    volatile double saved;
    volatile long long converted;
    if(!first)return 0;
    a=D(first+0x69);
    if(!a)return 0;
    threshold=35u-(u32)(s32)(s8)mode;
    value=B(second+0x18);
    if(value<22)threshold+=(22u-(u32)value)*2u;
    rating=cm_fn_005407e0(second,first,0);
    a=D(second+0x39);
    b=D(first+0x39);
    if(a==b) {
        a=D(first+0x42);b=D(first+0x3e);
        a=cm_fn_0054cdb0(0x00ae2c90,b,a);
        if((s32)a>150) {
            a=D(second+0x42);b=D(second+0x3e);
            a=cm_fn_0054cdb0(0x00ae2c90,b,a);
            if((s32)a>150) {
                a=D(second+0x42);b=D(second+0x3e);
                live=(long double)(s32)rating;
                live*=F(0x00968940);
                /* Native007bde22 FSTP m64 rounds before the next call. */
                saved=(double)live;
                a=cm_fn_0054cdb0(0x00ae2c90,b,a);
                c=(a+1300u)*5u;
                live=(long double)(s32)c;
                if(live>(long double)saved) {
                    a=D(second+0x42);b=D(second+0x3e);
                    a=cm_fn_0054cdb0(0x00ae2c90,b,a);
                    c=(a+1300u)*5u;
                    live=(long double)(s32)c;
                } else live=(long double)saved;
                converted=(long long)live; /* ADR0006: __ftol007bde6e. */
                rating=(s16)(u32)converted;
            }
        }
    }
    if(rating>5000) {
        c=10000u-(u32)(s32)rating;
        /* Native signed IMUL EF9DB22D, SAR6 and sign correction. */
        high=(s32)(((long long)(s32)c*(-274877907LL))>>32);
        high>>=6;
        high+=(u32)high>>31;
        threshold+=(u32)high;
    }
    a=D(first+0x69);
    value=B(a+0x19);
    value=(s32)(s8)cm_fn_00544410(a,(s8)value)-10;
    magnitude=value<0?0u-(u32)value:(u32)value;
    a=D(first);b=D(second);sum=a+b;
    remainder=(s32)sum%17;
    c=(u32)remainder+magnitude*4u-8u;
    return (u8)((s32)c>(s32)threshold);
}
