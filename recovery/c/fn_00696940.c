/* Preferred-image VA 00696940, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define H(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_00569e30(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053c590(u32,u8,u32);

/* Retain __ftol's temporary chopping control word rather than SSE3 FISTTP. */
__attribute__((target("no-sse3"), optimize("O2")))
u32 CM_THISCALL cm_fn_00696940(u32 self,u32 person,u32 club)
{
    u32 receiver,contract,kind,flag,value,result,index,base,address;
    s32 threshold;
    s8 count,active;
    long double t,squared;
    volatile long long converted;
    volatile long double discarded __attribute__((unused));
    receiver=D(0x00ae1050);
    contract=cm_fn_004cf5f0(receiver,person,club);
    if(!contract)return 0;
    kind=U(contract+0x35)&0x3fu;
    if(kind==1 || kind==3)return 0;
    receiver=D(0x00ae2a30);
    if(receiver && cm_fn_00569e30(receiver,person,0))return 0;
    t=(long double)(s32)W(club+0x80);
    t*=CM_GLOBAL(double,0x00968918);
    squared=t*t;
    converted=(long long)squared; /* Original __ftol call at 006969ad. */
    discarded=t; /* Private hypothesis: retain scaled operand through conversion. */
    threshold=(s32)(u32)converted;
    flag=cm_fn_00600a50(club,0,1,0,0)!=0;
    receiver=D(0x00ae1050);
    value=cm_fn_004d0c60(receiver,person,club,1,0xffffffffu,flag);
    if((s32)value>threshold) {
        count=(s8)(u8)cm_fn_0053c590(club,0,0);
        active=B(contract+0x3d);
        if(active<=0)return 0;
        result=count<15?0x200u:0x800u;
    } else {
        active=B(contract+0x3d);
        if(active<=0)return 0;
        count=(s8)(u8)cm_fn_0053c590(club,0,0);
        if(count>=15)return 0;
        result=0x200;
    }
    index=D(club);base=D(self);address=index*73u+base+6u;
    H(address)=(u16)(H(address)+0xfb1eu);
    index=D(club);base=D(self);address=index*73u+base+0x14u;
    H(address)=(u16)(H(address)+0xfb1eu);
    index=D(club);base=D(self);address=index*73u+base+0x16u;
    H(address)=(u16)(H(address)+0xfd12u);
    index=D(club);base=D(self);address=index*73u+base+6u;
    if(W(address)<1)H(address)=1;
    return result;
}
