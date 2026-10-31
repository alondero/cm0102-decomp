#include "cmre.h"
/* Preferred-image VA007f9a60, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/full EAX; scratche4 counterBYTE L3/messageL4, SEH96103b.
   Cap signed mode above3 only; retain negative modes and late formal reads.
   785790 consumes its index as BYTE (785790), so partial CL/BL upper bits
   are not inputs. Keep individual counter/value access order and the
   unguarded final recipient calculation, including negative source IDs. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_00785790(u32,u8,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e5160(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00546ab0(u32,u32);

u32 CM_STDCALL cm_fn_007f9a60(volatile u32 record_arg,volatile u8 mode_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xe4];
    u32 l=(u32)scratch,msg=l+4,record,value,count,row,first,second,id,base,club,person,owner;
    u8 mode,index;
    frame.state=0xffffffff; frame.handler=0x0096103b;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0077bbe0(msg,0x138e,0);
    mode=mode_arg;
    frame.state=0;
    if ((s8)mode>3) mode=3;
    count=(u32)(s32)(s8)mode;
    B(l+3)=1;
    cm_fn_00785790(msg,0,count);
    index=B(l+3);
    record=record_arg;
    B(l+3)=(u8)(index+1);
    value=(u32)(s32)(s8)B(record+0x66);
    cm_fn_00785790(msg,index,value);
    cm_fn_007e5160(record+0x1f,msg,l+3);
    value=(u32)(s32)(s8)B(record+0x59);
    index=B(l+3); B(l+3)=(u8)(index+1);
    cm_fn_00785790(msg,index,value);
    index=B(l+3); B(l+3)=(u8)(index+1);
    value=(u32)(s32)(s16)W(record+0x5a);
    cm_fn_00785790(msg,index,value);
    index=B(l+3); value=D(record+0x50); B(l+3)=(u8)(index+1);
    cm_fn_00785790(msg,index,value);
    cm_fn_007e4de0(record,msg,l+3);
    index=B(l+3); value=D(record+0x5e); B(l+3)=(u8)(index+1);
    cm_fn_00785790(msg,index,value);
    index=B(l+3); B(l+3)=(u8)(index+1); value=D(record+0x62);
    cm_fn_00785790(msg,index,value);
    index=B(l+3); B(l+3)=(u8)(index+1); value=(u32)(s32)(s8)B(record+0x58);
    cm_fn_00785790(msg,index,value);
    index=B(l+3); B(l+3)=(u8)(index+1); value=D(record+0x54);
    cm_fn_00785790(msg,index,value);
    if ((s8)mode>0) {
        row=record+0x37;
        do {
            index=B(l+3); value=D(row+0xc); B(l+3)=(u8)(index+1);
            cm_fn_00785790(msg,index,value);
            index=B(l+3); B(l+3)=(u8)(index+1); value=D(row);
            cm_fn_00785790(msg,index,value);
            index=B(l+3); value=D(row+4); B(l+3)=(u8)(index+1);
            cm_fn_00785790(msg,index,value);
            index=B(l+3); B(l+3)=(u8)(index+1); value=D(row+8);
            cm_fn_00785790(msg,index,value);
            index=B(l+3); B(l+3)=(u8)(index+1);
            first=(u32)(s32)(s8)B(row+0x25);
            second=(u32)(s32)(s8)B(row+0x26);
            value=second+first*8;
            cm_fn_00785790(msg,index,value);
            row+=0x67;
            --count;
        } while (count!=0);
    }
    index=B(l+3); B(l+3)=(u8)(index+1);
    cm_fn_00785790(msg,index,0x420e9);
    id=D(record+0x50);
    if ((s32)id<=-1) club=0;
    else {
        base=D(0x00ae23b4);
        club=base+id*581;
    }
    index=B(l+3); B(l+3)=(u8)(index+1);
    id=D(record+0x2b);
    base=D(0x00ae23bc);
    person=base+id*110;
    value=cm_fn_00546ab0(person,club);
    cm_fn_00785790(msg,index,value);
    id=D(record+0x50);
    base=D(0x00ae23b4);
    club=base+id*581;
    owner=D(0x00b7431c);
    value=cm_fn_007861e0(owner,msg,club);
    CM_FS(volatile u32,0)=frame.prev;
    return value;
}
