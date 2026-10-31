#include "cmre.h"

/* Preferred-image VA 0057ce10, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep signed WORD year alignment, partial SEH state, initializer order and
   error paths which still return self. Original callees stay explicit. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_57ce10;
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32,u32);
extern u32 CM_THISCALL cm_fn_0057e8f0(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00580bd0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0057ce10(u32 self,volatile u32 year_arg,volatile u32 competition_arg)
{
    volatile frame_57ce10 frame;
    u8 scratch[0x208];
    u32 local=(u32)scratch,year,value,original_year,allocation,name,extension,line,version;
    s32 remainder;
    frame.state=0xffffffffu;
    frame.handler=0x00958867;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(local+4)=self;
    cm_fn_00682200(self);
    value=competition_arg;
    original_year=year_arg;
    D(self+4)=value;
    year=original_year-1;
    frame.state=0;
    remainder=((s32)(s16)year-0x7c6)%4;
    D(self)=0x0096a0d4;
    B(self+0x42)=4;
    D(self+0x1c)=0xffffffffu;
    D(self+0x20)=0xffffffffu;
    D(self+0x45)=0;
    D(self+0xf7)=0;
    D(self+0xee)=0;
    D(self+0xf2)=0;
    D(self+0xfb)=0;
    B(self+0xf6)=1;
    B(self+0x50)=0;
    B(self+0xff)=7;
    B(self+0x100)=8;
    B(self+0x101)=9;
    W(self+0x40)=(u16)year;
    while (remainder!=0) {
        W(self+0x40)=(u16)(W(self+0x40)+1);
        year=W(self+0x40);
        remainder=((s32)(s16)year-0x7c6)%4;
    }
    W(self+0xd9)=0x24;
    allocation=cm_fn_00944cf1(0xe);
    D(local)=allocation;
    B((u32)&frame.state)=1;
    if (allocation!=0) allocation=cm_fn_0068d020(allocation);
    B((u32)&frame.state)=0;
    D(self+0x10)=allocation;
    B(self+0x51)=0xf;
    value=cm_fn_00687b10(self,1);
    if (value!=0) goto done;
    value=cm_fn_0057e8f0(self);
    if (value==0) {
        name=local+0x108;
        extension=local+8;
        cm_fn_00944cff(0x009c4674,0,0,name,extension);
        line=0x5e;
        goto diagnostic;
    }
    allocation=cm_fn_00944cf1(0x5ce);
    D(local)=allocation;
    B((u32)&frame.state)=2;
    if (allocation!=0) allocation=cm_fn_0049ee70(allocation,self);
    B((u32)&frame.state)=0;
    D(self+8)=allocation;
    if (allocation==0) {
        name=local+8;
        extension=local+0x108;
        cm_fn_00944cff(0x009c4674,0,0,name,extension);
        line=0x66;
        goto diagnostic;
    }
    B(self+0x44)=0xff;
    D(self+0x30)=0xffffffffu;
    D(self+0x2c)=10;
    allocation=cm_fn_00944e46(0x28);
    year=W(self+0x40);
    D(self+0xc)=allocation;
    if ((s16)original_year>(s16)year) cm_fn_00580bd0(self);
    year=W(self+0x40);
    if ((s32)(s16)year-(s32)(s16)original_year>2) D(self+0x45)=0;
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
