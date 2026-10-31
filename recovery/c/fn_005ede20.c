#include "cmre.h"
/* Preferred-image VA005ede20/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL8/full final position EAX. Keep cached current WORD, actual argument
   slot rewrites, signed WORD gates, late input/global reads and wrapping
   positions. Partial-register color upper bits are unobserved: 804030
   inputs6/9 ->55f990 inputs9,10/13 ->5ee150 inputs12,13/16, WORD only;
   803fd0 input9 follows the same input13 ->input16 WORD path. */
#define D(a) (*(volatile u32 *)(a))
typedef u16 word_5ede20 __attribute__((may_alias));
#define W(a) (*(volatile word_5ede20 *)(a))
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e5ad0(u32,u32);
extern u32 CM_THISCALL cm_fn_00804030(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00803fd0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);

u32 CM_CDECL cm_fn_005ede20(volatile u32 p1,volatile u32 p2,volatile u32 p3,
    volatile u32 p4,volatile u32 p5,volatile u32 p6,volatile u32 p7,volatile u32 p8)
{
    u16 current=W((u32)&p3);
    u32 item=p8,y=p2,x,maximum,color_a,color_b,text,mode,enabled,bottom;
    u32 value,width,new_x;
    if(item!=0){
        if(D(0x009cc2c4)!=0xffffffffu){
            cm_fn_00944c9f(0x00de1f64,0x0099ec1c,0x2f);
            maximum=W((u32)&p4);color_a=W(0x00ae3184);color_b=W(0x00aebe04);
            enabled=(s16)current<(s16)(u16)maximum?item:0;x=p1;
            text=0x00de1f64;mode=0;
        }else{
            maximum=W((u32)&p4);color_a=W(0x00ae3184);color_b=W(0x00aebe04);
            enabled=(s16)current<(s16)(u16)maximum?item:0;x=p1;
            text=0x0098e7a8;mode=1;
        }
        cm_fn_00804030(0x00b74450,x-20u,y,x,y+20u,0x30,color_b,12,mode,color_a,text,enabled,0);
        x-=22u;
    }else x=p1;
    if(D(0x009cc2c4)!=0xffffffffu){
        cm_fn_00944c9f(0x00de1f64,0x0099ec1c,0x2b);
        item=p7;maximum=W((u32)&p4);color_a=W(0x00ae3184);
        bottom=y+20u;enabled=(s16)current<(s16)(u16)maximum?item:0;
        color_b=W(0x00aebe04);text=0x00de1f64;mode=0;
    }else{
        item=p7;maximum=W((u32)&p4);color_b=W(0x00aebe04);
        bottom=y+20u;enabled=(s16)current<(s16)(u16)maximum?item:0;
        color_a=W(0x00ae3184);text=0x009cdc54;mode=1;
    }
    cm_fn_00804030(0x00b74450,x-20u,y,x,bottom,0x30,color_b,12,mode,color_a,text,enabled,0);
    value=(u32)(s32)(s16)W((u32)&p4);x-=22u;p4=value;
    cm_fn_0066f4e0(0x00de1f64,0x009cdc30,value,value);
    width=cm_fn_005e5ad0(2,0x00de1f64)+10u;p3=width;
    maximum=p4;value=(u32)(s32)(s16)current;
    cm_fn_0066f4e0(0x00de1f64,0x009cdc30,value,maximum);
    width=p3;new_x=x-width;color_b=W(0x00aebdec);p4=new_x;
    cm_fn_00803fd0(0x00b74450,new_x,y,x,bottom,2,0,12,2,color_b,0x00de1f64,1);
    value=p4;x=value-2u;
    if(D(0x009cc2c4)!=0xffffffffu){
        cm_fn_00944c9f(0x00de1f64,0x0099ec1c,0x2a);
        item=p6;color_a=W(0x00ae3184);enabled=(s16)current>1?item:0;
        color_b=W(0x00aebe04);text=0x00de1f64;mode=0;
    }else{
        item=p6;color_b=W(0x00aebe04);enabled=(s16)current>1?item:0;
        color_a=W(0x00ae3184);text=0x009cdc2c;mode=1;
    }
    cm_fn_00804030(0x00b74450,x-20u,y,x,bottom,0x30,color_b,12,mode,color_a,text,enabled,0);
    item=p5;x-=22u;
    if(item!=0){
        if(D(0x009cc2c4)!=0xffffffffu){
            cm_fn_00944c9f(0x00de1f64,0x0099ec1c,0x2e);
            value=p5;color_a=W(0x00ae3184);color_b=W(0x00aebe04);
            enabled=(s16)current>1?value:0;text=0x00de1f64;mode=0;
        }else{
            color_a=W(0x00ae3184);enabled=(s16)current>1?item:0;
            color_b=W(0x00aebe04);text=0x0098e7c4;mode=1;
        }
        cm_fn_00804030(0x00b74450,x-20u,y,x,bottom,0x30,color_b,12,mode,color_a,text,enabled,0);
    }
    return x-20u;
}
