/* Preferred-image VA008d3090/base00400000, target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1 RET4. Both indexed callers discard EAX. Original callees stay explicit. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u8 CM_THISCALL cm_fn_004486c0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_CDECL cm_fn_0053b620(u32);
extern u32 CM_CDECL cm_fn_005409b0(u32 *);
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern void CM_STDCALL cm_fn_008d0bb0(void *,void *);
extern s8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,s8);
extern u32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);

void CM_THISCALL cm_fn_008d3090(u32 self,u32 offer)
{
    volatile struct {u32 prev,handler,state;} frame;
    /* Base is original ESP+10 after four saved registers. Counter+1,
       flags+2, recipient count+3, overlapping date/index+4, news+14,
       recipient output+f4. Keep all adjacency; initialize only original bytes. */
    u8 scratch[0x16c];
    u32 work=(u32)scratch,news=work+0x14u,output=work+0xf4u;
    u32 person,mapped,club,ptr,node,value,index,a;
    u16 current_year,mapped_year;
    u8 slot,flag;
    s8 used;
    s32 remaining;
    unsigned long long result;
    frame.state=0xffffffffu;frame.handler=0x9642c6u;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    B(work+1u)=0;
    index=D(offer+12u);ptr=D(0xae1050u);person=D(0xae23bcu)+index*110u;
    mapped=cm_fn_004cf5a0(ptr,person);D(0xae1080u)=mapped;
    if(mapped && (B(mapped+0x35u)&0x3fu)!=5u) {
        index=D(mapped+4u);club=D(0xae23b4u)+index*581u;
    } else club=0;
    B(work+2u)=0;
    used=cm_fn_008d1130(self,offer,0x3eu,output,30);B(work+3u)=(u8)used;
    if(!used)goto done;
    cm_fn_0077bbe0((void *)news,0xfa0u,0);
    value=D(person);frame.state=0;cm_fn_00785790((void *)news,0,value);
    ptr=D(person+4u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,1,value);
    ptr=D(person+8u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,2,value);
    ptr=D(person+12u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,3,value);
    value=D(offer+8u);cm_fn_00785790((void *)news,4,value);
    value=club ? D(club) : 0xffffffffu;cm_fn_00785790((void *)news,5,value);
    value=D(offer+0x20u);cm_fn_00785790((void *)news,6,value);
    value=D(offer+0x24u);cm_fn_00785790((void *)news,7,value);
    value=D(offer+0x1bu);cm_fn_00785790((void *)news,8,value);
    value=(u32)(s32)(s8)B(offer+0x2cu);cm_fn_00785790((void *)news,9,value);
    value=cm_fn_008e7a70(offer,0,1,0,1);cm_fn_00785790((void *)news,10,value);
    value=(u32)(s32)(s8)B(offer+0x2fu);cm_fn_00785790((void *)news,11,value);
    value=D(offer+0x31u)&15u;result=cm_fn_00785790((void *)news,14,value);
    if(B(offer+0x2fu) && club) {
        value=((u32)(result>>32)&0xffffff00u)|B(person+0x3du);
        if(cm_fn_0053b620(value)) {
            mapped=cm_fn_004cf5a0(D(0xae1050u),person);
            if(mapped) {
                value=D(0xae2c90u);a=D(0xae2c94u);D(work+4u)=value;
                current_year=W(work+6u);D(work+8u)=a;mapped_year=W(mapped+0x2fu);
                if(current_year<mapped_year ||
                   (current_year==mapped_year && (s16)value<(s16)W(mapped+0x2du))) {
                    value=(u32)(s32)(s16)W(mapped+0x2du);cm_fn_00785790((void *)news,12,value);
                    value=W(mapped+0x2fu);
                } else {cm_fn_00785790((void *)news,12,0);value=0;}
                cm_fn_00785790((void *)news,13,value);
                value=(u32)((s32)(s8)(B(mapped+0x35u)<<2)>>2);
            } else {cm_fn_00785790((void *)news,12,0);cm_fn_00785790((void *)news,13,0);value=0;}
            cm_fn_00785790((void *)news,15,value);
        }
    }
    ptr=D(offer+4u);slot=24;B(work+4u)=slot;
    if(ptr) {
        value=D(ptr+2u);D(ptr+10u)=value;
        node=cm_fn_0068d1b0(D(offer+4u));
        while(node) {
            index=D(node);a=index*55u;person=D(0xae23bcu)+a*2u;
            if((s8)B(work+1u)<3) {
                value=D(person);index=(a&0xffffff00u)|slot;slot=(u8)(slot+1);
                result=cm_fn_00785790((void *)news,index,value);
                ptr=D(person+4u);index=((u32)result&0xffffff00u)|slot;slot=(u8)(slot+1);
                value=D(ptr+0x33u);result=cm_fn_00785790((void *)news,index,value);
                ptr=D(person+8u);index=((u32)result&0xffffff00u)|slot;slot=(u8)(slot+1);
                value=D(ptr+0x33u);result=cm_fn_00785790((void *)news,index,value);
                ptr=D(person+12u);index=((u32)result&0xffffff00u)|slot;slot=(u8)(slot+1);
                value=D(ptr+0x33u);cm_fn_00785790((void *)news,index,value);
            }
            B(work+1u)=(u8)(B(work+1u)+1u);
            node=cm_fn_0068d1b0(D(offer+4u));
        }
        B(work+4u)=slot;
    }
    value=(u32)(s32)(s8)B(work+1u);cm_fn_00785790((void *)news,23,value);
    ptr=D(person+0x24u);
    if(ptr && cm_fn_005409b0((u32 *)ptr)) {
        ptr=D(person+0x24u);flag=1;value=D(ptr);cm_fn_00785790((void *)news,17,value);
    } else {cm_fn_00785790((void *)news,17,0xffffffffu);flag=B(work+2u);}
    if(cm_fn_004486c0(D(0xad9dacu),offer,1))flag|=2;
    cm_fn_00785790((void *)news,16,(u32)(s32)(s8)flag);
    value=(u32)(s32)(s8)B(offer+0x11u);index=D(work+4u);cm_fn_00785790((void *)news,index,value);
    used=(s8)B(work+3u);
    if(used>0) {
        ptr=output;remaining=used;
        do {value=D(ptr);cm_fn_007861e0((void *)D(0xb7431cu),news,value);ptr+=4u;}while(--remaining);
    }
    if(B(offer+0x2fu)) {
        if((B(offer+0x30u)&15u)==5u) {
            cm_fn_0054a110((u16 *)(work+4u));B((u32)&frame.state)=1;
            cm_fn_008d0bb0((void *)(work+4u),0);
            value=D(0xae2c90u);a=D(0xae2c94u);D(work+12u)=value;D(work+16u)=a;
            value=((u16)(value>>16)==W(work+6u) && (u16)value==W(work+4u)) ? 0 : 5;
            cm_fn_00785790((void *)news,18,value);
            value=(u32)(s32)(s16)W(work+4u);cm_fn_00785790((void *)news,12,value);
            value=D(work+6u)&0xffffu;cm_fn_00785790((void *)news,13,value);
        } else cm_fn_00785790((void *)news,18,3);
    }
done:
    value=frame.prev;CM_FS(u32,0)=value;
}
