/* CM0102 3.9.68; preferred-image VA008ce0e0, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS7/RET28. Numeric native callees remain explicit boundaries; original
   register-prefix reconstructed interfaces are not composed here. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern void CM_THISCALL cm_fn_008674e0(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern void CM_CDECL cm_fn_005882d0(u32,u32);
extern void CM_THISCALL cm_fn_00867700(u32,u32,u32);
extern u8 CM_STDCALL cm_fn_004ca470(u32,u32);
extern void CM_THISCALL cm_fn_00842b00(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00842c00(u32,u32,u32);

static void adjust_status(u32 address,s32 delta)
{
    u8 old=B(address);
    s32 value=(s32)(s8)old+delta;
    if(value>100)B(address)=100;
    else if(value< -100)B(address)=156;
    else B(address)=(u8)((u32)old+(u32)delta);
}

u32 CM_THISCALL cm_fn_008ce0e0(u32 self,u32 person,u32 club,u32 amount,
                              u32 output,u32 mode_word,u32 mask_word,u32 clear)
{
    /* L follows the original SUB208; related strings and two locals share
       the original allocation. No original unwritten scratch is initialized. */
    u8 scratch[0x208];
    u32 l=(u32)scratch,record,receiver,a,b,target,index,result,flag=0;
    u8 mode=(u8)mode_word,mask=(u8)mask_word,old,high,threshold;
    s32 delta,days,compared;
    D(l+4u)=self;
    D(l)=0;
    if(club) {
        receiver=D(0x00ae2a38u);
        a=cm_fn_005a0590(receiver,club);
        if(B(a+0x165u)==1) {
            flag=1;
            D(l)=1;
        }
    }
    receiver=D(0x00ae1050u);
    record=cm_fn_004cf5f0(receiver,person,club);
    if(!record) {
        cm_fn_00944cff(0x00a8d514u,0,0,scratch+0x108,scratch+8);
        result=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,result,scratch+0x108,scratch+8,0x2f0cu);
        result=cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
        D(0x00b67a34u)=0;
        return result;
    }
    if((B(record+0x35u)&63u)==2) {
        a=D(person+0x61u);
        if(a) {
            if(flag) {
                a=(u32)(s32)(s16)W(0x00b77c7au);
                receiver=D(0x00ae1050u);
                a=D(0x00b74c26u+a*768u);
                a=D(a+0x39u);
                result=cm_fn_004d0c60(receiver,person,a,0,0xffffffffu,1);
                if((s32)amount>(s32)result)goto after_amount;
            }
            D(person+0x52u)=amount;
        }
    }
after_amount:
    if(mode==7) {
        if((B(record+0x4fu)&0xf0u)!=0x70)adjust_status(record+0x3du,-100);
    } else if(mode==255)goto final_helpers;
    old=B(record+0x4fu);
    high=(u8)(old&0xf0u);
    if(high==0) {
        index=(u32)(s32)(s8)mode-1u;
        if(index<=6) {
            target=D(0x008ce8f0u+index*4u);
            switch(target) {
            case 0x008ce254u:delta=(mask&3u)?-100:100;break;
            case 0x008ce2dau:delta=(mask&3u)?-100:50;break;
            case 0x008ce428u:delta=(mask&3u)?-100:25;break;
            case 0x008ce360u:if(!(mask&3u))goto after_adjustment;delta=-100;break;
            case 0x008ce3c4u:if(!(mask&1u))goto after_adjustment;delta=-50;break;
            case 0x008ce480u:delta=-20;break;
            case 0x008ce4a9u:delta=-100;break;
            default:__builtin_unreachable();
            }
            adjust_status(record+0x44u,delta);
            if(mode<=5 && (mask&(mode==5?1u:3u)))D(output)=1;
        }
    } else {
        old=(u8)(old>>4);
        if((s8)old<(s8)mode) {
            delta=(s16)(((s32)(s8)old-(s32)(s8)mode)*25);
            if(high==0x30 && mode==4)delta=-50;
            else if(delta< -125)delta=-125;
            adjust_status(record+0x44u,(s8)delta);
        }
    }
after_adjustment:
    b=D(0x00dd6eccu);
    a=D(0x00dd6ec8u);
    days=cm_fn_0054cdb0(0x00ae2c90u,a,b);
    if(days<7)goto update_mode;
    if(mode==1) {
        a=D(record+0x45u);
        if(!(a&0x4000u))goto update_mode;
        cm_fn_008674e0(record,0x4000u);
        goto random_pair;
    }
    old=B(record+0x4fu);
    if(!(old&0xf0u))goto update_mode;
    old=(u8)(old>>4);
    if((s8)old>=(s8)mode)goto update_mode;
    threshold=32;
    a=cm_fn_0090cfc0(3);
    compared=(s32)(a+15u);
    if((s32)(s8)B(person+0x59u)>compared)threshold=31;
    else {
        a=cm_fn_0090cfc0(3);
        compared=(s32)(a+5u);
        if((s32)(s8)B(person+0x59u)<compared)threshold=33;
    }
    a=cm_fn_0090cfc0(3);
    compared=(s32)(a+15u);
    if((s32)(s8)B(person+0x5bu)>compared)--threshold;
    else {
        a=cm_fn_0090cfc0(3);
        compared=(s32)(a+5u);
        if((s32)(s8)B(person+0x5bu)<compared)++threshold;
    }
    a=D(person+0x61u);
    if(a) {
        if((s8)B(a+15u)>15)++threshold;
        if(a && (s8)B(a+21u)>15)--threshold;
    }
    receiver=D(0x00b63d28u);
    a=cm_fn_0062ed70(receiver,person);
    if(a) {
        b=W(a+0x2cu);
        if((s16)b>180)threshold=(u8)(threshold-8u);
        else if((s16)b>90 && (s8)B(person+0x5bu)>5)threshold=(u8)(threshold-5u);
        if((s16)b>30)threshold=(u8)(threshold-2u);
    }
    old=(u8)(B(record+0x4fu)&0xf0u);
    if(old==0x10 && (s8)B(person+0x18u)<(s8)threshold) {
        if(mode==2)goto random_five;
        a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x5bu));
        if(!a)goto notify;
        a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x59u));
        if(!a)goto notify;
        a=cm_fn_0090cfc0(5);
        if(a)goto random_five;
notify:
        cm_fn_005882d0(person,club);
        cm_fn_00867700(record,0x4000u,0);
        adjust_status(record+0x44u,-5);
        goto update_mode;
random_five:
        a=cm_fn_0090cfc0(5);
        if(a)goto update_mode;
        a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x57u));
        if(a)goto clear_200;
        goto update_mode;
    }
    if(mode==3 && old==0x20 && (s16)W(club+0x80u)>=5750)goto update_mode;
    b=D(record+0x29u);
    a=D(record+0x25u);
    days=cm_fn_0054cdb0(0x00ae2c90u,a,b);
    compared=((s32)(s8)B(person+0x57u)+6)*5;
    if(days<compared)goto clear_200;
    receiver=D(0x00ae1050u);
    (void)receiver; /* Original ECX is dead to this native stdcall callee. */
    index=(u32)(s32)(s8)cm_fn_004ca470(person,record)-1u;
    if(index>4)goto update_mode;
    target=D(0x008ce90cu+index*4u);
    switch(target) {
    case 0x008ce79au:
        a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x5bu));
        if(!a)goto clear_200_adjust;
        a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x59u));
        if(!a)goto clear_200_adjust;
        a=cm_fn_0090cfc0(3);
        if(a)goto update_mode;
clear_200_adjust:
        cm_fn_00867700(record,0x200u,0);
        adjust_status(record+0x44u,-10);
        goto update_mode;
    case 0x008ce80bu:
        a=cm_fn_0090cfc0(10);
        if(!a)goto update_mode;
        goto random_pair;
    case 0x008ce81eu:
        if((s8)B(person+0x18u)>=(s8)threshold)goto update_mode;
        a=cm_fn_0090cfc0(5);
        if(!a || (s8)B(person+0x57u)<=10)goto update_mode;
        goto random_pair;
    case 0x008ce852u:
        if((s8)B(person+0x57u)<=15)goto update_mode;
        a=cm_fn_0090cfc0(3);
        if(!a)goto update_mode;
        goto random_pair;
    default:__builtin_unreachable();
    }
random_pair:
    a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x5bu));
    if(!a)goto clear_200;
    a=cm_fn_0090cfc0((u32)(s32)(s8)B(person+0x59u));
    if(a)goto update_mode;
clear_200:
    cm_fn_00867700(record,0x200u,0);
update_mode:
    old=B(record+0x4fu);
    flag=D(l);
    B(record+0x4fu)=(u8)((mode<<4)|(old&15u));
final_helpers:
    if(clear && !flag)B(record+0x4eu)=(u8)(B(record+0x4eu)&0xfbu);
    self=D(l+4u);
    receiver=D(self+0x856u);
    cm_fn_00842b00(receiver,person,0);
    receiver=D(self+0x856u);
    return cm_fn_00842c00(receiver,person,0);
}
