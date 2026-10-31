#include "cmre.h"
/* Preferred-image VA00906ac0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX, SEH965352, scratch208:
   allocation L0, year WORD L4, diagnostics L8/L108.
   906b89 loads a DWORD containing the year and unwritten upper bytes.
   Only ctor766e10 arguments8/10 receive it: original766e5e/766e67,
   766e9d/766ee3..766ef2 use their low WORDs exclusively. The base ctor
   receives neither argument. No upper-year value is invented here. */
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

u32 CM_THISCALL cm_fn_00906ac0(u32 self,volatile u32 input_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x208];
    u32 l=(u32)scratch,year,owner,eax,allocation,input,count=0,table;
    u32 common,label,version,index;
    frame.state=0xffffffffu;frame.handler=0x00965352;
    frame.prev=CM_FS(volatile u32,0);CM_FS(volatile u32,0)=(u32)&frame;
    year=W(0x00ae2c92);
    owner=D(self+0x13);W(l+4)=(u16)year;
    W(self+8)=7;
    eax=cm_fn_00944e46(0x1c);D(self+4)=eax;
    if (!eax) {
        cm_fn_00944cff(0x00aa1f80,0,0,l+0x108,l+8);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x108,l+8,0x70);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    allocation=cm_fn_00944cf1(0xa2);D(l)=allocation;
    input=input_arg;frame.state=0;
    if (allocation) {
        year=W(l+4);common=D(0x009cf590);count=1;
        label=D(0x009cfba8);
        eax=cm_fn_00766e10(allocation,owner,0,label,3000,0,common,3,year,10,year,input,1);
    } else eax=0;
    table=D(self+4);frame.state=0xffffffffu;D(table)=eax;
    /* Each null allocation leaves count unchanged. The three middle rows
       share constants but retain their own label read, SEH state and store. */
    for (index=1;index<=5;++index) {
        allocation=cm_fn_00944cf1(0x2b);D(l)=allocation;
        frame.state=index;
        if (allocation) {
            common=D(0x009cf590);
            if (index==1) label=D(0x009cfc3c);
            else if (index==2) label=D(0x009cfba0);
            else if (index==3) label=D(0x009cfb94);
            else if (index==4) label=D(0x009cfb98);
            else label=D(0x009cfb9c);
            eax=count++;
            eax=cm_fn_0092fe80(allocation,owner,eax,label,index==5?4000:5000,
                index==1?0:500,common,0,0,index==2?1:0,input,index==1?0:1);
        } else eax=0;
        table=D(self+4);frame.state=0xffffffffu;D(table+index*4u)=eax;
    }
    allocation=cm_fn_00944cf1(0x25d);D(l)=allocation;frame.state=6;
    if (allocation) {
        common=D(0x009cf590);label=D(0x009cfba4);
        eax=cm_fn_008af520(allocation,owner,count,label,0xdac,100,common,0,0,input,1);
    } else eax=0;
    table=D(self+4);D(table+0x18)=eax;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
