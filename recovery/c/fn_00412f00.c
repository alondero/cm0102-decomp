#include "cmre.h"

/* Preferred-image VA 00412f00, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl5/RET0. Partial-AX callers retain the full incidental EAX. */
extern u32 CM_THISCALL cm_fn_00842c00(u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define F(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_00412f00(u32 person, volatile u32 club,
                          volatile u32 third, volatile u32 fourth,u32 flag)
{
    u32 a,c,d,stats,bp,bx,owner,base;
    long double live;
    volatile long long converted;
    c=flag;
    stats=D(person+0x61);
    if(stats && c) {
        bp=fourth;
        fourth=(u32)(s32)(s16)bp;
        live=(long double)(s32)fourth;
        live*=F(0x00967888);
        converted=(long long)live; /* ADR0006: __ftol00412f34. */
        W(stats+0x0d)=(u16)(W(stats+0x0d)+(u16)converted);
        a=D(person+0x61);
        bx=10000;
        if((s16)W(a+0x0d)>(s16)bx)W(a+0x0d)=(u16)bx;
        a=D(person+0x61);
        c=(c&0xffff0000u)|W(a+0x0d);
        if((s16)c>(s16)bp)bp=c;
        W(a+0x0d)=(u16)bp;
        a=club;
        if(a) {
            c=D(a+0x53);
            a=D(person+0x1a);
            if(c==a) {
                bp=third;
                stats=D(person+0x61);
                third=(u32)(s32)(s16)bp;
                live=(long double)(s32)third;
                live*=F(0x00967880);
                converted=(long long)live; /* ADR0006: __ftol00412f8b. */
                W(stats+9)=(u16)(W(stats+9)+(u16)converted);
                a=D(person+0x61);
                if((s16)W(a+9)>(s16)bx)W(a+9)=(u16)bx;
                c=D(person+0x61);
                a=(a&0xffff0000u)|W(c+9);
                if((s16)a<=(s16)bp)a=bp;
                W(c+9)=(u16)a;
                goto player_last;
            }
        }
        bp=third;
        a=fourth;
        bx=(u32)(s32)(s16)bp;
        stats=D(person+0x61);
        third=bx;
        c=bx+a;
        club=c;
        live=(long double)(s32)club;
        live*=F(0x00967878);
        converted=(long long)live; /* ADR0006: __ftol00412fd8. */
        W(stats+9)=(u16)(W(stats+9)+(u16)converted);
        a=D(person+0x61);
        c=10000;
        if((s16)W(a+9)>(s16)c)W(a+9)=(u16)c;
        c=(u32)((s32)bx/2);
        a=fourth;
        a=(u32)((s32)a/2);
        d=D(person+0x61);
        a+=c;
        c=(u32)(s32)(s16)W(d+9);
        if((s32)c>(s32)a)a=c;
        W(d+9)=(u16)a;
        bx=10000;
player_last:
        live=(long double)(s32)third;
        stats=D(person+0x61);
        live*=F(0x00967880);
        converted=(long long)live; /* ADR0006: __ftol0041302a. */
        W(stats+0x0b)=(u16)(W(stats+0x0b)+(u16)converted);
        a=D(person+0x61);
        if((s16)W(a+0x0b)>(s16)bx)W(a+0x0b)=(u16)bx;
        a=D(person+0x61);
        c=(c&0xffff0000u)|W(a+0x0b);
        if((s16)c<=(s16)bp)c=bp;
        W(a+0x0b)=(u16)c;
        d=D(0x00de1f54);
        c=D(d+0x856);
        if(c)return cm_fn_00842c00(c,person,1);
        return a;
    }
    a=D(person+0x69);
    if(!a || c)return a;
    c=(c&0xffff0000u)|W(a+0x0c);
    bp=fourth;
    if((s16)c<=(s16)bp)c=bp;
    owner=club;
    W(a+0x0c)=(u16)c;
    if(owner) {
        a=D(owner+0x53);
        c=D(person+0x1a);
        if(a==c) {
            c=D(person+0x69);
            bx=third;
            a=(a&0xffff0000u)|W(c+8);
            if((s16)a<=(s16)bx)a=bx;
            W(c+8)=(u16)a;
            goto staff_last;
        }
    }
    bx=third;
    c=(u32)((s32)(s16)bx/2);
    a=(u32)((s32)(s16)bp/2);
    d=D(person+0x69);
    a+=c;
    c=(u32)(s32)(s16)W(d+8);
    if((s32)c>(s32)a)a=c;
    W(d+8)=(u16)a;
staff_last:
    stats=D(person+0x69);
    a=(a&0xffff0000u)|W(stats+0x0a);
    if((s16)a>(s16)bx)bx=a;
    W(stats+0x0a)=(u16)bx;
    c=D(0x00b67a44);
    if(!owner)return a;
    a=D(owner);
    a*=73u;
    d=D(c);
    W(a+d+6)=(u16)(W(a+d+6)+1000u);
    a=a+d+6;
    a=D(owner);
    base=D(c);
    a*=73u;
    c=a+base+6;
    a=(a&0xffff0000u)|W(c);
    if((s16)a<1)a=1;
    else if((s16)a>10000)a=10000;
    else a=(u32)(s32)(s16)a;
    W(c)=(u16)a;
    a=D(owner);
    c=D(0x00b67a44);
    a*=73u;
    d=D(c);
    c=(c&0xffff0000u)|W(a+d+6);
    a=a+d+6;
    if((s16)c<6500)W(a)=6500;
    return a;
}
