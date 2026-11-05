#include "cmre.h"
/* Preferred-image VA006bcbd0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Preserve the actual computed EDX:EAX pair, including final AL. */
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u8 CM_THISCALL cm_fn_00714c60(u32,u32);
/* Its pushed incoming-EAX high bytes are dead: native005b57c0 reads BYTE. */
extern u8 CM_THISCALL cm_fn_006f4600(u32);
extern u16 CM_THISCALL cm_fn_00717410(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))

__attribute__((target("no-sse3")))
unsigned long long CM_THISCALL cm_fn_006bcbd0(u32 self,volatile u32 person_arg,
                              volatile u32 output_arg,volatile u32 list_arg)
{
    volatile struct {
        u8 unused[3],count;
        u32 second_score,best,second,best_score,index,cursor,scratch,flag,base;
    } local;
    u32 output=output_arg,person=person_arg,bucket,target,score,ret=0,edx,edx_carry;
    s32 first,second,delta,age,weight;
    volatile long double live;
    volatile long long converted;
    unsigned long long pair;
    local.best_score=0xfffffc18u;
    local.second_score=0xfffffc18u;
    W(output)=0;
    local.count=0;
    first=(s8)B(person+0x102u);
    second=(s8)B(person+0x103u);
    edx_carry=(u32)second;
    local.best=0;
    local.second=0;
    bucket=self+((u32)first*12u+(u32)second)*90u+0x215eu;
    local.base=bucket;
    first=cm_fn_00714d30(person)==0;
    local.index=0;
    local.flag=(u32)first;
    if((s8)B(bucket+(u32)first+0x58u)>0) {
        local.cursor=bucket+(u32)first*44u;
        do {
            target=D(local.cursor);
            if((s8)B(target+0x19u)>=0) {
                score=0;
                if(cm_fn_00714c60(person,target)!=0) {
                    second=B(target+0x10au);
                    age=(s16)W(person+0x19cu);
                    local.count=(u8)(local.count+2u);
                    first=B(person+0x107u);
                    *(volatile u8 *)&person_arg=(u8)first;
                    *(volatile u8 *)&output_arg=(u8)second;
                    weight=(s8)cm_fn_006f4600(target);
                    local.scratch=(u32)weight;
                    delta=(s8)*(volatile u8 *)&output_arg-age/20;
                    delta-=(s8)*(volatile u8 *)&person_arg;
                    output_arg=local.scratch+(u32)delta;
                    live=(long double)(s32)output_arg;
                    live=live*(long double)CM_GLOBAL(volatile double,0x0096d2e0u);
                    converted=(long long)live; /* Original FTOL006bccd8. */
                    ret=(u32)converted;
                    W(output)=(u16)(W(output)+(u16)ret);
                    score=ret*2u;
                } else if((s8)B(target+0x2bu)>=0 && cm_fn_00717410(target)!=1u) {
                    first=B(target+0x10au);
                    if(first!=0 || (W(target+0x198u)&0x100u)!=0) {
                        local.count=(u8)(local.count+1u);
                        age=(s16)W(person+0x19cu);
                        delta=(s8)first-age/20;
                        second=(s8)B(target+0x2bu);
                        delta-=(s8)B(person+0x107u);
                        output_arg=(u32)delta;
                        live=(long double)(s32)output_arg;
                        output_arg=(u32)(second+1);
                        live=live*(long double)CM_GLOBAL(volatile double,0x0096d2e0u);
                        live=live/(long double)(s32)output_arg;
                        converted=(long long)live; /* Original FTOL006bcd69. */
                        score=(u32)converted;
                        W(output)=(u16)(W(output)+(u16)score);
                    }
                }
                if(list_arg!=0 && (s16)score>5) {
                    ret=local.best_score;
                    if((s16)score>(s16)ret) {
                        edx=local.best;
                        if(edx!=0) {
                            local.second=edx;
                            local.second_score=ret;
                        }
                        local.best=target;
                        local.best_score=score;
                    } else if((s16)score>(s16)local.second_score) {
                        local.second=target;
                        local.second_score=score;
                    }
                }
            }
            first=(s32)local.flag;
            edx_carry=local.base;
            first=(s8)B((u32)first+edx_carry+0x58u);
            local.index=local.index+1u;
            local.cursor=local.cursor+4u;
        } while((s32)local.index<first);
    }
    /* Without a second list call, native EAX's upper24 bits are zero:
       its last full definition is the nonnegative loop index, mode0/1,
       the zero output-list pointer, or the zero second-best pointer. */
    ret=0;
    edx=edx_carry;
    output=list_arg;
    if(output!=0) {
        if(local.best!=0) {
            pair=cm_fn_0068d0d0(output,edx,local.best,0x1be);
            edx=(u32)(pair>>32);
        }
        target=local.second;
        if(target!=0) {
            pair=cm_fn_0068d0d0(output,edx,target,0x1be);
            ret=(u32)pair;
            edx=(u32)(pair>>32);
        }
    }
    return ((unsigned long long)edx<<32)|(ret&0xffffff00u)|local.count;
}
