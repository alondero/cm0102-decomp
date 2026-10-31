#include "cmre.h"
/* Preferred-image VA0041a630, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/full EAX. Scratche0 is one message atL0; SEH953f0b.
   Retain guard-path EAX, repeated pointer/global loads, the thousandth RNG
   draw before the loop exit, and the final selected-slot reload.
   7875e0 is STD1/RET4 and does not consume incoming ECX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);

u32 CM_STDCALL cm_fn_0041a630(volatile u32 first_arg,volatile u32 second_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xe0];
    u32 l=(u32)scratch,first,second,value,pointer,name,id,base,offset,owner,draw,count;
    frame.prev=CM_FS(volatile u32,0);
    frame.state=0xffffffff; frame.handler=0x00953f0b;
    CM_FS(volatile u32,0)=(u32)&frame;
    value=frame.prev;
    first=first_arg;
    if (first==0) goto done;
    value=D(first+0x61);
    if (value==0) goto done;
    second=second_arg;
    if (second==0) goto done;
    value=D(second+0xcf);
    if (value==0) goto done;
    cm_fn_0077bbe0(l,0xbc9,0);
    value=D(first);
    frame.state=0;
    cm_fn_00785790(l,0,value);
    pointer=D(first+4); value=D(pointer+0x33); cm_fn_00785790(l,1,value);
    pointer=D(first+8); value=D(pointer+0x33); cm_fn_00785790(l,2,value);
    pointer=D(first+0xc); value=D(pointer+0x33); cm_fn_00785790(l,3,value);
    value=D(second); cm_fn_00785790(l,4,value);
    value=cm_fn_0090cfc0(3); cm_fn_00785790(l,5,value);
    value=cm_fn_00600a50(second,0,1,0,0); cm_fn_00785790(l,6,value);
    pointer=D(second+0xcf);
    if (pointer!=0) {
        value=cm_fn_00600a50(second,1,1,0,0);
        if (value==0) {
            pointer=D(second+0xcf); value=D(pointer); cm_fn_00785790(l,7,value);
            pointer=D(second+0xcf); name=D(pointer+4); value=D(name+0x33);
            cm_fn_00785790(l,8,value);
            pointer=D(second+0xcf); name=D(pointer+8); value=D(name+0x33);
            cm_fn_00785790(l,9,value);
            pointer=D(second+0xcf); name=D(pointer+0xc); value=D(name+0x33);
            goto field10;
        }
    }
    draw=cm_fn_0090cfc0(2);
    if (draw!=0) {
        count=0;
        do {
            draw=cm_fn_0090cfc0(5);
            ++count;
            if ((s32)count>=1000) break;
            pointer=D(second+draw*4+0x19f);
        } while (pointer==0);
        pointer=D(second+draw*4+0x19f);
    } else pointer=D(second+0xd3);
    if (pointer!=0) {
        id=D(pointer);
        if (id!=0xffffffff) {
            cm_fn_00785790(l,7,id);
            base=D(0x00ae23bc);
            offset=id*110;
            name=D(base+offset+4); value=D(name+0x33); cm_fn_00785790(l,8,value);
            base=D(0x00ae23bc);
            name=D(base+offset+8); value=D(name+0x33); cm_fn_00785790(l,9,value);
            base=D(0x00ae23bc);
            name=D(base+offset+0xc); value=D(name+0x33);
            goto field10;
        }
    }
    cm_fn_00785790(l,7,0xffffffff);
    cm_fn_00785790(l,8,0xffffffff);
    cm_fn_00785790(l,9,0xffffffff);
    value=0xffffffff;
field10:
    cm_fn_00785790(l,10,value);
    (void)D(0x00b7431c);
    value=cm_fn_007875e0(second);
    if (value!=0) {
        pointer=D(first+0x39);
        owner=D(0x00b7431c);
        value=cm_fn_007861e0(owner,l,pointer);
    } else {
        second=D(second+0x53);
        if (second==0) goto done;
        pointer=D(first+0x61);
        if ((s16)W(pointer+0xb)<=0x1c52) goto done;
        owner=D(0x00b7431c);
        value=cm_fn_007863f0(owner,l,second);
    }
done:
    CM_FS(volatile u32,0)=frame.prev;
    return value;
}
