/* Preferred-image VA008e7b50/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5 RET20. Final native __ftol supplies the actual EDX:EAX pair. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u8 CM_THISCALL cm_fn_005986b0(u32);
extern u16 CM_CDECL cm_fn_006e86d0(u32,u32,u32,u8,s16);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);

__attribute__((target("no-sse3")))
long long CM_THISCALL cm_fn_008e7b50(u32 self,u32 skip,u32 skip_list,u32 value_list,u32 adjust,u32 extra)
{
    u32 person,id,contract,club,owner,node,result,total,field,ptr,index;
    s32 age,group,a,b,c;
    u16 rate_first=0,rate_second=0;
    float multiplier=1.0f;
    double discount=1.0;
    long double x,y,z;
    id=D(self+0xcu);
    ptr=D(0xae1050u);
    person=D(0xae23bcu)+id*110u;
    contract=cm_fn_004cf5a0(ptr,person);
    D(0xae1080u)=contract;
    if(contract!=0) {
        group=B(contract+0x35u)&0x3f;
        if(group!=5) {id=D(contract+4u);club=D(0xae23b4u)+id*581u;}
        else club=0;
    } else club=0;
    age=(s8)B(person+0x18u);
    total=D(self+0x1bu);
    group=B(self+0x11u);
    if(group==11) {
        owner=D(person+0x39u);
        if(owner!=0) {
            ptr=D(person+0x61u);b=(s16)W(ptr+0xbu);
            ptr=D(owner+0xcfu);
            rate_first=cm_fn_006e86d0(ptr,person,owner,2,(s16)b);
            owner=D(person+0x39u);
            ptr=D(owner+0xcfu);
            rate_second=cm_fn_006e86d0(ptr,person,owner,1,(s16)((u32)rate_first*50u));
        }
    }
    if(club!=0 && adjust!=0) {
        group=(s8)B(self+0x40u);
        if(group>0) {
            ptr=D(0xae2a38u);ptr=cm_fn_005a0590(ptr,club);
            group=(s8)cm_fn_005986b0(ptr);
            index=(u32)(group+2);
            if(index<=3u) {
                /* The guarded original dispatch performs this DWORD read. */
                field=D(0x8e7edcu+index*4u);(void)field;
                switch(index) {
                case 0:multiplier=0.1f;discount=0.5;break;
                case 1:multiplier=0.3f;discount=0.75;break;
                case 2:
                    group=(s8)B(self+0x40u);
                    if(group<7) multiplier=0.9f;
                    else if(group<13) multiplier=0.7f;
                    else multiplier=0.4f;
                    break;
                case 3:if((s8)B(self+0x40u)>12)multiplier=0.9f;break;
                }
            }
        }
    }
    if(skip==0) {
        ptr=D(person+0x61u);
        if(ptr!=0) {
            field=D(self+0x35u);
            if((s32)field>0) {
                a=(s8)B(self+0x39u);
                x=(long double)a*(long double)Q(0x968ef0u);
                x=(long double)Q(0x967890u)-x;
                x=x*(long double)(s32)field;
                x=x*(long double)discount;
                total+=(u32)(long long)x; /* __ftol CALL008e7d16. */
            }
            field=D(self+0x3au);
            if((s32)field>0) {
                group=(s8)B(person+0x22u);
                if(group==0 && age<28) {
                    ptr=D(person+0x1au);b=(s16)W(ptr+0x8eu);
                    a=(s16)rate_second;a*=65;
                    if(a>b) {
                        c=age-24;
                        x=(long double)c;if(x<0)x=-x;
                        c=(s8)B(self+0x3eu);
                        x=x*(long double)Q(0x96d438u);
                        x=(long double)Q(0x967890u)-x;
                        y=(long double)c*(long double)Q(0x968ef0u);
                        y=(long double)Q(0x967890u)-y;
                        z=(long double)(a-b)*(long double)Q(0x96d250u);
                        z=z+(long double)Q(0x968e80u);
                        y=y*z;x=x*y;
                        z=(long double)Q(0x969040u);
                        if(x>z)x=(long double)Q(0x969040u);
                        y=(long double)(s32)field*x;
                        y=y*(long double)discount;
                        total+=(u32)(long long)y; /* __ftol CALL008e7dcf. */
                    }
                }
            }
            group=(s8)B(self+0x3fu);
            if(group>0 && extra!=0) {
                a=(s16)rate_second;b=(s16)rate_first;
                x=(long double)a*(long double)Q(0x9689b0u);
                if(!((long double)b>=x)) {
                    field=D(person+0x52u);
                    field*=(u32)group;
                    x=(long double)(s32)field;
                    y=(long double)(a-b)*(long double)Q(0x970450u);
                    x=x*y;x=x*(long double)discount;
                    x=x*(long double)Q(0x967898u);
                    total+=(u32)(long long)x; /* __ftol CALL008e7e40. */
                }
            }
        }
    }
    node=D(self+4u);
    if(node!=0 && skip_list==0) {
        field=D(node+2u);D(node+0xau)=field;
        node=D(self+4u);result=cm_fn_0068d1b0(node);
        if(result!=0) {
            do {
                id=D(result);
                if(value_list!=0) {
                    ptr=D(0xae23bcu)+id*110u;
                    owner=D(0xae1050u);
                    total+=cm_fn_004d0c60(owner,ptr,0,0,0xffffffffu,0);
                } else {
                    ptr=D(0xae23bcu);
                    total+=D(ptr+id*110u+0x52u);
                }
                node=D(self+4u);result=cm_fn_0068d1b0(node);
            } while(result!=0);
        }
    }
    x=(long double)(s32)total*(long double)multiplier;
    return (long long)x; /* __ftol CALL008e7ecc, both actual return DWORDs. */
}
