#include "cmre.h"
/* Preferred-image VA004A3250/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/ST0 extended precision. L=afterSUB204: conversion L0,
   adjacent diagnostic buffers L4/L104. Retain the physical jump-table read,
   the one float32 store in selector17, signed integer guards, and the original
   unguarded BYTE-denominator division in selectors14/15. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define F(a) CM_GLOBAL(volatile float,(u32)(a))
#define Q(a) CM_GLOBAL(volatile double,(u32)(a))
extern u32 CM_THISCALL cm_fn_004a4850(u32,u32);
extern long double CM_STDCALL cm_fn_004a2ec0(u32,volatile s8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static void diagnostic(u32 name,u32 extension,u32 line)
{
    u32 version;
    cm_fn_00944cff(0x0099ca94u,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,name,extension,line);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u)=0;
}

long double CM_THISCALL cm_fn_004a3250(u32 self,u32 stats,volatile s8 selector)
{
    volatile u8 scratch[0x204];
    u32 l=(u32)scratch,index,target,offset,kind,club,base,opponent,count,byte;
    s16 denominator;
    volatile long double value,other;
    if(stats==0) {
        diagnostic(l+0x104u,l+4u,0x9d2);
        goto zero;
    }
    index=(u32)(s32)selector-1u;
    if(index>0x11u) goto invalid;
    target=D(0x004a3970u+index*4u);
    switch(target) {
    case 0x004a32b3u: offset=0xc; break;
    case 0x004a32ccu: offset=0xe; break;
    case 0x004a32e5u: offset=0xf; break;
    case 0x004a32feu: offset=0x11; break;
    case 0x004a3317u: offset=0x12; break;
    case 0x004a3330u: offset=0x13; break;
    case 0x004a3349u: offset=0x14; break;
    case 0x004a3362u: offset=0x15; break;
    case 0x004a337bu: offset=0x16; break;
    case 0x004a3394u: offset=0x17; break;
    case 0x004a33adu: offset=0x18; break;
    case 0x004a33c6u: offset=0x19; break;
    case 0x004a34ceu: kind=13; goto comparative;
    case 0x004a35e1u: kind=14; goto comparative;
    case 0x004a36cau: kind=15; goto comparative;
    case 0x004a37b3u: kind=16; goto comparative;
    case 0x004a33dfu: kind=17; goto comparative;
    case 0x004a38ceu:
        byte=B(stats+0xcu);
        if(byte==0) goto zero;
        D(l)=B(stats+0xfu);
        value=(long double)(s32)D(l);
        D(l)=byte;
        return value/(long double)(s32)D(l);
    default: goto invalid;
    }
    D(l)=B(stats+offset);
    return (long double)(s32)D(l);

comparative:
    club=D(stats+8u);
    base=D(0x00ae23b4u);
    club=base+club*581u;
    if(club==0) {
        diagnostic(l+4u,l+0x104u,0x8e2);
        if(kind==17) D(l)=0;
        else value=(long double)F(0x00968970u);
    } else {
        opponent=cm_fn_004a4850(self,club);
        if(opponent==0) {
            if(kind==17) D(l)=0;
            else value=(long double)F(0x00968970u);
        } else {
            value=cm_fn_004a2ec0(opponent,1);
            if(kind==17) F(l)=(float)value;
        }
    }
    if(kind==17) {
        other=cm_fn_004a3250(self,stats,1);
        value=(long double)F(l);
        value=value*(long double)Q(0x00968968u);
        if(value>other) goto zero;
        byte=B(stats+0xcu);
        if(byte==0) goto zero;
        D(l)=(u32)(s32)(s16)W(stats+0x1au);
        value=(long double)(s32)D(l);
        D(l)=byte;
        return value/(long double)(s32)D(l);
    }
    count=D(stats+0x29u);
    D(l)=count;
    other=(long double)(s32)D(l);
    value=value*(long double)F(0x00968a7cu);
    /* FCOMPP/TEST AH,41 jumps only for an ordered greater-than result;
       equality and unordered results continue through the remaining guards. */
    if(value>other) goto zero;
    if(kind==13) {
        denominator=(s16)W(stats+0x1du);
        if(denominator<=0x14) goto zero;
        if(denominator<=0x1f4 && (s32)denominator<=(s32)count/4) goto zero;
        D(l)=(u32)(s32)(s16)W(stats+0x1fu);
        value=(long double)(s32)D(l);
        D(l)=(u32)(s32)denominator;
        value=value/(long double)(s32)D(l);
        return value*(long double)F(0x00968960u);
    }
    if(kind==14 || kind==15) {
        if((s32)count<0x96) goto zero;
        D(l)=(u32)(s32)(s16)W(stats+(kind==14 ? 0x21u : 0x23u));
        value=(long double)(s32)D(l);
        D(l)=B(stats+0xcu);
        return value/(long double)(s32)D(l);
    }
    denominator=(s16)W(stats+0x25u);
    if(denominator<=0x18) {
        if(denominator>4 && (s32)count/(s32)denominator<=0x2d) goto ratio;
        if((s32)count<=0x21c || denominator<=0xa) goto zero;
    }
ratio:
    D(l)=(u32)(s32)(s16)W(stats+0x27u);
    value=(long double)(s32)D(l);
    D(l)=(u32)(s32)denominator;
    value=value/(long double)(s32)D(l);
    return value*(long double)F(0x00968960u);
invalid:
    diagnostic(l+4u,l+0x104u,0xa7f);
zero:
    return (long double)F(0x00968970u);
}
