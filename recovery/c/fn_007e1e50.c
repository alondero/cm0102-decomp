#include "cmre.h"

/* Preferred-image VA 007e1e50, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch a0: date L0, message L8/date Lc, saved self L1c,
   temporaries L20/L28, record L30, dates L34/L44, copied bytes L50. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_7e1e50;
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e1e50(u32 self,u32 record,volatile u32 bytes)
{
    volatile frame_7e1e50 frame;
    u8 scratch[0xa0];
    u32 local=(u32)scratch,result,id,entry,source,destination,owner,value,i,base;
    u8 flag,count,bits;
    frame.state=0xffffffffu;
    frame.handler=0x0096046c;
    frame.prev=CM_FS(volatile u32,0);
    result=frame.prev;
    CM_FS(volatile u32,0)=(u32)&frame;
    flag=B(record+0x2c);
    result=(result&0xffffff00u)|flag;
    D(local+0x1c)=self;
    if (flag==0) {
        flag=B(record+0x2d);
        result=(result&0xffffff00u)|flag;
        if (flag==0) goto done;
    }
    id=D(record);
    result=cm_fn_007e82e0(self,id,1);
    entry=result;
    flag=B(record+0x2c);
    count=B(entry+4);
    count=(u8)(count+flag);
    B(entry+4)=count;
    if (count>=4) B(entry+4)=(u8)(count-4);
    if (B(record+0x2d)!=1) goto done;
    cm_fn_0054a110(local+0x34);
    frame.state=0;
    cm_fn_0054a110(local+0x44);
    frame.state=1;
    cm_fn_0054a110(local);
    B((u32)&frame.state)=2;
    result=cm_fn_007e8e10(self,local+0x30,record,1);
    if (result!=0) {
        value=cm_fn_0054c5b0(0x00ae2c90,local+0x28,1);
        B((u32)&frame.state)=3;
        cm_fn_00417c10(local,local+0x20,value);
        B((u32)&frame.state)=2;
        result=cm_fn_0054a3f0(local);
        while ((u16)result!=3) {
            value=cm_fn_0054c5b0(local,local+0x20,1);
            B((u32)&frame.state)=4;
            cm_fn_00417c10(local,local+0x28,value);
            B((u32)&frame.state)=2;
            result=cm_fn_0054a3f0(local);
        }
        cm_fn_00417c10(local+0x44,local+0x20,local);
        source=bytes;
        destination=local+0x50;
        B(local+0x3c)=0x27;
        B(local+0x3d)=0xff;
        B(local+0x3e)=0xff;
        B(local+0x3f)=0xff;
        for (i=0;i<0x13;++i) {
            value=D(source);
            D(destination)=value;
            source+=4;
            destination+=4;
        }
        owner=D(local+0x1c);
        W(destination)=W(source);
        owner=D(owner+7);
        B(destination+2)=B(source+2);
        cm_fn_0068d0d0(owner,local+0x30,0x70);
    }
    frame.state=0xffffffffu;
    cm_fn_0054a110(local+0xc);
    frame.state=5;
    B(local+8)=1;
    W(local+0xa)=0xffff;
    B(local+9)=0;
    cm_fn_00417c10(local+0xc,local+0x20,0x00ae2c90);
    bits=B(local+0x1b);
    flag=B(record+0x48);
    id=D(record);
    bits=(u8)((bits&0xf9)|((flag==0)?4:0));
    B(local+0x1b)=bits;
    owner=D(0x00ae28f0);
    B(local+0x18)=2;
    W(local+0x19)=1;
    B(local+0x14)=0x27;
    B(local+0x15)=0xff;
    B(local+0x16)=0xff;
    B(local+0x17)=0xff;
    cm_fn_00556940(owner,id,local+8);
    id=D(record);
    source=bytes;
    base=D(0x00ae23bc);
    destination=base+id*110;
    owner=D(0x00ae28f0);
    (void)owner;
    result=cm_fn_00557280(destination,local+8,source);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
