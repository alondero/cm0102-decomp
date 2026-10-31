#include "cmre.h"

/* Preferred-image VA 00475070, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original message/dialog helpers remain boundaries. All nineteen dialog
   slots and full EAX, including the diagnostic and final setter, are retained. */
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_00475070(volatile u32 first_arg,volatile u32 second_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x2e0];
    u32 local=(u32)scratch,first,second,manager,result,version,mode;
    u32 slot,value;
    frame.state=0xffffffffu;
    frame.handler=0x00955dfb;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0077bbe0(local,0,0);
    second=second_arg;
    first=first_arg;
    manager=D(0x00b7431c);
    frame.state=0;
    result=cm_fn_00786b70(manager,first,second,local);
    if (result==0) {
        cm_fn_00944cff(0x0098e598,0,0,local+0x1e0,local+0xe0);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                      local+0x1e0,local+0xe0,0x38f4);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    mode=D(local)==0x18;
    result=cm_fn_007fe600(0x00b74450,0x00475520,0x0047a9e0,1,0x0047c070,0);
    if (result==0) goto done;
    for (slot=0;slot<19;++slot) {
        if (slot==0) value=first;
        else if (slot==1) value=second;
        else if (slot==2) value=1;
        else if (slot==3) value=mode;
        else if (slot==4) value=0x41;
        else if (slot==5) value=8;
        else if (slot==6) value=6;
        else if (slot==9 || slot==10) value=0xffffffffu;
        else value=0;
        result=cm_fn_007ff270(0x00b74450,slot,value,0);
    }
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
