#include "cmre.h"

/* Preferred-image VA 007e3320, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratcha4: kind/count L2/L3, entry L18, datecache L1c/L20,
   record L34/dates L38,L48, copy L54, message L4/date L8.
   Self stays cached; the initial notification additionally requires record+2c=2. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e3320(u32 self,u32 record,volatile u32 bytes)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0xa4];
    u32 local=(u32)scratch,result,entry,id,value,extra,source,destination,owner,base,i;
    u8 flag,count,kind,bits,type;
    frame.state=0xffffffffu;
    frame.handler=0x009605b1;
    frame.prev=CM_FS(volatile u32,0);
    result=frame.prev;
    CM_FS(volatile u32,0)=(u32)&frame;
    B(local+3)=0;
    B(local+2)=0xff;
    flag=B(record+0x2c);
    result=(result&0xffffff00u)|flag;
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
        cm_fn_0054a110(local+0x38);
        frame.state=0;
        cm_fn_0054a110(local+0x48);
        value=D(0x00ae2c90);
        extra=D(0x00ae2c94);
        frame.state=1;
        D(local+0x1c)=value;
        D(local+0x20)=extra;
        B((u32)&frame.state)=2;
        result=cm_fn_007e8e10(self,local+0x34,record,1);
        if (result!=0) {
            value=cm_fn_0054c5b0(local+0x1c,local+0x2c,1);
            B((u32)&frame.state)=3;
            cm_fn_00417c10(local+0x48,local+0x24,value);
            source=bytes;
            destination=local+0x54;
            B(local+0x40)=0xff;
            B(local+0x41)=0x1d;
            B(local+0x42)=0xff;
            B(local+0x43)=0xff;
            for (i=0;i<0x13;++i) {
                value=D(source);
                D(destination)=value;
                source+=4;
                destination+=4;
            }
            owner=D(self+7);
            W(destination)=W(source);
            B((u32)&frame.state)=2;
            B(destination+2)=B(source+2);
            result=(u32)cm_fn_0068d0d0(owner,local+0x34,0x70);
            entry=D(local+0x18);
        }
        if (B(record+0x2c)==2) {
            B(local+3)=1;
            B(local+2)=2;
            type=1;
        } else type=B(local+2);
        frame.state=0xffffffffu;
    } else type=B(local+2);
    flag=B(record+0x2c);
    result=(result&0xffffff00u)|flag;
    if (flag!=0) {
        count=B(entry+4);
        count=(u8)(count+1);
        result=(result&0xffffff00u)|count;
        B(entry+4)=count;
        if (count==3 || (count>4 && (count&1)==1)) {
            flag=B(local+3);
            B(local+2)=1;
            flag=(u8)(flag+1);
            type=(count==3)?3:2;
            B(local+3)=flag;
        }
    }
    count=B(local+3);
    result=(result&0xffffff00u)|count;
    if (count==0) goto done;
    cm_fn_0054a110(local+8);
    count=B(local+3);
    frame.state=4;
    B(local+4)=count;
    W(local+6)=0xffff;
    B(local+5)=0;
    cm_fn_00417c10(local+8,local+0x24,0x00ae2c90);
    kind=B(local+2);
    bits=B(local+0x17);
    B(local+0x14)=kind;
    flag=B(record+0x48);
    B(local+0x10)=0xff;
    W(local+0x15)=(u16)(s16)(s8)type;
    bits=(u8)((bits&0xf9)|((flag==0)?4:0));
    id=D(record);
    B(local+0x17)=bits;
    owner=D(0x00ae28f0);
    B(local+0x11)=0x1d;
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
