#include "cmre.h"
/* Preferred-image VA00765f70/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full allocation-or-zero EAX; SEH95dabb/scratch218.
   Date L0, difference L8, intermediate L10, diagnostics L118/L18.
   Three arguments pushed before 54a3f0 belong to the seven-argument
   521f10 call; 54a3f0 consumes ECX only. Preserve every store/reload. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_CDECL cm_fn_00521f10(u32,u16,u16,u32,u8,u8,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);

u32 CM_THISCALL cm_fn_00765f70(u32 self,volatile u8 selector,
                              volatile u32 first_arg,volatile u32 second_arg,
                              volatile u32 numbered_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x218];
    u32 l=(u32)scratch,eax,first,second,numbered,allocation,version;
    u16 word;
    u8 byte;
    frame.state=0xffffffffu;frame.handler=0x0095dabb;
    frame.prev=CM_FS(volatile u32,0);
    byte=selector;CM_FS(volatile u32,0)=(u32)&frame;
    if(byte!=0xff){eax=0;goto done;}
    eax=D(self+0xb2);first=D(self+0xb6);D(l)=eax;D(l+4)=first;
    numbered=numbered_arg;frame.state=0;
    if(numbered!=0) D(numbered)=0;
    first=first_arg;second=second_arg;
    W(first)=3;W(second)=0;
    allocation=(u32)cm_fn_00944e46(0x138);
    if(allocation==0){
        cm_fn_00944cff(0x00a5ab00,0,0,l+0x118,l+0x18);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x118,l+0x18,0xa1);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
        eax=0;goto done;
    }
    word=W(l);W(allocation)=word;B(allocation+2)=0;B(allocation+6)=1;
    word=W(l);W(allocation+3)=word;B(allocation+5)=0;D(allocation+0x13)=1;
    eax=cm_fn_0054a3f0(l);
    cm_fn_00521f10(allocation,0,0,eax,2,3,0x10);
    eax=cm_fn_0054a3f0(l);
    cm_fn_00521f10(allocation,0,1,eax,1,3,0x10);
    cm_fn_00521f10(allocation,0,2,0xffffffffu,0xff,0xff,0);
    W(allocation+7)=0x82;W(allocation+9)=0;W(allocation+0xb)=0;
    word=W(self+0xba);word|=0x100;W(allocation+0xd)=word;
    B(allocation+0x17)=1;W(allocation+0x18)=4;W(allocation+0x1a)=2;
    W(allocation+0x1c)=4;W(allocation+0xf)=0;W(allocation+0x1e)=0;
    B(allocation+0x20)=0;B(allocation+0x21)=1;B(allocation+0x22)=0;
    D(allocation+0x5c)=0;D(allocation+0x60)=0;D(allocation+0x64)=0;
    eax=cm_fn_0054c5b0(l,l+0x10,1);
    cm_fn_00417c10(l,l+8,eax);
    word=W(allocation+3);W(allocation+0x68)=word;
    byte=B(allocation+5);B(allocation+0x6a)=byte;B(allocation+0x6e)=2;
    word=W(l);W(allocation+0x6b)=word;
    byte=B(l+2);byte=(u8)(byte-B(self+0x40));B(allocation+0x6d)=byte;
    D(allocation+0x7b)=0x10;W(allocation+0x6f)=0x96;
    W(allocation+0x71)=1;W(allocation+0x73)=0;
    word=W(self+0xba);W(allocation+0x75)=word;B(allocation+0x7f)=1;
    W(allocation+0x80)=2;W(allocation+0x82)=1;
    word=W(allocation+3);W(allocation+0x84)=0;W(allocation+0xd0)=word;
    byte=B(allocation+5);W(allocation+0x77)=0;W(allocation+0x86)=0;
    B(allocation+0x88)=0;B(allocation+0x89)=1;B(allocation+0x8a)=0;
    D(allocation+0xc4)=0;D(allocation+0xc8)=0;D(allocation+0xcc)=0;
    B(allocation+0xd2)=byte;B(allocation+0xd6)=1;
    word=W(l);W(allocation+0xd3)=word;
    byte=B(l+2);byte=(u8)(byte-B(self+0x40));
    D(allocation+0xe3)=0x10;W(allocation+0xd7)=0x8c;
    W(allocation+0xd9)=2;B(allocation+0xd5)=byte;W(allocation+0xdb)=0;
    word=W(self+0xba);W(allocation+0xe8)=2;W(allocation+0xea)=1;
    W(allocation+0xdd)=word;B(allocation+0xe7)=1;W(allocation+0xec)=0;
    W(allocation+0xdf)=0;W(allocation+0xee)=0;B(allocation+0xf0)=0;
    B(allocation+0xf1)=1;B(allocation+0xf2)=0;D(allocation+0x12c)=0;
    D(allocation+0x130)=0;D(allocation+0x134)=0;
    eax=allocation;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
