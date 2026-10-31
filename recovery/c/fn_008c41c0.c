#include "cmre.h"
/* Preferred-image VA008c41c0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full final destructor EAX; SEH963c68/scratch14 flagBYTEL3,
   listL4/cursorLe. Keep sticky removal flag, global DWORDAE1080,
   formal-address updates by8090, repeated request reloads, and original
   list dereferences before their null tests. 8de150 is actual STD4. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_008c2f50(u32,u32);
extern u32 CM_THISCALL cm_fn_008c5e40(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c8090(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_008de150(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);

u32 CM_THISCALL cm_fn_008c41c0(u32 self,volatile u32 player_arg,
                              volatile u32 request_arg,volatile u32 direct_arg,
                              volatile u32 flag_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x14];
    u32 l=(u32)scratch,eax,player,request,record,list,item,base,id,other;
    u32 manager,club,accum,pointer,receiver;
    u8 byte,kind;
    frame.state=0xffffffffu;frame.handler=0x00963c68;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    B(l+3)=0;cm_fn_0068d020(l+4);
    eax=direct_arg;frame.state=0;
    if(eax!=0){
        request=request_arg;id=D(request+0xc);
        manager=D(0x00ae1050);base=D(0x00ae23bc);
        eax=cm_fn_004cf5a0(manager,base+id*110u);D(0x00ae1080)=eax;
        club=0;
        if(eax!=0){
            byte=B(eax+0x35);byte&=0x3f;
            if(byte!=5){id=D(eax+4);base=D(0x00ae23b4);club=base+id*581u;}
        }
        request=request_arg;list=D(request+4);
        if(list==0) goto done;
        other=D(list+2);D(list+0xa)=other;
        request=request_arg;list=D(request+4);item=cm_fn_0068d1b0(list);
        if(item!=0){
            player=player_arg;
            for(;;){
                id=D(player);other=D(item);
                if(id==other) break;
                request=request_arg;list=D(request+4);item=cm_fn_0068d1b0(list);
                if(item==0) goto direct_cleanup;
            }
            request=request_arg;list=D(request+4);cm_fn_0068d1f0(list,item);
            id=D(player);base=D(self+0x86e);pointer=base+id*8u+2;
            byte=B(pointer);B(pointer)=(u8)(byte-1u);
            request=request_arg;accum=D(player+0x52);other=D(request+0x1b);accum+=other;
            other=flag_arg;cm_fn_008de150(request,player,accum,other);
            if(club!=0){
                eax=cm_fn_00600a50(club,0,1,0,0);
                if(eax==0){
                    cm_fn_008c8090(self,(u32)&request_arg,1);
                    request=request_arg;kind=B(request+0x2c);
                    if(kind==2 || kind==3 || kind==0x14 || kind==0x10 || kind==4 ||
                       (kind==0xf && B(request+0x2e)>=3)){
                        list=D(request+4);
                        if(W(list)!=0) goto done;
                        if(list!=0){
                            cm_fn_0068d040(list);cm_fn_00944c94(list);
                            request=request_arg;
                        }
                        D(request+4)=0;goto done;
                    }
                    goto direct_update;
                }
            }
            request=request_arg;
direct_update:
            D(request+0x1b)=accum;
            request=request_arg;cm_fn_008c2f50(self,request);
        }
direct_cleanup:
        request=request_arg;list=D(request+4);
        if(W(list)!=0) goto done;
        if(list!=0){cm_fn_0068d040(list);cm_fn_00944c94(list);}
        request=request_arg;D(request+4)=0;
        goto done;
    }
    player=player_arg;base=D(self+0x86e);id=D(player);
    byte=B(base+id*8u+2);
    if(byte==0) goto done;
    request=request_arg;id=D(request+0xc);base=D(0x00ae23bc);
    cm_fn_008c5e40(self,base+id*110u,l+4);
    other=D(l+6);D(l+0xe)=other;item=cm_fn_0068d1b0(l+4);
    while(item!=0){
        id=D(item);base=D(self);record=D(base+id*4u);
        request=request_arg;
        if(record!=request){
            list=D(record+4);
            if(list!=0){
                other=D(list+2);D(list+0xa)=other;
                list=D(record+4);item=cm_fn_0068d1b0(list);
                while(item!=0){
                    id=D(player);other=D(item);
                    if(id==other){
                        list=D(record+4);B(l+3)=1;cm_fn_0068d1f0(list,item);
                        id=D(player);base=D(self+0x86e);pointer=base+id*8u+2;
                        byte=B(pointer);B(pointer)=(u8)(byte-1u);break;
                    }
                    list=D(record+4);item=cm_fn_0068d1b0(list);
                }
                list=D(record+4);
                if(W(list)==0){
                    if(list!=0){cm_fn_0068d040(list);cm_fn_00944c94(list);}
                    D(record+4)=0;
                }
            }
            byte=B(l+3);
            if(byte!=0){
                accum=D(player+0x52);other=D(record+0x1b);accum+=other;
                cm_fn_008de150(record,player,accum,0);
                D(record+0x1b)=accum;cm_fn_008c2f50(self,record);
            }
        }
        item=cm_fn_0068d1b0(l+4);
    }
done:
    frame.state=0xffffffffu;receiver=l+4;eax=cm_fn_0068d040(receiver);
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
