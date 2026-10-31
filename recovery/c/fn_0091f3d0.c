#include "cmre.h"

/* Preferred-image VA 0091f3d0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch218: current date L0, constructed date L8, first copy L10,
   diagnostics L18/L118. 549ef0 consumes only low WORDs of its date args
   (549ef0..549f01). Preserve full EAX on every AL-only false exit. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00775070(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00775220(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006835c0(u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0091f1a0(u32);
extern u32 CM_THISCALL cm_fn_006847c0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0091f3d0(u32 self,u32 date,volatile u32 mode_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x218];
    u32 local=(u32)scratch,result,low,high,day_arg,year_arg,year,input_year,target;
    u32 value,base,owner,competition,index,count,name,extension,line,version;
    frame.state=0xffffffffu;
    frame.handler=0x00965b46;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    low=D(date);
    high=D(date+4);
    D(local+0x10)=low;
    D(local+0x14)=high;
    year_arg=W(self+0xb8);
    day_arg=(low&0xffff0000u)|W(self+0xb6);
    year_arg=(u16)(year_arg+W(self+0x40));
    frame.state=0;
    result=cm_fn_00549ef0(local+8,day_arg,year_arg);
    low=D(date);
    high=D(date+4);
    year=W(local+0xa);
    result=(result&0xffff0000u)|year;
    D(local)=low;
    D(local+4)=high;
    input_year=W(local+2);
    B((u32)&frame.state)=1;
    if (input_year<year) goto false_byte;
    target=D(local+8);
    if (input_year==year && (s16)low<(s16)target) goto false_byte;
    low=D(date);
    high=D(date+4);
    D(local)=low;
    input_year=W(local+2);
    D(local+4)=high;
    if (input_year!=year || (u16)low!=(u16)target) {
        if (W(date+2)!=2001) goto generic;
    }
    result=D(self+0x45);
    if (result!=0) goto generic;
    result=mode_arg;
    if (result==0) goto generic;
    value=D(0x009cfa18);
    base=D(0x00ae23a4);
    value=base+value*198;
    owner=D(0x00b74318);
    cm_fn_00775070(owner,value,0);
    index=0;
    if ((s16)W(self+0x3e)>0) {
        do {
            competition=D(self+4);
            value=(u32)(s32)(s16)index;
            base=D(self+0xb1);
            value=D(base+value*59);
            owner=D(0x00b74318);
            value=D(value+0x53);
            cm_fn_00775220(owner,value,competition);
            ++index;
        } while ((s16)index<(s16)W(self+0x3e));
    }
    count=D(self+0xf3);
    index=0;
    if ((s32)count>0) {
        value=0;
        do {
            base=D(self+0xef);
            value=D(base+value*4);
            if (value!=0) {
                competition=D(self+4);
                value=D(value+0x53);
                owner=D(0x00b74318);
                cm_fn_00775220(owner,value,competition);
            }
            count=D(self+0xf3);
            ++index;
            value=(u32)(s32)(s16)index;
        } while ((s32)value<(s32)count);
    }
    result=cm_fn_006835c0(self);
    if (result==0) {
        name=local+0x118;
        extension=local+0x18;
        line=0x3a4;
        goto diagnostic;
    }
    result=cm_fn_006827d0(self,0);
    if (result==0) {
        name=local+0x18;
        extension=local+0x118;
        line=0x3ab;
        goto diagnostic;
    }
    cm_fn_0091f1a0(self);
    D(self+0x45)=1;
generic:
    value=mode_arg;
    result=cm_fn_006847c0(self,date,value);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00ad4afc,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
false_byte:
    result&=0xffffff00u;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
