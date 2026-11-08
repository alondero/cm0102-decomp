/* Preferred-image VA 007572a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original helpers remain boundaries. Scratch offsets are relative to the
   stack pointer after SUB ESP,4f8h, before the four saved registers. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define C(a) CM_GLOBAL(volatile s8,(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00785830(u32,u32);
extern u32 CM_CDECL cm_fn_007e4fa0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00729500(u32,u32,u32,u32,u16,u16);

void CM_STDCALL cm_fn_007572a0(u32 source,u32 full,u32 output)
{
    volatile struct {u32 previous,handler,state;} frame;
    u8 scratch[0x4f8];
    u32 l=(u32)scratch, rec=l+0x10, counter=l+3;
    u32 first,second,third,person,club,mode,suffix,cluboff;
    u32 p,q,r,base,format,src,dst,n,i;
    s32 a,b,x,y,z;
    frame.state=0xffffffffu; frame.handler=0x0095d91b;
    frame.previous=CM_FS(u32,0); CM_FS(u32,0)=(u32)&frame;
    cm_fn_0054a110(rec);
    frame.state=0;
    person=cm_fn_00785830(source,0);
    first=cm_fn_00785830(source,1);
    second=cm_fn_00785830(source,2);
    third=cm_fn_00785830(source,3);
    club=cm_fn_00785830(source,4);
    mode=cm_fn_00785830(source,5);
    C(counter)=6;
    cm_fn_007e4fa0(source,counter,rec);
    i=(u8)C(counter); C(counter)=(u8)(i+1);
    suffix=cm_fn_00785830(source,i);
    cm_fn_0053e140(first,second,third,l+0x1d8,400,full?1:5,person);
    if(!full) {
        x=(s32)((mode+1u)*person)%7;
        cm_fn_0066f4e0(0x00de1f64,x>3?0x00a55640:0x00a55608,100,l+0x1d8);
        cm_fn_004ae660(output,0x00de1f64);
        p=person*110u+D(0x00ae23bc);
        if(cm_fn_0053e990(p,first,second,third,0)) {
            p=person*110u+D(0x00ae23bc);
            cm_fn_004ae8a0(output,l+0x1d8,0x7da,p);
        }
        goto done;
    }
    cm_fn_0053e140(first,second,third,l+0x368,400,5,person);
    a=C(rec+0x19); b=C(rec+0x1a);
    cm_fn_00944c9f(l+0x30,0x0098c114,a>b?a:b,a<b?a:b);
    b=C(rec+0x1a); a=C(rec+0x19);
    cluboff=club*581u;
    if(mode) {
        n=D(rec+0x15);
        base=D(0x00ae23b4);
        p=base+n*581u+4;
        n=D(rec+8); q=D(0x00ae23d0)+n*107u+0x38;
        x=C(p+0x33); y=C(base+cluboff+0x52); z=C(q+0x1a);
        format=a==b?0x00a55d48:(a>b?0x00a55be4:0x00a55a7c);
        cm_fn_0066f4e0(0x00de1f64,format,y,y,z,z,x,x,base+cluboff+0x38,l+0x30,q,p,l+0x1d8,l+0x368);
    } else {
        n=D(rec+8); q=D(0x00ae23d0)+n*107u+0x38;
        if(a==b) {n=D(rec+0x15); base=D(0x00ae23b4);}
        else {base=D(0x00ae23b4); n=D(rec+0x15);}
        p=base+n*581u+4;
        if(a==b) {z=C(q+0x1a); x=C(p+0x33); y=C(base+cluboff+0x52);}
        else {y=C(base+cluboff+0x52); x=C(p+0x33); z=C(q+0x1a);}
        if(a==b)
            cm_fn_0066f4e0(0x00de1f64,0x00a55924,100,y,y,x,x,z,z,base+cluboff+0x38,l+0x1d8,p,l+0x30,q);
        else
            cm_fn_0066f4e0(0x00de1f64,a>b?0x00a557cc:0x00a55674,100,100,x,x,y,y,100,z,z,l+0x1d8,p,base+cluboff+0x38,l+0x30,q);
    }
    if(a==b && suffix>=1 && suffix<=4) {
        base=D(0x00ae23b4); x=C(cluboff+base+0x52);
        format=suffix==1?0x00a21e0c:suffix==2?0x00a21684:suffix==3?0x00a21dc0:0x00a21d74;
        cm_fn_0066f4e0(l+0x48,format,x,x,cluboff+base+0x38);
        src=l+0x48; n=0; do{n++;}while(C(src+n-1));
        dst=0x00de1f64; while(C(dst))dst++;
        for(i=0;i<(n>>2);i++){D(dst)=D(src);dst+=4;src+=4;}
        for(i=0;i<(n&3);i++){C(dst)=C(src);dst++;src++;}
    }
    cm_fn_004ae660(output,0x00de1f64);
    p=cluboff+D(0x00ae23b4); cm_fn_004ae8a0(output,p+4,0x7d5,p);
    p=cluboff+D(0x00ae23b4); cm_fn_004ae8a0(output,p+0x38,0x7d5,p);
    n=D(rec+0x15); p=D(0x00ae23b4)+n*581u; cm_fn_004ae8a0(output,p+4,0x7d5,p);
    n=D(rec+0x15); p=D(0x00ae23b4)+n*581u; cm_fn_004ae8a0(output,p+0x38,0x7d5,p);
    n=D(rec+8); p=D(0x00ae23d0)+n*107u; cm_fn_004ae8a0(output,p+0x38,0x7d0,p);
    n=D(rec+2); i=D(rec); p=D(rec+0x15); q=D(rec+0x11); r=D(rec+0x1b);
    base=D(0x00b6b7c0);
    if(cm_fn_00729500(base,r,q,p,(u16)i,(u16)n))
        cm_fn_004ae8a0(output,l+0x30,0x7d3,D(rec+0x1b));
    r=person*110u;
    if(cm_fn_0053e990(r+D(0x00ae23bc),first,second,third,0)) {
        p=r+D(0x00ae23bc); cm_fn_004ae8a0(output,l+0x1d8,0x7d7,p);
        p=r+D(0x00ae23bc); cm_fn_004ae8a0(output,l+0x368,0x7d7,p);
    }
done:
    CM_FS(u32,0)=frame.previous;
}
