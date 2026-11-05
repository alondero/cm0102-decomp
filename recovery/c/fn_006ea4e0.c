#include "cmre.h"
/* Preferred-image VA006ea4e0/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS8/RET32. All five indexed callers, including the six callers of
   pass-through wrapper006cfcd0, kill the entire return before observation. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u32 CM_THISCALL cm_fn_006f5b20(u32,u32);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_006ea4e0(u32 self,volatile u32 arg1,
    volatile u32 arg2,volatile u32 arg3,volatile u32 arg4,
    volatile u32 arg5,volatile u32 arg6,volatile u32 arg7,volatile u32 arg8)
{
    u32 player=arg1,output=arg7,code,state,address,other,a,c;
    s32 side,value,quotient,threshold;
    volatile long double live;
    volatile long long converted;
    D(output)=0x3fc00000;
    a=B(player+0x19);
    if((s8)a<0) {D(output)=0x41200000;return;}
    code=arg3;a=B(code);
    if(a==7) {
        D(output)=0x3fa00000;
        state=D(self);side=(s8)B(player+0x27);
        address=state+(u32)(side*54)+0xf5c9;
        a=B(address);B(address)=(u8)(a+1);
        goto counters;
    }
    if(a!=1)return;
    state=D(self);a=B(state+0x8eb3);
    if(a)return;
    side=(s8)B(player+0x27);
    address=state+(u32)(side*54)+0xf5c9;
    a=B(address);B(address)=(u8)(a+1);
    a=(u8)arg6;
    if(a) {
        a=cm_fn_0090cfc0(3000);
        if((s16)a>375) {
            W(player+0x35)=(u16)(W(player+0x35)+0xfd12u);
            B(code)=5;D(output)=0x40a00000;
            a=B(player+0x24);
            if(a)B(player+0x26)=9;else B(player+0x25)=9;
            goto counters;
        }
        if((s16)a<=50) {
            a=cm_fn_00714d30(player);
            /* Original PUSH EAX has arbitrary upper bits; 006f5b20
               observes only the low argument byte before returning 0/1. */
            a=cm_fn_006f5b20(player,(u8)a);
            if(!(u8)a)goto counters;
        }
        state=D(self);
        live=(long double)F(0x00968e70);
        value=(s16)W(state+0x8ed0);quotient=value/11;
        if((s16)quotient>45) {
            /* Native006ea600 rewrites the fifth DWORD formal slot. */
            arg5=(u32)(s32)(s16)quotient;
            live=(long double)(s32)arg5;
            live=(long double)F(0x0096d64c)/live;
        }
        live=live*(long double)F(0x0096d648);
        converted=(long long)live; /* Native FTOL006ea614; ADR0006. */
        W(player+0x35)=(u16)(W(player+0x35)+(u16)converted);
        a=B(player+0x24);
        if(a) {
            a=(u8)(a+1);B(player+0x24)=(u8)a;
            a=(u8)arg4;
            B(code)=3;D(output)=0x40600000;B(player+0x26)=(u8)a;
        }else {
            c=(u8)arg4;
            B(player+0x24)=1;B(code)=2;D(output)=0x40000000;
            B(player+0x25)=(u8)c;
        }
        goto counters;
    }
    a=cm_fn_0090cfc0(15);
    threshold=(s8)arg5;
    if(threshold>(s32)(a+7u)) {
        W(player+0x35)=(u16)(W(player+0x35)+0xfc18u);
        B(code)=4;D(output)=0x40a00000;
        a=B(player+0x24);
        if(a) {a=(u8)arg4;B(player+0x26)=(u8)a;}
        else {c=(u8)arg4;B(player+0x25)=(u8)c;}
        a=(u8)arg8;if(a)B(player+0x16)=1;
        goto counters;
    }
    a=cm_fn_0090cfc0(6);
    /* At006ea6b1 the argument PUSH remains live: [ESP28] is arg5. */
    threshold=(s8)arg5;
    if(threshold<=(s32)a) {
        W(player+0x35)=(u16)(W(player+0x35)-25u);
        goto counters;
    }
    state=D(self);
    live=(long double)F(0x00968e70);
    value=(s16)W(state+0x8ed0);quotient=value/11;
    if((s16)quotient>45) {
        /* Native006ea6e8 rewrites the fifth DWORD formal slot. */
        arg5=(u32)(s32)(s16)quotient;
        live=(long double)(s32)arg5;
        live=(long double)F(0x0096d64c)/live;
    }
    live=live*(long double)F(0x0096d648);
    converted=(long long)live; /* Native FTOL006ea6fc; ADR0006. */
    W(player+0x35)=(u16)(W(player+0x35)+(u16)converted);
    a=B(player+0x24);
    if(a) {
        /* This branch captures arg4 before incrementing player24. */
        c=(u8)arg4;
        a=(u8)(a+1);B(player+0x24)=(u8)a;
        B(code)=3;D(output)=0x40600000;B(player+0x26)=(u8)c;
    }else {
        a=(u8)arg4;
        B(player+0x24)=1;B(code)=2;D(output)=0x40000000;
        B(player+0x25)=(u8)a;
    }
counters:
    a=B(player+0x14);other=arg2;
    B(player+0x14)=(u8)(a+1);
    c=B(other+0x15);B(other+0x15)=(u8)(c+1);
}
