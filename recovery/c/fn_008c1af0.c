#include "cmre.h"
/* Preferred-image VA008c1af0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5/RET20/full incidental EAX; scratch208 dateL0/diagL8,L108.
   Preserve pointer loss on realloc failure, count-before-record stores,
   deferred fee argument to599920, wrapping multiply/divide7 and all reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_004cf6d0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007c5fe0(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0085fa00(u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u32 CM_THISCALL cm_fn_00599920(u32,u32);
extern u32 CM_THISCALL cm_fn_00843e10(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00896c60(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053c190(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cca70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0b50(u32,u32);
extern u32 CM_THISCALL cm_fn_008cb4f0(u32,u32);
extern u32 CM_THISCALL cm_fn_008cb590(u32,u32);
extern u32 CM_THISCALL cm_fn_008cb630(u32,u32);
extern u32 CM_THISCALL cm_fn_008c7e20(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004196a0(u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008c1af0(u32 self,volatile u32 person_arg,
    volatile u32 club_arg,volatile u32 option_arg,volatile u32 fee_arg,
    volatile u32 mode_arg)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,person=person_arg,receiver,eax,capacity,count,base;
    u32 bytes,row,club,value,pointer,mode,option,contract,fee,product,version;
    receiver=D(0x00ae1050);eax=cm_fn_004cf6d0(receiver,person);
    if(eax!=0) return eax;
    capacity=D(self+0x866);count=D(self+0x86a);
    if(capacity==count){
        base=D(self+0x862);capacity+=100u;D(self+0x866)=capacity;bytes=capacity*37u;
        if(base==0) eax=(u32)cm_fn_00944e46(bytes);
        else eax=(u32)cm_fn_00945667(base,bytes);
        D(self+0x862)=eax;
        if(eax==0){
            cm_fn_00944cff(0x00a8d514,0,0,l+0x108,l+8);
            version=cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,0x009a8120,version,l+0x108,l+8,0xce9,0x00a8e998);
            cm_fn_0066f4e0(0x00ae28bc,0x009a8108);
            cm_fn_0066f4e0(0x00ae26c4,0x009a7ff8,0x00ae24d0);
            return cm_fn_005e8290(0x00ae28bc,0x00ae26c4,0);
        }
    }
    count=D(self+0x86a);club=club_arg;base=D(self+0x862);row=base+count*37u;
    D(self+0x86a)=count+1u;D(row+0x10)=0xffffffffu;
    value=0xffffffffu;if(club!=0) value=D(club);D(row+0x14)=value;
    cm_fn_00417c10(row+0x1d,l,0x00ae2c90);
    value=D(person);D(row)=value;
    pointer=D(person+4);value=D(pointer+0x33);D(row+4)=value;
    pointer=D(person+8);value=D(pointer+0x33);D(row+8)=value;
    pointer=D(person+0xc);value=D(pointer+0x33);mode=mode_arg;
    D(row+0xc)=value;B(row+0x1c)=(mode!=0)?5:3;D(row+0x18)=0;
    eax=mode;if(eax!=0) return eax;
    eax=D(person+0x61);
    if(eax!=0){
        option=option_arg;receiver=D(0x00b74350);
        eax=cm_fn_007c5fe0(receiver,person,0,0,0xffffffffu,option,0);
    }
    if(club!=0){
        receiver=D(0x00ae1050);eax=cm_fn_004cf5a0(receiver,person);
        fee=fee_arg;contract=eax;
        if(fee==0xffffffffu){
            if(contract!=0 && (B(contract+0x35)&0x3fu)==2){
                eax=cm_fn_0085fa00(contract);
                value=(u32)(s32)(s16)(u16)eax;product=value*D(contract+0xc);
                fee=(u32)((s32)product/7);
            }else fee=0;
        }
        receiver=D(0x00ae2a38);eax=cm_fn_005a0590(receiver,club);
        cm_fn_00599920(eax,fee);
        receiver=D(self+0x856);cm_fn_00843e10(receiver,person,club,4,2);
        pointer=D(person+0x39);
        if(pointer!=0){
            receiver=D(0x00de1d08);cm_fn_00896c60(receiver,pointer,person,1,1);
            pointer=D(person+0x39);receiver=D(0x00de1d08);
            cm_fn_00896c60(receiver,pointer,person,0,1);
            pointer=D(person+0x39);cm_fn_0053c190(pointer,person,0,1,0);
        }
        cm_fn_00417c10(person+0x3e,l,0x00ae2c90);
    }
    receiver=D(0x00ae1050);eax=cm_fn_004cf5a0(receiver,person);D(0x00ae1080)=eax;
    if(eax!=0){
        value=D(eax+4);base=D(0x00ae23b4);eax=base+value*581u;
        if(eax!=0){
            receiver=D(0x00ae1050);eax=cm_fn_004cf5a0(receiver,person);D(0x00ae1080)=eax;
            if(eax!=0){value=D(eax+4);base=D(0x00ae23b4);eax=base+value*581u;}
            receiver=D(0x00ae1050);cm_fn_004cca70(receiver,person,eax);
        }
    }
    receiver=D(0x00ae1050);cm_fn_004d0b50(receiver,person);
    D(person+0x52)=0;B(person+0x6d)=0;cm_fn_008cb4f0(self,person);
    cm_fn_008cb590(self,person);cm_fn_008cb630(self,person);
    cm_fn_008c7e20(self,person,0);receiver=D(0x00ad9c6c);
    return cm_fn_004196a0(receiver,person);
}
