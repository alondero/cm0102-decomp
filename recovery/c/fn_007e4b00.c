#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 a,u32 b,u32 c,u32 d,u32 e);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 a,u32 b,u32 c);
u32 CM_CDECL cm_fn_007e4b00(u32 first,u32 second)
{
    volatile u8 scratch[0x200]; s8 a,b,c,d; s32 delta1,delta2;
    if(first==0u || second==0u) goto format;
    a=*(volatile s8 *)(first+0x19u); b=*(volatile s8 *)(first+0x1au);
    if(a>=b) return 0u;
    c=*(volatile s8 *)(second+0x1au); d=*(volatile s8 *)(second+0x19u);
    delta1=(s32)b-(s32)a; delta2=(s32)c-(s32)d;
    if(delta1>delta2) return 1u;
    if(delta1!=delta2) return 0u;
    if(b>c) return 1u;
    return 0u;
format:
    cm_fn_00944cff(0x00a7a088u,0u,0u,(u32)&scratch[0x100],(u32)&scratch[0]);
    { u32 text=cm_fn_0090d130();
      cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&scratch[0x100],(u32)&scratch[0],0xd8u); }
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0u);
    CM_GLOBAL(volatile u32,0x00b67a34)=0u;
    return 0u;
}

