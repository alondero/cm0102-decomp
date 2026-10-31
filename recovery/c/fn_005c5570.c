#include "cmre.h"

/* Preferred-image VA 005c5570, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch2e4: allocation L0, message L4, diagnostics Le4/L1e4.
   Keep copied argument slot, late record reads, SEH states and full EAX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_5c5570;
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_007875e0(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005c5570(u32 self,volatile u32 record_arg)
{
    volatile frame_5c5570 frame;
    u8 scratch[0x2e4];
    u32 local=(u32)scratch,previous,record,result,first,second,recipient,owner,value,other,version;
    previous=CM_FS(volatile u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095a059;
    frame.prev=previous;
    record=record_arg;
    CM_FS(volatile u32,0)=(u32)&frame;
    first=D(record+0xc);
    second=D(record+0x10);
    recipient=first==second ? D(record+0x14) : second;
    result=D(self+0x34d4);
    if (result==0) {
        result=cm_fn_00944cf1(0xe);
        D(local)=result;
        frame.state=0;
        if (result!=0) result=cm_fn_0068d020(result);
        frame.state=0xffffffffu;
        D(self+0x34d4)=result;
        if (result==0) {
            cm_fn_00944cff(0x009cd0a0,0,0,local+0x1e4,local+0xe4);
            version=cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x1e4,local+0xe4,0x982);
            result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
            D(0x00b67a34)=0;
            goto done;
        }
    }
    owner=D(self+0x34d4);
    result=(u32)cm_fn_0068d0d0(owner,(u32)&record_arg,4);
    if (recipient==0) goto done;
    record=record_arg;
    if (B(record+0x18)!=1) goto done;
    owner=D(0x00b7431c);
    result=cm_fn_007875e0(owner,recipient);
    if (result==0) goto done;
    record=record_arg;
    B(record+0x18)=2;
    record=record_arg;
    value=D(record+0x10);
    owner=D(0x00b7431c);
    result=cm_fn_007875e0(owner,value);
    if (result==0) {
        record=record_arg;
        value=D(record+0x14);
        owner=D(0x00b7431c);
        result=cm_fn_007875e0(owner,value);
        if (result==0) goto done;
    }
    cm_fn_0077bbe0(local+4,6,0);
    record=record_arg;
    frame.state=1;
    other=D(record+0x10);
    value=D(other);
    cm_fn_00785790(local+4,0,value);
    record=record_arg;
    other=D(record+0x14);
    value=D(other);
    cm_fn_00785790(local+4,1,value);
    record=record_arg;
    other=D(record+0xc);
    value=D(other);
    cm_fn_00785790(local+4,2,value);
    record=record_arg;
    value=(u32)(s32)(s16)W(record);
    cm_fn_00785790(local+4,3,value);
    record=record_arg;
    value=W(record+2);
    cm_fn_00785790(local+4,4,value);
    cm_fn_00785790(local+4,5,2);
    record=record_arg;
    recipient=D(record+0x10);
    first=D(record+0xc);
    if (first==recipient) recipient=D(record+0x14);
    owner=D(0x00b7431c);
    result=(u32)cm_fn_007861e0(owner,local+4,recipient);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
