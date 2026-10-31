#include "cmre.h"
/* Preferred-image VA 008bf400, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch68: club L0, direction L8, saved self Lc, date L10,
   message L18, message date L3d. Full EAX survives early exits. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0085e410(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053c590(u32,u8,u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0085e590(u32,u32,u32,u32,u8,u8,u8);
extern u32 CM_THISCALL cm_fn_004d01e0(u32,u32);
extern u32 CM_THISCALL cm_fn_005563f0(u32,u32);
extern u32 CM_CDECL cm_fn_00540980(u32,u8);
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
extern u32 CM_THISCALL cm_fn_0085a470(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cc0f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c7560(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c7070(u32,u32,u32,u16,u32,u32);

u32 CM_THISCALL cm_fn_008bf400(u32 self,volatile u32 person_arg,volatile u32 club_arg,volatile u32 start_arg,volatile u32 end_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x68];
    u32 l=(u32)scratch,eax,club,person,start,end,current,lo,hi,base,selected,saved_self;
    u16 year,other;
    frame.state=0xffffffff; frame.handler=0x00963b28;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l+0xc)=self;
    eax=cm_fn_0085e410(l+0x18);
    club=club_arg;
    frame.state=0;
    eax=cm_fn_00540a50(club,l+8,1);
    D(l)=eax;
    if (eax!=0 && D(l+8)!=0) D(l)=0;
    eax=cm_fn_0053c590(club,0,0);
    if ((s8)(u8)eax>=50) goto done;
    person=person_arg;
    base=D(0x00ae1050);
    eax=cm_fn_004cf6d0(base,person);
    if (eax) goto done;
    eax=start_arg;
    lo=D(eax); hi=D(eax+4);
    year=W(0x00ae2c92);
    eax=(eax&0xffff0000)|year;
    D(l+0x10)=lo; D(l+0x14)=hi;
    other=W(l+0x12);
    if (other>year) goto done;
    current=D(0x00ae2c90);
    if (other==year && (s16)lo>(s16)current) goto done;
    end=end_arg;
    lo=D(end); hi=D(end+4);
    D(l+0x10)=lo; D(l+0x14)=hi;
    other=W(l+0x12);
    if (other<year) goto done;
    if (other==year) {
        eax=(s16)lo<(s16)current;
        if (eax) goto done;
    }
    base=D(0x00ae1050);
    eax=cm_fn_004cf5a0(base,person);
    if (!eax) goto done;
    lo=D(person+0x46); eax=D(person+0x4a);
    D(l+0x10)=lo;
    other=W(l+0x12);
    D(l+0x14)=eax;
    year=W(end+2);
    eax=(eax&0xffff0000)|year;
    if (other<year || (other==year && (s16)lo<(s16)W(end))) {
        eax=cm_fn_00417c10(person+0x46,l+0x10,end);
        base=D(0x00ae1050);
        eax=cm_fn_004cf5a0(base,person);
        eax=cm_fn_00417c10(eax+0x2d,l+0x10,end);
    }
    selected=D(l);
    club=club_arg;
    eax=selected?selected:club;
    eax=cm_fn_0085e590(l+0x18,person,eax,end,0,1,1);
    start=start_arg;
    eax=cm_fn_00417c10(l+0x3d,l+0x10,start);
    base=D(0x00ae1050);
    eax=cm_fn_004d01e0(base,l+0x18);
    base=D(0x00ae28f0);
    eax=cm_fn_005563f0(base,person);
    eax=cm_fn_00540980(person,1);
    eax=(eax&0xffffff00)|B(person+0x6d);
    if ((u8)eax==2) B(person+0x6d)=0x10;
    eax=selected?selected:club;
    base=D(0x00dd7ef0);
    eax=cm_fn_0085add0(eax,0);
    if (eax) {
        eax=selected?selected:club;
        base=D(0x00dd7ef0);
        eax=cm_fn_0085a470(base,eax,0);
    }
    if (selected && selected!=club) {
        base=D(0x00ae1050);
        eax=cm_fn_004cc0f0(base,person,selected);
    }
    saved_self=D(l+0xc);
    eax=cm_fn_008c7560(saved_self,person,club,0);
    if (!eax) goto done;
    eax=cm_fn_0054c5b0(end,l+0x10,7);
    eax=cm_fn_00417c10(end,l,eax);
    eax=start_arg;
    lo=D(club+0x53);
    year=W(eax+2);
    eax=cm_fn_008c7070(saved_self,person,lo,year,1,1);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
