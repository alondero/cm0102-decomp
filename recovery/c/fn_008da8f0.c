#include "cmre.h"
/* Preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ABI: ret 12 at 008db38f; arguments at saved-register ESP+294/298/29c.
   Receivers and stack arguments follow the explicit ECX/PUSH sequences:
   008da918-91a, 008daaed-af3, 008dac3e-c64, 008daf1c-f29,
   008daf36-f6b, 008db018-024, 008db369-372.
   0066f4e0 is variadic: caller cleanup varies at 008dacc0/008dad22.
   Callees remain original boundaries. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_THISCALL cm_fn_00786d30(u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae770(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,const char *,u32,u32);
extern u32 CM_CDECL cm_fn_0053b610(u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c3b0(short *,int,short,char);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,const char *,...);
extern u32 CM_CDECL cm_fn_0090c520(u32,int,short,int);
#define R(a) CM_GLOBAL(volatile u32,(u32)(a))
#define BYTE(a) CM_GLOBAL(volatile u8,(u32)(a))
#define CLUB R(0xae23b4)
#define PERSON R(0xae23bc)
#define S(a) ((const char *)(a))
#define OUT 0xde1f64u
#define GET(i) ((u32)cm_fn_00785830((void *)obj,(i)))
#define P(o) ((u32)(scratch+(o)-0x10))
#define F(fmt,...) cm_fn_0066f4e0(OUT,S(fmt),__VA_ARGS__)

u32 CM_STDCALL cm_fn_008da8f0(u32 obj,u32 mode,u32 dest)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x274];
    u32 a,b,c,d,e,f,g,h,j,k,idx,rec,tab,base,off,other,ptr,fmt,r;
    s32 x,y;
    u8 status,n,flag,dateflag;
    unsigned long long wide;
    frame.state=0xffffffffu;
    frame.handler=0x9644c1;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    status=(u8)GET(9);
    idx=(u32)(s32)CM_GLOBAL(volatile s16,0xb77c7a);
    rec=R(0xb74c26+idx*0x300);
    if(rec && status!=14 && status!=3 && status!=16 && status!=2 &&
       status!=18 && status!=17 && status!=255) {
        a=GET(6); b=GET(7);
        tab=R(0xde1f54);
        if((s32)a<(s32)R(tab+4)) ptr=R(tab)+a*4;
        else ptr=R(tab);
        ptr=R(ptr);
        if(!ptr || R(ptr+0x24)!=b) {
            a=R(obj+0xd6); status=255;
            cm_fn_00786d30(R(0xb7431c),rec,a,9,0xffffffffu,obj);
        } else {
            n=BYTE(ptr+0x2c);
            if(n) {
                a=R(ptr+8); base=CLUB; base+=a*0x245;
                if(R(rec+0x39)==base) {
                    if(n==18 || n==3) goto update_same;
                    if(n!=13 && n!=14 && n!=19) goto updated;
                    n=13;
                } else {
                    if(n==17 || n==2) goto update_same;
                    if(n!=12 && n!=14 && n!=19) goto updated;
                    n=12;
                }
update_same:
                idx=(u32)(s32)CM_GLOBAL(volatile s16,0xb77c7a);
                a=R(obj+0xd6);
                rec=R(0xb74c26+idx*0x300);
                status=n;
                cm_fn_00786d30(R(0xb7431c),rec,a,9,(u32)(s32)(s8)n,obj);
            }
        }
    }
updated:
    a=GET(0); b=GET(1); c=GET(2); d=GET(3); e=GET(4);
    if(!mode) {
        h=GET(10); flag=(u8)GET(12); j=GET(17);
        BYTE(P(0x20))=(u8)j;
        cm_fn_0053e140(b,c,d,P(0x90),300,5,a);
        r=cm_fn_0053b610(R(P(0x20)));
        off=e*0x245;
        base=CLUB;
        x=(s8)BYTE(base+off+0x52);
        ptr=base+off+0x38;
        if(r && flag) F(0xa96ae4,x,x,P(0x90),ptr);
        else F(h ? 0xa96a90 : 0xa96a3c,x,x,ptr,P(0x90));
        cm_fn_004ae660((void *)dest,OUT);
        ptr=off+CLUB;
        cm_fn_004ae8a0(dest,S(ptr+0x38),0x7d5,ptr);
        off=a*110;
        r=cm_fn_0053e990(off+PERSON,b,c,d,0);
        if(r) r=cm_fn_004ae8a0(dest,S(P(0x90)),0x7d7,off+PERSON);
        goto done;
    }
    f=GET(5); g=GET(8); h=GET(10);
    n=(u8)GET(11); flag=(u8)GET(12); dateflag=(u8)GET(13);
    j=GET(14); k=GET(15);
    if(n) {
        cm_fn_0090c520((u32)(s32)(s8)n,(int)P(0x34),50,1);
        if(n==1) cm_fn_0066f4e0(P(0x68),S(0xa97210));
        else cm_fn_0066f4e0(P(0x68),S(0xa91ba0),P(0x34));
    }
    cm_fn_0053e140(b,c,d,P(0x90),300,1,a);
    if((s32)h>(s32)g && !flag) {
        R(P(0x1c))=h;
        frame.state=0;
        wide=cm_fn_0043f460((const int *)P(0x1c),6);
        cm_fn_00441c10(P(0x34),50,(u32)wide,(u32)(wide>>32),6);
        base=CLUB; off=e*0x245; other=base+f*0x245;
        y=(s8)BYTE(other+0x52); x=(s8)BYTE(base+off+0x52);
        if(!n) F(0xa97158,y,y,100,x,x,other+0x38,P(0x90),base+off+0x38,P(0x34));
        else F(0xa9705c,y,y,100,x,x,P(0x34),P(0x68),other+0x38,P(0x90),base+off+0x38);
    } else {
        R(P(0x1c))=g;
        frame.state=1;
        wide=cm_fn_0043f460((const int *)P(0x1c),6);
        cm_fn_00441c10(P(0x34),50,(u32)wide,(u32)(wide>>32),6);
        base=CLUB; off=e*0x245;
        if(!n && !((s32)g>0 && !flag) && (s32)f<=-1) {
            x=(s8)BYTE(base+off+0x52);
            F(0xa96e98,100,x,x,P(0x90),base+off+0x38);
        } else {
            other=base+f*0x245;
            y=(s8)BYTE(other+0x52); x=(s8)BYTE(base+off+0x52);
            if(!n && (s32)g>0 && !flag)
                F(0xa96fa8,100,y,y,100,x,x,P(0x34),other+0x38,P(0x90),base+off+0x38);
            else if(!n || (s32)g<=0)
                F(n?0xa96cfc:0xa96f10,y,y,100,x,x,other+0x38,P(0x90),base+off+0x38);
            else F(0xa96da8,y,y,100,x,x,P(0x34),P(0x68),other+0x38,P(0x90),base+off+0x38);
        }
    }
    frame.state=0xffffffffu;
    cm_fn_004ae660((void *)dest,OUT);
    if(dateflag) {
        wide=cm_fn_00549ef0((void *)P(0x88),j,k);
        frame.state=2;
        cm_fn_0054c3b0((short *)(u32)wide,(int)P(0x1bc),200,(char)255);
        frame.state=0xffffffffu;
        F(0xa96ca8,100,P(0x1bc));
        cm_fn_004ae770(dest,OUT);
    }
    fmt=0; ptr=0;
    if(status==14) {
        if(f!=0xffffffffu) fmt=0xa96c7c;
        else {fmt=0xa96c38; ptr=off+CLUB;}
    } else if(status==13) {
        fmt=f!=0xffffffffu?0xa96c38:0xa96bf8; ptr=off+CLUB;
    } else if(f!=0xffffffffu && status==12) {
        if(!flag) {ptr=CLUB+f*0x245; fmt=0xa96c38;}
    } else if(status==3) {ptr=off+CLUB; fmt=0xa96bb4;}
    else if(status==2) {
        if(f!=0xffffffffu) {ptr=CLUB+f*0x245; fmt=0xa96bb4;}
    } else if(status==16) fmt=0xa96b90;
    else if(status==18) {ptr=off+CLUB; fmt=0xa96b3c;}
    else if(status==17) {ptr=CLUB+f*0x245; fmt=0xa96b3c;}
    if(fmt) {
        if(ptr) {x=(s8)BYTE(ptr+0x52); F(fmt,x,x,ptr+0x38);}
        else cm_fn_0066f4e0(OUT,S(fmt));
        if(BYTE(OUT)) cm_fn_004ae770(dest,OUT);
    } else BYTE(OUT)=0;
    ptr=off+CLUB;
    cm_fn_004ae8a0(dest,S(ptr+0x38),0x7d5,ptr);
    if(f!=0xffffffffu) {
        ptr=CLUB+f*0x245;
        cm_fn_004ae8a0(dest,S(ptr+0x38),0x7d5,ptr);
    }
    off=a*110;
    r=cm_fn_0053e990(off+PERSON,b,c,d,0);
    if(r) r=cm_fn_004ae8a0(dest,S(P(0x90)),0x7d7,off+PERSON);
done:
    CM_FS(u32,0)=frame.prev;
    return r;
}
