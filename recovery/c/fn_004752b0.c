#include "cmre.h"

/* Preferred-image VA 004752b0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch2e4: saved BYTEs L2/L3, message L4, diagnostics Le4/L1e4.
   The first getter BYTE stays cached; the other two are loaded at their
   original late setter sites. Full EAX is retained for indirect callers. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785830(u32,u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_004752b0(volatile u32 first_arg,volatile u32 second_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x2e4];
    static const u32 values[19]={0,0,2,0,0x41,8,6,0,0,0xffffffffu,
                                0xffffffffu,0,0,0,0,0,0,0,0};
    u32 local=(u32)scratch,first,second,owner,result,version,value,index;
    u8 cached;
    frame.state=0xffffffffu;
    frame.handler=0x00955e1b;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0077bbe0(local+4,0,0);
    second=second_arg;
    first=first_arg;
    owner=D(0x00b7431c);
    frame.state=0;
    result=cm_fn_00786b70(owner,first,second,local+4);
    if (result==0) {
        cm_fn_00944cff(0x0098e598,0,0,local+0x1e4,local+0xe4);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x1e4,local+0xe4,0x3929);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
    } else {
        cached=(u8)cm_fn_00785830(local+4,2);
        B(local+2)=(u8)cm_fn_00785830(local+4,4);
        B(local+3)=(u8)cm_fn_00785830(local+4,5);
        result=cm_fn_007fe600(0x00b74450,0x00475520,0x0047a9e0,1,0x0047c070,0);
        if (result!=0) {
            for (index=0;index<19;++index) {
                if (index==0) value=first;
                else if (index==1) value=second;
                else if (index==11) value=(u32)(s32)(s8)cached;
                else if (index==12) value=(u32)(s32)(s8)B(local+2);
                else if (index==13) value=(u32)(s32)(s8)B(local+3);
                else value=values[index];
                result=cm_fn_007ff270(0x00b74450,index,value,0);
            }
        }
    }
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
