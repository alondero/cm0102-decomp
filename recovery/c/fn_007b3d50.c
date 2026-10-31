#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self,u32 target);
extern void CM_THISCALL cm_fn_0068d0d0(u32 self,u32 buffer,u32 mode);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
void CM_THISCALL cm_fn_007b3d50(u32 self,u32 arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u32 edi,buffer,i,current;
    frame.prev=CM_FS(u32,0); frame.state=0xffffffffu; frame.handler=0x0095f4abu; CM_FS(u32,0)=(u32)&frame;
    edi=*(volatile u32 *)(self+0x170u);
    if(edi!=0u && arg!=*(volatile u32 *)(self+0x190u)) {
        cm_fn_0068d1f0(0x00b74438u,edi);
        cm_fn_00944c94((void *)edi);
        *(volatile u32 *)(self+0x170u)=0u;
    }
    *(volatile u32 *)(self+0x190u)=arg;
    if((s32)arg<=-1) goto done;
    current=*(volatile u32 *)(self+0x170u);
    if(current!=0u) goto done;
    buffer=cm_fn_00944cf1(0x51a8u);
    frame.state=0u;
    if(buffer==0u) {buffer=0u;goto store;}
    current=*(volatile u32 *)(self+0x190u);
    *(volatile u32 *)buffer=current;
    for(i=0u;i<50u;++i) *(volatile u32 *)(buffer+4u+i*0x1a2u)=0xffffffffu;
    cm_fn_0068d0d0(0x00b74438u,buffer,4u);
store:
    *(volatile u32 *)(self+0x170u)=buffer;
done:
    CM_FS(u32,0)=frame.prev;
}

