#include "cmre.h"

/* Preferred-image VA006A6310/base00400000; pinned 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full EAX retained. L=afterSUB198; before saved EBP,
   ESP=L-C, afterwards L-10. CounterL3, date/packed recordL4C,
   textL6C, person-name bufferLD0. Original785830 reads only BYTE index;
   the first eighteen indices are 0..17. Native7E4FA0 writes packed0..1E.
   All original callees remain boundaries, including the legacy ABI records. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00785830(u32,u8);
extern u32 CM_CDECL cm_fn_007e4fa0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_THISCALL cm_fn_006a69c0(u32,u32,u32,u32,u32,u32,u32,u32,
                                     u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00729500(u32,u32,u32,u32,u16,u16);
extern u32 CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);

static u32 field(u32 source,u32 counter_pointer)
{
    u8 counter=B(counter_pointer);
    B(counter_pointer)=(u8)(counter+1u);
    return cm_fn_00785830(source,counter);
}

u32 CM_THISCALL cm_fn_006a6310(u32 self,u32 source,u32 mode,u32 destination)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x198];
    u32 l=(u32)scratch,a,first,second,third,first_offset,second_offset;
    u32 c,d,format,pointer,base,index,result,max,min;
    u32 p[15];
    static const u8 getter_offsets[18]={0x2c,0xff,0xff,0xff,0x38,0x24,
        0x1c,0xff,0x30,0x3c,0x18,0x14,0x48,0x10,0x40,0x28,0x44,0x20};

    frame.state=0xffffffffu;
    frame.handler=0x0095cf2bu;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l+0x34u)=self;
    B(l+3u)=0;
    cm_fn_0054a110(l+0x4cu);
    frame.state=0;
    if (mode==0) {
        first=cm_fn_00785830(source,0);
        second=cm_fn_00785830(source,1);
        third=cm_fn_00785830(source,2);
        if ((first&0x40u)!=0) {
            if ((first&8u)!=0) format=0x00a0bd0cu;
            else if ((first&0x20u)!=0) format=0x00a0bcecu;
            else if ((first&1u)!=0) format=0x00a0bcccu;
            else if ((first&2u)!=0) format=0x00a0bca8u;
            else format=0x00a0bc88u;
            cm_fn_0066f4e0(0x00de1f64u,format);
        } else {
            if ((first&8u)!=0) {
                a=D(0x00ae23b4u);
                a+=third*581u;
                c=(u32)(s32)(s8)B(a+0x52u);
                pointer=a+0x38u;
                format=0x00a0bc48u;
            } else if ((first&0x20u)!=0) {
                d=D(0x00ae23b4u);
                a=d+third*581u;
                c=(u32)(s32)(s8)B(a+0x52u);
                pointer=a+0x38u;
                format=0x00a0bc0cu;
            } else if ((first&1u)!=0) {
                c=D(0x00ae23b4u);
                a=c+second*581u;
                c=(u32)(s32)(s8)B(a+0x52u);
                pointer=a+0x38u;
                format=0x00a0bbccu;
            } else if ((first&2u)!=0 && (first&0x1000u)!=0) {
                a=D(0x00ae23b4u);
                a+=third*581u;
                c=(u32)(s32)(s8)B(a+0x52u);
                pointer=a+0x38u;
                format=0x00a0bb8cu;
            } else {
                d=D(0x00ae23b4u);
                a=d+third*581u;
                c=(u32)(s32)(s8)B(a+0x52u);
                pointer=a+0x38u;
                format=0x00a0bb50u;
            }
            cm_fn_0066f4e0(0x00de1f64u,format,c,c,pointer);
        }
        cm_fn_004ae660(destination,0x00de1f64u);
        c=D(0x00ae23b4u);
        a=c+second*581u;
        cm_fn_004ae8a0(destination,a+0x38u,0x7d5,a);
        a=D(0x00ae23b4u)+third*581u;
        a=cm_fn_004ae8a0(destination,a+0x38u,0x7d5,a);
        goto done;
    }

    a=field(source,l+3u);
    D(l+0x2cu)=a;
    first=field(source,l+3u);
    second=field(source,l+3u);
    third=field(source,l+3u);
    for(index=4;index<18;++index) {
        a=field(source,l+3u);
        if(getter_offsets[index]!=0xffu) D(l+getter_offsets[index])=a;
    }
    cm_fn_007e4fa0(source,l+3u,l+0x4cu);
    a=D(l+0x3cu);
    c=D(l+0x18u);
    d=D(l+0x30u);
    D(l+8u)=a;
    a=D(l+0x38u);
    D(l+0xcu)=c;
    D(l+4u)=d;
    min=(s32)third<(s32)a ? third : a;
    max=(s32)third>(s32)a ? third : a;
    cm_fn_0066f4e0(l+0x6cu,0x00a0bd30u,max,min);
    d=D(l+0x20u);
    second_offset=second*581u;
    first_offset=first*581u;
    p[14]=d;
    d=D(l+0x40u);
    a=D(l+0x44u);
    c=D(l+0x28u);
    p[13]=a;
    a=D(l+0x10u);
    p[12]=c;
    c=D(l+0x48u);
    p[11]=d;
    d=D(l+0x14u);
    p[10]=a;
    p[9]=c;
    c=D(l+0x1cu);
    p[8]=d;
    d=D(l+0x24u);
    p[7]=l+4u;
    p[6]=c;
    c=D(l+0x2cu);
    p[5]=d;
    p[4]=l+0x6cu;
    a=D(l+0x54u);
    p[3]=c;
    c=D(0x00ae23d0u);
    d=a*107u;
    a=D(0x00ae23b4u);
    p[2]=d+c;
    p[1]=second_offset+a;
    p[0]=first_offset+a;
    c=D(l+0x34u);
    cm_fn_006a69c0(c,p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],
                    p[8],p[9],p[10],p[11],p[12],p[13],p[14]);
    a=D(l+0x4eu);
    c=D(l+0x4cu);
    d=D(l+0x61u);
    p[4]=a;
    a=D(l+0x5du);
    p[3]=c;
    c=D(l+0x67u);
    base=D(0x00b6b7c0u);
    result=cm_fn_00729500(base,c,a,d,(u16)p[3],(u16)p[4]);
    cm_fn_004ae660(destination,0x00de1f64u);
    d=D(0x00ae23b4u);
    a=first_offset+d;
    cm_fn_004ae8a0(destination,a+4u,0x7d5,a);
    a=D(0x00ae23b4u)+second_offset;
    cm_fn_004ae8a0(destination,a+4u,0x7d5,a);
    a=D(l+0x54u);
    d=D(0x00ae23d0u);
    a=a*107u+d;
    cm_fn_004ae8a0(destination,a+0x38u,0x7d0,a);
    a=D(l+4u);
    if(a!=0xffffffffu) {
        c=D(0x00ae23bcu);
        cm_fn_0053dba0(c+a*110u,l+0xd0u,0xc8,1);
        a=D(l+4u);
        base=D(0x00ae23bcu);
        a=cm_fn_004ae8a0(destination,l+0xd0u,0x7db,base+a*110u);
    }
    a=D(l+8u);
    if(a!=0xffffffffu) {
        c=D(0x00ae23bcu);
        cm_fn_0053dba0(c+a*110u,l+0xd0u,0xc8,1);
        a=D(l+8u);
        base=D(0x00ae23bcu);
        a=cm_fn_004ae8a0(destination,l+0xd0u,0x7db,base+a*110u);
    }
    a=D(l+0xcu);
    if(a!=0xffffffffu) {
        c=D(0x00ae23bcu);
        cm_fn_0053dba0(c+a*110u,l+0xd0u,0xc8,1);
        a=D(l+0xcu);
        base=D(0x00ae23bcu);
        a=cm_fn_004ae8a0(destination,l+0xd0u,0x7db,base+a*110u);
    }
    if(result!=0) {
        a=D(l+0x67u);
        a=cm_fn_004ae8a0(destination,l+0x6cu,0x7d3,a);
    }
done:
    CM_FS(volatile u32,0)=frame.prev;
    return a;
}
