#include "cmre.h"

/* Preferred-image VA 0040aae0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch208: allocation L0, saved self L4, diagnostics L8/L108.
   Virtual slot3c at967510 resolves to actual original entry40ae90.
   Vtable read precedes field28 store; slot read follows that store. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_00518640(u32);
extern u32 CM_THISCALL cm_fn_0051fc00(u32,u8);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_0040b950(u32);
extern u32 CM_THISCALL cm_fn_0040b620(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_THISCALL cm_fn_0040bec0(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00518790(u32);
extern u32 CM_THISCALL cm_fn_0040c720(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0040aae0(u32 self,volatile u16 year_arg,volatile u32 competition_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x208];
    u32 local=(u32)scratch,value,year,name,extension,line,version,vtable,target;
    frame.state=0xffffffffu;
    frame.handler=0x00953759;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(local+4)=self;
    cm_fn_00518640(self);
    value=competition_arg;
    year=year_arg;
    D(self+4)=value;
    frame.state=0;
    D(self)=0x009674d4;
    W(self+0x40)=(u16)year;
    B(self+0xab)=0;
    B(self+0x44)=0xff;
    D(self+0x30)=0xffffffffu;
    D(self+0x2c)=4;
    B(self+0x43)=1;
    B(self+0x42)=2;
    D(self+0x1c)=0xffffffffu;
    D(self+0x20)=0xffffffffu;
    D(self+0xb2)=0;
    B(self+0x50)=0x29;
    B(self+0x49)=5;
    B(self+0x4a)=3;
    value=cm_fn_0051fc00(self,1);
    if (value!=0) goto done;
    value=D(self+0x2c);
    if ((s32)value>0) {
        value=cm_fn_00944e46(value*4);
        D(self+0xc)=value;
    } else D(self+0xc)=0;
    B(self+0x51)=0xf;
    B(self+0xb1)=0;
    value=cm_fn_0040b950(self);
    if (value==0) {
        name=local+0x108; extension=local+8; line=0x4d;
        goto diagnostic;
    }
    value=cm_fn_0040b620(self);
    if (value==0) {
        name=local+8; extension=local+0x108; line=0x54;
        goto diagnostic;
    }
    vtable=D(self);
    D(self+0x28)=0x3d0900;
    target=D(vtable+0x3c);
    value=((u32 (CM_THISCALL *)(u32,u32,u32,u32,u32))target)
        (self,0xffffffffu,self+0x3c,self+0x3a,0);
    D(self+0xa3)=value;
    value=cm_fn_00944cf1(0x5ce);
    D(local)=value;
    B((u32)&frame.state)=1;
    if (value!=0) value=cm_fn_0049ee70(value,self);
    B((u32)&frame.state)=0;
    D(self+8)=value;
    if (value==0) {
        name=local+8; extension=local+0x108; line=0x63;
        goto diagnostic;
    }
    cm_fn_0040bec0(self);
    value=D(self+0xb2);
    if (value!=0) {
        cm_fn_009452ca(value);
        D(self+0xb2)=0;
    }
    value=cm_fn_00518790(self);
    if (value==0) {
        name=local+8; extension=local+0x108; line=0x73;
        goto diagnostic;
    }
    cm_fn_0040c720(self);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00987a40,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return self;
}
