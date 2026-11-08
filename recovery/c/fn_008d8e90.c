/* Preferred-image VA008d8e90/base00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS8 RET32. Indexed direct callers discard EAX before use.
   Original callees remain explicit numeric boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define Q(a) CM_GLOBAL(volatile double,(u32)(a))
extern u32 CM_THISCALL cm_fn_004cc9e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
/* Original prologue saves ECX at ESP+1c (after EBX); RET18;
   this edge pushes third stack argument 0, excluding the full-width
   EBP+18 forwarding branch; reachable EBP+14/+18 uses read bytes.
   Every return leaves ST0. */
extern long double CM_THISCALL cm_fn_00846920(u32,u32,u32,u32,s8,s8,u32 *);
extern s8 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,s8);

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_008d8e90(u32 self,u32 person,u32 club,u32 optional,
                               s8 kind,u32 offer,u32 contract,u32 list,u32 extra)
{
    volatile struct {u32 prev,handler,state;} frame;
    /* Original ESP after four saved registers: flags+24, selected+28,
       news+34, output+114. Index -1 at 008d8f98 overwrites flags.
       Keep the whole related range contiguous and otherwise uninitialized. */
    u8 scratch[0x168];
    u32 flags=(u32)scratch,selected=flags+4u,news=flags+0x10u,output=flags+0xf0u;
    u32 mapped,node,index,ptr,value,base,candidate,minimum_bits;
    u32 helper_flags;
    s8 count,used,slot,j,lowest;
    s16 score;
    s32 remaining;
    volatile float rounded;
    long double x;
    frame.state=0xffffffffu;frame.handler=0x96444bu;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    index=D(offer+12u);D(flags)=0x2fu;count=0;
    base=D(0xae23bcu);mapped=base+index*110u;
    if(kind==2) {
        if(list && W(list)) {
            value=D(list+2u);D(list+10u)=value;
            node=cm_fn_0068d1b0(list);
            while(node) {
                if(count<3) {
                    index=D(node);base=D(0xae23b4u);
                    D(selected+(u32)(s32)count*4u)=base+index*581u;
                    count=(s8)(count+1);
                } else {
                    lowest=-1;minimum_bits=0x2711u;
                    for(j=0;j<3;j=(s8)(j+1)) {
                        ptr=D(selected+(u32)(s32)j*4u);
                        /* MOV SI changes the low word; the comparison uses DX. */
                        score=(s16)W(ptr+0x80u);
                        if(score<(s16)minimum_bits) {
                            lowest=j;minimum_bits=(ptr&0xffff0000u)|(u16)score;
                        }
                    }
                    index=D(node);base=D(0xae23b4u);candidate=base+index*581u;
                    score=(s16)W(candidate+0x80u);
                    if(score>(s16)minimum_bits)D(selected+(u32)(s32)lowest*4u)=candidate;
                }
                node=cm_fn_0068d1b0(list);
            }
        }
    } else if(contract)D(flags)=0x2du;
    else if(kind==3) {value=D(club);index=D(offer+8u);D(flags)=0x2du+(value!=index);}
    value=D(flags);used=cm_fn_008d1130(self,offer,value,output,30);
    if(!used)goto done;
    cm_fn_0077bbe0((void *)news,0xfa4u,0);frame.state=0;
    value=D(person);cm_fn_00785790((void *)news,0,value);
    ptr=D(person+4u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,1,value);
    ptr=D(person+8u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,2,value);
    ptr=D(person+12u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,3,value);
    value=optional ? D(optional) : 0xffffffffu;cm_fn_00785790((void *)news,4,value);
    value=D(club);cm_fn_00785790((void *)news,5,value);
    cm_fn_00785790((void *)news,6,(u32)(s32)kind);
    value=D(offer+0x20u);cm_fn_00785790((void *)news,7,value);
    value=D(offer+0x24u);cm_fn_00785790((void *)news,8,value);
    value=(u32)(s32)(s8)B(offer+0x1fu);cm_fn_00785790((void *)news,9,value);
    cm_fn_00785790((void *)news,10,extra);
    value=D(offer+0x1bu);cm_fn_00785790((void *)news,11,value);
    value=D(mapped);cm_fn_00785790((void *)news,12,value);
    ptr=D(mapped+4u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,13,value);
    ptr=D(mapped+8u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,14,value);
    ptr=D(mapped+12u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,15,value);
    if(contract) {
        value=D(contract+4u);cm_fn_00785790((void *)news,16,value);
        value=(u32)((s32)(s8)(B(contract+0x35u)<<2)>>2);
    } else {cm_fn_00785790((void *)news,16,0xffffffffu);value=0;}
    cm_fn_00785790((void *)news,17,value);
    ptr=optional ? D(optional+0xcfu) : 0;
    if(ptr) {
        value=D(ptr);cm_fn_00785790((void *)news,18,value);
        ptr=D(optional+0xcfu);ptr=D(ptr+4u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,19,value);
        ptr=D(optional+0xcfu);ptr=D(ptr+8u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,20,value);
        ptr=D(optional+0xcfu);ptr=D(ptr+12u);value=D(ptr+0x33u);
    } else {
        cm_fn_00785790((void *)news,18,0xffffffffu);
        cm_fn_00785790((void *)news,19,0xffffffffu);
        cm_fn_00785790((void *)news,20,0xffffffffu);value=0xffffffffu;
    }
    cm_fn_00785790((void *)news,21,value);
    ptr=D(club+0xcfu);
    if(ptr) {
        value=D(ptr);cm_fn_00785790((void *)news,22,value);
        ptr=D(club+0xcfu);ptr=D(ptr+4u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,23,value);
        ptr=D(club+0xcfu);ptr=D(ptr+8u);value=D(ptr+0x33u);cm_fn_00785790((void *)news,24,value);
        ptr=D(club+0xcfu);ptr=D(ptr+12u);value=D(ptr+0x33u);
    } else {
        cm_fn_00785790((void *)news,22,0xffffffffu);
        cm_fn_00785790((void *)news,23,0xffffffffu);
        cm_fn_00785790((void *)news,24,0xffffffffu);value=0xffffffffu;
    }
    cm_fn_00785790((void *)news,25,value);
    if(contract) {
        slot=(s8)(B(contract+0x4fu)>>4);j=B(offer+0x1fu)==1;ptr=D(self+0x856u);
        x=cm_fn_00846920(ptr,person,club,0,j,slot,&helper_flags);
        rounded=(float)x;
    } else {
        value=D(club);index=D(person);ptr=D(0xae1050u);
        ptr=cm_fn_004cc9e0(ptr,index,value);
        if(!ptr) {
            cm_fn_00785790((void *)news,26,0);value=0;goto value_ready;
        }
        slot=(s8)(B(ptr+0x4fu)>>4);j=B(offer+0x1fu)==1;ptr=D(self+0x856u);
        x=cm_fn_00846920(ptr,person,club,0,j,slot,&helper_flags);
        rounded=(float)x;
    }
    value=helper_flags;cm_fn_00785790((void *)news,26,value);
    x=(long double)rounded;x=x*(long double)Q(0x968908u);
    value=(u32)(long long)x; /* original __ftol CALL008d9365, EAX narrowed. */
value_ready:
    cm_fn_00785790((void *)news,27,value);
    value=list ? (u32)(s32)(s16)W(list) : 0;cm_fn_00785790((void *)news,28,value);
    for(slot=0;slot<count;slot=(s8)(slot+1)) {
        ptr=D(selected+(u32)(s32)slot*4u);value=D(ptr);
        cm_fn_00785790((void *)news,(u32)(u8)(slot+29),value);
    }
    if(used>0) {
        ptr=output;remaining=used;
        do {
            value=D(ptr);cm_fn_007861e0((void *)D(0xb7431cu),news,value);
            if(kind==2 && count>0) {
                value=D(ptr);
                for(slot=0;slot<count;slot=(s8)(slot+1)) {
                    if(value==D(selected+(u32)(s32)slot*4u)) {
                        D(selected+(u32)(s32)slot*4u)=0;break;
                    }
                }
            }
            ptr+=4u;
        } while(--remaining);
    }
    if(count>0) {
        ptr=selected;remaining=count;
        do {
            value=D(ptr);
            if(value)cm_fn_007861e0((void *)D(0xb7431cu),news,value);
            ptr+=4u;
        } while(--remaining);
    }
done:
    value=frame.prev;CM_FS(u32,0)=value;
}
