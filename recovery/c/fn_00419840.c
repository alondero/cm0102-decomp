#include "cmre.h"
/* Preferred-image VA00419840, base00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Indexed caller continuations, including the two forwarding
   wrappers and their callers, discard EAX or consume only a replacement AL. */
extern void CM_THISCALL cm_fn_0054a110(volatile u8 *);
extern unsigned long long CM_THISCALL cm_fn_00417c10(volatile u8 *,volatile u8 *,const volatile u8 *);
extern u32 CM_THISCALL cm_fn_0054c5b0(volatile u8 *,volatile u8 *,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *,u32,const volatile u8 *,s16);
extern void CM_STDCALL cm_fn_00419ad0(u32,u32,s8,s16);
extern void CM_CDECL cm_fn_00547af0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_00419840(u32 self,volatile u32 person_arg,
                              volatile u32 other_arg,volatile u32 mode_arg)
{
    /* L is native ESP after SUB24. One object preserves all scratch aliases:
       L+0 copy destination/saved self, L+8 and L+10 date temporaries,
       L+18 record ID followed by the L+1c eight-byte date, then SEH at L+24. */
    volatile struct { u32 scratch[9];u32 prev,handler; s32 state; } local;
    volatile u8 *l=(volatile u8 *)&local.scratch[0];
    u32 person,detail,a,b,c,index,duration,kind;
    s32 rating;
    s8 mode;
    long double live,factor;
    volatile long long converted;
    unsigned long long copy_result;
    local.state=-1;
    local.handler=0x00953eb0;
    local.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&local.prev;
    D((u32)l)=self;
    cm_fn_0054a110(l+0x1c);
    person=person_arg;
    local.state=0;
    if(!person)goto done;
    a=D(person+0x61);
    if(!a)goto done;
    a=D(person);
    b=D(0x00ae2364);
    if((s32)a>=(s32)b)goto done;
    c=D(self+0xc)+a;
    a=B(c);
    if(a&1u)goto done;
    B(c)=(u8)(a|1u);
    detail=D(person+0x61);
    a=cm_fn_0090cfc0(41);
    rating=W(detail+0xb);
    person_arg=a;
    live=(long double)(s32)person_arg;
    person_arg=(u32)rating;
    live*=F(0x00967898);
    live=F(0x00967890)-live;
    factor=(long double)(s32)person_arg;
    factor*=live;
    converted=(long long)factor; /* ADR0006: physical __ftol CALL004198d8. */
    W(detail+0xb)=(s16)(u32)converted;
    detail=D(person+0x61);
    a=cm_fn_0090cfc0(41);
    person_arg=a;
    live=(long double)(s32)person_arg;
    rating=W(detail+9);
    live*=F(0x00967898);
    person_arg=(u32)rating;
    live=F(0x00967890)-live;
    factor=(long double)(s32)person_arg;
    factor*=live;
    converted=(long long)factor; /* ADR0006: physical __ftol CALL0041990d. */
    W(detail+9)=(s16)(u32)converted;
    detail=D(person+0x61);
    a=cm_fn_0090cfc0(41);
    rating=W(detail+0xd);
    person_arg=a;
    live=(long double)(s32)person_arg;
    person_arg=(u32)rating;
    live*=F(0x00967898);
    live=F(0x00967890)-live;
    factor=(long double)(s32)person_arg;
    factor*=live;
    converted=(long long)factor; /* ADR0006: physical __ftol CALL00419945. */
    W(detail+0xd)=(s16)(u32)converted;
    a=D(person+0x39);
    if(a) {
        b=D(a+0xcf);
        cm_fn_00547af0(person,b,1);
        a=D(person+0x39);
        a=cm_fn_00600a50(a,1,1,0,0);
        if(!a) {
            a=D(person+0x39);
            b=D(a+0xcf);
            cm_fn_00547af0(b,person,1);
        }
        index=0;
        do {
            a=D(person+0x39);
            detail=D(a+index+0xd7);
            if(detail && detail!=person) {
                a=(u32)(s32)(s8)B(detail+0x59);
                a=cm_fn_0090cfc0(a);
                if(a) {
                    a=(u32)(s32)(s8)B(detail+0x5b);
                    a=cm_fn_0090cfc0(a);
                    if(!a)cm_fn_00547af0(detail,person,1);
                }
            }
            index+=4;
        } while(index<200);
        a=D(person+0x39);
        b=D(a+0xbf);
        cm_fn_00547af0(b,person,1);
        self=D((u32)l);
    }
    a=D(person);
    D((u32)l+0x18)=a;
    (void)cm_fn_00417c10(l+0x1c,l,(const volatile u8 *)0x00ae2c90);
    mode=(s8)mode_arg;
    if(mode==4)duration=cm_fn_0090cfc0(31)+2u;
    else duration=cm_fn_0090cfc0(150)+21u;
    a=cm_fn_0054c5b0(l+0x1c,l+8,duration);
    B((u32)&local.state)=1;
    copy_result=cm_fn_00417c10(l+0x1c,l+0x10,(const volatile u8 *)a);
    a=D(self);
    B((u32)&local.state)=0;
    /* Native00417c10 returns packed first date DWORD in EDX at00417c35;
       the following original list insertion forwards this physical input. */
    (void)cm_fn_0068d0d0((volatile u8 *)a,(u32)(copy_result>>32),l+0x18,12);
    if(mode==1){a=other_arg;kind=2;duration=0xffffffffu;}
    else if(mode==4){a=other_arg;kind=4;}
    else {a=other_arg;kind=1;duration=0xffffffffu;}
    if(!a)a=D(person+0x39);
    cm_fn_00419ad0(person,a,(s8)kind,(s16)duration);
done:
    CM_FS(u32,0)=local.prev;
}
