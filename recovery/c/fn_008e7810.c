#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format,u32 zero1,u32 zero2,u32 buffer1,u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 message,u32 buffer,u32 mode);

void CM_THISCALL cm_fn_008e7810(u32 self,u32 first_ptr,u32 second_ptr)
{
    volatile u8 scratch[0x200];
    if (first_ptr==0u || second_ptr==0u) {
        cm_fn_00944cff(0x00a9e318u,0u,0u,(u32)&scratch[0x100],(u32)&scratch[0]);
        { u32 text=cm_fn_0090d130();
          cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&scratch[0x100],(u32)&scratch[0],0x485u); }
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0u);
        CM_GLOBAL(volatile u32,0x00b67a34u)=0u;
        return;
    }

    *(volatile u8 *)(self+0x2du)=0u;
    *(volatile u32 *)self=0u;
    *(volatile u8 *)(self+0x11u)=0x0bu;
    *(volatile u32 *)(self+4u)=0u;
    u32 second_value=*(volatile u32 *)second_ptr;
    *(volatile u32 *)(self+8u)=second_value;
    u32 first_value=*(volatile u32 *)first_ptr;
    *(volatile u32 *)(self+0x1bu)=0u;
    *(volatile u8 *)(self+0x39u)=0x0au;
    *(volatile u8 *)(self+0x3eu)=0x0au;
    *(volatile u32 *)(self+0x35u)=0u;
    *(volatile u32 *)(self+0x28u)=0xffffffffu;
    *(volatile u32 *)(self+0x24u)=0xffffffffu;
    u8 mode=(u8)(*(volatile u8 *)(self+0x30u)&0xf0u);
    *(volatile u32 *)(self+0x3au)=0u;
    *(volatile u8 *)(self+0x3fu)=0u;
    *(volatile u8 *)(self+0x40u)=0u;
    *(volatile u8 *)(self+0x2cu)=0u;
    *(volatile u8 *)(self+0x10u)=0u;
    *(volatile u8 *)(self+0x2eu)=0u;
    *(volatile u8 *)(self+0x2fu)=0u;
    *(volatile u32 *)(self+0x0cu)=first_value;
    *(volatile u8 *)(self+0x1fu)=1u;
    *(volatile u8 *)(self+0x30u)=mode;
}
