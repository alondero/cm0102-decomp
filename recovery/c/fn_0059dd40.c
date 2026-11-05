#include "cmre.h"

/* Preferred-image VAs, base 00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2 / RET8; all three indexed callers test full EAX. */
extern u8 CM_THISCALL cm_fn_005986b0(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_STDCALL cm_fn_0059f300(u32 *,u32,u32,s8);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))

static int below(s32 hi,u32 lo,u32 threshold)
{
    s32 high=(s32)threshold<0?-1:0;
    return hi<high || (hi==high && lo<threshold);
}

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059dd40(u32 self,u32 club,u32 account)
{
    u32 a,c,lo,threshold,delta,first,second,total;
    s32 hi;
    u8 mode;
    volatile double scale;
    long double live;
    volatile long long converted;
    /* The native ECX save is reloaded before 0059f300, whose original body
       overwrites ECX before using it; its four stack arguments are explicit. */
    (void)self;
    mode=cm_fn_005986b0(account);
    if(mode==0xfe)scale=2.0;
    else if(mode==0xff)scale=W(club+0x80)>=3500?0.4:2.0;
    else {
        if(mode)return 0;
        c=(u32)(s32)W(club+0x80);
        if((s32)c>3500)return 0;
        hi=(s32)D(account+4);
        threshold=c*25u;
        if(hi>((s32)threshold<0?-1:0))return 0;
        if(hi==((s32)threshold<0?-1:0)) {
            lo=D(account);
            if(lo>=threshold)return 0;
        }
        scale=0.4;
    }
    a=cm_fn_0090cfc0(7000);
    c=(u32)(s32)W(club+0x80);
    a+=3500u;
    if((s32)c>=(s32)a)return 0;
    a=D(club+0xbf);
    if(a && D(a+0x69)) {
        live=(long double)CM_GLOBAL(volatile double,0x00969000);
        live/=scale;
        converted=(long long)live; /* ADR0006: __ftol CALL 0059de10. */
        if(cm_fn_0090cfc0((u32)converted))return 0;
        a=D(club+0xbf);a=D(a+0x69);
        a=(u32)(s32)B(a+0x20);
        if(!cm_fn_0090cfc0(a))return 0;
        c=(u32)(s32)W(club+0x80);
        if((s32)c<1250) {
            lo=D(account);threshold=0u-c*450u;hi=(s32)D(account+4);
            if(below(hi,lo,threshold)){delta=c*450u;goto update;}
        }
        c=(u32)(s32)W(club+0x80);
        lo=D(account);threshold=c*0xfffffed4u;hi=(s32)D(account+4);
        delta=c*(below(hi,lo,threshold)?300u:150u);
    } else {
        live=(long double)CM_GLOBAL(volatile double,0x00969000);
        live/=scale;
        converted=(long long)live; /* ADR0006: __ftol CALL 0059decd. */
        if(cm_fn_0090cfc0((u32)converted))return 0;
        if(!cm_fn_0090cfc0(10))return 0;
        c=(u32)(s32)W(club+0x80);
        if((s32)c<1250) {
            lo=D(account);threshold=0u-c*450u;hi=(s32)D(account+4);
            if(below(hi,lo,threshold)){delta=c*450u;goto update;}
        }
        c=(u32)(s32)W(club+0x80);
        lo=D(account);threshold=c*0xfffffed4u;hi=(s32)D(account+4);
        delta=c*(below(hi,lo,threshold)?300u:150u);
    }
update:
    first=D(account+0x154);second=D(account+0xb4);
    first+=delta;second+=delta;
    D(account+0x154)=first;
    D(account+0xb4)=second;
    total=delta+lo;
    a=D(account+4);
    D(account)=total;
    D(account+4)=a+((s32)delta<0?0xffffffffu:0u)+(total<delta);
    cm_fn_0059f300((u32 *)club,delta,0,4);
    return 1;
}
