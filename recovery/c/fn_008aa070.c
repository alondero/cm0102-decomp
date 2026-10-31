#include "cmre.h"
extern u32 CM_THISCALL cm_fn_005b57d0(u32 self,u32 arg1,u32 arg2);
extern void CM_CDECL cm_fn_00944cff(u32 format,u32 zero1,u32 zero2,u32 buffer1,u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 message,u32 buffer,u32 mode);

u32 CM_THISCALL cm_fn_008aa070(u32 self,u32 arg1,u32 arg2)
{
    volatile u8 scratch[0x200];
    u8 low=(u8)arg1;
    if ((s8)low<0 || (s8)low>0x0a) {
        cm_fn_00944cff(0x00a8b94cu,0u,0u,(u32)&scratch[0x100],(u32)&scratch[0]);
        { u32 text=cm_fn_0090d130();
          cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&scratch[0x100],(u32)&scratch[0],0x1a25u); }
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0u);
        CM_GLOBAL(volatile u32,0x00b67a34u)=0u;
        return 0u;
    }
    if (*(volatile u32 *)(self+0xe40u)!=0u)
        return 0u;

    volatile u32 *source=(volatile u32 *)(self+0xe44u);
    volatile u32 *destination=(volatile u32 *)(self+0x4a63u);
    u32 i;
    for (i=0u;i<0xf07u;++i)
        destination[i]=source[i];
    volatile u16 *source_word=(volatile u16 *)(source+0xf07u);
    volatile u16 *destination_word=(volatile u16 *)(destination+0xf07u);
    *destination_word=*source_word;
    *((volatile u8 *)(destination_word+1))=*((volatile u8 *)(source_word+1));
    *(volatile u32 *)(self+0xc4deu)=1u;
    (void)cm_fn_005b57d0(self+0xe44u,arg1,arg2);
    *(volatile u32 *)(self+0x4a5fu)=1u;
    return 1u;
}

