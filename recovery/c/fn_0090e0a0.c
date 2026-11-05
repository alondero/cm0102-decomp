#include "cmre.h"
/* Preferred-image VA0090e0a0/base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL7/RET28; retain the last helper's upper EAX and final AL slot. */
extern u32 CM_CDECL cm_fn_00946a04(void);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32,u32,u32,u32);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
extern u32 CM_CDECL cm_fn_0053ab90(u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

static u32 span_0090e0a0(u32 span_low,u32 span_high)
{
    u32 a=cm_fn_00946a04();
    unsigned long long product=cm_fn_00945e40(a,(u32)((s32)a>>31),span_low,span_high);
    return (u32)cm_fn_00945e80((u32)product,(u32)(product>>32),32768,0);
}

__attribute__((target("no-sse3")))
u32 CM_STDCALL cm_fn_0090e0a0(volatile u32 club_arg,volatile u32 mode_arg,
                             volatile u32 person_arg,volatile u32 detail_index_arg,
                             volatile u32 flag5_arg,volatile u32 flag6_arg,
                             volatile u32 flag7_arg)
{
    volatile struct { u32 scratch[10];u32 prev,handler,state; } local;
    u32 l=(u32)&local.scratch[0];
    u32 a,b,c,d,club,person,detail,year,age,mode,tries,target,src,dst,i,score;
    s16 rating;
    long long product;
    long double live;
    volatile long long converted;
    local.state=0xffffffffu;local.handler=0x009655a0;
    local.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&local.prev;
    a=D(0x00ae236c);if((s32)a>=30000)a=30000;D(l)=a;
    c=person_arg;
    club=club_arg;
    a=D(0x00ae23bc);person=a+c*110u;
    d=detail_index_arg;a=D(0x00ae23c4);
    person_arg=a+d*70u;
    d=W(0x00ae2c92);year=(d/3u)*3u;
    D(person)=c;
    a=D(club+0x53);
    if(a) {
        a=cm_fn_00539c60(a);D(person+4)=a;
        c=D(club+0x53);a=cm_fn_0053a1b0(c);D(person+8)=a;
        d=D(0x00ae23f8);D(person+12)=d;
    } else {
        a=cm_fn_00946a04();c=D(0x00ae2398);
        product=(long long)(s32)a*(long long)(s32)c;
        a=(u32)cm_fn_00945e80((u32)product,(u32)((unsigned long long)product>>32),32768,0);
        c=D(0x00ae23f0);d=c+a*60u;D(person+4)=d;
        a=cm_fn_00946a04();c=D(0x00ae239c);
        product=(long long)(s32)a*(long long)(s32)c;
        a=(u32)cm_fn_00945e80((u32)product,(u32)((unsigned long long)product>>32),32768,0);
        c=D(0x00ae23f4);d=c+a*60u;D(person+8)=d;
        a=D(0x00ae23f8);D(person+12)=a;
    }
    a=flag5_arg;
    if(a) {
        age=year-17u;a=span_0090e0a0(365,0);
        a=(u32)cm_fn_00549ef0(l+8,a,age);
        local.state=0;
        (void)cm_fn_00417c10(person+0x10,l+0x10,a);
    } else {
        a=span_0090e0a0(14,0);age=year-a-17u;
        a=span_0090e0a0(365,0);
        a=(u32)cm_fn_00549ef0(l+0x10,a,age);
        local.state=1;
        (void)cm_fn_00417c10(person+0x10,l+8,a);
    }
    local.state=0xffffffffu;
    a=cm_fn_0053ab90(person,0x00ae2c90);W(person+0x18)=(u16)(s16)(s8)a;
    c=D(club+0x53);D(person+0x1a)=c;
    a=D(club);c=D(0x009d0120);
    if(a==c || a==D(0x009d0134) || a==D(0x009d0128) ||
       a==D(0x009d01a0) || a==D(0x009d0124)) {
        a=D(0x009cf424);d=a*145u;a=D(0x00ae23a8);
        D(person+0x1e)=a+d*2u;
    } else D(person+0x1e)=0;
    B(person+0x22)=0;B(person+0x23)=0;
    a=cm_fn_0054a110(l+0x10);local.state=2;
    (void)cm_fn_00417c10(person+0x29,l+8,a);
    local.state=0xffffffffu;
    a=cm_fn_0054a110(l+0x10);local.state=3;
    (void)cm_fn_00417c10(person+0x31,l+8,a);
    local.state=0xffffffffu;
    a=cm_fn_0053b900(club);
    if(a) {
        D(person+0x39)=0;D(person+0x24)=club;
        B(person+0x3d)=0;B(person+0x28)=11;
    } else {
        D(person+0x39)=club;D(person+0x24)=0;
        B(person+0x3d)=11;B(person+0x28)=0;
    }
    age=year-1u;a=span_0090e0a0(365,0);
    a=(u32)cm_fn_00549ef0(l+0x10,a,age);local.state=4;
    (void)cm_fn_00417c10(person+0x3e,l+8,a);
    year+=6u;local.state=0xffffffffu;
    a=span_0090e0a0(365,0);
    a=(u32)cm_fn_00549ef0(l+0x18,a,year);local.state=5;
    (void)cm_fn_00417c10(person+0x46,l+0x20,a);
    c=(u32)(s32)(s16)W(club+0x80);local.state=0xffffffffu;
    D(person+0x52)=0;D(person+0x4e)=(u32)((s32)c/5);
    a=span_0090e0a0(10,0);
    B(person+0x57)=10;B(person+0x56)=(u8)(a+1u);
    B(person+0x58)=10;B(person+0x59)=10;B(person+0x5a)=10;
    B(person+0x5b)=10;B(person+0x5c)=10;
    d=person_arg;
    B(person+0x5d)=10;B(person+0x5f)=2;
    D(person+0x61)=d;D(person+0x65)=0;D(person+0x69)=0;B(person+0x6d)=0;
    a=cm_fn_0053b900(club);c=flag6_arg;
    B(person+0x5e)=a?(c?8:4):(c?2:1);
    a=D(l);D(l)=a;D(l+4)=(u32)((s32)a>>31);
    tries=0;mode=(u32)(s32)(s8)mode_arg;
    for(;;) {
        a=cm_fn_00946a04();c=D(l+4);b=D(l);
        {
            unsigned long long v=cm_fn_00945e40(a,(u32)((s32)a>>31),b,c);
            a=(u32)cm_fn_00945e80((u32)v,(u32)(v>>32),32768,0);
        }
        c=D(0x00ae23c4);++tries;d=0;
        if(mode<=7u) {
            target=D(0x0090e740+mode*4u);
            src=c+a*70u;
            switch(target) {
            case 0x0090e4d8:d=B(src+0x0f)==20;break;
            case 0x0090e4f4:d=(s8)B(src+0x11)>=15;if(d)d=(s8)B(src+0x18)>=15;break;
            case 0x0090e51b:d=(s8)B(src+0x11)>=15;if(d)d=(s8)B(src+0x19)>=15;break;
            case 0x0090e542:d=(s8)B(src+0x11)>=15;if(d)d=(s8)B(src+0x17)>=15;break;
            case 0x0090e55e:d=(s8)B(src+0x13)>=15;if(d)d=(s8)B(src+0x18)>=15;break;
            case 0x0090e57a:d=(s8)B(src+0x13)>=15;if(d)d=(s8)B(src+0x19)>=15;break;
            case 0x0090e596:d=(s8)B(src+0x13)>=15;if(d)d=(s8)B(src+0x17)>=15;break;
            case 0x0090e5b2:d=(s8)B(src+0x15)>=15;break;
            default:__builtin_unreachable(); /* Guarded immutable original table. */
            }
        }
        if(d || tries>=1000u)break;
    }
    detail=person_arg;club=club_arg;
    src=c+a*70u;dst=detail;
    for(i=0;i<17u;++i){d=D(src);D(dst)=d;src+=4;dst+=4;}
    d=W(src);W(dst)=(u16)d;
    a=cm_fn_0053b900(club);rating=(s16)W(club+0x80);
    if(!a) {
        if(rating>5000){live=(long double)rating;live*=Q(0x00970a80);live+=Q(0x00968ea8);goto convert;}
        score=(u32)((s32)rating/25)-100u;
    } else {
        if(rating>5000)score=(u32)((s32)rating/100)+40u;
        else {live=(long double)rating;live*=Q(0x00970a78);live-=Q(0x00968f50);goto convert;}
    }
    goto adjust;
convert:
    converted=(long long)live; /* ADR0006: __ftol0090e66d, both input branches. */
    score=(u32)converted;
adjust:
    if(flag5_arg){a=span_0090e0a0(41,0);score+=0xffffffe2u-a;}
    if(flag6_arg){a=span_0090e0a0(41,0);score+=0xffffffe2u-a;}
    detail=person_arg;a=detail_index_arg;
    W(detail+5)=(u16)score;W(detail+7)=(u16)score;
    D(detail)=a;W(detail+9)=0;W(detail+0x0b)=0;W(detail+0x0d)=0;
    (void)D(0x00dd7ef0); /* Native loads ECX; native85add0 does not consume it. */
    a=cm_fn_0085add0(club,0);
    if(a){c=flag7_arg;B(detail+4)=(u8)c;}else B(detail+4)=0;
    c=club+0xd7u;b=0;
    while(D(c)!=0) {++b;c+=4;if(b>=50u)goto done;}
    D(c)=person;
done:
    CM_FS(u32,0)=local.prev;
    return (a&0xffffff00u)|(u8)b;
}
