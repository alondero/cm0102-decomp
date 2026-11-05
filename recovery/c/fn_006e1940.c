#include "cmre.h"
/* Preferred-image VA006e1940/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8, full EAX selected pointer (or zero). */
extern u32 CM_CDECL cm_fn_006e1cf0(u32,u32,u32);
extern u8 CM_CDECL cm_fn_00544410(u32,u32);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006e1940(u32 self,volatile u32 list_arg,volatile u32 count_arg)
{
    u32 cursor,person,pointer,score,best=0,best_score=0,remaining,a;
    s32 count=(s8)(u8)count_arg,attribute,remainder,word_score;
    volatile long double live;
    volatile long long converted;
    if(count<=0)return 0;
    cursor=list_arg;
    /* Native006e1966/006e196a reuse the actual two formal DWORD slots:
       second becomes cursor, first becomes remaining signed-BYTE count. */
    count_arg=cursor;list_arg=(u32)count;
    do {
        cursor=count_arg;person=D(cursor);
        if(person) {
            pointer=D(person+0x61);
            if(pointer) {
                a=D(self+0x2f);
                score=cm_fn_006e1cf0(person,0,a);
                pointer=D(self+0x1f);
                if(pointer) {
                    a=B(pointer+0x17);
                    attribute=(s8)cm_fn_00544410(pointer,(u8)a);
                    a=D(person);remainder=(s32)a%20;
                    if(attribute<remainder) {
                        /* Native006e19b9 reloads the pointer itself and
                           divides its signed address, not the pointee ID. */
                        a=D(self+0x1f);remainder=(s32)a%20;
                        live=(long double)remainder;
                        word_score=(s16)(u16)score;
                        live=live*(long double)Q(0x00967898);
                        live=(long double)Q(0x009689c0)-live;
                        live=(long double)word_score*live;
                        converted=(long long)live; /* Native FTOL006e19e2; ADR0006. */
                        score=(u32)converted;
                    }
                }
                if(!best || (s16)(u16)score>(s16)(u16)best_score) {
                    best=person;best_score=score;
                }
            }
        }
        cursor=count_arg;remaining=list_arg;
        cursor+=4;--remaining;count_arg=cursor;list_arg=remaining;
    }while(remaining);
    return best;
}
