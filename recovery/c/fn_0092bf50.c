#include "cmre.h"

/* Preferred-image VA 0092bf50, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch208, savedselfL4,allocationL0,diagnosticsL8/L108. Keep original
   WORD year loop, initializer order, BYTE SEH states and return self. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_92bf50;
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_0092da40(u32);
extern u32 CM_THISCALL cm_fn_0092d200(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0092bf50(u32 self,volatile u32 year_arg,volatile u32 competition_arg)
{
    volatile frame_92bf50 frame;
    u8 scratch[0x208];
    u32 local=(u32)scratch,value,original_year,year,allocation,version,name,extension,line;
    s32 remainder;
    frame.state=0xffffffffu;
    frame.handler=0x00965f89;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(local+4)=self;
    cm_fn_00682200(self);
    value=competition_arg;
    original_year=year_arg;
    D(self+4)=value;
    year=original_year+1;
    frame.state=0;
    remainder=((s32)(s16)year-0x7ca)%4;
    D(self)=0x009712f0;
    B(self+0x42)=4;
    D(self+0x1c)=0xffffffffu;
    D(self+0x20)=0xffffffffu;
    B(self+0x4b)=1;
    B(self+0x50)=0;
    W(self+0x40)=(u16)year;
    while (remainder!=0) {
        W(self+0x40)=(u16)(W(self+0x40)+1);
        year=W(self+0x40);
        remainder=((s32)(s16)year-0x7ca)%4;
    }
    B(self+0x51)=0xf;
    W(self+0x36)=0;
    W(self+0x38)=32;
    allocation=cm_fn_00945501(32,6);
    D(self+0x14)=allocation;
    if (allocation==0) {
        name=local+0x108;
        extension=local+8;
        cm_fn_00944cff(0x00ad5888,0,0,name,extension);
        line=0x58;
        goto diagnostic;
    }
    value=cm_fn_00687b10(self,1);
    if (value!=0) goto done;
    W(self+0xd9)=0x28;
    B(self+0x44)=0xff;
    D(self+0x30)=0xffffffffu;
    D(self+0x2c)=8;
    allocation=cm_fn_00944e46(32);
    D(self+0xc)=allocation;
    cm_fn_0092da40(self);
    value=cm_fn_0092d200(self);
    if (value==0) {
        name=local+8;
        extension=local+0x108;
        cm_fn_00944cff(0x00ad5888,0,0,name,extension);
        line=0x72;
        goto diagnostic;
    }
    allocation=cm_fn_00944cf1(0x5ce);
    D(local)=allocation;
    B((u32)&frame.state)=1;
    if (allocation!=0) allocation=cm_fn_0049ee70(allocation,self);
    B((u32)&frame.state)=0;
    D(self+8)=allocation;
    if (allocation==0) {
        cm_fn_00944cff(0x00ad5888,0,0,local+8,local+0x108);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+8,local+0x108,0x7a);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
    }
    if (W(self+0x40)==(u16)original_year) W(self+0x36)=32;
    else D(self+0x45)=0;
    D(self+0xb1)=0;
    W(self+0x3e)=0;
    goto done;
diagnostic:
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return self;
}
