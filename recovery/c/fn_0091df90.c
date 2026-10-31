#include "cmre.h"

/* Preferred-image VA 0091df90, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Signed year remainder, WORD wrap, and original call/store order retained.
   Construction helpers remain original boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32,u32);
extern u32 CM_THISCALL cm_fn_0091fa80(u32);
extern u32 CM_THISCALL cm_fn_0091ff30(u32);
extern u32 CM_THISCALL cm_fn_0091ecf0(u32);
extern u32 CM_THISCALL cm_fn_0091ec00(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static void diagnostic(u32 name,u32 ext,u32 line)
{
    u32 version;
    cm_fn_00944cff(0x00ad4afc,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
}

u32 CM_THISCALL cm_fn_0091df90(u32 self,volatile u32 year_arg,
                              volatile u32 competition_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x208];
    u32 local=(u32)scratch,year,competition,value,first,second,result,count;
    frame.state=0xffffffffu;
    frame.handler=0x00965ab9;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    D(local+4)=self;
    cm_fn_00682200(self);
    year=year_arg;
    competition=competition_arg;
    D(self)=0x00970d78;
    W(self+0x40)=(u16)year;
    D(self+4)=competition;
    B(self+0x42)=4;
    first=D(0x009cf774);
    D(self+0x1c)=first;
    second=D(0x009cf778);
    frame.state=0;
    value=((u32)(s32)(s16)year-0x7d0u)&0x80000003u;
    D(self+0x20)=second;
    B(self+0x50)=0;
    if ((s32)value<0) value=((value-1)|0xfffffffcu)+1;
    if (value==1) W(self+0x40)=(u16)(year-1);
    for (;;) {
        value=((u32)(s32)(s16)W(self+0x40)-0x7d0u)&0x80000003u;
        if ((s32)value<0) value=((value-1)|0xfffffffcu)+1;
        if (value==0) break;
        W(self+0x40)=(u16)(W(self+0x40)+1);
    }
    D(self+0x45)=0;
    B(self+0x51)=0xa;
    result=cm_fn_00687b10(self,1);
    if (result!=0) goto done;
    W(self+0xd9)=0x28;
    cm_fn_0091fa80(self);
    D(self+0x2c)=3;
    result=cm_fn_00944cf1(0x5ce);
    D(local)=result;
    B((u32)&frame.state)=1;
    if (result!=0) result=cm_fn_0049ee70(result,self);
    B((u32)&frame.state)=0;
    D(self+8)=result;
    if (result==0) {
        diagnostic(local+0x108,local+8,0x63);
        goto done;
    }
    cm_fn_0091ff30(self);
    B(self+0x44)=0xff;
    D(self+0x30)=0xffffffffu;
    count=D(self+0x2c);
    if ((s32)count>0) {
        result=cm_fn_00944e46(count<<2);
        D(self+0xc)=result;
    } else D(self+0xc)=0;
    result=cm_fn_0091ecf0(self);
    if (result==0) {
        diagnostic(local+8,local+0x108,0x75);
        goto done;
    }
    result=cm_fn_0091ec00(self);
    if (result==0) diagnostic(local+8,local+0x108,0x7c);
done:
    CM_FS(u32,0)=frame.prev;
    return self;
}
