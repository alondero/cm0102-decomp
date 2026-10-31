#include "cmre.h"
typedef void (CM_THISCALL *dispose_element_fn)(u32 self,u32 mode);
extern void CM_THISCALL cm_fn_00687970(u32 self,u32 buffer);
extern void CM_FASTCALL cm_fn_0049f450(u32 object);
extern void CM_FASTCALL cm_fn_00682300(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 pointer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
void CM_THISCALL cm_fn_0066c160(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u32 value,index;
    frame.state=0xffffffffu; frame.handler=0x0095c748u; frame.prev=CM_FS(u32,0); CM_FS(u32,0)=(u32)&frame;
    *(volatile u32 *)self=0x0096ce84u;
    frame.state=0u; cm_fn_00687970(self,0u);
    value=*(volatile u32 *)(self+0xbau);
    if(value){cm_fn_009452ca(value);*(volatile u32 *)(self+0xbau)=0u;}
    value=*(volatile u32 *)(self+0xeeu); if(value)cm_fn_009452ca(value);
    value=*(volatile u32 *)(self+0x30u); index=0u;
    if((s32)value>=0) do {
        u32 array=*(volatile u32 *)(self+0x0cu);
        u32 element=*(volatile u32 *)(array+index*4u);
        if(element){u32 vt=*(volatile u32 *)element;u32 target=*(volatile u32 *)(vt+0u);((dispose_element_fn)target)(element,1u);}
        value=*(volatile u32 *)(self+0x30u); ++index;
    } while((s32)index<=(s32)value);
    value=*(volatile u32 *)(self+0x0cu);
    if(value){cm_fn_009452ca(value);*(volatile u32 *)(self+0x0cu)=0u;}
    value=*(volatile u32 *)(self+8u);
    if(value){cm_fn_0049f450(value);cm_fn_00944c94(value);}
    frame.state=0xffffffffu;cm_fn_00682300(self);CM_FS(u32,0)=frame.prev;
}
