#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format,u32 zero1,u32 zero2,u32 buffer1,u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 message,u32 buffer,u32 mode);

u32 CM_THISCALL cm_fn_007f8420(u32 self)
{
    volatile u8 scratch[0x200];
    s32 global_index=(s32)CM_GLOBAL(volatile s16,0x00b77c7au);
    u32 table_index=(u32)global_index*0x300u;
    u32 record=*(volatile u32 *)(0x00b74c26u+table_index);
    if (record==0u)
        return 0u;

    u8 raw=(u8)*(volatile u8 *)record;
    u8 global_byte=CM_GLOBAL(volatile u8,0x00ae2364u);
    u8 delta=(u8)(raw-global_byte+0x10u);
    if ((s8)delta<0 || (s8)delta>0x0f) {
        cm_fn_00944cff(0x00a7b094u,0u,0u,(u32)&scratch[0x100],(u32)&scratch[0]);
        { u32 text=cm_fn_0090d130();
          cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&scratch[0x100],(u32)&scratch[0],0x454u); }
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0u);
        CM_GLOBAL(volatile u32,0x00b67a34u)=0u;
        return 0u;
    }
    u32 selected=*(volatile u32 *)(self+0x0cu+(u32)(s32)(s8)delta*4u);
    if (selected==0u)
        return 0u;
    s32 row=(s32)*(volatile s8 *)selected;
    u32 offset=(u32)row*688u;
    s32 value=*(volatile s32 *)(selected+offset+0x191u);
    return value==-2 ? 1u : 0u;
}
