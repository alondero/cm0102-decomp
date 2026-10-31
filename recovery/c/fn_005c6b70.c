#include "cmre.h"

/* Preferred-image VA 005c6b70, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch230: raw date L0, date copies L8/L10, list L18, temporary L28.
   Retain unaligned DWORD date arguments, signed days, unsigned years, table
   reads, original helper boundaries and BYTE writes to linked SEH state. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_5c6b70;
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00669340(u32);
extern u32 CM_THISCALL cm_fn_005acbe0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL cm_fn_005c6b70(volatile u32 date_arg,volatile u32 club_arg,volatile u32 mode_arg)
{
    volatile frame_5c6b70 frame;
    u8 scratch[0x230];
    u32 local=(u32)scratch,date,club,mode,days=2,target,value,raw,record,index;
    u32 end_year,year,start_year,day,metadata,first_day,first_year,end_day,owner,result=0,version;
    frame.state=0xffffffffu;
    frame.handler=0x0095a131;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054a110(local+8);
    frame.state=0;
    cm_fn_0054a110(local+0x10);
    B((u32)&frame.state)=1;
    cm_fn_0068d020(local+0x18);
    club=club_arg;
    B((u32)&frame.state)=2;
    if (club==0) goto cleanup;
    date=date_arg;
    if (date==0) goto cleanup;
    mode=(u32)(s32)(s8)mode_arg;
    if (mode>11) goto diagnostic;
    target=D(0x005c6dc8+mode*4);
    (void)target;
    switch (mode) {
    case 1: case 10: break;
    case 2: days=3; break;
    case 0: case 3: case 4: case 11: days=1; break;
    default: goto diagnostic;
    }
    goto dates;
diagnostic:
    cm_fn_00944cff(0x009cd0a0,0,0,local+0x130,local+0x30);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x130,local+0x30,0xf18);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    days=1;
dates:
    value=cm_fn_0054c8e0(date,local,1);
    value=cm_fn_00417c10(local+8,local+0x28,value);
    raw=(value&0xffff0000u)|(u16)(s16)(s8)days;
    value=cm_fn_0054c5b0(date,local+0x28,raw);
    cm_fn_00417c10(local+0x10,local,value);
    mode=(u8)mode_arg;
    if (mode==0) goto ordinary;
    if (cm_fn_0053b900(club)!=0) goto ordinary;
    record=D(club+0x53);
    if (record==0) goto ordinary;
    if (B(record+0x11c)==0) goto ordinary;
    value=cm_fn_00669340(record);
    index=(u32)(s32)(s8)value*72;
    end_year=D(local+0x12);
    day=D(0x00b63d8a+index);
    metadata=D(0x00b63d8e + index);
    D(local)=day;
    D(local+4)=metadata;
    year=W(local+2);
    if ((u16)year>(u16)end_year) goto schedule;
    if ((u16)year==(u16)end_year) {
        value=W(local+0x10);
        if ((s16)day>=(s16)value) goto schedule;
    }
    day=D(0x00b63d99+index);
    metadata=D(0x00b63d9d+index);
    start_year=D(local+0xa);
    D(local)=day;
    D(local+4)=metadata;
    year=W(local+2);
    if ((u16)year>(u16)start_year) goto cleanup;
    if ((u16)year==(u16)start_year) {
        value=W(local+8);
        if ((s16)day>(s16)value) goto cleanup;
    }
    goto schedule;
ordinary:
    end_year=D(local+0x12);
schedule:
    value=D(club);
    end_day=D(local+0x10);
    first_year=D(local+0xa);
    first_day=D(local+8);
    owner=D(0x00ae2a58);
    value=cm_fn_005acbe0(owner,first_day,first_year,end_day,end_year,value,
                         local+0x18,0xffffffffu,0xfffffffeu,0xffffffffu);
    result=(u16)value==0;
cleanup:
    B((u32)&frame.state)=1;
    cm_fn_0068d040(local+0x18);
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
