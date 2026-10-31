#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32 self,u32 arg1,u32 arg2,u32 arg3,u32 arg4,u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self,u32 index,s32 value,u32 zero);
extern u8 CM_FASTCALL cm_fn_0054cfb0(u32 address);

void CM_CDECL cm_fn_008f4fa0(void)
{
    u32 initialized=cm_fn_007fe600(0x00b74450u,0x008f5080u,0x008f5f40u,0u,0x008f6690u,0u);
    if (initialized==0u)
        return;

    cm_fn_007ff270(0x00b74450u,0u,-1,0u);
    s32 value=(s8)cm_fn_0054cfb0(0x00ae2c90u);
    cm_fn_007ff270(0x00b74450u,1u,value,0u);
    u32 year=(u32)CM_GLOBAL(volatile u16,0x00ae2c92u);
    cm_fn_007ff270(0x00b74450u,2u,(s32)year,0u);
    cm_fn_007ff270(0x00b74450u,3u,1,0u);
    cm_fn_007ff270(0x00b74450u,4u,0,0u);
    cm_fn_007ff270(0x00b74450u,5u,0,0u);
    cm_fn_007ff270(0x00b74450u,6u,1,0u);
    cm_fn_007ff270(0x00b74450u,7u,1,0u);
    cm_fn_007ff270(0x00b74450u,8u,0,0u);
    cm_fn_007ff270(0x00b74450u,9u,0,0u);
}
