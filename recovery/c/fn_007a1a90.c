#include "cmre.h"

/* Preferred-image VA 007a1a90, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original partial EBX calculation contributes only its signed low BYTE to
   message field11. The threshold branches produce low values4 or2, or1
   on the earlier special branch. Scratch outputs remain uninitialized. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_007a25c0(u32,u32);
extern u8 CM_CDECL cm_fn_007bd9d0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007a2650(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_THISCALL cm_fn_007a1a90(u32 self,u32 first,volatile u32 second_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xe8];
    u32 local=(u32)scratch,prev,result,second,low,high,pointer,value;
    u32 manager,record,owner,field;
    s32 offset,threshold;
    u8 type;
    prev=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095f07b;
    frame.prev=prev;
    CM_FS(u32,0)=(u32)&frame;
    result=prev;
    if (first==0) goto done;
    result=D(first+0x39);
    if (result==0) goto done;
    second=second_arg;
    if (second==0) goto done;
    result=D(second+0x39);
    if (result==0) goto done;
    high=D(second+0x42);
    low=D(second+0x3e);
    result=cm_fn_0054cdb0(0x00ae2c90,low,high);
    offset=(s32)(s8)B(second+0x56);
    threshold=21-offset;
    if ((s32)result>1000 && (s8)B(second+0x59)>10) type=1;
    else {
        threshold=threshold*5+90;
        type=(s32)result>=threshold?4:2;
    }
    value=cm_fn_007a25c0(self,second);
    B(local+1)=(u8)value;
    cm_fn_007bd9d0(second,first,local+6,local+4);
    cm_fn_007a2650(self,first,second,local+2,local);
    cm_fn_0077bbe0(local+8,0x1392,0);
    value=D(first);
    frame.state=0;
    cm_fn_00785790(local+8,0,value);
    for (field=1;field<4;++field) {
        pointer=D(first+field*4);
        value=D(pointer+0x33);
        cm_fn_00785790(local+8,field,value);
    }
    value=D(second);
    cm_fn_00785790(local+8,4,value);
    for (field=1;field<4;++field) {
        pointer=D(second+field*4);
        value=D(pointer+0x33);
        cm_fn_00785790(local+8,field+4,value);
    }
    value=(u32)(s32)(s16)W(local+2);
    cm_fn_00785790(local+8,8,value);
    value=(u32)(s32)(s8)B(local);
    cm_fn_00785790(local+8,9,value);
    value=(u32)(s32)(s8)B(local+1);
    cm_fn_00785790(local+8,10,value);
    cm_fn_00785790(local+8,11,(u32)(s32)(s8)type);
    value=(u32)(s32)(s16)W(local+6);
    cm_fn_00785790(local+8,12,value);
    value=(u32)(s32)(s16)W(local+4);
    cm_fn_00785790(local+8,13,value);
    manager=D(0x00b63d28);
    record=cm_fn_0062ed70(manager,second);
    if (record!=0) {
        value=(u32)(s32)(s8)B(record);
        cm_fn_00785790(local+8,14,value);
        value=(u32)(s32)(s16)W(record+0x2c);
    } else {
        cm_fn_00785790(local+8,14,0xffffffffu);
        value=0xffffffffu;
    }
    cm_fn_00785790(local+8,15,value);
    owner=D(first+0x39);
    manager=D(0x00b7431c);
    result=cm_fn_007861e0(manager,local+8,owner);
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
