#include "cmre.h"
/* Preferred-image VA004a19f0, base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0; every native return sets full EAX to1. */
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32,u32);
extern u32 CM_THISCALL cm_fn_004ad680(u32);
extern u32 CM_THISCALL cm_fn_004ad7f0(u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_00945333(u32,u32,u32,u32,u32,u32);
extern long double CM_STDCALL cm_fn_004a2ec0(u32,s8);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_THISCALL cm_fn_00448490(u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_004a19f0(u32 self)
{
    u32 a,b,c,stats;
    long double live,x,y;
    volatile long long converted;
    a=D(self+0x502);
    b=D(a+4);
    c=D(0x00ae2c74);
    a=(u32)cm_fn_005c6710(c,b);
    if(!a) {
        a=cm_fn_004ad680(self);
        if(a && (s8)B(self+0x26)>=5) {
            D(self+0x50a)=1;
            D(self+0x562)=1;
        }
    }
    a=D(self+0x502);
    c=D(0x00ae2c74);
    b=D(a+4);
    a=(u32)cm_fn_005c6710(c,b);
    if(a)goto floating;
    a=D(self+0x502);
    if(B(a+0x42)==4)goto floating;
    a=D(self+0x2fa);
    if(a==0xffffffffu) {
        a=D(self+0xc);
        if(a) {
            if(B(self+0x18)!=2) {
                B(self+0x18)=2;
                D(0x00adae10)=self;
                b=D(self+0x14);
                a=D(self+0xc);
                (void)cm_fn_00945333(a,b,a,b,0x76,0x004ad1a0);
            }
            stats=D(self+0xc);
            /* Native004a2f3f FILD32/FIDIV32 returns extended ST0 for
               selector2, with no return rounding or remaining x87 value. */
            live=cm_fn_004a2ec0(stats,2);
            converted=(long long)live; /* ADR0006: __ftol CALL004a1aa1. */
            D(self+0x2fa)=(u32)converted;
            a=D(stats);
            D(self+0x2fe)=a;
            W(self+0x302)=0;
        }
    }
    c=D(self+0x2fa);
    if((s32)c<=-1)goto floating;
    a=D(self+0x91);
    if(a!=0xffffffffu && (s32)c<=(s32)a)goto floating;
    a=D(self+0x2fa);D(self+0x91)=a;
    a=D(self+0x2fe);D(self+0x95)=a;
    a=W(self+0x302);W(self+0x99)=(u16)a;
    a=D(self+0x502);
    b=W(a+0x40);W(self+0x99)=(u16)b;
    if((s16)W(self+0x500)>=3) {
        a=D(self+0x502);b=D(a+4);a=D(b);
        a=cm_fn_004b0700(a);
        if(a) {D(self+0x526)=1;D(self+0x562)=1;}
    }
floating:
    a=D(self+0x502);
    b=D(a+4);
    c=D(0x00ae2c74);
    a=(u32)cm_fn_005c6710(c,b);
    if(a)goto done;
    x=(long double)F(self+0x418);
    y=(long double)Q(0x00968948);
    /* Native AH40 tests C3: equality and unordered both enter this call. */
    if(!(x<y) && !(x>y)) (void)cm_fn_004ad7f0(self);
    x=(long double)F(self+0x418);
    y=(long double)Q(0x00968948);
    if(!(x>y))goto done;
    x=(long double)F(self+0x1af);
    y=(long double)Q(0x00968948);
    if(!(x<y) && !(x>y))goto copy_floating;
    x=(long double)F(self+0x418);
    y=(long double)F(self+0x1af);
    if(!(x>y))goto done;
copy_floating:
    a=D(self+0x418);
    D(self+0x1af)=a;
    (void)cm_fn_00448490(self+0x1b3,self+0x41c);
    a=B(self+0x434);B(self+0x1cb)=(u8)a;
    a=D(self+0x435);D(self+0x1cc)=a;
    a=W(self+0x439);W(self+0x1d0)=(u16)a;
    a=D(self+0x502);
    b=W(a+0x40);W(self+0x1d0)=(u16)b;
    if((s16)W(self+0x500)>=3) {
        a=D(self+0x502);b=D(a+4);a=D(b);
        a=cm_fn_004b0700(a);
        if(a && (s8)B(self+0x1cb)>3) {
            D(self+0x54e)=1;
            D(self+0x562)=1;
        }
    }
done:
    W(self+0x500)=(u16)(W(self+0x500)+1u);
    return 1;
}
