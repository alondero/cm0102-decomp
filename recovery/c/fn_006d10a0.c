#include "cmre.h"
/* Preferred-image VA006d10a0/base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4; final native countdown returns full EAX0. */
extern u32 CM_THISCALL cm_fn_00717410(u32 self);
extern u32 CM_THISCALL cm_fn_005b5290(u32 self,s8 mode,u32 value,u32 zero);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006d10a0(u32 self,volatile u32 mode_arg)
{
    u32 group=(u32)(s32)(s8)mode_arg;
    u32 item,detail,a,score,receiver;
    long double live;
    volatile long long converted;
    mode_arg=20;
    item=self+group*8920u+0x4796u;
    do {
        if((s8)B(item+0x19)>=0) {
            detail=D(item+0x6d);
            a=(u32)(s32)(s8)B(detail+0x43);
            a*=10u;
            live=(long double)(s32)a;
            a=(u32)(s32)(s8)B(detail+0x33);
            a*=10u;
            live+=(long double)(s32)a;
            a=(u32)(s32)(s8)B(detail+0x27);
            a*=10u;
            live+=(long double)(s32)a;
            live+=F(item+0x4d);
            converted=(long long)live; /* ADR0006: __ftol006d1110. */
            score=(u32)converted;
            a=cm_fn_00717410(item);
            if(a&1u)goto next;
            a=cm_fn_00717410(item);
            if(a&2u)score+=500u;
            else {
                a=cm_fn_00717410(item);
                if(a&4u) {
                    a=cm_fn_00717410(item);
                    if(a&0x800u)score-=500u;
                    else {
                        a=cm_fn_00717410(item);
                        if(a&0x80u)score-=500u;
                        else score-=1000u;
                    }
                } else {
                    a=cm_fn_00717410(item);
                    if(a&8u) {
                        a=cm_fn_00717410(item);
                        if(!(a&0x800u)) {
                            a=cm_fn_00717410(item);
                            if(!(a&0x80u))score-=300u;
                        }
                    } else {
                        a=cm_fn_00717410(item);
                        if(a&0x10u)score+=1000u;
                        else {
                            a=cm_fn_00717410(item);
                            if(a&0x20u)score+=2000u;
                        }
                    }
                }
            }
            if((s16)score>8500) {
                a=B(item+0x19);
                receiver=self+group*6371u+0x91e2u;
                (void)cm_fn_005b5290(receiver,(s8)a,0x00800000,0);
            }
        }
next:
        item+=0x1beu;
        mode_arg=mode_arg-1u;
    } while(mode_arg);
    return 0;
}
