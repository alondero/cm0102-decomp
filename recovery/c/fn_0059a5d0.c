#include "cmre.h"
/* Preferred-image VAs, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Candidate: native instruction reconstruction; not accepted. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern int CM_CDECL cm_fn_0090cfc0(int);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define F(a) CM_GLOBAL(volatile double,a)
#define L(o) (*(volatile u32 *)(scratch+(o)))
#define LW(o) (*(volatile u16 *)(scratch+(o)))
#define LF(o) (*(volatile float *)(scratch+(o)))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059a5d0(u32 self,u32 match,u32 data)
{
    /* L is ESP after SUB 21c; saved registers lie below L. */
    u8 scratch[0x21c];
    u32 a,b,c,d,si,di,bp,carry,line,p,q,target;
    long double t,live;
    volatile long long converted;
    volatile s32 numerator;
    a=data; b=0; si=0; di=0; L(4)=self; L(12)=0;
    if(a==0){p=(u32)(scratch+0x11c);q=(u32)(scratch+0x1c);line=0xf5f;goto entry_error;}
    bp=match;
    if(bp==0){p=(u32)(scratch+0x1c);q=(u32)(scratch+0x11c);line=0xf64;goto entry_error;}
    a=D(bp+0x1c);
    if(a!=b){
        a=cm_fn_0053b900((u32 *)a); if(a) return a;
        si=D(bp+0x1c);
        if(si==b){
            cm_fn_00944cff(0x9c8784,0,0,scratch+0x1c,scratch+0x11c);
            a=cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,a,scratch+0x1c,scratch+0x11c,0x20b5);
            a=cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
            D(0xb67a34)=0;si=0;
        }else{
            a=cm_fn_00540a50(si,(u32)(scratch+0x18),1);
            if(a!=b && L(0x18)==b) si=a;
            a=D(si); c=L(4);si=a*359u;a=D(c);si+=a;
        }
    }
    a=D(bp+0x20);
    if(a!=b){
        a=cm_fn_0053b900((u32 *)a);if(a)return a;
        di=D(bp+0x20);
        if(di==b){
            cm_fn_00944cff(0x9c8784,0,0,scratch+0x1c,scratch+0x11c);
            a=cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,a,scratch+0x1c,scratch+0x11c,0x20b5);
            a=cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
            D(0xb67a34)=0;di=0;
        }else{
            a=cm_fn_00540a50(di,(u32)(scratch+0x10),1);
            if(a!=b && L(0x10)==b)di=a;
            a=D(di);c=L(4);di=a*359u;a=D(c);di+=a;
        }
    }
    c=data;a=D(c+0x34);
    if(a!=b){
        d=D(c+0x38);bp=a;L(0)=bp;L(0x14)=d;
        a=cm_fn_0090cfc0(10);c=a+2u;
        /* Native signed IDIV occurs before any floating operation. */
        numerator=1;a=(u32)(numerator/(s32)c);L(4)=a;
        t=(long double)(s32)L(4);t+=F(0x968f98);t*=(long double)(s32)L(0);
        converted=(long long)t; a=(u32)converted; /* __ftol 0059a843 */
        b=a;
    }else{bp=0;L(0x14)=b;L(0)=bp;}
    c=match;a=D(c+0x1c);
    if(B(a+0x64)!=1){d=D(c+0x20);if(B(d+0x64)!=1){
        t=(long double)(s32)L(0);t*=F(0x9689e0);
        converted=(long long)t; a=(u32)converted; /* __ftol 0059a879 */
        c=match;bp=a;goto costs;
    }}
    a=D(c+0x18);
    if(a){
        a=D(a+0x3c);L(4)=a;
        t=(long double)(s32)L(4);
        if((s32)a>17500)t*=F(0x968fe0);
        else if((s32)a>12500)t*=F(0x968968);
        else if((s32)a>5000){if((s32)bp<5000)t*=F(0x968e00);else t*=F(0x96a6a8);}
        else t*=F(0x969510);
    }else{
        t=(long double)(s32)L(0);
        if((s32)bp>20000)t*=F(0x96a6a0);
        else if((s32)bp>10000)t*=F(0x96a698);
        else if((s32)bp>5000)t*=F(0x968e48);
        else t*=F(0x968968);
    }
    converted=(long long)t; a=(u32)converted; /* __ftol 0059a8c3 */
    c=match;bp=a;a=(a&0xffffff00u)|B(c+0x40);
    if((u8)a && (s32)bp<500)bp=500;
costs:
    a=D(c+0x1c);
    if(B(a+0x64)==1){
        if(W(a+0x80)!=0){
            c=(c&0xffff0000u)|W(a+0x80);LW(4)=(u16)c;
            a=cm_fn_0090cfc0(200);L(8)=a;
            t=(long double)(s32)L(8);live=(long double)(s32)L(0);
            d=(u32)(s32)(s16)LW(4);live*=F(0x968928);L(8)=d;t+=live;
            live=(long double)(s32)L(8);live*=F(0x968968);t+=live;t+=F(0x96a6b0);
            converted=(long long)t; a=(u32)converted; /* __ftol 0059a948 */
            L(0)=a;
            if((s16)LW(4)>0x128e){
                t=(long double)(s32)L(0);t*=F(0x968928);
                converted=(long long)t; a=(u32)converted; /* __ftol 0059a968 */
                L(0)=a;
            }
        }else{
            a=cm_fn_0090cfc0(400);L(8)=a;
            t=(long double)(s32)L(8);live=(long double)(s32)L(0);live*=F(0x968928);t+=live;t+=F(0x96a690);
            converted=(long long)t; a=(u32)converted; /* __ftol 0059aa1a */
            L(0)=a;
        }
    }else{
        a=(a&0xffff0000u)|W(a+0x80);
        if((u16)a){
            a=(u32)(s32)(s16)a;L(8)=a;t=(long double)(s32)L(8);t*=F(0x9689e0);
            converted=(long long)t; a=(u32)converted; /* __ftol 0059aa41 */
            a=cm_fn_0090cfc0((s32)a);
        }else a=cm_fn_0090cfc0(250);
        L(8)=a;t=(long double)(s32)L(8);LF(4)=(float)t;
        a=cm_fn_0090cfc0(250);L(8)=a;
        t=(long double)(s32)L(8);t+=(long double)LF(4);live=(long double)(s32)L(0);t+=live;
        converted=(long long)t; a=(u32)converted; /* __ftol 0059aaa8 */
        L(0)=a;
    }
    d=match;a=(u32)(s32)(s8)B(d+0x3b);a++;
    if(a<=3){
        target=D(0x59af24+a*4u);
        switch(target){
        case 0x59aac9:L(12)=25000;goto scale;
        case 0x59aad6:L(12)=75000;goto scale;
        case 0x59abb7:L(12)=125000;goto scale;
        case 0x59aae3:L(12)=0;break;
        }
    }
    a=(a&0xffffff00u)|B(d+0x3a);
    if((u8)a==8 || (u8)a==7){
        c=D(d+0x1c);a=(u32)(s32)(s16)W(c+0x80);c=D(d+0x20);c=(u32)(s32)(s16)W(c+0x80);a+=c;
        if((s32)a>10000)L(12)=125000;
        else{c=(s32)a<=7500; c--;c&=50000;c+=25000;L(12)=c;}
    }else if((u8)a==6 || (u8)a==4){
        p=(u8)a;c=D(d+0x14);a=(a&0xffff0000u)|W(c+0x69);a=(u32)(s32)(s16)a;
        if((s32)a<=5)a=5;
        c=a*(p==6?125000u:25000u);
        d=(u32)((s32)c/20);L(12)=d;d=match;
    }
scale:
    a=D(d+0x1c);c=D(a+0x53);
    a=(a&0xffffff00u)|(c?B(c+0x7e):3u);a=(u32)(s32)(s8)a;a--;
    if(a){
        a--;q=a;a=0x66666667;
        if(q==0){
            L(12)=(u32)((s32)L(12)/5);b=(u32)((s32)b/2);bp=(u32)((s32)bp/4);L(0)=(u32)((s32)L(0)/2);
        }else{
            L(12)=(u32)((s32)L(12)/10);b=(u32)((s32)b/10);bp=(u32)((s32)bp/15);L(0)=(u32)((s32)L(0)/10);
        }
        d=match;
    }
    a=D(d+0x1c);
    if(a && c){
        a=D(c);c=D(0x9cf510);
        if(a==c || a==D(0x9cf2dc) || a==D(0x9cf3e4)){
            L(12)=(u32)((s32)L(12)/5);b=(u32)((s32)b/2);bp=(u32)((s32)bp/4);L(0)=(u32)((s32)L(0)/2);d=match;
        }
    }
    c=L(0);a=(a&0xffffff00u)|B(d+0x3c);b-=c;c=L(0x14);b-=bp;b+=c;bp=b;
    if((u8)a)goto shared;
    a=D(d+0x1c);c=D(d+0x14);a=D(a+0x57);if(a!=c)goto shared;
    live=(long double)(s32)L(12);t=live;t*=F(0x969040);
    converted=(long long)t; a=(u32)converted; /* __ftol 0059ad2c; live remains on original x87 stack */
    d=D(si+0x158);b=D(si+0xb8);c=D(si);d+=a;b+=a;D(si+0x158)=d;
    live*=F(0x9689e0);d=(u32)((s32)a>>31);p=c;c+=a;carry=c<p;D(si+0xb8)=b;D(si)=c;a=D(si+4);a+=d+carry;D(si+4)=a;
    converted=(long long)live; a=(u32)converted; /* __ftol 0059ad62 */
    d=D(di+0x158);b=D(di+0xb8);c=D(di);d+=a;b+=a;D(di+0x158)=d;d=(u32)((s32)a>>31);p=c;c+=a;carry=c<p;
    D(di+0xb8)=b;D(di)=c;b=D(di+4);b+=d+carry;D(di+4)=b;
    a=D(si+0x140);c=D(si+0xa0);b=D(si);a+=bp;c+=bp;D(si+0x140)=a;a=bp;d=(u32)((s32)a>>31);p=b;b+=a;carry=b<p;
    D(si+0xa0)=c;c=match;D(si)=b;di=D(si+4);di+=d+carry;D(si+4)=di;
    a=D(c+0x1c);c=(c&0xffffff00u)|B(a+0x6d);if(!(u8)c)return a;
    a=D(a+0x69);if(!a)return a;
    d=D(a+0x3c);L(0x10)=d;a=cm_fn_0090cfc0(15);a++;L(8)=a;
    t=(long double)(s32)L(8);t*=F(0x967898);t+=F(0x969520);t*=(long double)(s32)L(0x10);
    converted=(long long)t; a=(u32)converted; /* __ftol 0059ae09 */
    d=D(si+0x12c);di=D(si+0x8c);c=D(si);d+=a;di+=a;D(si+0x12c)=d;d=(u32)((s32)a>>31);carry=c<a;c-=a;
    D(si+0x8c)=di;D(si)=c;a=D(si+4);a-=d+carry;D(si+4)=a;return a;
shared:
    t=(long double)(s32)L(12);t*=F(0x968968);
    converted=(long long)t; a=(u32)converted; /* __ftol 0059ae50 */
    d=D(si+0xb8);c=a;a=D(si+0x158);b=D(si);a+=c;d+=c;D(si+0x158)=a;a=c;D(si+0xb8)=d;d=(u32)((s32)a>>31);
    p=b;b+=a;carry=b<p;D(si)=b;a=D(si+4);a+=d+carry;D(si+4)=a;
    d=D(di+0x158);b=D(di+0xb8);d+=c;a=c;D(di+0x158)=d;b+=c;c=D(di);d=(u32)((s32)a>>31);p=c;c+=a;carry=c<p;
    D(di+0xb8)=b;D(di)=c;a=D(di+4);a+=d+carry;D(di+4)=a;
    a=bp;d=(u32)((s32)a>>31);a-=d;d=D(si+0xa0);c=a;a=D(si+0x140);c=(u32)((s32)c>>1);bp=D(si);a+=c;d+=c;
    D(si+0x140)=a;a=c;D(si+0xa0)=d;d=(u32)((s32)a>>31);p=bp;bp+=a;carry=bp<p;a=c;D(si)=bp;b=D(si+4);b+=d+carry;D(si+4)=b;
    d=D(di+0x140);si=D(di+0xa0);d+=c;D(di+0x140)=d;si+=c;c=D(di);D(di+0xa0)=si;d=(u32)((s32)a>>31);p=c;c+=a;carry=c<p;D(di)=c;a=D(di+4);a+=d+carry;D(di+4)=a;return a;
entry_error:
    cm_fn_00944cff(0x9c8784,0,0,(void *)p,(void *)q);
    a=cm_fn_0090d130();cm_fn_00944c9f(0xae24d0,0x9870e8,a,p,q,line);
    a=cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);D(0xb67a34)=0;return a;
}
