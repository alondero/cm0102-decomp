#include "cmre.h"
/* Preferred-image VA007ec2a0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX, SEH960a72, scratch20c:
   allocation L0/self L4/year WORD L8/diagnostics Lc,L10c.
   The uninitialized upper year WORD is passed only as ctor766e10 args8/10.
   Original766e5e..766f1b uses only their low WORDs; 766f50 forwards EDX
   to932c30, which overwrites it at932c68 before use/call. No initialization
   of the original unwritten year bytes is inferred. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_00766e10(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008af520(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007ec2a0(u32 self,volatile u32 input_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x20c];
    u32 l=(u32)scratch,year,owner,eax,allocation,input,count=0,table;
    u32 common,label,version,index,receiver;
    frame.state=0xffffffffu;frame.handler=0x00960a72;
    frame.prev=CM_FS(volatile u32,0);CM_FS(volatile u32,0)=(u32)&frame;
    year=W(0x00ae2c92);owner=D(self+0x13);
    D(l+4)=self;W(l+8)=(u16)year;W(self+8)=7;
    eax=cm_fn_00944e46(0x1c);D(self+4)=eax;
    if (!eax) {
        cm_fn_00944cff(0x00a7ad00,0,0,l+0x10c,l+0xc);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x10c,l+0xc,0x69);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    allocation=cm_fn_00944cf1(0x2b);D(l)=allocation;
    input=input_arg;frame.state=0;
    if (allocation) {
        count=1;label=D(0x009cfee0);
        eax=cm_fn_0092fe80(allocation,owner,0,label,4000,2000,0xffffffffu,0,0,0,input,1);
    } else eax=0;
    table=D(self+4);frame.state=0xffffffffu;D(table)=eax;
    for (index=1;index<=2;++index) {
        allocation=cm_fn_00944cf1(0x25d);D(l)=allocation;frame.state=index;
        if (allocation) {
            common=D(0x009cf930);label=D(0x009cfee0+index*4u);
            eax=count++;
            eax=cm_fn_008af520(allocation,owner,eax,label,0,0,common,0,3,input,1);
        } else eax=0;
        table=D(self+4);frame.state=0xffffffffu;D(table+index*4u)=eax;
    }
    allocation=cm_fn_00944cf1(0xa2);D(l)=allocation;
    year=W(l+8);frame.state=3;
    if (allocation) {
        common=D(0x009cf930);label=D(0x009cfeec);eax=count++;
        eax=cm_fn_00766e10(allocation,owner,eax,label,4000,2000,common,3,year,10,year,input,1);
    } else eax=0;
    receiver=D(l+4);frame.state=0xffffffffu;table=D(receiver+4);D(table+0xc)=eax;
    for (index=4;index<=5;++index) {
        allocation=cm_fn_00944cf1(0x25d);D(l)=allocation;frame.state=index;
        if (allocation) {
            common=D(0x009cf92c);label=D(0x009cfee0+index*4u);
            eax=count++;
            eax=cm_fn_008af520(allocation,owner,eax,label,0,0,common,0,3,input,1);
        } else eax=0;
        receiver=D(l+4);frame.state=0xffffffffu;table=D(receiver+4);D(table+index*4u)=eax;
    }
    allocation=cm_fn_00944cf1(0xa2);D(l)=allocation;frame.state=6;
    if (allocation) {
        common=D(0x009cf92c);label=D(0x009cfef8);
        eax=cm_fn_00766e10(allocation,owner,count,label,1000,500,common,3,year,10,year,input,1);
    } else eax=0;
    receiver=D(l+4);table=D(receiver+4);D(table+0x18)=eax;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
