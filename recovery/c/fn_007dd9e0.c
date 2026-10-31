#include "cmre.h"

/* Preferred-image VA 007dd9e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain full EAX, including pointer upper bits on an AL-only no-action exit.
   The 79-byte copy is forward DWORD/WORD/BYTE, with the original intervening
   receiver load and SEH BYTE update. Original helpers remain boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d0d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007dd9e0(u32 self,u32 record,volatile u32 input)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x9c];
    u32 local=(u32)scratch,prev,result,key,source,target,receiver,value,i;
    u32 manager,table,input_value;
    u8 flag,count,type,mode,bits;
    prev=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x00960026;
    frame.prev=prev;
    CM_FS(u32,0)=(u32)&frame;
    flag=B(record+0x2c);
    result=(prev&0xffffff00u)|flag;
    D(local+0x18)=self;
    B(local+1)=0;
    B(local+2)=0xff;
    if (flag==0) {
        flag=B(record+0x2d);
        result=(result&0xffffff00u)|flag;
        if (flag==0) goto done;
    }
    key=D(record);
    result=cm_fn_007e82e0(self,key,1);
    flag=B(record+0x2c);
    if (flag>0) {
        flag=B(result+4);
        flag=(u8)(flag+1);
        B(result+4)=flag;
        result=(u32)flag/3;
        if (flag%3==0) {
            B(local+1)=1;
            B(local+2)=1;
            B(local+3)=3;
        }
    }
    if (B(record+0x2d)==1) {
        cm_fn_0054a110(local+0x30);
        frame.state=0;
        cm_fn_0054a110(local+0x40);
        frame.state=1;
        result=cm_fn_007e8e10(self,local+0x2c,record,1);
        if (result!=0) {
            result=cm_fn_0054c770(0x00ae2c90,local+0x24,1);
            B((u32)&frame.state)=2;
            cm_fn_00417c10(local+0x40,local+0x1c,result);
            source=input;
            B(local+0x38)=0xa;
            B(local+0x39)=0xff;
            B(local+0x3a)=0xff;
            B(local+0x3b)=0xff;
            target=D(local+0x18);
            for (i=0;i<19;++i) {
                value=D(source+i*4);
                D(local+0x4c+i*4)=value;
            }
            receiver=D(target+7);
            value=W(source+76);
            W(local+0x98)=(u16)value;
            B((u32)&frame.state)=1;
            value=B(source+78);
            B(local+0x9a)=(u8)value;
            result=cm_fn_0068d0d0(receiver,local+0x2c,0x70);
        }
        count=B(local+1);
        B(local+2)=2;
        count=(u8)(count+1);
        B(local+3)=1;
        B(local+1)=count;
        frame.state=0xffffffffu;
    }
    count=B(local+1);
    result=(result&0xffffff00u)|count;
    if (count==0) goto done;
    cm_fn_0054a110(local+8);
    count=B(local+1);
    B(local+4)=count;
    frame.state=3;
    W(local+6)=0xffff;
    B(local+5)=0;
    cm_fn_00417c10(local+8,local+0x1c,0x00ae2c90);
    type=B(local+3);
    mode=B(local+2);
    W(local+0x15)=(u16)(s16)(s8)type;
    flag=B(record+0x48);
    B(local+0x14)=mode;
    bits=B(local+0x17);
    B(local+0x10)=0xa;
    bits=(u8)(bits&0xfd);
    B(local+0x11)=0xff;
    key=D(record);
    bits=(u8)((bits&0xfb)|(flag==0?4:0));
    B(local+0x12)=0xff;
    manager=D(0x00ae28f0);
    B(local+0x13)=0xff;
    B(local+0x17)=bits;
    cm_fn_00556940(manager,key,local+4);
    key=D(record);
    input_value=input;
    table=D(0x00ae23bc);
    manager=D(0x00ae28f0);
    /* 557280 overwrites incoming ECX before use and returns with RET 12. */
    (void)manager;
    result=cm_fn_00557280(table+key*110,local+4,input_value);
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
