#include "cmre.h"

/* Preferred-image VAs, base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4. The wrapper006ce9ae forwards full EAX, which the native
   three-iteration countdown sets to zero on every returning path. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006ce9c0(u32 self,u32 category)
{
    u8 categories[3];
    u32 index,offset,a,b,c,d,e,f,total,numerator;
    long double live;
    volatile long long converted;
    categories[0]=(u8)category;
    categories[1]=3;
    categories[2]=4;
    for(index=0;index!=3;++index) {
        offset=(u32)(s32)(s8)categories[index];
        a=D(self+offset*4u+0x8f52);
        b=D(self+offset*4u+0x8f16);
        total=a+b;
        if((s32)total>0) {
            live=(long double)(s32)a;
            live/=(long double)(s32)total;
            live*=F(0x00968960);
            converted=(long long)live; /* ADR0006: __ftol006cea1e. */
            a=(u32)converted;
            B(self+offset+0xf5dd)=(u8)a;
            B(self+offset+0xf609)=(u8)(100u-(u8)a);
        } else {
            B(self+offset+0xf5dd)=50;
            B(self+offset+0xf609)=50;
        }
        a=D(self+offset*4u+0x8f2a);
        b=D(self+offset*4u+0x8f3e);
        total=a+b;
        if((s32)total>0) {
            live=(long double)(s32)a;
            live/=(long double)(s32)total;
            live*=F(0x00968960);
            converted=(long long)live; /* ADR0006: __ftol006cea6f. */
            a=(u32)converted;
            B(self+offset+0xf5d8)=(u8)a;
            B(self+offset+0xf60e)=(u8)(100u-(u8)a);
        } else {
            B(self+offset+0xf5d8)=50;
            B(self+offset+0xf60e)=50;
        }
        a=D(self+offset*4u+0x8f66);
        b=D(self+offset*4u+0x8f02);
        total=a+b;
        if((s32)total>0) {
            live=(long double)(s32)a;
            live/=(long double)(s32)total;
            live*=F(0x00968960);
            converted=(long long)live; /* ADR0006: __ftol006ceac0. */
            a=(u32)converted;
            B(self+offset+0xf613)=(u8)a;
            B(self+offset+0xf5d3)=(u8)(100u-(u8)a);
        } else {
            B(self+offset+0xf613)=50;
            B(self+offset+0xf5d3)=50;
        }
        c=D(self+offset*4u+0x8f2a);
        d=D(self+offset*4u+0x8f02);
        a=D(self+offset*4u+0x8f52);
        b=D(self+offset*4u+0x8f66);
        total=d+c;total+=a;total+=b;
        e=D(self+offset*4u+0x8f3e);total+=e;
        f=D(self+offset*4u+0x8f16);total+=f;
        if((s32)total>0) {
            numerator=d+c;numerator+=a;
            live=(long double)(s32)numerator;
            live/=(long double)(s32)total;
            live*=F(0x00968960);
            converted=(long long)live; /* ADR0006: __ftol006ceb3a. */
            a=(u32)converted;
            B(self+offset+0xf5ce)=(u8)a;
            B(self+offset+0xf604)=(u8)(100u-(u8)a);
        } else {
            B(self+offset+0xf5ce)=50;
            B(self+offset+0xf604)=50;
        }
    }
    return 0;
}
