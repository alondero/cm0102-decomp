#include "cmre.h"
extern void CM_STDCALL cm_fn_009463af(u32 destination,u32 stride,u32 count,u32 constructor,u32 destructor);
extern void CM_FASTCALL cm_fn_006e9460(u32 self);
u32 CM_THISCALL cm_fn_006b7e50(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u32 result=self,prev;
    frame.state=0xffffffffu; frame.handler=0x0095d30cu; frame.prev=CM_FS(u32,0); CM_FS(u32,0)=(u32)&frame;
    cm_fn_009463af(self+0x4796u,0x1beu,0x28u,0x6eada0u,0x6ebd60u);
    frame.state=0u;
    cm_fn_009463af(self+0x9032u,0xd0u,2u,0x5961c0u,0x6dbb10u);
    ((volatile u8 *)&frame.state)[0]=1u;
    cm_fn_009463af(self+0x91e2u,0x18e3u,2u,0x5b27c0u,0x5a53c0u);
    ((volatile u8 *)&frame.state)[0]=2u;
    cm_fn_009463af(self+0xc3a8u,0x18e3u,2u,0x5b27c0u,0x5a53c0u);
    ((volatile u8 *)&frame.state)[0]=3u;
    cm_fn_006e9460(self+0xf5b2u);
    prev=frame.prev;
    *(volatile u32 *)(self+0x478eu)=0u;
    CM_FS(u32,0)=prev;
    return result;
}

