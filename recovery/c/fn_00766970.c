#include "cmre.h"

/* Preferred-image VA 00766970, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch218: current date L0, date temporaries L8/L10, diagnostics L18/L118.
   Nested weekday calls take no stack args: the three earlier pushes belong
   to the following seven-argument row setter. Keep their full EAX values. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00766970(u32 self,volatile u8 type_arg,
                              volatile u32 rounds_arg,volatile u32 flags_arg,
                              volatile u32 numbered_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x218];
    u32 local=(u32)scratch,type,output,rounds,flags,allocation,low,high,year,value,version,result;
    frame.prev=CM_FS(volatile u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095db6b;
    type=type_arg;
    CM_FS(volatile u32,0)=(u32)&frame;
    result=0;
    if (type!=0xff) goto done;
    output=numbered_arg;
    if (output!=0) D(output)=1;
    rounds=rounds_arg;
    flags=flags_arg;
    W(rounds)=3;
    W(flags)=0;
    allocation=cm_fn_00944e46(0xc3);
    if (allocation==0) {
        cm_fn_00944cff(0x00a5ab3c,0,0,local+0x118,local+0x18);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x118,local+0x18,0xb6);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    low=D(self+0xee);
    high=D(self+0xf2);
    D(local)=low;
    D(local+4)=high;
    W(allocation)=(u16)low;
    year=W(local+2);
    year=(u16)(year-W(self+0x40));
    frame.state=0;
    W(allocation+2)=(u16)year;
    B(allocation+4)=1;
    D(allocation+0x3d)=1;
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,0,0,value,2,3,16);
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,0,1,value,1,3,16);
    cm_fn_0068a1c0(allocation,0,2,0xffffffffu,0xffffffffu,0xffffffffu,0);
    value=cm_fn_0054c5b0(local,local+0x10,1);
    cm_fn_00417c10(local,local+8,value);
    low=W(local);
    W(allocation+0x41)=(u16)low;
    year=W(local+2);
    year=(u16)(year-W(self+0x40));
    W(allocation+0x43)=(u16)year;
    B(allocation+0x45)=1;
    D(allocation+0x7e)=1;
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,1,0,value,2,3,16);
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,1,1,value,1,3,16);
    cm_fn_0068a1c0(allocation,1,2,0xffffffffu,0xffffffffu,0xffffffffu,0);
    value=cm_fn_0054c5b0(local,local+8,1);
    cm_fn_00417c10(local,local+0x10,value);
    low=W(local);
    W(allocation+0x82)=(u16)low;
    year=W(local+2);
    year=(u16)(year-W(self+0x40));
    B(allocation+0x86)=1;
    W(allocation+0x84)=(u16)year;
    D(allocation+0xbf)=1;
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,2,0,value,2,3,16);
    value=cm_fn_0054a3f0(local);
    cm_fn_0068a1c0(allocation,2,1,value,1,3,16);
    cm_fn_0068a1c0(allocation,2,2,0xffffffffu,0xffffffffu,0xffffffffu,0);
    result=allocation;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
