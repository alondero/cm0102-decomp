/* Preferred-image VA00522c70/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET0C/full EAX. Cache signed index before overwriting its actual
   argument slot's low byte. Scratch is the original uninitialized 20h area:
   year copy L4, day copy L0, date L8, helper outputs L10/L18. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32 self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_0054cab0(u32 self, u32 output, u8 weeks);
extern u32 CM_THISCALL cm_fn_00417c10(u32 self, u32 output, u32 source);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, u32 output, u32 days);
extern u32 CM_THISCALL cm_fn_005ac920(u32 self, u32 year, u32 day, u32 tick,
    u32 forward, u32 owner, u32 index, u32 second, u32 third,
    u32 unused0, u32 unused1, u32 unused2);

u32 CM_THISCALL cm_fn_00522c70(u32 self, volatile u32 index_arg,
                              volatile u32 second_arg, u32 third_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20];
    u32 local=(u32)scratch, index, offset, rows, temporary, value, day, owner, receiver, second, third, result;
    u16 year, number;
    frame.state=0xffffffffu;
    frame.handler=0x00957818u;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    index=(u32)(s32)(s16)(u16)index_arg;
    rows=D(self+0xa3u);
    offset=index*104u;
    year=(u16)(s16)(s8)B(rows+offset+5u);
    year=(u16)(year+W(self+0x40u));
    day=W(rows+offset+3u);
    (void)cm_fn_00549ef0(local+8u,day,year);
    frame.state=0;
    temporary=cm_fn_0054cab0(local+8u,local+0x10u,2);
    (void)cm_fn_00417c10(local+8u,local+0x18u,temporary);
    value=D(local+0xau);
    day=D(local+8u);
    third=third_arg;
    second=second_arg;
    D(local+4u)=value;
    owner=D(self+4u);
    D(local)=day;
    *(volatile u8 *)&index_arg=0;
    owner=D(owner);
    receiver=D(0x00ae2a58u);
    result=cm_fn_005ac920(receiver,local+4u,local,(u32)&index_arg,1,
        owner,index,second,third,0xffffffffu,0xffffffffu,0xffffffffu);
    if (result!=0) {
        rows=D(self+0xa3u);
        number=W(rows+offset+7u);
        year=W(result+0x28u);
        number=(u16)(number+3u);
        W(result+0x30u)=number;
        day=W(result+0x2au);
        temporary=(u32)cm_fn_00549ef0(local+0x18u,day,year);
        (void)cm_fn_00417c10(local+8u,local+0x10u,temporary);
        temporary=cm_fn_0054c5b0(local+8u,local+0x18u,1);
        (void)cm_fn_00417c10(local+8u,local+0x10u,temporary);
        value=D(local+0xau);
        day=D(local+8u);
        second=second_arg;
        D(local+4u)=value;
        owner=D(self+4u);
        D(local)=day;
        *(volatile u8 *)&index_arg=0;
        owner=D(owner);
        receiver=D(0x00ae2a58u);
        result=cm_fn_005ac920(receiver,local+4u,local,(u32)&index_arg,1,
            owner,index,second,third,0xffffffffu,0xffffffffu,0xffffffffu);
        if (result!=0) {
            rows=D(self+0xa3u);
            number=W(rows+offset+7u);
            number=(u16)(number+4u);
            W(result+0x30u)=number;
        }
    }
    CM_FS(u32,0)=frame.prev;
    return result;
}
