#include "cmre.h"
/* Preferred-image VA004a3ee0/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5/RET20; every normal terminal returns full EAX1, fatal terminal EAX0. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern long double CM_STDCALL cm_fn_004a2ec0(u32,volatile s8);
extern long double CM_THISCALL cm_fn_004a3250(u32,u32,volatile s8);
extern u32 CM_THISCALL cm_fn_004a4850(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static void diagnostic(u32 name,u32 extension,u32 line)
{
    u32 version;
    cm_fn_00944cff(0x0099ca94,0,0,name,extension);
    /* The three earlier pushes belong to this following formatter. */
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
}

static u32 associated(u32 self,u32 stats,u32 l)
{
    u32 index=D(stats+8);
    u32 base=D(0x00ae23b4);
    u32 address=base+index*581u;
    if(address)return cm_fn_004a4850(self,address);
    diagnostic(l+4,l+0x104,0x8e2);
    return 0;
}

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_004a3ee0(u32 self,volatile u32 stats_arg,
                              volatile u32 selector_arg,volatile u32 out_arg,
                              volatile u32 capacity_arg,volatile u32 flag_arg)
{
    /* Native L=ESP after SUB204: conversion DWORD/float L0, two adjacent
       diagnostic buffers L4/L104. No scratch initialization is added. */
    volatile u8 scratch[0x204];
    u32 l=(u32)scratch,a,b,c,index,target,name,src,dst,count,offset;
    u32 stats;
    s32 selector,amount,denominator;
    long double prior,live,own;
    volatile long long converted;
    a=out_arg;
    if(!a || (s32)capacity_arg<51) {
        diagnostic(l+4,l+0x104,0xbe5);
        return 0;
    }
    selector=(s32)(s8)selector_arg;
    index=(u32)selector-13u;
    name=0x0099cc54;
    if(index<=4) {
        target=D(0x004a47ec+index*4);
        if(target==0x004a3f26)name=0x0098f858;
        else name=0x0099cc4c; /* all other pinned rows target004a3f1f */
    }
    count=0xffffffffu;src=name;
    do {
        if(!count)break;
        --count;a=B(src);++src;
    }while((u8)a);
    count=~count;src-=count;
    dst=out_arg;
    a=count>>2;
    while(a) {b=D(src);D(dst)=b;src+=4;dst+=4;--a;}
    a=count&3;
    while(a) {b=B(src);B(dst)=(u8)b;++src;++dst;--a;}
    stats=stats_arg;
    if(!stats) {
        diagnostic(l+0x104,l+4,0xbfd);
        return 0;
    }
    index=(u32)selector-1u;
    if(index>16) {
        diagnostic(l+4,l+0x104,0xcc7);
        return 0;
    }
    target=D(0x004a4800+index*4);
    switch(target) {
    case 0x004a3fa5:
        a=B(stats+0x0d);
        if(a && flag_arg) {
            b=B(stats+0x0c);b-=a;c=out_arg;
            cm_fn_00944c9f(c,0x0099cc58,b,a);
        } else {
            c=out_arg;a=B(stats+0x0c);
            cm_fn_00944c9f(c,0x0098806c,a);
        }
        return 1;
    case 0x004a4017:offset=0x0e;goto byte_value;
    case 0x004a4027:offset=0x0f;goto byte_value;
    case 0x004a4054:
        a=B(stats+0x10);
        if(a && flag_arg) {
            c=out_arg;b=B(stats+0x11);
            cm_fn_00944c9f(c,0x0099cc58,a,b);
        } else {
            c=out_arg;
            cm_fn_00944c9f(c,0x0098806c,a);
        }
        return 1;
    case 0x004a40c4:offset=0x12;goto byte_value;
    case 0x004a40f1:offset=0x13;goto byte_value;
    case 0x004a4101:offset=0x14;goto byte_value;
    case 0x004a412e:offset=0x15;goto byte_value;
    case 0x004a415b:offset=0x16;goto byte_value;
    case 0x004a416b:offset=0x17;goto byte_value;
    case 0x004a4198:offset=0x18;goto byte_value;
    case 0x004a41c5:offset=0x19;goto byte_value;
    case 0x004a41d5:
        a=associated(self,stats,l);
        if(a) {
            prior=cm_fn_004a2ec0(a,1);
            F(l)=(float)prior; /* native FSTPm32, only selector17 */
        } else D(l)=0;
        own=cm_fn_004a3250(self,stats,1);
        prior=(long double)F(l);
        prior*=Q(0x00968968);
        if(!(prior>own)) { /* includes unordered: native AH&(C3|C0) */
            a=B(stats+0x0c);
            if(a) {
                c=(u32)(s32)(s16)W(stats+0x1a);D(l)=c;
                live=(long double)(s32)D(l);
                D(l)=a;
                live/=(long double)(s32)D(l);
                c=out_arg;
                cm_fn_00944c9f(c,0x0098fe94,(double)live);
            }
        }
        return 1;
    case 0x004a42ec:
    case 0x004a4420:
    case 0x004a4502:
    case 0x004a4616:
        a=associated(self,stats,l);
        if(a)prior=cm_fn_004a2ec0(a,1);
        else prior=(long double)F(0x00968970);
        amount=(s32)D(stats+0x29);D(l)=(u32)amount;
        own=(long double)(s32)D(l);
        prior*=F(0x00968a7c);
        if(prior>own)return 1; /* unordered follows the native work path */
        if(target==0x004a42ec) {
            denominator=(s32)(s16)W(stats+0x1d);
            if(denominator<=20)return 1;
            if(denominator<=500) {
                /* Native signed /4 uses sign correction before SAR. */
                a=(u32)amount+(((u32)(amount>>31))&3u);
                if(denominator<=((s32)a>>2))return 1;
            }
            c=(u32)(s32)(s16)W(stats+0x1f);D(l)=c;
            live=(long double)(s32)D(l);
            live*=F(0x00968960);
            D(l)=(u32)denominator;
            live/=(long double)(s32)D(l);
            converted=(long long)live; /* __ftol CALL004a43d4, site1. */
            c=out_arg;
            cm_fn_00944c9f(c,0x0098e8d4,(u32)converted);
            return 1;
        }
        if(target==0x004a4420 || target==0x004a4502) {
            if(amount<150)return 1;
            offset=target==0x004a4420?0x21:0x23;
            c=(u32)(s32)(s16)W(stats+offset);D(l)=c;
            live=(long double)(s32)D(l);
            a=B(stats+0x0c);D(l)=a;
            live/=(long double)(s32)D(l);
            c=out_arg;
            cm_fn_00944c9f(c,0x0099ccc8,(double)live);
            return 1;
        }
        denominator=(s32)(s16)W(stats+0x25);
        if(denominator<=24) {
            if(denominator<=4 || amount/denominator>45) {
                if(amount<=540 || denominator<=10)return 1;
            }
        }
        c=(u32)(s32)(s16)W(stats+0x27);D(l)=c;
        live=(long double)(s32)D(l);
        live*=F(0x00968960);
        D(l)=(u32)denominator;
        live/=(long double)(s32)D(l);
        converted=(long long)live; /* __ftol CALL004a4724, site2. */
        c=out_arg;
        cm_fn_00944c9f(c,0x0098e8d4,(u32)converted);
        return 1;
    }
    __builtin_unreachable(); /* pinned read-only dispatch table exhausted */
byte_value:
    a=B(stats+offset);c=out_arg;
    cm_fn_00944c9f(c,0x0098806c,a);
    return 1;
}
