#include "cmre.h"
/* Preferred-image VA006a0e70/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX; SEH95cdcb/scratche8 dateL0/messageL8.
   Early returns retain FS predecessor/date DWORD/boolean as listed. Final
   417c10 returns the local output address (compiler stack layout under
   semantic), forwarded through all three direct caller wrappers.
   Preserve repeated row reads/RNG order/late signed-BYTE formal. 544410
   reads only the low BYTE of its second argument at544414. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00786780(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007862d0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00544410(u32,u8);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);

u32 CM_THISCALL cm_fn_006a0e70(u32 self,volatile u32 input_arg,
                              volatile u8 flag_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xe8];
    u32 l=(u32)scratch,eax,input,person,date_extra,id,pointer,value,base,row;
    u32 chance,manager,limit;
    s32 threshold,score;
    u16 currentyear,year;
    u8 kind,byte;
    frame.state=0xffffffffu;frame.handler=0x0095cdcb;
    eax=CM_FS(volatile u32,0);frame.prev=eax;
    CM_FS(volatile u32,0)=(u32)&frame;
    input=input_arg;
    if(input==0) goto done;
    person=D(input+0xcf);
    if(person==0) goto done;
    eax=D(0x00dd6ec8);date_extra=D(0x00dd6ecc);
    currentyear=W(0x00ae2c92);
    D(l)=eax;D(l+4)=date_extra;
    year=W(l+2);
    if(year>currentyear) goto done;
    if(year==currentyear){
        eax=((s16)(u16)eax>(s16)W(0x00ae2c90));
        if(eax!=0) goto done;
    }
    cm_fn_0077bbe0(l+8,0x1777,0);
#define SET(n,v) cm_fn_00785790(l+8,n,(u32)(v))
    id=D(person);frame.state=0;SET(0,id);
    pointer=D(person+4);value=D(pointer+0x33);SET(1,value);
    pointer=D(person+8);value=D(pointer+0x33);SET(2,value);
    pointer=D(person+0xc);value=D(pointer+0x33);SET(3,value);
    value=D(input);SET(4,value);
    pointer=D(input+0xbf);
    if(pointer!=0){
        pointer=D(pointer+4);value=D(pointer+0x33);SET(5,value);
        pointer=D(input+0xbf);pointer=D(pointer+8);value=D(pointer+0x33);SET(6,value);
        pointer=D(input+0xbf);pointer=D(pointer+0xc);value=D(pointer+0x33);SET(7,value);
        pointer=D(input+0xbf);value=D(pointer);
    }else{
        SET(5,0xffffffffu);SET(6,0xffffffffu);SET(7,0xffffffffu);
        value=0xffffffffu;
    }
    SET(10,value);
    id=D(input);base=D(self);row=base+id*73u;
    threshold=(s32)(s16)W(row+6)-500;
    score=(s32)(s16)W(row+8);
    if(score<threshold){
        eax=cm_fn_0090cfc0(3);
        if(eax==0){kind=2;goto kind_ready;}
    }
    id=D(input);base=D(self);row=base+id*73u;
    threshold=(s32)(s16)W(row+6)-500;
    score=(s32)(s16)W(row+0xa);
    if(score<threshold){
        eax=cm_fn_0090cfc0(3);
        if(eax==0){kind=1;goto kind_ready;}
    }
    id=D(input);base=D(self);row=base+id*73u;
    threshold=(s32)(s16)W(row+6)-500;
    score=(s32)(s16)W(row+0xc);
    if(score<threshold){
        eax=cm_fn_0090cfc0(3);
        if(eax==0){
            eax=cm_fn_0090cfc0(5);
            kind=(eax!=0)?4:3;
            goto kind_ready;
        }
    }
    kind=0;
kind_ready:
    SET(8,(u32)(s32)(s8)kind);
    byte=flag_arg;SET(9,(u32)(s32)(s8)byte);
    chance=cm_fn_0090cfc0(10);
    pointer=D(person+0x69);
    if(pointer!=0){
        byte=B(pointer+0x1b);
        eax=cm_fn_00544410(pointer,byte);
        if((s8)(u8)eax>10) chance=1;
    }
    SET(11,chance);
    limit=D(0x00ae2364);id=D(person);limit-=16u;
    if((s32)id>=(s32)limit){
        manager=D(0x00b7431c);
        cm_fn_00786780(manager,l+8);
    }else if((s16)W(input+0x80)>5000){
        pointer=D(input+0x53);manager=D(0x00b7431c);
        cm_fn_007863f0(manager,l+8,pointer);
    }else{
        pointer=D(input+0x57);
        if(pointer!=0){
            id=D(pointer);value=D(0x009cf764);
            if(id!=value){
                manager=D(0x00b7431c);
                cm_fn_007862d0(manager,l+8,pointer);
            }
        }
    }
    id=D(input);base=D(self);
    eax=cm_fn_00417c10(base+id*73u+0x18,l,0x00ae2c90);
#undef SET
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
