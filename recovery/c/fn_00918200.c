#include "cmre.h"
/* Preferred-image VA00918200, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/fullEAX returns self. Scratch208 allocationL0/selfL4/diagL8,L108.
   SignedWORD year normalization and lowBYTE SEH state stores retained. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32,u8);
extern u32 CM_THISCALL cm_fn_00919e30(u32);
extern u32 CM_THISCALL cm_fn_009191e0(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_THISCALL cm_fn_0091b760(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00918200(u32 self,volatile u32 year_arg,volatile u32 competition_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x208];
    u32 l=(u32)scratch,year,competition,parent,eax,value,name,extension,line,version;
    s32 difference;
    u8 mode;
    frame.state=0xffffffff; frame.handler=0x00965957;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l+4)=self;
    cm_fn_00682200(self);
    year=year_arg; competition=competition_arg;
    D(self)=0x00970cc4; W(self+0x40)=(u16)year; D(self+4)=competition;
    B(self+0x42)=4;
    parent=D(0x009cf77c);
    difference=(s16)year-2000;
    frame.state=0;
    D(self+0x1c)=parent; D(self+0x20)=0xffffffff;
    D(self+0xf7)=0; D(self+0xee)=0; D(self+0xf2)=0; D(self+0xfb)=0;
    B(self+0xf6)=1; B(self+0x50)=0;
    if (difference%4==1) W(self+0x40)=(u16)(year-1);
    difference=(s16)W(self+0x40)-2000;
    while (difference%4!=0) {
        W(self+0x40)=(u16)(W(self+0x40)+1);
        difference=(s16)W(self+0x40)-2000;
    }
    if ((s16)W(self+0x40)<2004) W(self+0x40)=2004;
    eax=cm_fn_00944cf1(14);
    D(l)=eax;
    B((u32)&frame.state)=1;
    if (eax) eax=cm_fn_0068d020(eax);
    else eax=0;
    B((u32)&frame.state)=0;
    D(self+0x10)=eax; B(self+0x51)=15;
    eax=cm_fn_00687b10(self,1);
    if (eax) goto done;
    W(self+0xd9)=36;
    cm_fn_00919e30(self);
    mode=B(self+0xf6);
    if (mode==0) D(self+0x2c)=11;
    else if (mode==1 || mode==2) D(self+0x2c)=10;
    else if (mode==3) D(self+0x2c)=9;
    value=D(self+0x2c);
    B(self+0x44)=0xff; D(self+0x30)=0xffffffff;
    if ((s32)value>0) {
        eax=cm_fn_00944e46(value*4);
        D(self+0xc)=eax;
    } else D(self+0xc)=0;
    D(self+0x45)=0;
    eax=cm_fn_009191e0(self);
    if (!eax) {
        name=l+0x108; extension=l+8; line=0x7f;
        goto diagnostic;
    }
    eax=cm_fn_00944cf1(0x5ce);
    D(l)=eax;
    B((u32)&frame.state)=2;
    if (eax) eax=cm_fn_0049ee70(eax,self);
    else eax=0;
    B((u32)&frame.state)=0;
    D(self+8)=eax;
    if (!eax) {
        name=l+8; extension=l+0x108; line=0x87;
        goto diagnostic;
    }
    if ((s16)year>=(s16)W(self+0x40)) cm_fn_0091b760(self);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00ad4a6c,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return self;
}
