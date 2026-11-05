#include "cmre.h"
/* Preferred-image VA004178a0/base00400000, target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12; exact full EAX0/1 on all native returns. */
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
u32 CM_THISCALL cm_fn_004178a0(u32 self,volatile u32 first,
                             volatile u32 second,volatile float weight)
{
    u8 scratch[512];
    u32 a,d,index,base,message;
    s16 count=(s16)W(self+4);
    volatile long double live;
    if(count>=(s16)W(self+6)) {
        (void)cm_fn_00944cff(0x009880b0,0,0,scratch+256,scratch);
        message=cm_fn_0090d130((u32)scratch+256,(u32)scratch,191);
        (void)cm_fn_00944c9f(0x00ae24d0,0x009870e8,message,
                            (u32)scratch+256,(u32)scratch,191);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 1;
    }
    d=first;
    if(!d) {
        (void)cm_fn_00944cff(0x009880b0,0,0,scratch,scratch+256);
        message=cm_fn_0090d130((u32)scratch,(u32)scratch+256,198);
        (void)cm_fn_00944c9f(0x00ae24d0,0x009870e8,message,
                            (u32)scratch,(u32)scratch+256,198);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    a=(u32)(s32)count;
    d=D(d);
    index=a*14u;
    a=D(self);
    D(a+index)=d;
    a=second;
    if(a)d=D(a);else d=0xffffffffu;
    a=(u32)(s32)(s16)W(self+4);
    /* FLD captures the argument before the aliased DWORD write. A volatile
       extended temporary retains the x87 load, including NaN conversion. */
    live=(long double)weight;
    index=a*14u;
    a=D(self);
    D(a+index+4)=d;
    a=(u32)(s32)(s16)W(self+4);
    index=a*14u;
    a=D(self);
    CM_GLOBAL(volatile float,a+index+8)=(float)live;
    a=(u32)(s32)(s16)W(self+4);
    index=a*14u;
    base=D(self);
    W(base+index+12)=0;
    W(self+4)=(u16)(W(self+4)+1u);
    a=W(self+4);
    return (u32)((s16)a==(s16)W(self+6));
}
