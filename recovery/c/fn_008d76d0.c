#include "cmre.h"
/* Preferred-image VAs, base00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4. Retain full EAX despite immediate indexed callers discarding it. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))

u32 CM_THISCALL cm_fn_008d76d0(u32 self,u32 item)
{
    u32 prior=CM_FS(u32,0);
    volatile struct { u32 prev,handler,state; } frame;
    /* L is ESP after SUB158. Message at L, recipients at L+E0. */
    u8 scratch[0x158];
    u32 a,b,row,club,index,result;
    s32 remaining;
    s8 count;
    frame.state=0xffffffffu;
    frame.handler=0x009643ab;
    frame.prev=prior;
    CM_FS(u32,0)=(u32)&frame;
    index=D(item+0xc);
    a=D(0x00ae1050);
    row=D(0x00ae23bc)+index*110u;
    result=cm_fn_004cf5a0(a,row);
    D(0x00ae1080)=result;
    if(result && (B(result+0x35)&0x3fu)!=5) {
        index=D(result+4);
        club=D(0x00ae23b4)+index*581u;
    } else club=0;
    count=(s8)cm_fn_008d1130(self,item,0x2d,(u32)(scratch+0xe0),30);
    result=(u32)cm_fn_0077bbe0(scratch,0xfa3,0);
    a=D(row);
    frame.state=0;
    result=(u32)cm_fn_00785790(scratch,0,a);
    a=D(row+4);b=D(a+0x33);
    result=(u32)cm_fn_00785790(scratch,1,b);
    a=D(row+8);b=D(a+0x33);
    result=(u32)cm_fn_00785790(scratch,2,b);
    a=D(row+0xc);b=D(a+0x33);
    result=(u32)cm_fn_00785790(scratch,3,b);
    a=club?D(club):0xffffffffu;
    result=(u32)cm_fn_00785790(scratch,4,a);
    a=D(item+8);result=(u32)cm_fn_00785790(scratch,5,a);
    a=D(item+4);result=(u32)cm_fn_00785790(scratch,6,a);
    a=D(item+0x20);result=(u32)cm_fn_00785790(scratch,7,a);
    a=D(item+0x24);result=(u32)cm_fn_00785790(scratch,8,a);
    a=(u32)(s32)(s8)B(item+0x1f);
    result=(u32)cm_fn_00785790(scratch,9,a);
    if(count>0) {
        index=0xe0;
        remaining=count;
        do {
            a=D((u32)scratch+index);
            b=D(0x00b7431c);
            result=(u32)cm_fn_007861e0((void *)b,(u32)scratch,a);
            index+=4;
            --remaining;
        } while(remaining!=0);
    }
    prior=frame.prev;
    CM_FS(u32,0)=prior;
    return result;
}
