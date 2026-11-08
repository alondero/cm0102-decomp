/* Preferred-image VA008d6bf0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Incoming ECX is forwarded to 008d1130. RET8. Numeric callees remain original.
   Direct caller replaces AL/AX before the date helpers; their live arithmetic
   uses SI/DI, so incidental EAX is not a semantic result. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,s8);
extern s32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);
void CM_THISCALL cm_fn_008d6bf0(u32 self,u32 offer,u32 second)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x15c];
    u32 msg=(u32)scratch+4u,out=msg+0xe0u,person,index,base,p,owner,node,value;
    u8 count=0,field=14;
    s8 recipients;
    frame.state=0xffffffffu;frame.handler=0x96436bu;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    index=D(offer+12u);base=D(0xae23bcu);person=base+index*110u;
    recipients=cm_fn_008d1130(self,second,1,out,30);
    cm_fn_0077bbe0((void *)msg,0xfa2u,0);
    value=D(person);frame.state=0;
    cm_fn_00785790((void *)msg,0,value);
    p=D(person+4u);value=D(p+0x33u);cm_fn_00785790((void *)msg,1,value);
    p=D(person+8u);value=D(p+0x33u);cm_fn_00785790((void *)msg,2,value);
    p=D(person+12u);value=D(p+0x33u);cm_fn_00785790((void *)msg,3,value);
    value=D(offer+8u);cm_fn_00785790((void *)msg,4,value);
    p=cm_fn_004cf5a0(D(0xae1050u),person);D(0xae1080u)=p;
    if(p && (B(p+0x35u)&0x3fu)!=5u) {
        index=D(p+4u);base=D(0xae23b4u);owner=base+index*581u;
    } else owner=0;
    value=D(owner);cm_fn_00785790((void *)msg,5,value);
    value=D(second+0x20u);cm_fn_00785790((void *)msg,6,value);
    value=D(second+0x24u);cm_fn_00785790((void *)msg,7,value);
    value=D(offer+0x20u);cm_fn_00785790((void *)msg,8,value);
    value=D(offer+0x24u);cm_fn_00785790((void *)msg,9,value);
    value=D(offer+0x1bu);cm_fn_00785790((void *)msg,10,value);
    value=(u32)(s32)(s8)B(second+0x2cu);cm_fn_00785790((void *)msg,11,value);
    value=cm_fn_008e7a70(offer,0,1,0,1);cm_fn_00785790((void *)msg,12,value);
    p=D(offer+4u);
    if(p) {
        value=D(p+2u);D(p+10u)=value;
        node=cm_fn_0068d1b0(D(offer+4u));
        while(node) {
            index=D(node);base=D(0xae23bcu);person=base+index*110u;
            if((s8)count<3) {
                value=D(person);cm_fn_00785790((void *)msg,field++,value);
                p=D(person+4u);value=D(p+0x33u);cm_fn_00785790((void *)msg,field++,value);
                p=D(person+8u);value=D(p+0x33u);cm_fn_00785790((void *)msg,field++,value);
                p=D(person+12u);value=D(p+0x33u);cm_fn_00785790((void *)msg,field++,value);
            }
            p=D(offer+4u);count=(u8)(count+1u);node=cm_fn_0068d1b0(p);
        }
    }
    cm_fn_00785790((void *)msg,13,(u32)(s32)(s8)count);
    if(recipients>0) {
        s32 left=recipients;
        do {value=D(out);p=D(0xb7431cu);cm_fn_007861e0((void *)p,msg,value);out+=4u;} while(--left);
    }
    CM_FS(u32,0)=frame.prev;
}
