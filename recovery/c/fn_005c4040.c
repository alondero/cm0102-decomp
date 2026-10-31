#include "cmre.h"
/* Preferred-image VA005c4040/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full boolean; SEH959f9e/scratch5c selfL4/deltaWORDL8,
   attemptBYTEL3/datesLc,L1c/copiesL14,L24/temporariesL2c..L54.
   EBP/EDI upper words and delta's unwritten upper word are unobserved:
   comparisons use BP/DI; 54c5b0/54c8e0 use SI, negate/recurse through
   the same WORD contract, then overwrite ESI before any wider use.
   Preserve both append calls' actual EDX, late fields and argument-slot stores. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u16);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u16);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_STDCALL cm_fn_005c6b70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c6f00(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c5570(u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

u32 CM_THISCALL cm_fn_005c4040(u32 self,volatile u32 record_arg,
                              volatile u32 array_arg,volatile u32 mode_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x5c];
    u32 l=(u32)scratch,array,record,other,eax,start,end,delta,root;
    u32 list,cursor,first,second,year,day,valid,club,opponent,index,remaining;
    unsigned long long pair;
    frame.state=0xffffffffu;frame.handler=0x00959f9e;
    frame.prev=CM_FS(volatile u32,0);CM_FS(volatile u32,0)=(u32)&frame;
    array=array_arg;D(l+4)=self;other=D(array);record=record_arg;
    first=W(other+0xe);start=W(record+0xe);end=W(record+0x10);
    if(first!=0xffffu){
        if((s16)first>(s16)(u16)start) start=first;
        second=W(other+0x10);
        if((s16)second<(s16)(u16)end) end=second;
    }
    delta=(u16)(s16)(s8)(u8)((u8)mode_arg*2u+1u);
    end=(u16)(end-delta);W(l+8)=(u16)delta;
    if((s16)(u16)start>=(s16)(u16)end){
        CM_FS(volatile u32,0)=frame.prev;return 0;
    }
    B(l+3)=0;cm_fn_0054a110(l+0xc);frame.state=0;
    cm_fn_0054a110(l+0x1c);B((u32)&frame.state)=1;
    while((s8)B(l+3)<5){
        eax=cm_fn_0090cfc0((u32)((s32)(s16)(u16)end-(s32)(s16)(u16)start));
        root=D(l+4);start+=eax;
        eax=cm_fn_0054c5b0(root+0xa7,l+0x2c,(u16)start);
        cm_fn_00417c10(l+0xc,l+0x34,eax);
        delta=W(l+8);eax=cm_fn_0054c5b0(l+0xc,l+0x3c,(u16)delta);
        cm_fn_00417c10(l+0x1c,l+0x44,eax);
        list=D(record+0x12);
        if(list==0) break;
        first=D(list+2);D(list+0xa)=first;list=D(record+0x12);
        cursor=cm_fn_0068d1b0(list);valid=1;
        while(cursor!=0){
            eax=cm_fn_0054c8e0(l+0xc,l+0x4c,2);
            first=D(eax);second=D(eax+4);D(l+0x14)=first;
            year=W(l+0x16);D(l+0x18)=second;day=W(cursor+2);
            B((u32)&frame.state)=2;
            if(year<day || (year==day && (s16)(u16)first<(s16)W(cursor))){
                valid=0;
            }else{
                eax=cm_fn_0054c5b0(l+0x1c,l+0x54,2);
                first=D(eax);second=D(eax+4);day=W(cursor+2);
                D(l+0x24)=first;D(l+0x28)=second;year=W(l+0x26);
                if(year>day || (year==day && (s16)(u16)first>(s16)W(cursor))) valid=0;
            }
            B((u32)&frame.state)=1;
            if(valid==0) break;
            record=record_arg;list=D(record+0x12);cursor=cm_fn_0068d1b0(list);
        }
        first=B(l+3);record=record_arg;B(l+3)=(u8)(first+1u);
        if(valid!=0) break;
    }
    index=D(record);root=D(0x00ae23b4);club=root+index*581u;
    first=(u8)mode_arg;
    if((s8)first>0){
        array=array_arg;remaining=(u32)(s32)(s8)first;
        do{
            other=D(array);root=D(0x00ae23b4);index=D(other);
            opponent=root+index*581u;
            eax=cm_fn_005c6b70(l+0xc,opponent,0);
            if(eax!=0){
                root=D(l+4);eax=cm_fn_005c6f00(root,club,l+0xc,opponent,0);
                if(eax!=0){
                    cm_fn_005c5570(root,eax);record=record_arg;
                    eax=D(record+0x12);
                    if(eax==0){
                        eax=(u32)cm_fn_00944cf1(0xe);mode_arg=eax;
                        B((u32)&frame.state)=3;
                        if(eax!=0) eax=cm_fn_0068d020(eax);
                        B((u32)&frame.state)=1;D(record+0x12)=eax;
                    }
                    other=D(array);eax=D(other+0x12);
                    if(eax==0){
                        eax=(u32)cm_fn_00944cf1(0xe);mode_arg=eax;
                        B((u32)&frame.state)=4;
                        if(eax!=0) eax=cm_fn_0068d020(eax);
                        else eax=0;
                        other=D(array);B((u32)&frame.state)=1;D(other+0x12)=eax;
                    }
                    list=D(record+0x12);pair=cm_fn_0068d0d0(list,l+0xc,l+0xc,8);
                    other=D(array);list=D(other+0x12);
                    cm_fn_0068d0d0(list,(u32)(pair>>32),l+0xc,8);
                }
            }
            array+=4;eax=cm_fn_0054c5b0(l+0xc,l+0x54,2);
            cm_fn_00417c10(l+0xc,l+0x4c,eax);--remaining;
        }while(remaining!=0);
    }
    CM_FS(volatile u32,0)=frame.prev;
    return 1;
}
