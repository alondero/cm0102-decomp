#include "cmre.h"
/* Preferred-image VA007e6720/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/RET4/full incidental EAX; SEH9607b0/scratch6c rawdateL0,
   dateL8/temporaryL10/copied79-byte recordL18. Keep unaligned DWORD
   date/flag reads, copied-club replacements, partial AX and late reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32,u32);
extern u32 CM_CDECL cm_fn_00542740(u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern u32 CM_THISCALL cm_fn_005ac7a0(u32,u32);
extern u32 CM_THISCALL cm_fn_005ae870(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0085c260(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005aa680(u32,u32,u32);

u32 CM_STDCALL cm_fn_007e6720(volatile u32 record_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x6c];
    u32 l=(u32)scratch,record,eax,club,away,pointer,base,id,first,second;
    u32 source,destination,index,year,day,flags,receiver,preferred;
    frame.prev=CM_FS(volatile u32,0);frame.state=0xffffffffu;
    frame.handler=0x009607b0;CM_FS(volatile u32,0)=(u32)&frame;
    eax=2500;record=record_arg;club=D(record+0x1c);
    if((s16)W(club+0x80)<2500) goto done;
    away=D(record+0x20);
    if((s16)W(away+0x80)<2500) goto done;
    source=record;destination=l+0x18;
    for(index=0;index<19;++index){
        first=D(source);D(destination)=first;source+=4;destination+=4;
    }
    year=W(record+0x28);day=W(record+0x2a);
    first=W(source);W(destination)=(u16)first;source+=2;destination+=2;
    first=B(source);B(destination)=(u8)first;
    eax=(u32)cm_fn_00549ef0(l+8,day,year);
    frame.state=0;eax=cm_fn_0054c8e0(eax,l,1);
    year=W(l+2);second=D(record+0x14);first=W(l);
    eax=(eax&0xffff0000u)|first;
    W(l+0x65)=W(l+0x65)&0xfbffu;W(l+0x40)=(u16)year;
    receiver=D(0x00ae2c74);B((u32)&frame.state)=2;
    W(l+0x42)=(u16)eax;B(l+0x53)=0xff;B(l+0x52)=9;B(l+0x5a)=0xff;
    W(l+0x4c)=0;W(l+0x4a)=0;W(l+0x48)=0;
    eax=(u32)cm_fn_005c6710(receiver,second);preferred=0;
    if(eax!=0){
        eax=D(l+2)&0xffffu;eax&=0x80000001u;
        if(eax==0){
            eax=cm_fn_00549f70(l+0x10,20,5,1998,0xffffffffu);
            day=W(l);
            if((s16)day<=(s16)W(eax)){
                eax=D(record+0x1c);pointer=D(eax+0x53);
                if((s32)D(pointer+0x88)>=70){
                    away=D(record+0x20);pointer=D(away+0x53);
                    if((s32)D(pointer+0x88)>=70){
                        eax=cm_fn_00542740(eax);
                        if(eax!=0){
                            away=D(record+0x20);eax=cm_fn_00542740(away);
                            if(eax!=0) preferred=1;
                        }
                    }
                }
            }
        }
    }
    if(preferred!=0){
        receiver=D(0x00ae2a58);eax=cm_fn_005ac7a0(receiver,record);
        if(eax!=0) record=eax;
        eax=D(0x009cf794);flags=D(l+0x65);D(l+0x18)=eax;
        flags=(flags&0xfdffu)|0x100u;B(l+0x58)=0;B(l+0x59)=0;
        W(l+0x65)=(u16)flags;id=eax;base=D(0x00ae23d0);
        D(l+0x2c)=base+id*107u;
        eax=(base&0xffff0000u)|W(record+0x4d);eax=(eax&0xfdffu)|0x100u;
    }else{
        if(W(record+0x32)!=0 && W(record+0x30)!=0) goto done;
        year=W(record+0x28);
        if((year&3u)==0){
            eax=cm_fn_00549f70(l+0x10,1,2,2000,0xffffffffu);
            eax=(u32)(s32)(s16)W(eax);day=W(record+0x2a);
            if((s32)day>(s32)eax){
                eax=cm_fn_00549f70(l+0x10,1,8,2000,0xffffffffu);
                first=(u32)(s32)(s16)W(eax);day=W(record+0x2a);
                if((s32)day<(s32)first) goto done;
            }
        }
        receiver=D(0x00ae2a58);eax=cm_fn_005ac7a0(receiver,record);
        if(eax!=0) record=eax;
        club=D(l+0x34);pointer=D(club+0x53);eax=cm_fn_0053b4f0(pointer);
        away=D(l+0x38);D(l+0x34)=eax;pointer=D(away+0x53);
        eax=cm_fn_0053b4f0(pointer);club=D(l+0x34);D(l+0x38)=eax;
        first=D(club);D(l+0x24)=first;first=D(eax);D(l+0x28)=first;
        eax=D(0x009cf798);flags=D(l+0x65);D(l+0x18)=eax;
        flags=(flags&0xfeffu)|0x200u;W(l+0x65)=(u16)flags;
        id=eax;base=D(0x00ae23d0);D(l+0x2c)=base+id*107u;
        eax=(base&0xffff0000u)|W(record+0x4d);eax=(eax&0xfeffu)|0x200u;
    }
    W(record+0x4d)=(u16)eax;club=D(l+0x34);eax=D(club+0x6e);
    if(eax!=0){
        second=D(l+0x40);first=D(l+0x42);receiver=D(0x00ae2a58);
        eax=cm_fn_005ae870(receiver,eax,first,second,0xffffffffu);
        if(eax==0){club=D(l+0x34);eax=D(club+0x6e);if(eax!=0) goto existing;}
    }
    receiver=D(0x00dd7ef4);eax=cm_fn_0085c260(receiver,0x20,l+0x18,0xffffffffu);
    goto selected;
existing:
    first=D(eax);D(l+0x30)=eax;D(l+0x1c)=first;
selected:
    if(D(l+0x30)==0){W(record+0x4d)=W(record+0x4d)&0xfcffu;goto done;}
    receiver=D(0x00ae2a58);eax=cm_fn_005aa680(receiver,l+0x18,1);
done:
    CM_FS(volatile u32,0)=frame.prev;return eax;
}
