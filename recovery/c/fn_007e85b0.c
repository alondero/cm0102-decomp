#include "cmre.h"
/* Preferred-image VA007e85b0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX boolean; SEH9608b1/scratch220. Current date L0
   is deliberately overwritten by 549ef0; list L8, copied cursor L12,
   second date L18, diagnostics L120/L20. Preserve full partial-register
   first-day/year arguments to 5acbe0, and signed low-WORD final count. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0055cae0(u32,u32,u8,u8);
extern u32 CM_THISCALL cm_fn_00556e90(u32,u32,u8,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_005acbe0(u32,u32,u32,u16,u16,u32,u32,u32,u8,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e85b0(u32 self,volatile u32 club_arg,
                              volatile u32 filter_arg,volatile s16 amount_arg,
                              volatile u32 match_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x220];
    u32 l=(u32)scratch,club,id,receiver,index,pointer,entry,eax,edx,match;
    u32 first,second,version,other,base,count,result;
    u16 code,year,limit;
    u8 a,b,side;
    frame.state=0xffffffffu;frame.handler=0x009608b1;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    club=club_arg;id=D(club);
    b=B(self+5);a=B(self+4);receiver=D(0x00ae28f0);
    index=cm_fn_0055cae0(receiver,id,a,b);
    if(index==0xffffffffu) goto no;
    pointer=D(self+0xb);
    if(pointer==0){
        cm_fn_00944cff(0x00a7a858,0,0,l+0x120,l+0x20);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x120,l+0x20,0x330);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto no;
    }
    b=B(self+5);id=D(club);receiver=D(0x00ae28f0);
    cm_fn_00556e90(receiver,id,b,0);
    pointer=D(self+0xb);code=W(pointer+index*14u+6);
    if(code==0xffffu) goto no;
    receiver=D(0x00ae28f0);base=D(receiver+4);
    eax=filter_arg;entry=base+(u32)(s32)(s16)code*20u;
    if(eax!=0 && (B(entry+0x13)&2u)==0) goto no;
    first=D(0x00ae2c90);second=D(0x00ae2c94);year=W(entry+6);
    D(l)=first;D(l+4)=second;
    limit=W(l+2);
    if(limit<year || (limit==year && (s16)(u16)first<(s16)W(entry+4))){
        match=match_arg;
        if(match==0) goto no;
        cm_fn_0068d020(l+8);
        first=W(match+0x28);second=W(match+0x2a);
        frame.state=0;
        cm_fn_00549ef0(l,second,first);
        first=D(entry+4);limit=(u16)D(l+2);second=D(entry+8);
        D(l+0x18)=first;year=W(l+0x1a);
        *(volatile u8 *)&frame.state=1;
        D(l+0x1c)=second;
        if(year>limit || (year==limit && (s16)(u16)first>(s16)W(l))){
            frame.state=0xffffffffu;cm_fn_0068d040(l+8);goto no;
        }
        eax=club_arg;other=D(match+0x1c);
        if(D(eax+0x39)==other || D(eax+0x24)==other){
            side=B(match+0x40);
        }else{
            other=D(match+0x20);side=B(match+0x41);
        }
        year=W(entry+6);first=(first&0xffff0000u)|W(entry+4);
        eax=(eax&0xffff0000u)|year;
        id=D(other);edx=D(l);receiver=D(0x00ae2a58);
        cm_fn_005acbe0(receiver,first,eax,(u16)edx,limit,id,l+8,
                       0xffffffffu,0xfe,(side!=0));
        eax=D(l+0xa);D(l+0x12)=eax;count=0;
        pointer=(u32)cm_fn_0068d1b0(l+8);
        while(pointer!=0){
            if(B(pointer+0x43)==0xff){
                eax=D(pointer);
                if((s32)eax>=0){
                    edx=D(match);
                    if((s32)edx>=0){
                        base=D(0x00adadfc);pointer=D(base+eax*4u);
                        a=B(pointer+0x50);
                        if(a!=0xff){
                            pointer=D(base+edx*4u);
                            if(a==B(pointer+0x50)) ++count;
                        }
                    }
                }
            }
            pointer=(u32)cm_fn_0068d1b0(l+8);
        }
        a=B(entry);result=((s16)(u16)(s16)(s8)a>=(s16)(u16)count);
        frame.state=0xffffffffu;cm_fn_0068d040(l+8);
        goto done;
    }
    if(W(entry+2)==0xffffu){
        first=(u32)(s32)(s8)B(entry+1);
        second=(u32)(s32)amount_arg;edx=(u32)(s32)(s8)B(entry);
        result=((s32)(first+second)<=(s32)edx);
    }else{
        match=match_arg;
        if(match==0){result=1;goto done;}
        first=W(match+0x28);second=W(match+0x2a);
        eax=(u32)cm_fn_00549ef0(l+0x18,second,first);
        first=D(entry+8);second=D(entry+4);frame.state=2;
        eax=cm_fn_0054cdb0(eax,second,first);
        edx=(u32)(s32)(s16)W(entry+2);
        result=((s32)eax<(s32)edx);
    }
    goto done;
no:
    result=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
