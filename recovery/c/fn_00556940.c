#include "cmre.h"
/* Preferred-image VA00556940/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full incidental EAX; scratch204 remainingL0/diagL4,L104.
   Keep WORD counts/capacity wraps, BYTE flag merges, every late reload,
   failed-realloc pointer loss, unused-slot initialization and qsort EAX.
   7e8510 forwards ECX to original THIS2 7e82e0 before overwriting it. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007e8510(u32,u32,u16);
extern u32 CM_THISCALL cm_fn_00556c70(u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_00556940(u32 self,volatile u32 key_arg,
                              volatile u32 record_arg)
{
    u8 scratch[0x204];
    u32 l=(u32)scratch,record=record_arg,index=0,eax,base,receiver,key,row;
    u32 offset,count,capacity,bytes,pointer,first,second,line,version;
    u16 word;
    u8 kind,current,flags;
    do{
        eax=(u32)(s32)(s16)(u16)index;
        kind=B(record+eax+0xc);eax=(eax&0xffffff00u)|kind;
        if(kind!=0xff){
            key=key_arg;base=D(self);receiver=D(base+(u32)(s32)(s8)kind*4u);
            eax=cm_fn_007e82e0(receiver,key,0);
            if(eax!=0 && (s16)W(eax+6)>-1){
                eax=(u32)(s32)(s16)W(eax+6);base=D(self+4);eax=base+eax*20u;
                current=B(eax);
                if(current!=0xff){
                    kind=B(record);
                    if(kind!=0xff){B(eax)=(u8)(kind+current);goto merged;}
                }
                word=W(record+2);W(eax+2)=(u16)(W(eax+2)+word);
merged:
                flags=B(record+0x13);B(record+0x13)=flags|1u;
                flags=B(eax+0x13);B(eax+0x13)=flags|1u;
                return eax;
            }
            B(record+0x13)=B(record+0x13)&0xfeu;
        }
        ++index;
    }while((s16)(u16)index<4);
    word=W(self+0xa);eax=(eax&0xffff0000u)|word;
    row=D(self+4);index=0;
    if((s16)(u16)eax>0){
        do{
            if(B(row)==0xff && W(row+2)==0xffffu){
                eax=cm_fn_00556c70(row,record);
                if((u16)index!=0xffffu) goto attach;
                break;
            }
            ++index;row+=20u;
        }while((s16)(u16)index<(s16)(u16)eax);
    }
    count=W(self+0xa);capacity=W(self+8);eax=(eax&0xffff0000u)|capacity;
    if((s16)(u16)count<(s16)(u16)eax){
        base=D(self+4);eax=(u32)(s32)(s16)(u16)count;
        W(self+0xa)=(u16)(count+1u);
        eax=cm_fn_00556c70(base+eax*20u,record);
        word=W(self+0xa);index=(index&0xffff0000u)|(u16)(word-1u);
        goto attach;
    }
    if((s16)(u16)eax>=32000){
        first=l+4;second=l+0x104;line=0x401;goto error;
    }
    base=D(self+4);eax+=100u;W(self+8)=(u16)eax;
    eax=(u32)(s32)(s16)(u16)eax;bytes=eax*20u;
    if(base==0) eax=(u32)cm_fn_00944e46(bytes);
    else eax=(u32)cm_fn_00945667(base,bytes);
    base=eax;D(self+4)=eax;
    if(base==0){first=l+0x104;second=l+4;line=0x410;goto error;}
    word=W(self+0xa);index=(index&0xffff0000u)|word;
    offset=(u32)(s32)(s16)(u16)index*20u;
    eax=index+1u;W(self+0xa)=(u16)eax;
    eax=cm_fn_00556c70(base+offset,record);
    word=W(self+0xa);eax=(eax&0xffff0000u)|word;base=D(self+4);
    capacity=W(self+8);row=base+(u32)(s32)(s16)(u16)eax*20u;
    if((s16)(u16)eax<(s16)(u16)capacity){
        do{
            B(row)=0xff;W(row+2)=0xffffu;++eax;row+=20u;
        }while((s16)(u16)eax<(s16)W(self+8));
    }
attach:
    pointer=record+0xc;D(l)=4;
    do{
        kind=B(pointer);eax=(eax&0xffffff00u)|kind;
        if(kind!=0xff){
            key=key_arg;base=D(self);receiver=D(base+(u32)(s32)(s8)kind*4u);
            eax=cm_fn_007e8510(receiver,key,(u16)index);
        }
        eax=D(l);++pointer;--eax;D(l)=eax;
    }while(eax!=0);
    if(B(record+0xf)==0xff) return eax;
    word=W(self+0x10);eax=(eax&0xffff0000u)|word;
    if((u16)eax==W(self+0x12)){
        base=D(self+0xc);eax+=20u;
        bytes=(u32)(s32)(s16)(u16)eax*8u;W(self+0x10)=(u16)eax;
        eax=(u32)cm_fn_00945667(base,bytes);D(self+0xc)=eax;
        if(eax==0){first=l+4;second=l+0x104;line=0x43e;goto error;}
    }
    count=(u32)(s32)(s16)W(self+0x12);base=D(self+0xc);eax=key_arg;
    D(base+count*8u)=eax;
    offset=eax*110u;base=D(0x00ae23bc);receiver=D(base+offset+0x1a);
    eax=D(self+0xc);count=(u32)(s32)(s16)W(self+0x12);key=D(receiver);
    D(eax+count*8u+4)=key;
    W(self+0x12)=(u16)(W(self+0x12)+1u);word=W(self+0x12);
    count=(u32)(s32)(s16)word;eax=D(self+0xc);
    return cm_fn_00945333(eax,eax,count,8,0x0055e5f0);
error:
    cm_fn_00944cff(0x009b8f18,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
    return eax;
}
