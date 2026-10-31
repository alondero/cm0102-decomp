#include "cmre.h"

/* Preferred-image VA 007e4410, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB a0. Keep the shared scratch offsets, forward 4/2/1
   copy, linked SEH state and partial AL results on the no-message exits. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_7e4410;
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e4410(u32 self,u32 record,u32 bytes)
{
    volatile frame_7e4410 frame;
    u8 scratch[0xa0];
    u32 local=(u32)scratch,result,entry,owner,source,destination,value,i,id,base;
    u8 flag,count,kind,bits;
    frame.state=0xffffffffu;
    frame.handler=0x009606c6;
    frame.prev=CM_FS(volatile u32,0);
    result=frame.prev;
    CM_FS(volatile u32,0)=(u32)&frame;
    flag=B(record+0x2c);
    result=(result&0xffffff00u)|flag;
    D(local+0x1c)=self;
    B(local+1)=0;
    B(local+2)=0xff;
    if (flag==0) {
        flag=B(record+0x2d);
        result=(result&0xffffff00u)|flag;
        if (flag==0) goto done;
    }
    id=D(record);
    result=cm_fn_007e82e0(self,id,1);
    entry=result;
    flag=B(record+0x2d);
    result=(result&0xffffff00u)|flag;
    D(local+0x18)=entry;
    if (flag==1) {
        cm_fn_0054a110(local+0x34);
        frame.state=0;
        cm_fn_0054a110(local+0x44);
        frame.state=1;
        result=cm_fn_007e8e10(self,local+0x30,record,1);
        if (result!=0) {
            value=cm_fn_0054c5b0(0x00ae2c90,local+0x28,1);
            B((u32)&frame.state)=2;
            result=cm_fn_00417c10(local+0x44,local+0x20,value);
            source=bytes;
            destination=local+0x50;
            B(local+0x3c)=0x28;
            B(local+0x3d)=0xff;
            B(local+0x3e)=0xff;
            B(local+0x3f)=0xff;
            owner=D(local+0x1c);
            for (i=0;i<0x13;++i) {
                value=D(source);
                D(destination)=value;
                source+=4;
                destination+=4;
            }
            W(destination)=W(source);
            owner=D(owner+7);
            B((u32)&frame.state)=1;
            B(destination+2)=B(source+2);
            result=(u32)cm_fn_0068d0d0(owner,local+0x30,0x70);
            entry=D(local+0x18);
        }
        B(local+1)=1;
        B(local+2)=2;
        B(local+3)=1;
        frame.state=0xffffffffu;
    }
    flag=B(record+0x2c);
    result=(result&0xffffff00u)|flag;
    if (flag>0) {
        count=B(entry+4);
        count=(u8)(count+1);
        B(entry+4)=count;
        result=(u32)count/5;
        if ((u32)count%5==0) {
            count=B(local+1);
            B(local+2)=1;
            count=(u8)(count+1);
            B(local+3)=5;
            B(local+1)=count;
        }
    }
    count=B(local+1);
    result=(result&0xffffff00u)|count;
    if (count==0) goto done;
    cm_fn_0054a110(local+8);
    count=B(local+1);
    frame.state=3;
    B(local+4)=count;
    W(local+6)=0xffff;
    B(local+5)=0;
    cm_fn_00417c10(local+8,local+0x20,0x00ae2c90);
    kind=B(local+2);
    bits=B(local+0x17);
    value=(u32)(s32)(s8)B(local+3);
    B(local+0x14)=kind;
    flag=B(record+0x48);
    W(local+0x15)=(u16)value;
    bits=(u8)((bits&0xf9)|((flag==0)?4:0));
    id=D(record);
    B(local+0x17)=bits;
    owner=D(0x00ae28f0);
    B(local+0x10)=0x28;
    B(local+0x11)=0xff;
    B(local+0x12)=0xff;
    B(local+0x13)=0xff;
    cm_fn_00556940(owner,id,local+4);
    id=D(record);
    source=bytes;
    base=D(0x00ae23bc);
    destination=base+id*110;
    owner=D(0x00ae28f0);
    (void)owner;
    result=cm_fn_00557280(destination,local+4,source);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
