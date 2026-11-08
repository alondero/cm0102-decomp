#include "cmre.h"
/* CM0102 3.9.68 SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VA007c7580/base00400000. THIS7/RET28. The sole indexed
   caller kills EAX immediately. Original drawing/serialization callees stay
   explicit. Partial-register color arguments have WORD-only consumers. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern void CM_THISCALL cm_fn_0055fba0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_0055f990(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_00803fd0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);

static u32 magnitude(u32 value)
{
    u32 sign=(u32)((s32)value>>31);
    return (value^sign)-sign;
}
static u32 quotient(u32 numerator,u32 denominator)
{
    volatile s32 divisor=(s32)denominator;
    return (u32)((s32)numerator/divisor);
}
static u32 receiver(void)
{
    u32 index=(u32)(s32)(s16)W(0x00b77c7a);
    return D(0x00b74c4c+index*0x300u);
}

void CM_THISCALL cm_fn_007c7580(u32 self,u32 left,u32 top,u32 right,u32 bottom,
                               u32 discrete,u32 text1,u32 text2)
{
    u32 a,b,c,t,dx,dy,x,y,step,range,accum,start,end,width,height,low,high;
    u8 text[20];
    if(discrete==1){
        a=D(self+12);b=D(self+8);c=D(self+16);
        ++a;D(self+12)=a;t=(s32)b>(s32)c?b:c;
        if(t==c) D(self+16)=c+1u;else D(self+8)=b+1u;
    }
    a=D(self+16);b=D(self+8);dy=magnitude(a-b);
    b=D(self+4);a=D(self+12);dx=magnitude(a-b);
    cm_fn_0055fba0(receiver(),left,top,right,bottom,dx,0,dy,0,0x80000,0,0xffffffffu);
    if(discrete==1){
        a=D(self+12);b=D(self+8);--a;D(self+12)=a;
        c=D(self+16);t=(s32)b>(s32)c?b:c;
        if(t==c) D(self+16)=c-1u;else D(self+8)=b-1u;
    }
    a=W(0x00aebdf0);
    cm_fn_0055f990(receiver(),1,left,top,left,bottom,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdf0);
    cm_fn_0055f990(receiver(),1,left-1u,top-1u,left-1u,bottom+1u,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdea);
    cm_fn_0055f990(receiver(),1,left,bottom,right,bottom,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdea);
    cm_fn_0055f990(receiver(),1,left-1u,bottom+1u,right+1u,bottom+1u,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdea);
    cm_fn_0055f990(receiver(),1,right,top,right,bottom,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdea);
    cm_fn_0055f990(receiver(),1,right+1u,top-1u,right+1u,bottom+1u,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdf0);
    cm_fn_0055f990(receiver(),1,left,top,right,top,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00aebdf0);
    cm_fn_0055f990(receiver(),1,left-1u,top-1u,right+1u,top-1u,0,0,0x400,a,0,0,0,0,0,0,0,0,0xffffffffu);
    a=W(0x00ae3184);
    cm_fn_00803fd0(0x00b74450,left,bottom+20u,right,bottom+30u,0,a,12,1,a,text1,1);
    a=W(0x00ae3184);
    cm_fn_00803fd0(0x00b74450,left,top-20u,right,top,0,a,1,1,a,text2,1);
    a=D(self+12);b=D(self+4);range=a-b;step=1;
    if((s32)range>1){
        width=right-left;accum=width;
        do{
            if((s32)quotient(accum,magnitude(range))>15) break;
            accum+=width;++step;
        }while((s32)step<(s32)range);
    }
    x=D(self+4);a=D(self+12);
    if((s32)x<=(s32)a){
        do{
            cm_fn_00944c9f((u32)text,0x0098806c,x);
            if(discrete==1){
                end=D(self+12)+1u;start=D(self+4);width=magnitude(right-left);
                a=quotient((x-start)*width,magnitude(end-start));
                start=D(self+4);
                low=(s32)end>(s32)start?magnitude(a)+left:right-magnitude(a);
                b=magnitude(end-start);
                a=quotient((x-start+1u)*width,b);
                start=D(self+4);
                high=(s32)end>(s32)start?magnitude(a)+left:right-magnitude(a);
            }else{
                width=magnitude(right-left);start=D(self+4);
                b=(x-start)*width;end=D(self+12);
                a=quotient(b,magnitude(end-start));
                end=D(self+12);start=D(self+4);
                t=(s32)end>(s32)start?magnitude(a)+left:right-magnitude(a);
                low=t-10u;high=t+10u;
            }
            a=W(0x00ae3184);
            cm_fn_00803fd0(0x00b74450,low,bottom,high,bottom+20u,0,a,12,1,a,(u32)text,1);
            x+=step;a=D(self+12);
        }while((s32)x<=(s32)a);
    }
    a=D(self+16);b=D(self+8);range=a-b;step=1;
    c=magnitude(range);
    if((s32)c>1){
        do{
            b=discrete==1?magnitude(range+1u):c;
            a=quotient((top-bottom)*step,b);
            if((s32)magnitude(a)>10) break;
            ++step;
        }while((s32)step<(s32)c);
    }
    b=D(self+8);a=D(self+16);y=(s32)b<(s32)a?b:a;
    for(;;){
        a=D(self+8);b=D(self+16);a=(s32)a>(s32)b?a:b;
        if((s32)y>(s32)a) break;
        cm_fn_00944c9f((u32)text,0x0098806c,y);
        if(discrete==1){
            end=D(self+16);start=D(self+8);
            if((s32)end>(s32)start){
                ++end;D(self+16)=end;
                height=magnitude(bottom-top);
                b=magnitude(end-start);
                a=quotient((y-start)*height,b);
                low=(s32)end>(s32)start?magnitude(a)+top:bottom-magnitude(a);
                a=quotient((y-start+1u)*height,magnitude(end-start));
                high=(s32)end>(s32)start?magnitude(a)+top:bottom-magnitude(a);
                D(self+16)=end-1u;
            }else{
                height=magnitude(bottom-top);++start;
                a=D(self+16);b=magnitude(a-start);D(self+8)=start;
                a=quotient((y-start)*height,b);
                end=D(self+16);
                low=(s32)end>(s32)start?magnitude(a)+top:bottom-magnitude(a);
                a=quotient((y-start+1u)*height,magnitude(end-start));
                end=D(self+16);
                high=(s32)end>(s32)start?magnitude(a)+top:bottom-magnitude(a);
                D(self+8)=start-1u;
            }
        }else{
            start=D(self+8);height=magnitude(bottom-top);end=D(self+16);
            a=quotient((y-start)*height,magnitude(end-start));
            end=D(self+16);
            t=(s32)end>(s32)start?magnitude(a)+top:bottom-magnitude(a);
            low=t-10u;high=t+10u;
        }
        a=W(0x00ae3184);
        cm_fn_00803fd0(0x00b74450,left-20u,low,left,high,0,a,12,1,a,(u32)text,1);
        y+=magnitude(step);
    }
}
