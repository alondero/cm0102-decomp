#include "cmre.h"

/* Preferred-image VAs, base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Four stack DWORDs / RET16; retain full incidental EAX on both exits. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))

u32 CM_STDCALL cm_fn_008de150(u32 item,u32 other,u32 argument,u32 mode)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0xe0];
    u32 prior,a,c,d,record,club_base,club,related,p;
    prior=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x009645db;
    frame.prev=prior;
    CM_FS(u32,0)=(u32)&frame;
    /* L is ESP after SUB E0. The reporting object is L+0; linked
       predecessor/handler/state occupy original L+E0/E4/E8. */
    a=D(item+0xc);
    c=D(0x00ae1050);
    d=D(0x00ae23bc);
    record=d+a*110u;
    a=cm_fn_004cf5a0(c,record);
    club_base=D(0x00ae23b4);
    D(0x00ae1080)=a;
    if(a && (B(a+0x35)&0x3fu)!=5u){
        a=D(a+4);
        related=club_base+a*581u;
    }else related=0;
    a=D(item+8);
    club=club_base+a*581u;
    c=D(0x00b7431c);
    (void)c; /* Native incoming ECX is unused by original 007875e0. */
    a=cm_fn_007875e0(club);
    if(a){
        cm_fn_0077bbe0(scratch,0xfad,0);
        d=D(related);
        frame.state=0;
        cm_fn_00785790(scratch,0,d);
        a=D(club);
        cm_fn_00785790(scratch,1,a);
        c=D(record);
        cm_fn_00785790(scratch,2,c);
        d=D(record+4);a=D(d+0x33);
        cm_fn_00785790(scratch,3,a);
        c=D(record+8);d=D(c+0x33);
        cm_fn_00785790(scratch,4,d);
        a=D(record+0xc);c=D(a+0x33);
        cm_fn_00785790(scratch,5,c);
        d=D(other);
        cm_fn_00785790(scratch,6,d);
        a=D(other+4);c=D(a+0x33);
        cm_fn_00785790(scratch,7,c);
        d=D(other+8);a=D(d+0x33);
        cm_fn_00785790(scratch,8,a);
        c=D(other+0xc);d=D(c+0x33);
        cm_fn_00785790(scratch,9,d);
        cm_fn_00785790(scratch,10,argument);
        c=D(item+0x1b);
        cm_fn_00785790(scratch,11,c);
        d=(u32)(s32)(s16)mode;
        cm_fn_00785790(scratch,12,d);
        p=D(0x00b7431c);
        a=(u32)cm_fn_007861e0((void *)p,(u32)scratch,club);
    }
    prior=frame.prev;
    CM_FS(u32,0)=prior;
    return a;
}
