#include "cmre.h"
/* Preferred-image VA005ae380/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX0or1; SEH959816/scratch2c4: countL0,dateL4,Lc,
   recordL14,L64 (79B each), date outputsLb4,Lbc, diagnosticsLc4,L1c4.
   Full copied records preserve all untouched fields. Original jump-table
   read, signed WORD comparisons, late formal reload and error-success retained. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_THISCALL cm_fn_0054a2d0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_005abc30(u32,u32);
extern u32 CM_THISCALL cm_fn_005aa680(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005ad780(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_005ab510(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_005aac70(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005ae380(u32 self, volatile u32 record_arg,
                              volatile u32 list_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x2c4];
    u32 l=(u32)scratch, list, head, source, destination, i, eax;
    u32 day,year,target,pointer1,pointer2,club,other,argument,result=1;
    u16 first,second,word;
    frame.state=0xffffffffu; frame.handler=0x00959816;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054a110(l+0xc);
    frame.state=0;
    cm_fn_0054a110(l+4);
    list=list_arg;
    B((u32)&frame.state)=1;
    D(l)=0;
    eax=D(list+2); D(list+0xa)=eax;
    head=cm_fn_0068d1b0(list);
    source=record_arg;
    if(source==0 || head==0) goto error;
    destination=l+0x64;
    for(i=0;i<19;++i){D(destination)=D(source);source+=4;destination+=4;}
    W(destination)=W(source); destination+=2; source+=2;
    B(destination)=B(source);
    source=head; destination=l+0x14;
    for(i=0;i<19;++i){D(destination)=D(source);source+=4;destination+=4;}
    W(destination)=W(source); destination+=2; source+=2;
    B(destination)=B(source);
    year=W(head+0x28); day=W(head+0x2a);
    /* 549ef0 observes only the first WORD and masks the second to16 bits. */
    eax=(u32)cm_fn_00549ef0(l+0xbc,day,year);
    cm_fn_00417c10(l+0xc,l+0xb4,eax);
    eax=(u32)(s32)(s16)cm_fn_0054a3f0(l+0xc);
    if(eax<=6){
        target=D(0x005ae684+eax*4u);
        switch(target){
        case 0x005ae451:eax=6;break;
        case 0x005ae458:eax=1;break;
        case 0x005ae45f:eax=2;break;
        case 0x005ae466:eax=5;break;
        default:eax=D(l);break;
        }
    }else eax=D(l);
    cm_fn_0054a2d0(l+0xc,eax,l+4);
    pointer1=D(l+0x78); pointer2=D(l+0x28); club=D(l+0x80);
    first=W(pointer1+0x69); second=W(pointer2+0x69);
    eax=cm_fn_0053b900(club);
    if(eax!=0) first=(u16)(first+0x15u);
    other=D(l+0x30);
    eax=cm_fn_0053b900(other);
    if(eax!=0) second=(u16)(second+0x15u);
    if((s16)first>(s16)second) goto prefer_head;
    if(first!=second) goto prefer_argument;
    pointer1=D(l+0x80); pointer2=D(l+0x30);
    word=W(pointer1+0x80);
    if((s16)word<=(s16)W(pointer2+0x80)) goto prefer_argument;
prefer_head:
    if(B(l+0x57)!=0xff) goto prefer_argument;
    cm_fn_005abc30(self,head);
    argument=record_arg;
    cm_fn_005aa680(self,argument,0);
    first=W(l+4); second=W(l+6);
    /* 5ae525 follows PUSH0: ESP+50 is L3c, not L40. */
    W(l+0x3e)=first; W(l+0x3c)=second;
    eax=cm_fn_005ad780(self,l+0x14,l,0);
    if(eax!=0){
        cm_fn_0068d040(eax);
        cm_fn_00944c94(eax);
        if(B(l+0x4e)==1){result=0;goto done;}
        cm_fn_005ab510(self,l+0x14);
    }
    cm_fn_005aa680(self,l+0x14,0);
    cm_fn_005aac70(self,l+0x14,argument,0);
    goto done;
prefer_argument:
    first=W(l+4); second=W(l+6);
    /* 5ae5a3 follows three pushes: ESP+a8 is L8c. */
    W(l+0x8e)=first; W(l+0x8c)=second;
    eax=cm_fn_005ad780(self,l+0x64,l,0);
    if(eax!=0){
        cm_fn_0068d040(eax);
        cm_fn_00944c94(eax);
        if(B(l+0x9e)==1){result=0;goto done;}
        cm_fn_005ab510(self,l+0x64);
    }
    cm_fn_005aa680(self,l+0x64,0);
    cm_fn_005aac70(self,l+0x64,head,0);
    goto done;
error:
    cm_fn_00944cff(0x009cc208,0,0,l+0x1c4,l+0xc4);
    eax=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x1c4,l+0xc4,0xde6);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
