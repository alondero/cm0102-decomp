#include "cmre.h"
/* Preferred-image VA0079e5f0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/fullEAX, scratch218: saved selfL0/allocationL4/tagL8..L15,
   diagnosticsL18/L118. Ordinals advance only for successful allocations. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0079e5f0(u32 self,volatile u32 argument)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x218];
    u32 l=(u32)scratch,eax,owner,counter=0,value,code,saved,base,ordinal,version;
    frame.state=0xffffffff; frame.handler=0x0095ee66;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    owner=D(self+0x13);
    D(l)=self;
    W(self+8)=5;
    eax=cm_fn_00944e46(20);
    D(self+4)=eax;
    if (!eax) {
        cm_fn_00944cff(0x00a6a2e0,0,0,l+0x118,l+0x18);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x118,l+0x18,0x62);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    B(l+8)=5; D(l+9)=0xffffffff; D(l+0xd)=0xffffffff;
    D(l+0x11)=0xffffffff; B(l+0x15)=10;
    eax=cm_fn_00944cf1(0x2b);
    D(l+4)=eax;
    value=argument;
    frame.state=0;
    if (eax) {
        counter=1;
        code=D(0x009cfa20);
        eax=cm_fn_0092fe80(eax,owner,0,code,9000,8000,0xffffffff,1,l+8,0,value,1);
    } else eax=0;
    saved=D(l);
    frame.state=0xffffffff;
    base=D(saved+4); D(base)=eax;
    eax=cm_fn_00944cf1(0x2b);
    D(l+4)=eax;
    frame.state=1;
    if (eax) {
        code=D(0x009cfa24);
        ordinal=counter; ++counter;
        eax=cm_fn_0092fe80(eax,owner,ordinal,code,9000,8000,0xffffffff,1,0,0,value,1);
    } else eax=0;
    saved=D(l);
    frame.state=0xffffffff;
    base=D(saved+4); D(base+4)=eax;
    eax=cm_fn_00944cf1(0x2b);
    D(l+4)=eax;
    frame.state=2;
    if (eax) {
        code=D(0x009cfa30);
        ordinal=counter; ++counter;
        eax=cm_fn_0092fe80(eax,owner,ordinal,code,7000,5250,0xffffffff,1,0,0,value,1);
    } else eax=0;
    saved=D(l);
    frame.state=0xffffffff;
    base=D(saved+4); D(base+8)=eax;
    eax=cm_fn_00944cf1(0x2b);
    D(l+4)=eax;
    frame.state=3;
    if (eax) {
        code=D(0x009cfa28);
        ordinal=counter; ++counter;
        eax=cm_fn_0092fe80(eax,owner,ordinal,code,9000,8000,0xffffffff,1,0,0,value,1);
    } else eax=0;
    saved=D(l);
    frame.state=0xffffffff;
    base=D(saved+4); D(base+12)=eax;
    eax=cm_fn_00944cf1(0x2b);
    D(l+4)=eax;
    frame.state=4;
    if (eax) {
        code=D(0x009cfa2c);
        eax=cm_fn_0092fe80(eax,owner,counter,code,7000,6000,0xffffffff,1,0,0,value,1);
    } else eax=0;
    saved=D(l); base=D(saved+4); D(base+16)=eax;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
