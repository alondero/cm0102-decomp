#include "cmre.h"
/* Preferred-image VA008da5e0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/fullEAX from final availability/send boundary.
   SEH964496/scratchf8 zeroBYTEL3/dateL4/primaryclubLc/currentdateL10/
   messageL18. GlobalAE1080 is a DWORD store. Keep the second lookup's
   unchecked dereference, cached club base and original late date reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008cee70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007875e0(u32,u32);

u32 CM_THISCALL cm_fn_008da5e0(u32 self,volatile u32 input_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xf8];
    u32 l=(u32)scratch,input,person,manager,base,id,eax,other,primary;
    u32 pointer,value,current,extra,date,yearword;
    u16 currentyear;
    u8 byte;
    frame.state=0xffffffffu;frame.handler=0x00964496;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    input=input_arg;
    id=D(input+0xc);
    B(l+3)=0;
    manager=D(0x00ae1050);base=D(0x00ae23bc);
    person=base+id*110u;
    eax=cm_fn_004cf5a0(manager,person);
    base=D(0x00ae23b4);
    D(0x00ae1080)=eax;
    if(eax!=0 && (B(eax+0x35)&0x3f)!=5){
        id=D(eax+4);other=base+id*581u;
    }else other=0;
    id=D(input+8);
    primary=base+id*581u;
    D(l+0xc)=primary;
    cm_fn_0054a110(l+4);
    frame.state=0;
    if(other!=0){
        manager=D(0x00ae1050);
        eax=cm_fn_004cf5a0(manager,person);
        /* Original repeats the lookup and dereferences even a null result. */
        if((B(eax+0x35)&0x3f)==5) other=0;
    }
    cm_fn_0077bbe0(l+0x18,0xfa5,0);
#define SET(n,v) cm_fn_00785790(l+0x18,n,(u32)(v))
    id=D(person);
    B((u32)&frame.state)=1;
    SET(0,id);
    pointer=D(person+4);value=D(pointer+0x33);SET(1,value);
    pointer=D(person+8);value=D(pointer+0x33);SET(2,value);
    pointer=D(person+0xc);value=D(pointer+0x33);SET(3,value);
    value=D(input+8);SET(4,value);
    if(other!=0) value=D(other);else value=0xffffffffu;
    SET(5,value);
    value=D(input+0x20);SET(6,value);
    value=D(input+0x24);SET(7,value);
    value=D(input+0x1b);SET(8,value);
    value=(u32)(s32)(s8)B(input+0x2c);SET(9,value);
    eax=cm_fn_008e7a70(input,0,1,0,1);SET(10,eax);
    value=(u32)(s32)(s8)B(input+0x2f);SET(12,value);
    value=(u32)(s32)(s8)B(input+0x11);SET(17,value);
    cm_fn_008cee70(self,input,l+4);
    current=D(0x00ae2c90);extra=D(0x00ae2c94);
    date=D(l+4);
    D(l+0x10)=current;
    currentyear=W(l+0x12);
    D(l+0x14)=extra;
    yearword=D(l+6);
    if(currentyear<(u16)yearword ||
       (currentyear==(u16)yearword && (s16)(u16)current<(s16)(u16)date)){
        SET(14,(u32)(s32)(s16)(u16)date);
        value=D(l+6)&0xffffu;SET(15,value);
        byte=(u8)(B(input+0x30)<<4);
        value=(u32)((s32)(s8)byte>>4);
    }else{
        SET(14,0);SET(15,0);value=0;
    }
    SET(13,value);
    pointer=D(input+4);
    if(pointer!=0) byte=B(pointer);else byte=B(l+3);
    value=(u32)(s32)(s8)byte;SET(11,value);
    if(other!=0){
        manager=D(0x00b7431c);
        eax=cm_fn_007875e0(manager,other);
        if(eax!=0 && B(input+0x2c)!=12){
            byte=B(input+0x2f);
            if(byte==0 || (B(input+0x30)&15)==0){
                value=D(other);SET(16,value);
                manager=D(0x00b7431c);
                cm_fn_007861e0(manager,l+0x18,other);
            }
        }
    }
    primary=D(l+0xc);manager=D(0x00b7431c);
    eax=cm_fn_007875e0(manager,primary);
    if(eax!=0){
        value=D(primary);SET(16,value);
        manager=D(0x00b7431c);
        eax=(u32)cm_fn_007861e0(manager,l+0x18,primary);
    }
#undef SET
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
