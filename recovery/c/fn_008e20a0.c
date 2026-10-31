#include "cmre.h"
/* Preferred-image VA008e20a0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full last helper EAX; SEH96476b/scratch20c cachedfieldsL0,
   L4,L8,Lc,L10/dateL10/nameL18(300B)/date textL144(200B).
   Slot8 is stored atLc while slot9 is pending; slot9 atL10 is later
   overwritten by the date constructor. Keep full variadic argument order,
   signed language BYTEs, late global bases and branch-specific formal reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054ba20(u32,u32,u16,u8);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);

u32 CM_STDCALL cm_fn_008e20a0(volatile u32 message_arg,volatile u32 mode_arg,
                             volatile u32 output_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x20c];
    u32 l=(u32)scratch,mode,message,eax,person,first,second,third;
    u32 club_a,club_b,offset_a,offset_b,base,manager,index,pointer,output;
    u32 lang_a,lang_b,different;
    frame.state=0xffffffffu;frame.handler=0x0096476b;
    frame.prev=CM_FS(volatile u32,0);mode=mode_arg;
    CM_FS(volatile u32,0)=(u32)&frame;
    message=message_arg;
    if(mode!=0){
        person=(u32)cm_fn_00785830(message,0);
        eax=(u32)cm_fn_00785830(message,1);D(l+4)=eax;
        eax=(u32)cm_fn_00785830(message,2);D(l)=eax;
        eax=(u32)cm_fn_00785830(message,3);D(l+8)=eax;
        club_a=(u32)cm_fn_00785830(message,4);
        club_b=(u32)cm_fn_00785830(message,5);
        eax=(u32)cm_fn_00785830(message,8);D(l+0xc)=eax;
        eax=(u32)cm_fn_00785830(message,9);D(l+0x10)=eax;
        offset_b=club_b*581u;index=(u32)(s32)(s16)W(0x00b77c7a);
        manager=D(0x00b74c26+index*768u);base=D(0x00ae23b4);
        pointer=D(base+offset_b+0xcf);different=(pointer==0 || pointer!=manager);
        third=D(l+8);second=D(l);first=D(l+4);
        cm_fn_0053e140(first,second,third,l+0x18,300,1,person);
        second=D(l+0x10);first=D(l+0xc);
        eax=(u32)cm_fn_00549ef0(l+0x10,first,second);
        frame.state=0;cm_fn_0054ba20(eax,l+0x144,200,0xff);frame.state=0xffffffffu;
        if(different!=0){
            base=D(0x00ae23b4);offset_a=club_a*581u;
            lang_b=(u32)(s32)(s8)B(base+offset_b+0x52);
            lang_a=(u32)(s32)(s8)B(base+offset_a+0x52);
            cm_fn_0066f4e0(0x00de1f64,0x00a9c6d8,
                lang_b,lang_b,100,lang_a,lang_a,100,
                base+offset_b+0x38,l+0x18,base+offset_a+0x38,l+0x144);
        }else{
            offset_a=club_a*581u;base=D(0x00ae23b4);
            lang_a=(u32)(s32)(s8)B(base+offset_a+0x52);
            cm_fn_0066f4e0(0x00de1f64,0x00a9c644,
                100,lang_a,lang_a,100,l+0x18,base+offset_a+0x38,l+0x144);
        }
        output=output_arg;cm_fn_004ae660(output,0x00de1f64);
        base=D(0x00ae23b4);pointer=base+offset_a;
        cm_fn_004ae8a0(output,pointer+0x38,0x7d5,pointer);
        base=D(0x00ae23b4);pointer=base+offset_b;
        cm_fn_004ae8a0(output,pointer+0x38,0x7d5,pointer);
        third=D(l+8);second=D(l);first=D(l+4);
        base=D(0x00ae23bc);person*=110u;
        eax=cm_fn_0053e990(base+person,first,second,third,0);
        if(eax!=0){
            base=D(0x00ae23bc);pointer=base+person;
            eax=cm_fn_004ae8a0(output,l+0x18,0x7d7,pointer);
        }
    }else{
        person=(u32)cm_fn_00785830(message,0);
        first=(u32)cm_fn_00785830(message,1);
        second=(u32)cm_fn_00785830(message,2);
        third=(u32)cm_fn_00785830(message,3);
        cm_fn_0053e140(first,second,third,l+0x18,300,5,person);
        cm_fn_0066f4e0(0x00de1f64,0x00a9c604,100,l+0x18);
        output=output_arg;cm_fn_004ae660(output,0x00de1f64);
        base=D(0x00ae23bc);person*=110u;
        eax=cm_fn_0053e990(base+person,first,second,third,0);
        if(eax!=0){
            base=D(0x00ae23bc);pointer=base+person;output=output_arg;
            eax=cm_fn_004ae8a0(output,l+0x18,0x7d7,pointer);
        }
    }
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
