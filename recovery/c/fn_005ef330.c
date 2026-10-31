#include "cmre.h"
/* Preferred-image VA005ef330, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/full EAX. Incoming ECX is forwarded unchanged to5ef5e0, whose
   5ef5fe saves it as the receiver. Both callers loadECX fromB5ded8.
   Scratch204: categoryBYTE L0, diagnosticsL4/L104. 5ef5e0 consumes only
   the category BYTE (5ef60d); upper three original stack bytes stay unknown. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_005ef5e0(u32,u8,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005ef330(u32 self,volatile u32 record_arg,volatile u32 id_arg,volatile u32 score_arg)
{
    u8 scratch[0x204];
    u32 l=(u32)scratch,record,id,limit,date,value;
    u8 category;
    record=record_arg;
    if (record==0) goto diagnostic;
    id=id_arg;
    if ((s32)id<0) goto diagnostic;
    limit=D(0x00ae2350)-1;
    if ((s32)id>(s32)limit) goto diagnostic;
    limit=D(0x00ae2364);
    date=D(record);
    limit-=16;
    if ((s32)date<(s32)limit) return 0;
    if (id==D(0x009cf218)) B(l)=1;
    else if (id==D(0x009cf224)) B(l)=2;
    else if (id==D(0x009cf244)) B(l)=3;
    else if (id==D(0x009cf260)) B(l)=4;
    else if (id==D(0x009cf2ac)) B(l)=5;
    else if (id==D(0x009cf2c0)) B(l)=6;
    else if (id==D(0x009cf2e4)) B(l)=7;
    else if (id==D(0x009cf2fc)) B(l)=8;
    else if (id==D(0x009cf300)) B(l)=9;
    else if (id==D(0x009cf314)) B(l)=10;
    else if (id==D(0x009cf31c)) B(l)=11;
    else if (id==D(0x009cf338)) B(l)=12;
    else if (id==D(0x009cf2dc)) B(l)=13;
    else if (id==D(0x009cf360)) B(l)=14;
    else if (id==D(0x009cf36c)) B(l)=15;
    else if (id==D(0x009cf3e4)) B(l)=16;
    else if (id==D(0x009cf408)) B(l)=17;
    else if (id==D(0x009cf430)) B(l)=18;
    else if (id==D(0x009cf434)) B(l)=19;
    else if (id==D(0x009cf448)) B(l)=20;
    else if (id==D(0x009cf45c)) B(l)=21;
    else if (id==D(0x009cf488)) B(l)=22;
    else if (id==D(0x009cf4a8)) B(l)=23;
    else if (id==D(0x009cf4dc)) B(l)=24;
    else if (id==D(0x009cf4f8)) B(l)=25;
    else if (id==D(0x009cf510)) B(l)=26;
    else {
        value=(id&0xffffff00u)|(id!=D(0x009cf484));
        --value; value&=0x1c; --value;
        B(l)=(u8)value;
    }
    value=score_arg;
    category=B(l);
    return cm_fn_005ef5e0(self,category,record,value);

diagnostic:
    cm_fn_00944cff(0x009ce408,0,0,l+0x104,l+4);
    value=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,l+0x104,l+4,0x11b);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
