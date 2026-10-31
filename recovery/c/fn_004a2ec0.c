#include "cmre.h"
/* Preferred-image VA004a2ec0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/ST0 extended precision. Scratch204: conversion L0,
   diagnostic buffers L4/L104. Physical dispatch table reads retained. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

long double CM_STDCALL cm_fn_004a2ec0(u32 stats,volatile s8 selector)
{
    u8 scratch[0x204];
    u32 l=(u32)scratch,index,target,offset,sum,i,denominator,name,extension,line,version;
    volatile long double value;
    if (!stats) {
        name=l+0x104; extension=l+4; line=0x901;
        goto diagnostic;
    }
    index=(u32)(s32)selector-1;
    if (index>18) goto invalid;
    target=D(0x004a3150+index*4);
    switch (target) {
    case 0x004a2f1d:
        D(l)=B(stats+4);
        return (long double)(s32)D(l);
    case 0x004a2f34:
        denominator=B(stats+5);
        if (!denominator) goto zero;
        value=(long double)(s32)D(stats+6);
        D(l)=denominator;
        return value/(long double)(s32)D(l);
    case 0x004a2f5a: offset=0xa; goto sum_five;
    case 0x004a2f7d: offset=0xf; goto sum_five;
    case 0x004a2fa0: offset=0x14; goto sum_five;
    case 0x004a2fc3: return (long double)(s32)D(stats+0x19);
    case 0x004a2fd0: return (long double)(s32)D(stats+0x2d);
    case 0x004a2fdd: return (long double)(s32)D(stats+0x41);
    case 0x004a2fea: return (long double)(s32)D(stats+0x55);
    case 0x004a2ff7: offset=0x69; break;
    case 0x004a300e: offset=0x6a; break;
    case 0x004a3025: offset=0x6b; break;
    case 0x004a303c: offset=0x6d; break;
    case 0x004a3053: offset=0x6e; break;
    case 0x004a306a: offset=0x6f; break;
    case 0x004a3081: offset=0x70; break;
    case 0x004a3098: offset=0x71; break;
    case 0x004a30af: offset=0x72; break;
    case 0x004a30c6: offset=0x73; break;
    default: goto invalid;
    }
    D(l)=B(stats+offset);
    return (long double)(s32)D(l);
sum_five:
    sum=0;
    for (i=0;i<5;++i) sum+=(u32)(s32)(s8)B(stats+offset+i);
    D(l)=sum;
    return (long double)(s32)D(l);
invalid:
    name=l+4; extension=l+0x104; line=0x9a1;
diagnostic:
    cm_fn_00944cff(0x0099ca94,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
zero:
    return (long double)F(0x00968970);
}
