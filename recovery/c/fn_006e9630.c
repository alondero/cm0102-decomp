#include "cmre.h"
/* Preferred-image VAs, base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS6/RET24. Wrapper006cfd34 forwards full EAX unchanged. */
extern u32 CM_STDCALL __attribute__((regparm(3)))
    cm_fn_006c38c0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u8 CM_THISCALL cm_fn_00714d30(volatile u8 *);
extern u32 CM_THISCALL cm_fn_006f5b20(u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006e9630(u32 self,volatile u32 item_arg,volatile u32 out_arg,
                              volatile u32 category,volatile u32 score,
                              volatile u32 flag,volatile u32 float_out_arg)
{
    /* Native caches these pointers before its first output store; later byte
       formals and the overwritten score slot retain their original accesses. */
    u32 item=item_arg;
    u32 float_out=float_out_arg;
    u32 out=out_arg;
    u32 a,c,d,index;
    s32 quotient;
    long double live;
    volatile long long converted;
    D(float_out)=0x3fc00000;
    index=(u32)(s32)(s8)B(item+0x27);
    c=D(self);
    a=c+index*54u+0xf5c9;
    d=B(a);B(a)=(u8)(d+1u);
    c=B(item+0x14);B(item+0x14)=(u8)(c+1u);
    a=(a&0xffffff00u)|B(out);
    if((u8)a!=1)return a;
    c=D(self);
    a=(a&0xffffff00u)|B(c+0x8eb3);
    if((u8)a)return a;
    a=(a&0xffffff00u)|(u8)flag;
    if((u8)a)goto alternative;
    /* Native006c38c0 first overwrites EDX from its stack argument, before
       reading it. The unused physical EDX input is not an engine input.
       Its EAX and ECX inputs below are the original live values. Only AL
       is consumed before either following RNG call overwrites full EAX;
       later ECX/EDX uses have explicit native overwrites. */
    a=cm_fn_006c38c0(a,0,c,item);
    if(a&1u)goto alternative;
    a=cm_fn_0090cfc0(15);
    d=a+9u;
    a=(u32)(s32)(s8)score;
    if((s32)a>(s32)d) {
        W(item+0x35)=(u16)(W(item+0x35)+0xfc18u);
        B(out)=4;D(float_out)=0x40a00000;
        return a;
    }
    a=cm_fn_0090cfc0(5);
    c=a+3u;
    a=(u32)(s32)(s8)score;
    if((s32)a<=(s32)c) {
        W(item+0x35)=(u16)(W(item+0x35)-25u);
        return a;
    }
    d=D(self);
    live=(long double)F(0x00968e70);
    c=(u32)(s32)(s16)W(d+0x8ed0);
    quotient=(s32)(((long long)(s32)c*0x2e8ba2e9LL)>>32);
    quotient>>=1;
    quotient+=(u32)quotient>>31;
    if((s16)quotient>45) {
        score=(u32)(s32)(s16)quotient;
        live=(long double)(s32)score;
        live=(long double)F(0x0096d64c)/live;
    }
    live*=F(0x0096d648);
    converted=(long long)live; /* ADR0006: physical __ftol CALL006e972c. */
    a=(u32)converted;
    W(item+0x35)=(u16)(W(item+0x35)+(u16)a);
    a=(a&0xffffff00u)|B(item+0x24);
    if((u8)a) {
        d=(u8)category;
        a=(a&0xffffff00u)|(u8)((u8)a+1u);
        B(item+0x24)=(u8)a;
        B(out)=3;D(float_out)=0x40600000;
        B(item+0x26)=(u8)d;
    } else {
        a=(a&0xffffff00u)|(u8)category;
        B(item+0x24)=1;
        B(out)=2;D(float_out)=0x40000000;
        B(item+0x25)=(u8)a;
    }
    return a;
alternative:
    a=cm_fn_0090cfc0(3000);
    if((s16)a>475) {
        W(item+0x35)=(u16)(W(item+0x35)+0xfd12u);
        B(out)=6;D(float_out)=0x40a00000;
        a=(a&0xffffff00u)|B(item+0x24);
        if((u8)a)B(item+0x26)=9;
        else B(item+0x25)=9;
        return a;
    }
    if((s16)a<=50) {
        /* Native714d30 changes only AL. Preserve its known upper EAX for
           the following push; native6f5b20 reads only that argument's byte. */
        a=(a&0xffffff00u)|cm_fn_00714d30((volatile u8 *)item);
        a=cm_fn_006f5b20(item,a);
        if(!(u8)a)return a;
    }
    a=(a&0xffffff00u)|B(item+0x24);
    W(item+0x35)=(u16)(W(item+0x35)+0xfe89u);
    if((u8)a) {
        c=(u8)category;
        a=(a&0xffffff00u)|(u8)((u8)a+1u);
        B(item+0x24)=(u8)a;
        B(out)=3;D(float_out)=0x40600000;
        B(item+0x26)=(u8)c;
    } else {
        d=(u8)category;
        B(item+0x24)=1;
        B(out)=2;D(float_out)=0x40000000;
        B(item+0x25)=(u8)d;
    }
    return a;
}
