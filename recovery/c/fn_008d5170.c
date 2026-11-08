/* Preferred-image VA008d5170/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2 RET8; signed WORD reason. Incidental EAX is dead through the direct
   callers' date-helper low-WORD arithmetic or overwritten in the third caller. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,s8);
extern s32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);
void CM_THISCALL cm_fn_008d5170(u32 self,u32 offer,s16 reason)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x164];
    u32 msg=(u32)scratch+12u,out=msg+0xe0u,person,index,base,p,owner,first,club,value,flags;
    s8 recipients;
    frame.state=0xffffffffu;frame.handler=0x96432bu;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    index=D(offer+12u);base=D(0xae23bcu);person=base+index*110u;
    index=D(offer+8u);base=D(0xae23b4u);club=base+index*581u;
    p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
    if(p && (B(p+0x35u)&0x3fu)!=5u) {
        index=D(p+4u);base=D(0xae23b4u);first=base+index*581u;
    } else first=0;
    flags=B(offer+0x2cu)==2u?45u:46u;
    recipients=cm_fn_008d1130(self,offer,flags,out,30);
    if(recipients) {
        p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
        if(p && (B(p+0x35u)&0x3fu)!=5u) {
            index=D(p+4u);base=D(0xae23b4u);owner=base+index*581u;
        } else owner=0;
        p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
        if(p && (B(p+0x35u)&0x3fu)==5u)owner=0;
        if(reason==6 && B(offer+0x2fu))owner=0;
        cm_fn_0077bbe0((void *)msg,0xfa1u,0);
        value=D(person);frame.state=0;cm_fn_00785790((void *)msg,0,value);
        p=D(person+4u);value=D(p+0x33u);cm_fn_00785790((void *)msg,1,value);
        p=D(person+8u);value=D(p+0x33u);cm_fn_00785790((void *)msg,2,value);
        p=D(person+12u);value=D(p+0x33u);cm_fn_00785790((void *)msg,3,value);
        value=owner?D(owner):0xffffffffu;cm_fn_00785790((void *)msg,4,value);
        value=D(offer+8u);cm_fn_00785790((void *)msg,5,value);
        value=D(offer+4u);cm_fn_00785790((void *)msg,6,value);
        value=(u32)(s32)(s8)B(offer+0x2cu);cm_fn_00785790((void *)msg,7,value);
        cm_fn_00785790((void *)msg,8,(u32)(s32)reason);
        value=(u32)(s32)(s8)B(offer+0x1fu);cm_fn_00785790((void *)msg,9,value);
        value=cm_fn_008e7a70(offer,0,0,0,1);cm_fn_00785790((void *)msg,10,value);
        if(owner)owner=D(owner+0x57u);
        value=owner?D(owner):0xffffffffu;cm_fn_00785790((void *)msg,11,value);
        if(recipients>0) {
            s32 left=recipients;
            do {
                value=D(out);cm_fn_00785790((void *)msg,12,value==club || value==first);
                value=D(out);p=D(0xb7431cu);cm_fn_007861e0((void *)p,msg,value);out+=4u;
            } while(--left);
        }
    }
    CM_FS(u32,0)=frame.prev;
}
