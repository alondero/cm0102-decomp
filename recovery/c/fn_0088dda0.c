#include "cmre.h"

/* Preferred-image VA 0088dda0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Saved index uses signed low BYTE; virtual and constructor arguments reload
   the original DWORD later. Partially written output DWORDs stay uninitialized
   above their WORDs, which are all the constructor consumes. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static u32 diagnostic(u32 name,u32 ext,u32 line)
{
    u32 version,result;
    cm_fn_00944cff(0x00a8b4a0,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return result;
}

u32 CM_THISCALL cm_fn_0088dda0(u32 self,volatile u32 stage_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x410];
    u32 local=(u32)scratch,index,count,clubs,copied,table,offset,value;
    u32 scan,vtable,target,arg,schedule,object,result,round_flag,rounds;
    u32 competition;
    u8 flag_c;
    u16 year;
    frame.state=0xffffffffu;
    frame.handler=0x009630ae;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    index=(u32)(s32)(s8)stage_arg;
    D(local+4)=index;
    count=31-(index&1);
    clubs=cm_fn_00944e46(count*4);
    if (clubs==0) {
        result=diagnostic(local+0x210,local+0x10,0x32b);
        goto done;
    }
    copied=0;
    scan=0;
    D(local)=scan;
    do {
        if ((s16)scan>=(s16)W(self+0xb6)) break;
        table=D(self+0xb2);
        offset=(u32)(s32)(s16)scan*4;
        value=D(table+offset);
        if (value!=0) {
            index=(u32)(s32)(s16)copied;
            ++copied;
            D(clubs+index*4)=value;
            table=D(self+0xb2);
            scan=D(local);
            D(table+offset)=0;
        }
        ++scan;
        D(local)=scan;
    } while ((s16)copied<(s16)count);
    vtable=D(self);
    arg=stage_arg;
    target=D(vtable+0x3c);
    schedule=((u32 (CM_THISCALL *)(u32,u32,u32,u32,u32))target)
        (self,arg,local+8,local+0xc,0);
    object=cm_fn_00944cf1(0xb2);
    D(local)=object;
    frame.state=0;
    if (object!=0) {
        flag_c=B(self+0x51);
        round_flag=D(local+0xc);
        arg=stage_arg;
        year=W(self+0x40);
        competition=D(self+4);
        rounds=D(local+8);
        result=cm_fn_00522e00(object,self,(s16)count,clubs,(s16)rounds,
            competition,schedule,year,(u8)arg,1,(u16)round_flag,flag_c,0,0,0,0);
    } else result=0;
    table=D(self+0xc);
    index=D(local+4);
    frame.state=0xffffffffu;
    D(table+index*4)=result;
    cm_fn_009452ca(clubs);
    result=cm_fn_009452ca(schedule);
    table=D(self+0xc);
    if (D(table+index*4)==0)
        result=diagnostic(local+0x310,local+0x110,0x347);
    else D(self+0x30)=index;
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
