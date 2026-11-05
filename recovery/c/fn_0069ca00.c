#include "cmre.h"
/* Preferred-image VA0069ca00/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8; both indexed callers overwrite EAX before observing it. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern void CM_STDCALL cm_fn_0069c7e0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0069d1b0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_0069ba50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0086df90(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1d0(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785d40(u32,u32,u32,u32);
/* Actual EDX from deletion is forwarded into the following allocation. */
extern unsigned long long CM_THISCALL cm_fn_0068d1f0(u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_0069ca00(u32 self,volatile u32 club_arg,volatile u32 person_arg)
{
    /* L is native ESP after SUB238 and before four register saves.
       Dates L26/L2e overlap the eventual38-byte record L10..35;
       error buffers and later message object share L38..237. */
    volatile struct { u8 scratch[0x238];u32 prev,handler,state; } local;
    u32 l=(u32)local.scratch,a,b,c,d,club,person,old,dest,index,node;
    u32 cursor,record,src,dst,count,code,out4,out5,higher;
    long double live;
    volatile long long converted;
    unsigned long long physical;
    local.state=0xffffffffu;local.handler=0x0095cd07;
    local.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&local.prev;
    (void)cm_fn_0054a110(l+0x26);local.state=0;
    (void)cm_fn_0054a110(l+0x2e);local.state=1;
    person=person_arg;
    a=D(person+0x39);
    if(a) {a=cm_fn_0090cfc0(4);if(a) {
        club=club_arg;a=cm_fn_0053b900(club);
        if(!a) {
            a=D(person+0x39);
            if(a) {
                a=D(person+0x42);c=D(person+0x3e);
                a=cm_fn_0054cdb0(0x00ae2c90,c,a);
                if((s32)a<180) {
                    a=B(person+0x59);
                    if((s8)a>1){a=(u8)(a-1u);B(person+0x59)=(u8)a;}
                    c=(u32)(s32)(s8)B(person+0x59);
                    a=cm_fn_0090cfc0((u32)((s32)c/3));
                    if(!a){c=D(person+0x39);cm_fn_0069c7e0(person,c,club);}
                    goto coefficients;
                }
            }
            a=(u32)(s32)(s8)B(person+0x59)+5u;
            a=cm_fn_0090cfc0(a);
            if(!a){c=D(person+0x39);cm_fn_0069c7e0(person,c,club);}
coefficients:
            old=D(person+0x39);c=W(old+0x80);a=D(old);
            higher=(s16)c>(s16)W(club+0x80);
            c=D(self);dest=c+a*73u+6u;d=(u32)(s32)(s16)W(dest);
            D(l+4)=d;live=(long double)(s32)d;
            if(higher) {
                live*=Q(0x009689d8);converted=(long long)live; /* ADR0006 CALL0069cb31. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+8u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x00969040);
                converted=(long long)live; /* ADR0006 CALL0069cb5e. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+10u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x009689d8);
                converted=(long long)live; /* ADR0006 CALL0069cb8b. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+12u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x009689d8);
            } else {
                live*=Q(0x0096a6d8);converted=(long long)live; /* ADR0006 CALL0069cbcb. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+8u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x009689d0);
                converted=(long long)live; /* ADR0006 CALL0069cbf8. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+10u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x0096a6d8);
                converted=(long long)live; /* ADR0006 CALL0069cc25. */
                W(dest)=(u16)converted;
                a=D(person+0x39);a=D(a);c=D(self);dest=c+a*73u+12u;d=(u32)(s32)(s16)W(dest);
                D(l+4)=d;live=(long double)(s32)d;live*=Q(0x0096a6d8);
            }
            converted=(long long)live; /* ADR0006 CALL0069cc52, both branches. */
            W(dest)=(u16)converted;
            /* Native69cc5a..6a reads unwritten L10/L16 and writes only
               L10/L12..15. Both success predecessors overwrite L10..35
               by a full38-byte copy before any consumer; errors return
               without reading that record. These stack-only dead writes
               are omitted, with no invented input or initialization. */
            c=D(person+0x39);a=D(c);c=D(self);
            (void)cm_fn_00417c10(c+a*73u+0x20u,l+4,0x00ae2c90);
            goto after_coefficients;
        }
        goto after_coefficients;
    }}
    club=club_arg;
after_coefficients:
    d=D(0x00ae2364);a=D(person);d-=16u;
    if((s32)a>=(s32)d) {
        a=D(person+0x69);if((s16)W(a+8)<1500)W(a+8)=1500;
        a=D(person+0x69);if((s16)W(a+10)<1250)W(a+10)=1250;
        a=D(person+0x69);if((s16)W(a+12)<250)W(a+12)=250;
    }
    a=D(person+0x69);
    if(a){a=cm_fn_0069d1b0(self,person,club,0,5);D(l+4)=a;}else D(l+4)=0xfffc2f70u;
    a=D(self+8);c=D(a+2);D(a+10)=c;
    c=D(self+8);index=0;
    if((s16)W(c)>0)for(;;) {
        a=cm_fn_0068d1b0(c);D(l+12)=a;
        if(!a){out4=l+0x138;out5=l+0x38;code=0x307e;goto error;}
        d=D(a+14);c=D(club);if(d==c)goto copy;
        c=D(self+8);++index;b=(u32)(s32)(s16)W(c);if((s32)index>=(s32)b)break;
    }
    cm_fn_0069ba50(self,club,0);
    a=D(self+8);d=D(a+2);D(a+10)=d;
    c=D(self+8);index=0;
    if((s16)W(c)<=0){out4=l+0x38;out5=l+0x138;code=0x30a8;goto error;}
    for(;;) {
        a=cm_fn_0068d1b0(c);D(l+12)=a;
        if(!a){out4=l+0x38;out5=l+0x138;code=0x3099;goto error;}
        d=D(a+14);b=D(club);if(d==b)break;
        c=D(self+8);++index;b=(u32)(s32)(s16)W(c);
        if((s32)index>=(s32)b){out4=l+0x38;out5=l+0x138;code=0x30a8;goto error;}
    }
copy:
    record=a;src=a;dst=l+0x10;
    for(count=0;count<9;++count){d=D(src);D(dst)=d;src+=4;dst+=4;}
    d=W(src);W(dst)=(u16)d;
    d=D(record+10);a=D(person);
    if(d==a) {
        a=club_arg;c=D(0x00dd7f00);B(l+3)=0;
        cursor=cm_fn_0086df90(c,a,0);
        if(cursor) {
            c=D(cursor+6);D(cursor+10)=c;
            a=cm_fn_0068d1d0(cursor);
            while(a) {
                node=a;d=D(node+4);a=D(node);
                a=cm_fn_0054cdb0(0x00ae2c90,a,d);
                if((s32)a<180 && B(node+8)==150) {
                    c=D(node+9);a=D(person);if(c==a){B(l+3)=1;break;}
                }
                a=cm_fn_0068d1d0(cursor);
            }
            cm_fn_0068d040(cursor);(void)cm_fn_00944c94(cursor);
            a=B(l+3);
            if(a) {
                (void)cm_fn_0077bbe0(l+0x38,0x1773,0);B((u32)&local.state)=2;
                d=club_arg;a=D(d);(void)cm_fn_00785790(l+0x38,0,a);
                (void)cm_fn_00785790(l+0x38,1,1);
                c=D(0x00b7431c);(void)cm_fn_00785d40(c,l+0x38,person,0);goto done;
            }
        }
        (void)cm_fn_0077bbe0(l+0x38,0x1773,0);B((u32)&local.state)=3;
    } else {(void)cm_fn_0077bbe0(l+0x38,0x1773,0);B((u32)&local.state)=4;}
    d=club_arg;a=D(d);(void)cm_fn_00785790(l+0x38,0,a);
    (void)cm_fn_00785790(l+0x38,1,0);
    c=D(0x00b7431c);(void)cm_fn_00785d40(c,l+0x38,person,0);
    a=D(l+4);b=D(l+0x22);B((u32)&local.state)=1;
    if((s32)a>(s32)b) {
        b=D(person);node=D(l+12);d=D(node+6);
        if((u32)(b==0)!=d) {
            c=D(self+8);D(l+0x16)=b;D(l+0x22)=a;
            physical=cm_fn_0068d1f0(c,node);
            c=D(self+8);
            (void)cm_fn_0068d0d0(c,(u32)(physical>>32),l+0x10,38);
        }
    }
    goto done;
error:
    (void)cm_fn_00944cff(0x009f18a4,0,0,out4,out5);
    a=cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0,0x009870e8,a,out4,out5,code);
    (void)cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
done:
    CM_FS(u32,0)=local.prev;
}
