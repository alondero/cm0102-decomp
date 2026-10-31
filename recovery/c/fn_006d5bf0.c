/* WIP: original/recovered import-event mismatch, cases 0/17/35; wrapper matches listing, no instruction-backed cause found. */
#include "cmre.h"
extern void cm_fn_006d9e50(void);
extern u32 *CM_CDECL cm_fn_0090bde0(u32 first,u32 second);
extern void CM_THISCALL cm_fn_008025e0(u32 self,u32 first,u32 second,u32 third,u32 fourth);
u32 CM_THISCALL cm_fn_006d5bf0(u32 self,u8 enabled)
{
    u32 value=0xffffffffu;
    u32 address=0x00ad9c64u;
    u32 i;
    *(volatile u16 *)(self+0x0au)=(u16)value;
    *(volatile u32 *)(self+0x2d68u)=value;
    *(volatile u16 *)(self+6u)=0u;
    *(volatile u16 *)(self+8u)=0u;
    *(volatile u16 *)(self+0x0cu)=0u;
    *(volatile u16 *)(self+0x0eu)=0u;
    *(volatile u8 *)(self+5u)=0u;
    *(volatile u8 *)(self+2u)=0u;
    *(volatile u8 *)self=1u;
    *(volatile u8 *)(self+0x2db0u)=0x7du;
    *(volatile u8 *)(self+1u)=0xffu;
    *(volatile u8 *)(self+3u)=0u;
    *(volatile u32 *)(self+0x2dacu)=0u;
    *(volatile u8 *)(self+4u)=0u;
    CM_GLOBAL(volatile u16,0x00b68670u)=1u;
    CM_GLOBAL(volatile u16,0x00b68672u)=0x2bcu;
    CM_GLOBAL(volatile u8,0x00b68674u)=0u;
    CM_GLOBAL(volatile u32,0x00b68778u)=address;
    CM_GLOBAL(volatile u32,0x00b6877cu)=address;
    cm_fn_006d9e50();
    for(i=0u;i<16u;++i) {
        *(volatile u8 *)(self+0x10u+i)=0xffu;
        *(volatile u8 *)(self+0x20u+i)=0xffu;
        *(volatile u32 *)(self+0x2d6cu+i*4u)=0u;
    }
    if(enabled!=0u) {
        s16 di=*(volatile s16 *)0x00b77c7au;
        u32 *buffer=cm_fn_0090bde0(0x009c382cu,0x00a159a0u);
        u32 derived=(u32)(s32)di+self+0x20u;
        cm_fn_008025e0(0x00b74450u,derived,2u,(u32)buffer,1u);
    }
    return self;
}
