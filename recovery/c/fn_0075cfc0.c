#include "cmre.h"

/* Preferred-image VA 0075cfc0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD4/full EAX, message Le0-frame scratch L0. Preserve late arguments,
   18 setters, RNG calls/order, original receivers and guard-path EAX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_75cfc0;
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00757f30(u32);
extern u32 CM_THISCALL cm_fn_007dab90(u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_STDCALL cm_fn_0075cfc0(volatile u32 first,volatile u32 second,
                             volatile u32 third,volatile u32 fourth)
{
    volatile frame_75cfc0 frame;
    u8 scratch[0xe0];
    u32 local=(u32)scratch,previous,result,subject,a,b,record,value,kind,other,owner;
    s32 rating;
    previous=CM_FS(volatile u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095da1b;
    frame.prev=previous;
    CM_FS(volatile u32,0)=(u32)&frame;
    result=previous;
    subject=first;
    if (subject==0) goto done;
    result=D(subject+0xcf);
    if (result==0) goto done;
    result=cm_fn_0053b900(subject);
    if (result!=0) goto done;
    a=fourth;
    if (a==0) goto done;
    result=D(a+0x61);
    if (result==0) goto done;
    b=third;
    if (b==0) goto done;
    result=D(b+0x61);
    if (result==0) goto done;
    cm_fn_0077bbe0(local,0xbe4,0);
    record=D(subject+0xcf);
    frame.state=0;
    value=D(record);
    cm_fn_00785790(local,0,value);
    record=D(subject+0xcf);
    other=D(record+4);
    value=D(other+0x33);
    cm_fn_00785790(local,1,value);
    record=D(subject+0xcf);
    other=D(record+8);
    value=D(other+0x33);
    cm_fn_00785790(local,2,value);
    record=D(subject+0xcf);
    other=D(record+0xc);
    value=D(other+0x33);
    cm_fn_00785790(local,3,value);
    value=D(a);
    cm_fn_00785790(local,4,value);
    other=D(a+4);
    value=D(other+0x33);
    cm_fn_00785790(local,5,value);
    other=D(a+8);
    value=D(other+0x33);
    cm_fn_00785790(local,6,value);
    other=D(a+0xc);
    value=D(other+0x33);
    cm_fn_00785790(local,7,value);
    value=D(b);
    cm_fn_00785790(local,8,value);
    other=D(b+4);
    value=D(other+0x33);
    cm_fn_00785790(local,9,value);
    other=D(b+8);
    value=D(other+0x33);
    cm_fn_00785790(local,10,value);
    other=D(b+0xc);
    value=D(other+0x33);
    cm_fn_00785790(local,11,value);
    value=D(subject);
    cm_fn_00785790(local,12,value);
    other=second;
    value=D(other);
    cm_fn_00785790(local,13,value);
    value=cm_fn_0090cfc0(5);
    rating=(s8)B(a+0x5b);
    value+=10;
    if (rating>(s32)value) value=cm_fn_0090cfc0(2);
    else value=cm_fn_0090cfc0(2)+2;
    cm_fn_00785790(local,14,value);
    kind=cm_fn_00757f30(subject);
    cm_fn_00785790(local,15,kind);
    record=D(subject+0x53);
    if (record!=0) other=D(record);
    else other=0xffffffffu;
    owner=D(0x00b74430);
    value=cm_fn_007dab90(owner,subject,kind,other);
    cm_fn_00785790(local,16,value);
    cm_fn_00785790(local,17,2);
    owner=D(0x00b7431c);
    result=(u32)cm_fn_007861e0(owner,local,subject);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
