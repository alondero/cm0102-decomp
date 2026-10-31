#include "cmre.h"
/* Preferred-image VA005aac70, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratche4: allocation L0, message L4. Caller005aec3e rewrites onlyDX
   before pushing EDX: preserve the full EDX:EAX pair from the final call. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_00785830(u32,u8);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u8);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u8,u32);
/* 0068d0d0 forwards entry EDX through 00944cf1 on its null-allocation path.
   Keep that implicit input in EDX, with the same two original stack words. */
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);

unsigned long long CM_THISCALL cm_fn_005aac70(u32 self,volatile u32 first_arg,volatile u32 second_arg,volatile u32 date_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xe4];
    u32 l=(u32)scratch,object,container,first,second,date,value,base;
    unsigned long long result;
    frame.prev=CM_FS(volatile u32,0);
    frame.handler=0x00959649; frame.state=0xffffffff;
    CM_FS(volatile u32,0)=(u32)&frame;
    object=D(self+0x1128);
    if (!object) {
        object=cm_fn_00944cf1(14);
        D(l)=object;
        frame.state=0;
        if (object) object=cm_fn_0068d020(object);
        frame.state=0xffffffff;
        D(self+0x1128)=object;
    }
    object=D(self+0x1128);
    value=D(object+2); D(object+0xa)=value;
    container=D(self+0x1128);
    object=cm_fn_0068d1b0(container);
    first=first_arg;
    while (object) {
        value=cm_fn_00785830(object,0);
        if (value!=D(first+0xc)) goto advance;
        value=cm_fn_00785830(object,1);
        if (value!=D(first+0x10)) goto advance;
        value=cm_fn_00785830(object,2);
        if (value!=D(first)) goto advance;
        value=cm_fn_00785830(object,14);
        if (value!=(u32)(s32)(s8)B(first+0x40)) goto advance;
        value=cm_fn_00785830(object,15);
        if (value==(u32)(s32)(s8)B(first+0x41)) {
            value=W(first+0x2a);
            cm_fn_00785790(object,5,value);
            value=W(first+0x28);
            result=cm_fn_00785790(object,6,value);
            goto done;
        }
advance:
        container=D(self+0x1128);
        object=cm_fn_0068d1b0(container);
    }
    cm_fn_0077bbe0(l+4,1,0);
    value=D(first+0xc);
    frame.state=1;
    cm_fn_00785790(l+4,0,value);
    value=D(first+0x10); cm_fn_00785790(l+4,1,value);
    value=D(first); cm_fn_00785790(l+4,2,value);
    value=(u32)(s32)(s16)W(first+0x32); cm_fn_00785790(l+4,3,value);
    value=(u32)(s32)(s16)W(first+0x30); cm_fn_00785790(l+4,4,value);
    value=W(first+0x2a); cm_fn_00785790(l+4,5,value);
    value=W(first+0x28); cm_fn_00785790(l+4,6,value);
    value=(u32)(s32)(s8)B(first+0x40); cm_fn_00785790(l+4,14,value);
    value=(u32)(s32)(s8)B(first+0x41); cm_fn_00785790(l+4,15,value);
    second=second_arg;
    if (second) {
        value=D(second+0xc); cm_fn_00785790(l+4,7,value);
        value=D(second+0x10); cm_fn_00785790(l+4,8,value);
        value=D(second); cm_fn_00785790(l+4,9,value);
        value=(u32)(s32)(s16)W(second+0x32); cm_fn_00785790(l+4,10,value);
        value=(u32)(s32)(s16)W(second+0x30); cm_fn_00785790(l+4,11,value);
        value=W(second+0x2a); cm_fn_00785790(l+4,12,value);
        value=W(second+0x28);
    } else {
        cm_fn_00785790(l+4,7,0xffffffff);
        date=date_arg;
        value=(u32)(s32)(s16)W(date); cm_fn_00785790(l+4,12,value);
        value=W(date+2);
    }
    result=cm_fn_00785790(l+4,13,value);
    base=D(self+0x1128);
    result=cm_fn_0068d0d0(base,(u32)(result>>32),l+4,0xdf);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
