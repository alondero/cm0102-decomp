#include "cmre.h"
/* Original preferred-image VAs below refer to CM0102 3.9.68,
 * SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Receivers and stack arguments follow the PUSH/ECX sequences at
 * 00483ab6-00483ae5, 00483b0a-00483b96, 00483c1c-00483d89,
 * 00483dbc-00483fc0 and 0048401c-004841ae. Boundaries are not composed. */
extern u32 CM_CDECL cm_fn_004afa80(u32,u32);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_CDECL cm_fn_004bf930(volatile u8 *,u32,volatile u32 *,s32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_0054b550(void *,u32,u16,u8);
extern u32 CM_THISCALL cm_fn_005c2990(u32,u32);
extern u32 CM_THISCALL cm_fn_005c29b0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32,u32);
extern u32 CM_THISCALL cm_fn_005fe290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_THISCALL cm_fn_00729500(u32,u32,u32,u32,u16,u16);
extern u32 CM_STDCALL cm_fn_007e6f20(u32);
extern u32 CM_THISCALL cm_fn_008040a0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00804100(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define OUT 0x00de1f64u
#define UI 0x00b74450u
#define LONGROW(i,a,b,c,d,e,f,g,h,j,k) cm_fn_008040a0(UI,i,a,b,c,d,e,f,g,h,j,k)
#define ROW(i,a,b,c,d,e,f,g,h) cm_fn_00804100(UI,i,a,b,c,d,e,f,g,h)
u32 CM_CDECL cm_fn_00483a90(u32 p,u32 q,u32 x,u32 y,s32 z)
{
    volatile struct { u32 prev,handler,state; } seh;
    union { u32 align[506]; u8 b[0x7e8]; } scratch;
    u32 other,ret=0,r,t,color,packed,first,second,n;
    u8 score;
    int same,lookup_failure_one;
    seh.state=0xffffffffu;
    seh.handler=0x955f61;
    seh.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&seh;
    second=(seh.prev&0xffff0000u)|W(q+0x28); first=W(q+0x2a);
    cm_fn_00549ef0(scratch.b+8,first,second);
    seh.state=0;
    cm_fn_0054b550(scratch.b+8,OUT,2000,255);
    if(z>0 && D(q+0x14)!=0 && D(D(0xadadfc)+D(q)*4)!=0 &&
       cm_fn_005c2990(D(0xae2c74),D(q+0x14))==0 &&
       cm_fn_007e6f20(D(q+0x14))==0) {
        color=W(0xae3184); t=W(0xaebe04);
        LONGROW(x,0,y,0x30,t,12,1,color,OUT,(u32)z,q);
    } else {
        color=W(0xae3184); t=W(0xaebe04);
        ROW(x,0,y,0x30,t,12,1,color,OUT);
    }
    other=D(q+0x1c);
    same=(p==other);
    if(same) {
        score=B(q+0x47);
        other=D(q+0x20);
        D((u32)scratch.b)=other;
    } else {
        D((u32)scratch.b)=other;
        score=B(q+0x47);
    }
    lookup_failure_one=same;
    if(score!=255) {
        if(same) { first=(u32)(s32)(s8)score; second=(u32)(s32)(s8)B(q+0x48); }
        else { second=(u32)(s32)(s8)score; first=(u32)(s32)(s8)B(q+0x48); }
        cm_fn_0066f4e0(OUT,0x990bb4,first,second);
        goto lookup;
    }
    /* Preferred-image VAs (pinned image identified above): 00483c9e
     * reaches 00483d4d; ordinary scores jump via 00483db7 to 00483c1c.
     * Failure there reaches 00483d8e (EDI=1), unlike 00483d43-00483d48. */
    lookup_failure_one=1;
    score=B(q+0x43);
    if(score==254) {
        cm_fn_0066f4e0(OUT,0x9918e0);
        goto plain_one;
    }
    if(same && score==253) {
        cm_fn_0066f4e0(OUT,0x9918dc);
        goto lookup;
    }
    if(score!=255) {
        if(same) { first=(u32)(s32)(s8)score; second=(u32)(s32)(s8)B(q+0x44); }
        else { second=(u32)(s32)(s8)score; first=(u32)(s32)(s8)B(q+0x44); }
        cm_fn_0066f4e0(OUT,0x990bb4,first,second);
        goto lookup;
    }
    r=cm_fn_005fe290(D(0xb63c98),q,0);
    if(r) cm_fn_0066f4e0(OUT,0x9918d4);
    else {
        n=0;
        do { t=B(0x98f858+n); ++n; } while(t);
        for(t=0;t<(n>>2);++t) D(OUT+t*4)=D(0x98f858+t*4);
        for(t=(n&~3u);t<n;++t) B(OUT+t)=B(0x98f858+t);
    }
    color=W(0xae3184); t=W(0xae3192);
    LONGROW(x,5,y,0x30,t,12,1,color,OUT,0x7d2,q);
    ret=0;
    goto rest;
lookup:
    second=W(q+0x28); first=W(q+0x2a);
    r=cm_fn_00729500(D(0xb6b7c0),D(q+8),D(q+12),D(q+16),(u16)first,(u16)second);
    if(r) {
        n=D(q+8); color=W(0xae3184); t=W(0xae3192);
        LONGROW(x,5,y,0x30,t,12,1,color,OUT,0x7d3,n);
        ret=1;
    } else {
        color=W(0xae3184); t=W(0xae3192);
        ROW(x,5,y,0x30,t,12,1,color,OUT);
        if(lookup_failure_one) ret=1;
    }
    goto rest;
plain_one:
    color=W(0xae3184); t=W(0xae3192);
    ROW(x,5,y,0x30,t,12,1,color,OUT);
    ret=1;
rest:
    r=cm_fn_00600a50(D((u32)scratch.b),0,1,0,0);
    other=D((u32)scratch.b);
    cm_fn_00944c9f(OUT,0x987ff0,other+0x38);
    other=D((u32)scratch.b);
    color=W(r?0xaebdec:0xae3184);
    LONGROW(x,1,y,1,0,1,3,color,OUT,0x7d5,other);
    r=cm_fn_0053b900((u32 *)p);
    if(!r) {
        other=D((u32)scratch.b); n=D(other+0x53); t=D(p+0x53);
        if(t!=n) {
            r=cm_fn_0053b3d0(n); color=W(0xaebdec);
            other=D((u32)scratch.b); n=D(other+0x53)+0x53;
            LONGROW(x,2,y,1,0,1,2,color,n,0x7d5,r);
        }
    }
    score=(u8)cm_fn_004afa80(p,q);
    cm_fn_0066f4e0(OUT,score==1?0x98f880:score==2?0x98f868:0x990b98,0xad9c64u);
    color=W(0xaebde8);
    ROW(x,3,y,1,0,12,2,color,OUT);
    cm_fn_004bf930((volatile u8 *)q,3,(volatile u32 *)(scratch.b+24),2000);
    cm_fn_00944c9f(OUT,0x987ff0,(u32)(scratch.b+24));
    r=cm_fn_004b0700(D(q));
    if(r) {
        t=D(q+0x14);
        if(t && D(D(0xadadfc)+D(q)*4) &&
           !cm_fn_005c2990(D(0xae2c74),t)) {
            second=W(q+0x28); first=W(q+0x2a);
            n=(u32)cm_fn_00549ef0(scratch.b,first,second);
            t=D(q+0x14); r=D(0xae2c74);
            B((u32)&seh.state)=1;
            r=cm_fn_005c29b0(r,t,n);
            B((u32)&seh.state)=0;
            if(r) {
                packed=W(q+0x32); t=D(q); n=W(q+0x30);
                packed=(((packed&0x7ff)|(t<<11))<<11)|(n&0x7ff);
                color=(packed&0xffff0000u)|W(0xaebdec);
                goto linked;
            }
        }
        color=W(0xaebdec);
    } else {
        t=D(q+0x14);
        r=(u32)cm_fn_005c6710(D(0xae2c74),t);
        W((u32)scratch.b)=W(r?0xaebdde:0xaebdd4);
        t=D(q+0x14);
        if(t && D(D(0xadadfc)+D(q)*4) && D(t)!=D(0x9cf684) &&
           !cm_fn_005c2990(D(0xae2c74),t)) {
            second=W(q+0x28); first=W(q+0x2a);
            n=(u32)cm_fn_00549ef0(scratch.b+16,first,second);
            t=D(q+0x14); r=D(0xae2c74);
            B((u32)&seh.state)=2;
            r=cm_fn_005c29b0(r,t,n);
            B((u32)&seh.state)=0;
            if(r) {
                packed=W(q+0x32); t=D(q); n=W(q+0x30);
                packed=(((packed&0x7ff)|(t<<11))<<11)|(n&0x7ff);
                color=D((u32)scratch.b);
                goto linked;
            }
        }
        color=D((u32)scratch.b);
    }
    ROW(x,4,y,1,0,1,2,color,OUT);
    goto done;
linked:
    LONGROW(x,4,y,1,0,1,2,color,OUT,0x7e5,packed);
done:
    t=seh.prev;
    CM_FS(u32,0)=t;
    return ret;
}
