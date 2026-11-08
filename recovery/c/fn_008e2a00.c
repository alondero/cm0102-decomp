/* Preferred-image VA008e2a00/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1 RET4. Sole indexed caller kills EAX in 0090cfc0. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_008c7560(u32,u32,u32,u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,s8);
void CM_THISCALL cm_fn_008e2a00(u32 self,u32 offer)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x164];
    u32 msg=(u32)scratch+12u,out=msg+0xe0u,person,index,base,p,owner,club,node,value,first=0;
    u8 count=0;
    s8 recipients;
    frame.state=0xffffffffu;frame.handler=0x9647abu;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    index=D(offer+12u);p=D(0xae1050u);base=D(0xae23bcu);person=base+index*110u;
    p=cm_fn_004cf5a0(p,person);base=D(0xae23b4u);D(0xae1080u)=p;
    if(p && (B(p+0x35u)&0x3fu)!=5u)owner=base+D(p+4u)*581u;
    else owner=0;
    index=D(offer+8u);club=base+index*581u;
    recipients=cm_fn_008d1130(self,offer,15,out,30);
    if(recipients) {
        cm_fn_0077bbe0((void *)msg,0xfb9u,0);
        value=D(person);frame.state=0;cm_fn_00785790((void *)msg,0,value);
        p=D(person+4u);value=D(p+0x33u);cm_fn_00785790((void *)msg,1,value);
        p=D(person+8u);value=D(p+0x33u);cm_fn_00785790((void *)msg,2,value);
        p=D(person+12u);value=D(p+0x33u);cm_fn_00785790((void *)msg,3,value);
        value=D(offer+8u);cm_fn_00785790((void *)msg,4,value);
        p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
        if(p && (B(p+0x35u)&0x3fu)!=5u) {
            index=D(p+4u);base=D(0xae23b4u);p=base+index*581u;
        } else p=0;
        value=0xffffffffu;
        if(p) {
            p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
            if(p && (B(p+0x35u)&0x3fu)!=5u) {
                index=D(p+4u);base=D(0xae23b4u);p=base+index*581u;
            } else p=0;
            value=D(p);
        }
        cm_fn_00785790((void *)msg,5,value);
        value=cm_fn_008c7560(self,person,club,0);cm_fn_00785790((void *)msg,6,value);
        p=D(offer+4u);
        if(p) {
            value=D(p+2u);D(p+10u)=value;node=cm_fn_0068d1b0(D(offer+4u));
            while(node) {
                index=D(node);base=D(0xae23bcu);person=base+index*110u;
                if(cm_fn_008c7560(self,person,owner,0)) {
                    if(!first)first=person;
                    count=(u8)(count+1u);
                }
                node=cm_fn_0068d1b0(D(offer+4u));
            }
        }
        cm_fn_00785790((void *)msg,7,(u32)(s32)(s8)count);
        if(first) {
            value=D(first);cm_fn_00785790((void *)msg,8,value);
            p=D(first+4u);value=D(p+0x33u);cm_fn_00785790((void *)msg,9,value);
            p=D(first+8u);value=D(p+0x33u);cm_fn_00785790((void *)msg,10,value);
            p=D(first+12u);value=D(p+0x33u);cm_fn_00785790((void *)msg,11,value);
        }
        if(recipients>0) {
            s32 left=recipients;
            do {value=D(out);p=D(0xb7431cu);cm_fn_007861e0((void *)p,msg,value);out+=4u;} while(--left);
        }
    }
    CM_FS(u32,0)=frame.prev;
}
