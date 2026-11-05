#include "cmre.h"
/* Preferred-image VA006d0f80/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/plainRET, fullEAX retained. Native EDI upper16 residue contributes
   only to score upper16, which is dead at both signed WORD comparisons. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006d0f80(u32 self)
{
    u32 role=D(self+0x4792),base=self+0x4796,best=0;
    u32 best_score=0,best_value=0;
    u8 count=(u8)((U(role+0x4d)&15u)+11u);
    for(u32 side=0;side<2;side++) {
        u32 record=base;
        for(u32 left=count;left!=0;left--) {
            u8 status=U(record+0x19);
            if((s8)status<0 && U(record+0x20)==255u)goto next;
            u16 raw=W(record+0x35);
            long double live=(long double)(s16)raw;
            live*=Q(0x96a350);
            volatile long long converted=(long long)live; /* __ftol CALL006d0fed. */
            u32 value=(u32)converted;
            u32 x=U(record+0x0c),y=U(record+0x10);
            u32 score=(2u*x+y)*25u;
            u32 z=U(record+3);
            score=score*5u+raw;
            score+=z*50u;
            z=U(record+6);score+=z*35u;
            if(best!=0 && ((s16)(u16)score<=(s16)(u16)best_score ||
                           (s16)(u16)value<(s16)(u16)best_value))goto next;
            best=record;best_score=score;best_value=value;
next:
            record+=0x1be;
        }
        base+=0x22d8;
    }
    if(best!=0) {u32 pointer=D(best+0x69),result=D(pointer);D(self+0x477a)=result;return result;}
    /* Each inner loop ends with EAX=0, then MOV AL,count; count is 11..26. */
    return count;
}
