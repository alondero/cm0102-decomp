#include "cmre.h"

/* Preferred-image VA 007a6f10, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep late year-argument WORD stores and DWORD reloads, explicit ECX at
   the 4d0c60 boundary, and AL-only final return. 549f70 consumes day WORD,
   month BYTE and year WORD; unknown high bits of MOVSX DX/CX are dead there. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
/* The original overwrites only the WORD inside a DWORD argument slot. */
typedef u16 argument_word __attribute__((__may_alias__));
#define ARG_WORD(a) (*(volatile argument_word *)(u32)(a))
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00540980(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_CDECL cm_fn_00547d90(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_004d45f0(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u16,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004c7950(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cc0f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_008626e0(u32,u32);
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
extern u32 CM_THISCALL cm_fn_0085a470(u32,u32,u32);

u32 CM_STDCALL cm_fn_007a6f10(volatile u32 person_arg,volatile u32 club_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x20];
    u32 local=(u32)scratch,club,person,slot,manager,result,year,month,day;
    u32 date,output,record,value;
    u8 flags;
    u16 target_year;
    frame.state=0xffffffffu;
    frame.handler=0x0095f1d0;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    club=club_arg;
    for (slot=0;slot<50;++slot)
        if (D(club+0xd7+slot*4)==0) break;
    if (slot==50) {
        result=0;
        goto done;
    }
    person=person_arg;
    D(club+0xd7+slot*4)=person;
    D(person+0x39)=club;
    D(person+0x4e)=0;
    manager=D(0x00ae1050);
    result=cm_fn_004d0c60(manager,person,club,1,0xffffffffu,0);
    D(person+0x52)=result;
    cm_fn_00540980(person,1);
    result=cm_fn_0090cfc0(4);
    if (result==0) cm_fn_00547d90(person,club,0);
    if (B(club+0x64)==3) {
        value=D(person+0x52);
        if ((s32)value>5000) D(person+0x52)=5000;
    }
    if ((s8)B(person+0x18)<19) {
        target_year=W(0x00ae2c92);
        ARG_WORD(&club_arg)=target_year;
        year=club_arg;
        year+=2;
        manager=D(0x00ae1050);
        (void)manager;
        month=cm_fn_004d45f0(club);
        result=cm_fn_0090cfc0(28);
        day=(u32)(s32)(s8)(u8)(result+1);
        date=cm_fn_00549f70(local,(u16)day,month,year,0xffffffffu);
        output=local+8;
        frame.state=0;
    } else {
        target_year=W(0x00ae2c92);
        ARG_WORD(&club_arg)=target_year;
        result=cm_fn_0090cfc0(6);
        year=club_arg;
        manager=D(0x00ae1050);
        (void)manager;
        year=result+year+1;
        month=cm_fn_004d45f0(club);
        result=cm_fn_0090cfc0(28);
        day=(u32)(s32)(s8)(u8)(result+1);
        date=cm_fn_00549f70(local+0x10,(u16)day,month,year,0xffffffffu);
        output=local+0x18;
        frame.state=1;
    }
    cm_fn_00417c10(person+0x46,output,date);
    value=D(person+0x52);
    manager=D(0x00ae1050);
    frame.state=0xffffffffu;
    cm_fn_004c7950(manager,person,value);
    result=cm_fn_00600a50(club,1,1,0,0);
    if (result!=0) {
        manager=D(0x00ae1050);
        cm_fn_004cc0f0(manager,person,club);
        manager=D(0x00ae1050);
        record=cm_fn_004cf5a0(manager,person);
        flags=B(record+0x4f);
        B(record+0x4f)=(u8)((flags&0xf)|0x60);
        manager=D(0x00ae1050);
        record=cm_fn_004cf5a0(manager,person);
        flags=B(record+0x35);
        B(record+0x35)=(u8)((flags&0xc1)|1);
    }
    manager=D(0x00ae1050);
    record=cm_fn_004cf5a0(manager,person);
    if (record!=0) {
        cm_fn_008626e0(record,person);
        B(record+0x3a)=0;
        result=cm_fn_00600a50(club,1,1,0,0);
        if (result==0) {
            target_year=(u16)(W(0x00ae2c92)+5);
            if (W(record+0x2f)>target_year) {
                W(record+0x2f)=target_year;
                W(record+0x2d)=0;
            }
        }
    }
    manager=D(0x00dd7ef0);
    (void)manager;
    result=cm_fn_0085add0(club,0);
    if (result!=0) {
        manager=D(0x00dd7ef0);
        result=cm_fn_0085a470(manager,club,0);
    }
    result=(result&0xffffff00u)|1;
done:
    CM_FS(u32,0)=frame.prev;
    return result;
}
