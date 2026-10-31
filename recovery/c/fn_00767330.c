#include "cmre.h"

/* Preferred-image VA 00767330, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB 158; message L0, 30 pointer outputs Le0.
   Preserve signed selector/count, cached row addresses and final full EAX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_767330;
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_CDECL cm_fn_00546ab0(u32,u32);
extern u32 CM_THISCALL cm_fn_008d1da0(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007862d0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_THISCALL cm_fn_00767330(u32 self,volatile u32 selector)
{
    volatile frame_767330 frame;
    u8 scratch[0x158];
    u32 local=(u32)scratch,index,row,base,person,slot,value,other,owner;
    u32 result,pointers,remaining;
    s8 count=0;
    frame.state=0xffffffffu;
    frame.handler=0x0095dbab;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0077bbe0(local,0x1b,0);
    index=(u32)(s32)(s8)selector;
    frame.state=0;
    row=self+index*11;
    base=D(0x00ae23bc);
    value=D(row+0x1d);
    other=D(self+0xd);
    person=base+value*110;
    cm_fn_00785790(local,0,other);
    value=D(self+0x11);
    cm_fn_00785790(local,1,value);
    value=D(row+0x1d);
    cm_fn_00785790(local,2,value);
    other=D(person+4);
    value=D(other+0x33);
    cm_fn_00785790(local,3,value);
    other=D(person+8);
    value=D(other+0x33);
    cm_fn_00785790(local,4,value);
    other=D(person+0xc);
    value=D(other+0x33);
    cm_fn_00785790(local,5,value);
    slot=self+index*11+0x21;
    value=D(slot);
    cm_fn_00785790(local,6,value);
    value=(u32)(s32)(s8)B(row+0x25);
    cm_fn_00785790(local,7,value);
    value=W(row+0x26);
    cm_fn_00785790(local,8,value);
    cm_fn_00785790(local,9,0x420e9);
    value=cm_fn_0090cfc0(3);
    cm_fn_00785790(local,10,value);
    value=D(person+0x61);
    cm_fn_00785790(local,11,value==0);
    value=D(person+0x61);
    if (value!=0) {
        value=cm_fn_005b7d20(value);
        cm_fn_00785790(local,12,(u32)(s32)(s16)value);
        other=D(slot);
        base=D(0x00ae23b4);
        value=cm_fn_00546ab0(person,base+other*0x245);
        cm_fn_00785790(local,13,value);
    }
    value=B(person+0x3d);
    result=(u32)cm_fn_00785790(local,14,value==12);
    owner=D(0x00de1f54);
    if (owner!=0) {
        result=D(person+0x61);
        if (result!=0) {
            result=cm_fn_008d1da0(owner,person,0x48,local+0xe0,0x1e);
            count=(s8)result;
        }
    }
    result=D(self+9);
    if (result!=0) {
        owner=D(0x00b7431c);
        result=(u32)cm_fn_007862d0(owner,local,result);
    } else {
        result=D(self+5);
        if (result!=0) {
            value=D(result+0x57);
            owner=D(0x00b7431c);
            result=(u32)cm_fn_007863f0(owner,local,value);
        }
    }
    if (count>0) {
        pointers=local+0xe0;
        remaining=(u32)(s32)count;
        do {
            result=D(pointers);
            other=D(self+9);
            value=D(result+0x57);
            if (value!=other) {
                owner=D(0x00b7431c);
                result=(u32)cm_fn_007861e0(owner,local,result);
            }
            pointers+=4;
            --remaining;
        } while (remaining!=0);
    }
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
