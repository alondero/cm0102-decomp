#include "cmre.h"

/* Preferred-image VA 007b1260, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Swap-with-last retains the exact forward field loads/stores, including
   two original date-copy boundaries. Search and sort remain original. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4920(u32,u32);
extern u32 CM_CDECL cm_fn_00946464(u32,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007b1260(u32 self,volatile u32 input)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x238];
    u32 local=(u32)scratch,arg,count,table,found,last,value,i,line,name,ext;
    u32 result,version;
    frame.state=0xffffffffu;
    frame.handler=0x0095f456;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0054a110(local+0x18);
    frame.state=0;
    cm_fn_0054a110(local+0x20);
    arg=input;
    frame.state=1;
    if (arg==0) {
        name=local+0x138; ext=local+0x38; line=0x733;
        goto error;
    }
    cm_fn_007e4920(arg,local+8);
    count=D(self+0x1f6);
    if (count==0) {
        name=local+0x38; ext=local+0x138; line=0x73d;
        goto error;
    }
    table=D(self+0x1ee);
    found=cm_fn_00946464(local+8,table,count,0x2d,0x007bce50);
    if (found==0) {
        name=local+0x38; ext=local+0x138; line=0x747;
        goto error;
    }
    count=D(self+0x1f6);
    table=D(self+0x1ee);
    last=table+count*45-45;
    for (i=0;i<4;++i) {
        value=D(last+i*4);
        D(found+i*4)=value;
    }
    cm_fn_00417c10(found+0x10,local,last+0x10);
    cm_fn_00417c10(found+0x18,local,last+0x18);
    value=B(last+0x20);
    B(found+0x20)=(u8)value;
    value=D(last+0x21);
    D(found+0x21)=value;
    value=D(last+0x25);
    D(found+0x25)=value;
    value=B(last+0x29);
    B(found+0x29)=(u8)value;
    value=B(last+0x2a);
    B(found+0x2a)=(u8)value;
    value=B(last+0x2b);
    B(found+0x2b)=(u8)value;
    value=B(last+0x2c);
    B(found+0x2c)=(u8)value;
    count=D(self+0x1f6);
    --count;
    D(self+0x1f6)=count;
    if (count!=0) {
        table=D(self+0x1ee);
        cm_fn_00945333(table,count,0x2d,0x007bce50);
    } else {
        table=D(self+0x1ee);
        cm_fn_009452ca(table);
        D(self+0x1ee)=0;
        D(self+0x1f2)=0;
    }
    D(self+0x2a4)=0;
    result=1;
    goto done;
error:
    cm_fn_00944cff(0x00a701b0,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    result=0;
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
