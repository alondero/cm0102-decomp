#include "cmre.h"
/* Preferred-image VA005aaf00/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full incidental EAX; actual5ce0a1 is E9 tail JMP here, not E8.
   SEH959670/scratch24 selfL0/phaseL4/remainingL8/dayWORDLc/dateLc,
   L14,L1c; cached slots overwrite L14. Preserve partial AX/AL returns,
   all helper EAX values, signed-BYTE limits, cursor/node order and reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054a3f0(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007e6380(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d080(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1d0(u32);
extern u32 CM_THISCALL cm_fn_0068d280(u32,u32);
extern u32 CM_THISCALL cm_fn_00522c70(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);

u32 CM_THISCALL cm_fn_005aaf00(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x24];
    u32 l=(u32)scratch,root=self,eax,code,list,item,club_a,club_b,base;
    u32 receiver,id,slots,node,record,tag,other,index,phase,remaining;
    u16 word,year;
    u8 byte;
    frame.state=0xffffffffu;frame.handler=0x00959670;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l)=root;eax=cm_fn_0054a3f0(0x00ae2c90);code=eax;
    if((u16)code==2 || (u16)code==3){
        cm_fn_0054c5b0(0x00ae2c90,l+0x14,3);
        frame.state=0;receiver=D(0x00b74434);
        eax=cm_fn_007e6380(receiver,l+0x14);
        if((u16)code==3){
            cm_fn_0054c5b0(0x00ae2c90,l+0xc,1);
            receiver=D(0x00b74434);*(volatile u8 *)&frame.state=1;
            eax=cm_fn_007e6380(receiver,l+0xc);
        }
        frame.state=0xffffffffu;
    }
    eax=D(root+0x1128);
    if(eax!=0){
        other=D(eax+2);D(eax+0xa)=other;
        list=D(root+0x1128);eax=cm_fn_0068d1b0(list);item=eax;
        if(item!=0){
            do{
                eax=(u32)cm_fn_00785830(item,0);base=D(0x00ae23b4);club_a=base+eax*581u;
                eax=(u32)cm_fn_00785830(item,1);base=D(0x00ae23b4);club_b=base+eax*581u;
                eax=(u32)cm_fn_00785830(item,0xe);
                if(eax!=0){receiver=D(0x00b7431c);eax=(u32)cm_fn_007861e0(receiver,item,club_a);}
                eax=(u32)cm_fn_00785830(item,0xf);
                if(eax!=0){receiver=D(0x00b7431c);eax=(u32)cm_fn_007861e0(receiver,item,club_b);}
                root=D(l);list=D(root+0x1128);eax=cm_fn_0068d1b0(list);item=eax;
            }while(item!=0);
            root=D(l);
        }
        list=D(root+0x1128);
        if(list!=0){eax=cm_fn_0068d040(list);eax=(u32)cm_fn_00944c94(list);}
        D(root+0x1128)=0;
    }
    if((u16)code==5 || (u16)code==6){
        eax=cm_fn_0054c8e0(0x00ae2c90,l+0x14,1);
        year=W(eax+2);
        if(year==W(0x00ae2c92)){
            eax=cm_fn_0054c8e0(0x00ae2c90,l+0x1c,1);
            word=W(eax);eax=(eax&0xffff0000u)|word;
            D(l+4)=0;W(l+0xc)=(u16)eax;
            byte=B(0x00b64738);eax=(eax&0xffffff00u)|byte;
            D(l+8)=3;
            do{
                index=0;
                if((s8)(u8)eax>0){
                    eax=D(l+0xc);phase=D(l+4);eax&=0xffffu;
                    eax=root+(phase+eax*3u)*4u;D(l+0x14)=eax;
                    do{
                        slots=D(l+0x14);eax=D(slots);
                        list=D(eax+(u32)(s32)(s8)(u8)index*4u);
                        node=D(list+6);D(list+0xa)=node;
                        if(node!=0){
                            for(;;){
                                record=D(node);eax=D(0x009cf684);tag=D(record);
                                if(tag!=eax) break;
                                if(B(record+0x3a)!=1) break;
                                club_a=D(record+0x1c);
                                eax=cm_fn_00600a50(club_a,1,1,0,0);
                                if(eax==0){
                                    club_b=D(record+0x20);
                                    eax=cm_fn_00600a50(club_b,1,1,0,0);
                                    if(eax==0){
                                        eax=cm_fn_0068d1d0(list);
                                        eax=cm_fn_0068d280(list,node);
                                    }else eax=cm_fn_0068d1d0(list);
                                }else eax=cm_fn_0068d1d0(list);
                                node=D(list+0xa);
                                if(node==0) break;
                            }
                            root=D(l);
                        }
                        byte=B(0x00b64738);eax=(eax&0xffffff00u)|byte;
                        index=(u8)(index+1u);
                    }while((s8)(u8)index<(s8)(u8)eax);
                }
                phase=D(l+4);remaining=D(l+8);++phase;--remaining;
                D(l+4)=phase;D(l+8)=remaining;
            }while(remaining!=0);
        }
    }
    if(W(0x00ae2a48)!=0){
        other=D(0x00ae2a4a);D(0x00ae2a52)=other;
        eax=cm_fn_0068d1b0(0x00ae2a48);
        while(eax!=0){
            id=D(eax);
            if((s32)id>=0){
                base=D(0x00adadfc);
                if(base!=0){
                    receiver=D(base+id*4u);
                    if(receiver!=0){
                        byte=B(eax+4);other=D(receiver+0x30);
                        index=(u32)(s32)(s8)byte;
                        if((s32)index<=(s32)other){
                            if(byte!=0xff){base=D(receiver+0xc);receiver=D(base+index*4u);}
                            if(B(receiver+0x43)==1){
                                other=D(eax+0xb);base=D(eax+7);word=W(eax+5);
                                eax=(eax&0xffff0000u)|word;
                                eax=cm_fn_00522c70(receiver,eax,base,other);
                            }
                        }
                    }
                }
            }
            eax=cm_fn_0068d1b0(0x00ae2a48);
        }
        eax=cm_fn_0068d080(0x00ae2a48);
    }
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
