#include "cmre.h"
typedef void (CM_THISCALL *dispose_element_fn)(u32 self, u32 mode);
extern void CM_THISCALL cm_fn_00687970(void *self, void *buffer);
extern void CM_FASTCALL cm_fn_0049f450(int *arg);
extern void CM_FASTCALL cm_fn_00682300(u32 *arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);
void CM_THISCALL cm_fn_0063ceb0(void *self)
{
 volatile struct {u32 prev,handler,state;} frame; u32 p=(u32)self,value,index;
 frame.state=0xffffffffu;frame.handler=0x0095b848u;frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
 *(volatile u32 *)p=0x0096c264u;frame.state=0u;cm_fn_00687970(self,(void *)0);
 value=*(volatile u32 *)(p+0xbau);if(value){cm_fn_009452ca(value);*(volatile u32 *)(p+0xbau)=0u;}
 value=*(volatile u32 *)(p+0xeeu);if(value)cm_fn_009452ca(value);
 value=*(volatile u32 *)(p+0x30u);index=0u;
 if((s32)value>=0)do{u32 array=*(volatile u32 *)(p+0x0cu);u32 element=*(volatile u32 *)(array+index*4u);if(element){u32 vt=*(volatile u32 *)element;dispose_element_fn f=(dispose_element_fn)*(volatile u32 *)vt;f(element,1u);}value=*(volatile u32 *)(p+0x30u);++index;}while((s32)index<=(s32)value);
 value=*(volatile u32 *)(p+0x0cu);if(value){cm_fn_009452ca(value);*(volatile u32 *)(p+0x0cu)=0u;}
 value=*(volatile u32 *)(p+8u);if(value){cm_fn_0049f450((int *)value);cm_fn_00944c94((void *)value);}
 frame.state=0xffffffffu;cm_fn_00682300((u32 *)p);CM_FS(u32,0)=frame.prev;
}
