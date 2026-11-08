/* Preferred-image VA00599E00, base00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native callees remain original boundaries. The RNG declaration expresses
   physical EAX, EDX and one native stack word; do not compose with its C body
   without reviewing that distinct boundary. All indexed records wrap at 32 bits.
   Direct callers TEST AL or overwrite EAX; the original writes only AL. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern void CM_STDCALL cm_fn_005a0410(u32 index,u32 arg,s8 flag);
extern unsigned long long __attribute__((regparm(2))) CM_CDECL
    cm_fn_0090cfc0(u32 entry_eax,u32 entry_edx,u32 bound);

static inline u32 record(u32 self)
{
    u32 id=D(self+8u);
    u32 base=D(0x00ae23b4);
    return base+id*0x245u;
}
static inline u32 charge(u32 delta,u32 extra,u32 adjustment)
{
    u32 thousands=(u32)((s32)delta/1000)+1u;
    return (thousands*6000u+delta)*125u
        +(u32)(s32)(s16)adjustment*75u+extra*50u;
}

u8 CM_THISCALL cm_fn_00599e00(u32 self,u32 first,u32 second,u32 arg,u32 flag)
{
    u32 base,id,p,old,current,delta,extra,adjustment=0,cost,high;
    u32 peer,related,low,other,amount,value,bound;
    /* Entry loads global before self+8, unlike subsequent record reloads. */
    base=D(0x00ae23b4);
    id=D(self+8u);
    if(cm_fn_0053b900((u32 *)(base+id*0x245u)))return 0;
    p=D(record(self)+0x69u);
    if(!p)return 0;
    old=D(p+0x40u);
    delta=first-old;
    if((s32)delta<0)delta=0;
    if(first==second){
        current=D(p+0x3cu);
        extra=0;
        if((s32)current>(s32)first){
            amount=current-old;
            if((s32)amount>0)delta+=amount;
        }
    }else{
        current=D(p+0x3cu);
        extra=second-current-delta;
        if((s32)extra<0)extra=0;
    }
    if(!delta&&!extra)return 1;
    if((s32)delta>(s32)old){
        amount=current-old;
        if((s32)amount>0){
            adjustment=delta-old;
            if((s32)adjustment>=(s32)amount)adjustment=amount;
        }
    }
    if((u8)flag){
        if((s32)current>120000)return 0;
        value=D(p+0x44u);
        amount=current+extra+delta;
        if((s32)value<(s32)amount)D(p+0x44u)=amount;
        cost=charge(delta,extra,adjustment);
        base=record(self);
        peer=D(base+0xbfu);
        if(peer){related=D(peer+0x69u);}else related=0;
        high=(u32)((s32)cost>>31);
        if(related){
            other=D(self+4u);
            if((s32)high>(s32)other || (high==other && cost>=D(self))){
                bound=(u32)(s32)(s8)B(related+0x20u);
                if((s32)cost>=(s32)(bound*500000u)){
                    if((s8)bound<20)B(related+0x20u)=(u8)(bound+1u);
                    return 0;
                }
            }
            p=D(base+0x69u);
            amount=(u32)(s32)(s16)(extra+delta);
            value=D(p+0x3cu); D(p+0x3cu)=value+amount;
            p=D(record(self)+0x69u);
            delta+=adjustment;
            value=D(p+0x40u); D(p+0x40u)=value+(u32)(s32)(s16)delta;
            peer=D(record(self)+0xbfu);
            related=D(peer+0x69u);
            bound=(u32)(s32)(s8)B(related+0x20u);
            value=(u32)cm_fn_0090cfc0(peer,bound,bound);
            if(!value){
                other=D(self+4u);
                low=D(self);
                if((s32)high>(s32)other || (high==other && cost>low)){
                    peer=D(record(self)+0xbfu);
                    related=D(peer+0x69u);
                    bound=(u32)(s32)(s8)B(related+0x20u);
                    if((s32)cost<(s32)(bound*500000u)){
                        value=D(self+0x154u);
                        amount=D(self+0xb4u);
                        D(self+0x154u)=value+cost;
                        D(self+0xb4u)=amount+cost;
                        value=low+cost;
                        D(self)=value;
                        D(self+4u)=other+high+(value<low);
                    }
                }
            }
            value=D(self+0x8cu);
            amount=D(self+0x12cu);
            low=D(self);
            D(self+0x8cu)=value+cost;
            D(self)=low-cost;
            other=D(self+4u);
            D(self+0x12cu)=amount+cost;
            D(self+4u)=other-high-(low<cost);
        }else{
            other=D(self+4u);
            if(((s32)high>(s32)other || (high==other && cost>=D(self)))
                && (s32)cost>=1000000){
                value=(u32)cm_fn_0090cfc0(cost,high,5);
                if(value)return 0;
            }
            p=D(record(self)+0x69u);
            value=D(p+0x3cu);
            D(p+0x3cu)=value+(u32)(s32)(s16)(extra+delta);
            p=D(record(self)+0x69u);
            delta+=adjustment;
            value=D(p+0x40u);
            D(p+0x40u)=value+(u32)(s32)(s16)delta;
            value=D(self+0x12cu);
            amount=D(self+0x8cu);
            low=D(self);
            D(self+0x12cu)=value+cost;
            D(self+0x8cu)=amount+cost;
            D(self)=low-cost;
            other=D(self+4u);
            D(self+4u)=other-high-(low<cost);
        }
    }else{
        value=D(p+0x44u);
        amount=current+extra+delta;
        if((s32)value<(s32)amount)D(p+0x44u)=amount;
        p=D(record(self)+0x69u);
        value=D(p+0x3cu);
        D(p+0x3cu)=value+(u32)(s32)(s16)(extra+delta);
        p=D(record(self)+0x69u);
        value=D(p+0x40u);
        D(p+0x40u)=value+(u32)(s32)(s16)(delta+adjustment);
        /* Native reads low money before computing the charge. */
        low=D(self);
        cost=charge(delta,extra,adjustment);
        value=D(self+0x12cu);
        amount=D(self+0x8cu);
        D(self+0x8cu)=amount+cost;
        D(self+0x12cu)=value+cost;
        D(self)=low-cost;
        other=D(self+4u);
        D(self+4u)=other-(u32)((s32)cost>>31)-(low<cost);
    }
    /* Native loads this global although 005A0410 does not read entry ECX. */
    value=D(self+8u);
    base=D(0x00ae2a38); (void)base;
    cm_fn_005a0410(value,arg,(s8)((s16)adjustment && !extra ? 0 : 1));
    if(first==second){
        /* A204 arm loads global before index scaling; both retain reads. */
        p=D(record(self)+0x69u);
        value=D(p+0x3cu);
        D(p+0x40u)=value;
    }
    return 1;
}
