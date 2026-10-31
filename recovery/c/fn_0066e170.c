#include "cmre.h"
/* Preferred-image VA0066e170/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX, SEH95c882, scratch20c:
   allocationL0/copy of inputL4/yearWORDL8/diagnosticsLc,L10c.
   66e2b6 reads year plus unwritten upperWORD, passed only as766e10
   args8/10: original766e5e..766f1b consumes only lowWORDs;766f50's
   932c30 boundary overwrites incomingEDX at932c68 before use/call.
   No initialization of the year upperWORD is inferred. */
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

u32 CM_THISCALL cm_fn_0066e170(u32 self,volatile u32 input_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x20c];
    u32 l=(u32)scratch,year,input,owner,eax,allocation,count=0,table;
    u32 value,sign,label,other,index,version;
    frame.state=0xffffffffu;frame.handler=0x0095c882;
    frame.prev=CM_FS(volatile u32,0);CM_FS(volatile u32,0)=(u32)&frame;
    year=W(0x00ae2c92);input=input_arg;owner=D(self+0x13);
    D(l+4)=input;W(l+8)=(u16)year;W(self+8)=7;
    eax=cm_fn_00944e46(0x1c);D(self+4)=eax;
    if (!eax) {
        cm_fn_00944cff(0x009eb1d4,0,0,l+0x10c,l+0xc);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x10c,l+0xc,0x74);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    value=D(0x00dd7110);sign=(s32)value<0?0xffffffffu:0;
    value=(value^sign)-sign;
    if ((s32)value<0x3e047) D(l+4)=0;
    allocation=cm_fn_00944cf1(0x25d);D(l)=allocation;frame.state=0;
    if (allocation) {
        label=D(0x009d0084);count=1;
        eax=cm_fn_008af520(allocation,owner,0,label,2500,1200,0xffffffffu,0,0,input,1);
    } else eax=0;
    table=D(self+4);frame.state=0xffffffffu;D(table)=eax;
    allocation=cm_fn_00944cf1(0xa2);D(l)=allocation;frame.state=1;
    if (allocation) {
        year=W(l+8);label=D(0x009d0094);eax=count++;
        eax=cm_fn_00766e10(allocation,owner,eax,label,1500,750,0xffffffffu,2,year,10,year,input,1);
    } else eax=0;
    table=D(self+4);frame.state=0xffffffffu;D(table+4)=eax;
    for (index=2;index<=5;++index) {
        allocation=cm_fn_00944cf1(0x2b);D(l)=allocation;frame.state=index;
        if (allocation) {
            if (index==4) other=D(l+4);
            else other=input;
            if (index==2) label=D(0x009d008c);
            else if (index==3) label=D(0x009d0098);
            else if (index==4) label=D(0x009d009c);
            else label=D(0x009d00a0);
            eax=count++;
            eax=cm_fn_0092fe80(allocation,owner,eax,label,
                index==3?1500:(index==5?500:1000),index==5?400:500,
                0xffffffffu,0,0,index==4?1:0,other,index==2?0:1);
        } else eax=0;
        table=D(self+4);frame.state=0xffffffffu;D(table+index*4u)=eax;
    }
    allocation=cm_fn_00944cf1(0x2b);D(l)=allocation;frame.state=6;
    if (allocation) {
        other=D(l+4);label=D(0x009d0090);
        eax=cm_fn_0092fe80(allocation,owner,count,label,1000,500,0xffffffffu,0,0,0,other,1);
    } else eax=0;
    table=D(self+4);D(table+0x18)=eax;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
