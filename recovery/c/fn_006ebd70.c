#include "cmre.h"
/* Preferred-image VA006ebd70/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0. Keep the full terminal FTOL EAX, including callers which first
   rewrite AX from AL and then perform DWORD arithmetic. */
extern u8 CM_CDECL cm_fn_0053ce90(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006ebd70(u32 self)
{
    u32 side,root,link,club,person,pointer,lookup,kind,target,first,second,a;
    s32 difference,signed_value;
    volatile double delta,rating,adjustment=0.0,one=1.0;
    volatile long double live,factor,product,penalty;
    volatile long long converted;
    side=B(self+0x27);root=D(self+0x19e);
    link=D(root+0x4792);
    if((u8)side)club=D(link+0x20);else club=D(link+0x1c);
    person=D(self+0x69);
    a=cm_fn_0053ce90(person,club);
    root=D(self+0x19e);side=B(self+0x27);
    signed_value=(s8)a;
    link=D(root+0x4792);
    delta=(double)(long double)signed_value;
    if((u8)side)club=D(link+0x20);else club=D(link+0x1c);
    pointer=D(self+0x6d);
    signed_value=(s16)W(club+0x80);
    difference=(s16)W(pointer+0xd)-signed_value;
    live=(long double)difference;
    live=live*(long double)Q(0x00969048);
    live=live+(long double)Q(0x00968940);
    rating=(double)live;
    /* Native lower FCOMP clamps only ordered greater; upper TESTAH1
       clamps both less and unordered. Preserve the asymmetric NaN rule. */
    live=(long double)Q(0x0096d220);
    if(live>(long double)rating)rating=-4.0;
    else {
        live=(long double)Q(0x00968938);
        if(!(live>=(long double)rating))rating=4.0;
    }
    link=D(root+0x4792);
    if((u8)side)club=D(link+0x20);else club=D(link+0x1c);
    lookup=D(0x00ae1050);person=D(self+0x69);
    a=cm_fn_004cf5f0(lookup,person,club);
    if(a) {
        signed_value=(s8)B(a+0x44);kind=B(a+0x4f);
        live=(long double)signed_value;
        kind=(u8)kind>>4;
        live=live*(long double)Q(0x00968ef8);
        live=live+(long double)Q(0x00968968);
        adjustment=(double)live;
        if(kind<=8) {
            target=D(0x006ebfe4+kind*4);
            switch(target) {
            case 0x006ebec2:
                live=(long double)adjustment;
                live=live-(long double)Q(0x009688b0);break;
            case 0x006ebece:
                live=(long double)adjustment;
                live=live+(long double)Q(0x00968e18);break;
            case 0x006ebeda:
                live=(long double)adjustment;
                live=live+(long double)Q(0x00968950);break;
            case 0x006ebee6:
                live=(long double)adjustment;
                live=live-(long double)Q(0x00968930);break;
            case 0x006ebef2:
                live=(long double)adjustment;
                live=live-(long double)Q(0x00968e08);break;
            case 0x006ebefe:
                live=(long double)adjustment;
                live=live-(long double)Q(0x00968ea8);break;
            case 0x006ebf0a:
                live=(long double)adjustment;
                live=live-(long double)Q(0x00968ff8);break;
            default:goto adjustment_done;
            }
            adjustment=(double)live;
        }
    }
adjustment_done:
    side=B(self+0x27);root=D(self+0x19e);
    link=D(root+0x4792);
    if((u8)side)club=D(link+0x20);else club=D(link+0x1c);
    a=cm_fn_0053b900(club);
    if(a) {
        person=D(self+0x69);
        a=B(person+0x22);
        factor=(long double)a;
        factor=factor*(long double)Q(0x0096d520);
    }else {
        person=D(self+0x69);
        second=D(person+0x42);first=D(person+0x3e);
        a=cm_fn_0054cdb0(0x00ae2c90,first,second);
        factor=(long double)(s32)a;
    }
    live=(long double)Q(0x00968908);
    if(factor>live) {
        factor=factor-(long double)Q(0x00968908);
        factor=factor*(long double)Q(0x0096a6b8);
        factor=factor+(long double)Q(0x00967890);
    }else factor=(long double)one;
    /* Native retains this age factor below the terminal conversion,
       then discards it with FSTP006ebfde. It has no further consumer. */
    pointer=D(self+0x6d);signed_value=(s8)B(pointer+0x2f);
    product=(long double)signed_value;
    product=product*factor;
    penalty=(long double)delta;
    penalty=penalty*(long double)delta;
    penalty=penalty+(long double)Q(0x009688a0);
    penalty=penalty*(long double)Q(0x00967880);
    live=product-penalty;
    live=live+(long double)rating;
    live=live+(long double)adjustment;
    live=live+(long double)Q(0x00968968);
    converted=(long long)live; /* Native FTOL006ebfd7; ADR0006. */
    return (u32)converted;
}
