#include "cmre.h"
/* Preferred-image VA0056a100, base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16. Both indexed callers overwrite EAX before any use. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u8 CM_THISCALL cm_fn_00418f60(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_CDECL cm_fn_00547af0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
/* Native417c10 returns output pointer in EAX and its final copied date
   DWORD in EDX. The latter is a live physical input to native68d0d0. */
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern void CM_STDCALL cm_fn_0056a3b0(u32,u32,u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_0056a100(u32 self,volatile u32 item_arg,
                               volatile u32 other_arg,volatile u32 third_arg,
                               volatile u32 flag_arg)
{
    /* L is native ESP after SUB2c. Scratch offsets retain all overlapping
       helper outputs and the 17-byte record immediately before SEH. */
    volatile struct { u8 scratch[44];u32 prev,handler,state; } local;
    u32 L=(u32)&local.scratch[0];
    u32 item,a,c,d,target,other,flag,offset,edx_in;
    unsigned long long copied;
    long double live;
    volatile long long converted;
    local.state=0xffffffffu;
    local.handler=0x009580d0;
    local.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&local.prev;
    D(L)=self;
    (void)cm_fn_0054a110(L+32);
    item=item_arg;
    local.state=0;
    if(!item)goto done;
    a=D(item+0x61);
    if(!a)goto done;
    a=D(item);c=D(0x00ae2364);
    if((s32)a>=(s32)c)goto done;
    c=D(self+4);
    if(B(a+c))goto done;
    c=D(0x00ad9c6c);
    if(cm_fn_00418f60(c,item,0))goto done;
    d=D(item);a=D(self+4);a+=d;
    if(B(a))goto done;
    c=B(0x00ae233c);
    if(!(u8)c)goto done;
    B(a)=1;
    target=D(item+0x61);
    a=cm_fn_0090cfc0(21);
    c=(u32)(s32)(s16)W(target+0x0b);
    item_arg=a;
    live=(long double)(s32)item_arg;
    item_arg=c;
    live*=Q(0x00967898);
    live=(long double)Q(0x00967890)-live;
    live*=(long double)(s32)item_arg;
    converted=(long long)live; /* __ftol CALL0056a1ca, site1. */
    W(target+0x0b)=(u16)converted;
    target=D(item+0x61);
    a=cm_fn_0090cfc0(21);
    d=(u32)(s32)(s16)W(target+9);
    item_arg=a;
    live=(long double)(s32)item_arg;
    item_arg=d;
    live*=Q(0x00967898);
    live=(long double)Q(0x00967890)-live;
    live*=(long double)(s32)item_arg;
    converted=(long long)live; /* __ftol CALL0056a1ff, site2. */
    W(target+9)=(u16)converted;
    target=D(item+0x61);
    a=cm_fn_0090cfc0(21);
    item_arg=a;
    live=(long double)(s32)item_arg;
    a=(u32)(s32)(s16)W(target+0x0d);
    live*=Q(0x00967898);
    item_arg=a;
    live=(long double)Q(0x00967890)-live;
    live*=(long double)(s32)item_arg;
    converted=(long long)live; /* __ftol CALL0056a237, site3. */
    flag=flag_arg;
    W(target+0x0d)=(u16)converted;
    a=D(item+0x39);
    if(a) {
        c=D(a+0xcf);
        cm_fn_00547af0(item,c,1);
        d=D(item+0x39);
        a=cm_fn_00600a50(d,1,1,0,0);
        if(!a) {
            a=D(item+0x39);c=D(a+0xcf);
            cm_fn_00547af0(c,item,1);
        }
        if(!(u8)flag) {
            offset=0;
            do {
                d=D(item+0x39);target=D(d+offset+0xd7);
                if(target && target!=item) {
                    a=(u32)(s32)(s8)B(target+0x59);
                    a=cm_fn_0090cfc0(a);
                    if((s32)a>5) {
                        c=(u32)(s32)(s8)B(target+0x5b);
                        a=cm_fn_0090cfc0(c);
                        if(!a)cm_fn_00547af0(target,item,1);
                    }
                }
                offset+=4;
            }while((s32)offset<200);
            self=D(L);
        }
        d=D(item+0x39);a=D(d+0xbf);
        cm_fn_00547af0(a,item,1);
    }
    other=other_arg;
    c=D(item);D(L+24)=c;
    d=D(other);D(L+28)=d;
    (void)cm_fn_00417c10(L+32,L,0x00ae2c90);
    a=cm_fn_0090cfc0(20);a+=2;
    a=cm_fn_0054c5b0(L+32,L+8,a);
    B((u32)&local.state)=1;
    copied=cm_fn_00417c10(L+32,L+16,a);
    B((u32)&local.state)=0;
    edx_in=(u32)(copied>>32);
    if((u8)flag) {
        a=cm_fn_0090cfc0(10);
        /* Bound10 native RNG ends MOV EAX,EDX after signed IDIV; both
           return registers are equal on every returning path. */
        edx_in=a;
        B(L+40)=0;
        if(a)B(L+40)=(u8)flag;
    } else B(L+40)=(u8)flag;
    c=D(self);
    (void)cm_fn_0068d0d0(c,edx_in,L+24,17);
    if(!other)other=D(item+0x39);
    c=third_arg;
    cm_fn_0056a3b0(item,other,0,c,flag);
done:
    CM_FS(u32,0)=local.prev;
}
