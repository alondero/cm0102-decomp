#include "cmre.h"
/* Preferred-image VA006e1cf0/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/fullEAX: native final MOV AX,CX preserves the explicitly tracked
   high half. No incoming volatile register is an engine argument. */
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u8 CM_CDECL cm_fn_0053ce90(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_006e1cf0(u32 first,u32 second,u32 club)
{
    u32 person,pointer,a,kind,target,current,carrier,date_first,date_second,constant;
    s32 delta_value,profile_value,club_value,threshold,byte_value;
    volatile double delta,rating,adjustment=0.0,one=1.0;
    volatile long double live,raw_age,factor,product,penalty;
    volatile long long converted;
    if(second)person=D(second+0x69);else person=first;
    if(!person)return (second&0xffff0000u)|0xd8f0u;
    a=cm_fn_0053ce90(person,club);
    pointer=D(person+0x61);
    delta_value=(s8)(u8)a;
    profile_value=(s16)W(pointer+0xd);
    club_value=(s16)W(club+0x80);
    delta=(double)(long double)delta_value;
    live=(long double)(profile_value-club_value);
    live=live*(long double)Q(0x969048);live=live+(long double)Q(0x968940);
    rating=(double)live;
    live=(long double)Q(0x96d220);
    if(live>(long double)rating)rating=-4.0;
    else {
        live=(long double)Q(0x968938);
        if(!(live>=(long double)rating))rating=4.0;
    }
    a=cm_fn_004cf5f0(D(0xae1050),person,club);
    if(a) {
        byte_value=(s8)B(a+0x44);kind=B(a+0x4f);
        live=(long double)byte_value;kind=(u8)kind>>4;
        live=live*(long double)Q(0x968ef8);live=live+(long double)Q(0x968968);
        adjustment=(double)live;
        if(kind<=8) {
            target=D(0x6e1fd8+kind*4u);
            switch(target) {
            case 0x6e1e06:live=(long double)adjustment;live-=Q(0x9688b0);break;
            case 0x6e1e12:live=(long double)adjustment;live+=Q(0x968e18);break;
            case 0x6e1e1e:live=(long double)adjustment;live+=Q(0x968950);break;
            case 0x6e1e2a:live=(long double)adjustment;live-=Q(0x968930);break;
            case 0x6e1e36:live=(long double)adjustment;live-=Q(0x968e08);break;
            case 0x6e1e42:live=(long double)adjustment;live-=Q(0x968ea8);break;
            case 0x6e1e4e:live=(long double)adjustment;live-=Q(0x968ff8);break;
            default:goto adjustment_done;
            }
            adjustment=(double)live;
        }
    }
adjustment_done:
    a=cm_fn_0053b900(club);
    if(a) {
        a=B(person+0x22);raw_age=(long double)a;
        raw_age=raw_age*(long double)Q(0x96d520);
    }else {
        date_second=D(person+0x42);date_first=D(person+0x3e);
        a=cm_fn_0054cdb0(0xae2c90,date_first,date_second);
        raw_age=(long double)(s32)a;
    }
    live=(long double)Q(0x968908);
    if(raw_age>live) {
        factor=raw_age-(long double)Q(0x968908);
        factor=factor*(long double)Q(0x96a6b8);
        factor=factor+(long double)Q(0x967890);
    }else factor=(long double)one;
    pointer=D(person+0x61);byte_value=(s8)B(pointer+0x2f);
    product=(long double)byte_value;product=product*factor;
    penalty=(long double)delta;penalty=penalty*(long double)delta;
    penalty=penalty+(long double)Q(0x9688a0);penalty=penalty*(long double)Q(0x967880);
    live=product-penalty;live=live+(long double)rating;
    live=live+(long double)adjustment;live=live+(long double)Q(0x968968);
    converted=(long long)live; /* Native FTOL006e1ef4, site1. */
    current=(u32)converted;
    byte_value=(s8)B(person+0x18);
    /* Native discards only factor here; raw_age remains for FCOMPP below. */
    if(byte_value<18)constant=0x968968;
    else if(byte_value<21)constant=0x969040;
    else if(byte_value<24)constant=0x9689d8;
    else if(byte_value>25)constant=0x9689b8;
    else goto after2;
    live=(long double)(s32)(s16)(u16)current;live*=Q(constant);
    converted=(long long)live; /* Native FTOL006e1f5a, site2. */
    current=(u32)converted;
after2:
    byte_value=(s8)B(person+0x59);
    threshold=(50-byte_value)*10;
    /* MOV EAX,50/SUB/LEA/SHL define this carrier before FNSTSW AX. */
    carrier=(u32)threshold;live=(long double)threshold;
    if(!(raw_age>=live)) {
        live=(long double)(s32)(s16)(u16)current;live*=Q(0x9689d8);
        converted=(long long)live; /* Native FTOL006e1f95, site3. */
        current=(u32)converted;carrier=current;
    }
    byte_value=(s8)B(person+0x5b);
    if(byte_value<10) {
        live=(long double)byte_value;
        profile_value=(s16)(u16)current;
        live*=Q(0x967898);live+=Q(0x9689d8);
        live=(long double)profile_value*live;
        converted=(long long)live; /* Native FTOL006e1fc7, site4. */
        current=(u32)converted;carrier=current;
    }
    return (carrier&0xffff0000u)|(current&0xffffu);
}
