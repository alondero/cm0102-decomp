#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 a,u32 b,u32 c,u32 d,u32 e);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 a,u32 b,u32 c);
u32 CM_THISCALL cm_fn_006b59a0(u32 self,u32 key,u32 option)
{
    volatile u8 scratch[0x200];
    u32 i, row, count, ptr;
    if (key == 0u) {
        cm_fn_00944cff(0x00a15708u,0u,0u,(u32)&scratch[0x100],(u32)&scratch[0]);
        { u32 text=cm_fn_0090d130();
          cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&scratch[0x100],(u32)&scratch[0],0x65eu); }
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,key);
        CM_GLOBAL(volatile u32,0x00b67a34)=key;
        return 0u;
    }
    count=*(volatile u32 *)(self+0x21u);
    if ((s32)count<=0) return 0u;
    ptr=*(volatile u32 *)(self+0x19u);
    row=ptr+0x20u;
    for(i=0u;(s32)i<(s32)count;++i,row+=0x69u) {
        if(option!=0u) {
            u32 valid=*(volatile u32 *)(row+0x3bu);
            if(valid==0u) goto next;
        }
        if(*(volatile u32 *)row==key || *(volatile u32 *)(row+4u)==key) return 1u;
next:;
    }
    return 0u;
}

