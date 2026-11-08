/* Preferred-image VA005CBDF0/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET8; sole direct caller005CBD9E discards EAX.
   L denotes ESP immediately after SUB ESP,238h. Original callees are boundaries. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_005ae6a0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d0d0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_005cc3e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_THISCALL cm_fn_005c6f00(u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_005c5f40(u32,u32,u32);
void CM_THISCALL cm_fn_005cbdf0(u32 self,u32 records,u32 count)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x238];
    u32 L=(u32)scratch, mask,date,ret,list,node,a,b,p,q,club,peer,flag;
    u32 outer,offset,inner,n,score,left,right,coin;
    s32 index,limit;
    u8 next;
    u16 y,z;
    frame.state=0xffffffffu;
    frame.handler=0x0095a3c6;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0054a110(L+0x1c);
    frame.state=0;
    cm_fn_0054a110(L+0x30);
    n=D(0x00ae2350);
    B((u32)&frame.state)=1;
    mask=cm_fn_00945501(n,1);
    D(L+8)=mask;
    if(!mask) {
        cm_fn_00944cff(0x009cd0a0,0,0,L+0x138,L+0x38);
        ret=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,ret,L+0x138,L+0x38,0x1738);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    date=records+3u;
    ret=cm_fn_0054c8e0(date,L+0x24,4);
    cm_fn_00417c10(L+0x1c,L+0x14,ret);
    ret=cm_fn_0054c5b0(date,L+0x24,4);
    cm_fn_00417c10(L+0x30,L+0x14,ret);
compare_dates:
    y=W(L+0x32);z=W(L+0x1e);
    if(y<z)goto pairs;
    if(y==z && (s16)W(L+0x30)<(s16)W(L+0x1c))goto pairs;
    B(L)=0;
day_kind:
    B(L+4)=0;
slot:
    b=D(L+4);a=D(L);
    /* Incoming ECX is loaded even though005AE6A0 replaces it. */
    p=D(0x00ae2a58);(void)p;
    list=cm_fn_005ae6a0(L+0x1c,a,b);
    p=D(list+2);D(list+10)=p;
    node=cm_fn_0068d1b0(list);
    while(node) {
        p=D(node+0x1c);p=D(p+0x53);
        if(p) {
            a=D(p);D(L+0x2c)=a;
            q=D(node+0x20);b=D(q+0x53);
            if(b) {
                b=D(b);D(L+0xc)=b;
                B(a+mask)=1;
                a=D(L+0xc);B(a+mask)=1;
                p=D(self+0x34f6);a=D(L+0x2c);p=D(p+a*4u);
                cm_fn_0068d0d0(p,L+0xc,4);
                p=D(self+0x34f6);a=D(L+0xc);p=D(p+a*4u);
                cm_fn_0068d0d0(p,L+0x2c,4);
            }
        }
        node=cm_fn_0068d1b0(list);
    }
    next=(u8)(B(L+4)+1u);B(L+4)=next;
    if(next<=1)goto slot;
    next=(u8)(B(L)+1u);B(L)=next;
    if(next<=2)goto day_kind;
    ret=cm_fn_0054c5b0(L+0x1c,L+0x24,1);
    /* VA005CBFF9 selects the advancing date, bypassing VA005CBEEA. */
    cm_fn_00417c10(L+0x1c,L+0x14,ret);
    goto compare_dates;
pairs:
    n=D(self+0x34fe);D(L)=n;
    if((s32)n<=0)goto free_mask;
    D(L+0x24)=0;D(L+0x14)=1;
    offset=0;
outer_loop:
    p=D(self+0x34fa);club=D(p+offset);D(L+4)=club;
    p=D(club);
    if(B(p+mask))goto advance_outer;
    limit=(s16)count;index=0;flag=1;
    if(limit<=0)goto restore_mask;
    q=records+0xc;
    do {
        if(!flag)goto first_allowed;
        a=D(q);
        if(a==0xffffffffu)flag=0;
        else {p=D(club+0x71);b=D(p);club=D(L+4);if(b==a)flag=0;}
        index++;q+=0x14;
    }while(index<limit);
    if(flag)goto restore_mask;
first_allowed:
    if(B(records+2)==1) {
        p=D(club+0x71);
        if(p) {a=D(0x009cfa08);b=D(p);if(b==a)goto restore_mask;}
    }
    inner=D(L+0x14);n=D(L);D(L+0x10)=inner;
    if((s32)inner>=(s32)n)goto restore_mask;
inner_loop:
    p=D(self+0x34fa);mask=D(L+8);peer=D(p+inner*4u);
    p=D(peer);
    if(B(p+mask))goto advance_inner;
    score=cm_fn_0090cfc0(500);D(L)=score;
    limit=(s16)count;index=0;flag=1;
    if(limit<=0)goto advance_inner;
    q=records+0xc;
    do {
        if(!flag)goto second_allowed;
        a=D(q);
        if(a==0xffffffffu)flag=0;
        else {p=D(peer+0x71);b=D(p);club=D(L+4);if(b==a)flag=0;}
        limit=(s16)count;index++;q+=0x14;
    }while(index<limit);
    if(flag)goto advance_inner;
second_allowed:
    if(B(records+2)==1) {
        p=D(peer+0x71);
        if(p) {a=D(0x009cfa08);b=D(p);if(b==a)goto advance_inner;}
    }
    a=D(club+0x88);
    if((s32)a<5 && (s32)D(peer+0x88)<5)goto advance_inner;
    a=D(peer);b=D(club);
    if(cm_fn_005cc3e0(self,b,a))goto advance_inner;
    a=D(club+0x71);b=D(peer+0x71);
    if(a==b) {a=cm_fn_0090cfc0(1000);score=D(L)+a;D(L)=score;}
    if((s32)D(L)<=400)goto advance_inner;
    p=D(self+0x34f6);offset=D(L+0x24);list=D(p+offset);
    a=D(list+2);D(list+10)=a;
    p=D(self+0x34f6);list=D(p+offset);
    node=cm_fn_0068d1b0(list);
    while(node) {
        a=D(node);b=D(peer);
        if(a==b)goto advance_inner;
        p=D(self+0x34f6);list=D(p+offset);
        node=cm_fn_0068d1b0(list);
    }
    coin=cm_fn_0090cfc0(2);
    right=cm_fn_0053b3d0(peer);
    date=records+3u;
    left=cm_fn_0053b3d0(club);
    ret=cm_fn_005c6f00(self,left,date,right,coin);
    if(!ret)goto restore_mask;
    cm_fn_005c5f40(self,ret,1);
    a=D(peer);mask=D(L+8);B(a+mask)=1;
    b=D(club);B(mask+b)=1;
    p=D(self+0x34f6);list=D(p+offset);
    cm_fn_0068d0d0(list,peer,4);
    p=D(self+0x34f6);a=D(L+0x10);list=D(p+a*4u);
    cm_fn_0068d0d0(list,club,4);
    goto restore_mask;
advance_inner:
    inner=D(L+0x10);n=D(self+0x34fe);inner++;D(L+0x10)=inner;
    if((s32)inner<(s32)n)goto inner_loop;
restore_mask:
    mask=D(L+8);
advance_outer:
    outer=D(L+0x14);offset=D(L+0x24);n=D(self+0x34fe);
    outer++;D(L+0x14)=outer;offset+=4u;outer--;D(L+0x24)=offset;D(L)=n;
    if((s32)outer<(s32)n)goto outer_loop;
free_mask:
    cm_fn_009452ca(mask);
done:
    p=frame.prev;
    CM_FS(u32,0)=p;
}
