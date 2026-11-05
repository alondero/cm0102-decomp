#include "cmre.h"
/* Preferred-image VAs, base 00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1 / RET4. Incidental full EAX is retained on every exit. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile double,(a))
#define S(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00599a20(u32 self,u32 flag)
{
    u32 a,b,c,d,index,club,staff,group,factor,amount,increase,total,first,second;
    long double t,lower,bound;
    volatile long long converted;
    d=D(0x00ae23b4);index=D(self+8);club=d+index*0x245u;
    if(club) {
        staff=D(club+0x69);
        if(staff && (s32)D(staff+0x3c)<1500) {
            t=(long double)(s32)W(club+0x80);t*=F(0x00968e18);
            converted=(long long)t; /* __ftol 00599a6b. */
            a=(u32)converted;D(staff+0x3c)=a;
            a=D(club+0x69);
            if((s32)D(a+0x3c)<1500)D(a+0x3c)=1500;
        }
        a=D(club);d=a*73u;a=D(0x00b67a44);c=D(a);
        a=(u32)(s32)W(d+c+8);
    }else a=0;
    lower=(long double)(s32)a;lower*=F(0x0096a5c0);
    bound=(long double)F(0x00968f20);
    if(!(lower>bound))lower=(long double)F(0x00968f20);
    t=(long double)(s32)D(club+0x73);t*=lower;
    converted=(long long)t; /* __ftol 00599ac5; lower discarded 00599ad5. */
    c=(u32)converted;
    a=(u32)(s32)W(club+0x80);
    lower=(long double)(s32)a;lower*=F(0x0096a680);
    bound=(long double)F(0x00968968);
    if(!(lower>bound))lower=(long double)F(0x00968968);
    t=(long double)(s32)c;t*=lower;
    converted=(long long)t; /* __ftol 00599b00; lower discarded 00599b0e. */
    a=(u32)converted;staff=D(club+0x69);
    if(staff){bound=(long double)(s32)D(staff+0x3c);bound*=F(0x0096a678);}
    else bound=(long double)F(0x0096a5e8);
    t=(long double)(s32)a;
    /* Native FST m64 saves this exact signed32 integer, then FCOMP ST1.
       C0 chooses the saved integer on less-than OR unordered. */
    if(!(t>=bound))t=(long double)(double)t;
    else if(staff){t=(long double)(s32)D(staff+0x3c);t*=F(0x0096a678);}
    else t=(long double)F(0x0096a5e8);
    converted=(long long)t; /* __ftol 00599b58. */
    a=(u32)converted;
    if(!a)return a;
    c=D(self+0x18);
    if((s32)c>=(s32)a)return a;
    b=(u8)(B(self+0x164)+1u);B(self+0x164)=(u8)b;
    if((s8)b<0 || (s8)b>8)B(self+0x164)=8;
    d=(u32)(s32)(s8)B(self+0x164);
    a-=c;t=(long double)(s32)a;t*=S(d*4u+0x009c8758);
    converted=(long long)t; /* __ftol 00599ba4. */
    b=(u32)converted;
    a=cm_fn_0090cfc0(11);c=(a+95u)*b;
    /* Native signed high-product division by 100 truncates toward zero. */
    increase=(u32)((s32)c/100);
    c=D(self+0x18);c+=increase;D(self+0x18)=c;total=c;
    a=D(club+0x57);
    (void)a; /* Both original branches load the same signed16 rating. */
    t=(long double)(s32)W(club+0x80);
    t*=F(0x00969048);t+=F(0x00968938);t*=F(0x00968ea8);
    converted=(long long)t; /* __ftol 00599c19. */
    amount=(u32)converted*increase;
    a=D(club+0x69);
    if(a){a=D(a+0x3c);if(a){t=(long double)(s32)a;t*=F(0x0096a678);}else t=(long double)F(0x0096a5e8);}
    else t=(long double)F(0x0096a5e8);
    t=(long double)(s32)total/t;
    S(self+0x160)=(float)t;
    bound=(long double)F(0x0096a678);
    if(t>bound)D(self+0x160)=0x3f600000;
    group=D(club+0x53);a=group?B(group+0x7e):3u;
    if((s8)a!=1)amount=(u32)((s32)amount/((s8)a==2?4:10));
    a=D(club+0x57);
    if(a){a=D(a);b=D(0x009cf570);if(a==b){factor=0x00968928;goto scale;}}
    a=D(club);b=D(0x009d0250);
    if(a==b){factor=0x00968940;goto scale;}
    if(a==D(0x009d0110)){factor=0x00968de8;goto scale;}
    if(a==D(0x009d0248)){factor=0x00969040;goto scale;}
    if(a==D(0x009d0244)){factor=0x009689d8;goto scale;}
    if(a==D(0x009d024c)){factor=0x00968de8;goto scale;}
    if(!group)goto update;
    c=D(group);a=D(0x009cf314);
    if(c!=a)goto update;
    factor=0x00968978;
scale:
    t=(long double)(s32)amount;t*=F(factor);
    converted=(long long)t; /* __ftol 00599d4a. */
    amount=(u32)converted;
update:
    if(flag) {
        first=D(self+0xac);second=D(self+0x14c);c=D(self);
        first+=amount;D(self+0xac)=first;second+=amount;
        a=c+amount;D(self+0x14c)=second;D(self)=a;
        b=D(self+4);b+=((s32)amount<0?0xffffffffu:0u)+(a<amount);
        D(self+4)=b;
    }
    d=(u32)(s32)W(club+0x80);a=cm_fn_0090cfc0(d);b=a;
    a=(u32)(s32)W(club+0x80);a=cm_fn_0090cfc0(a);
    c=b+a+1u;t=(long double)(s32)c;t*=F(0x0096a350);
    t+=F(0x00967890);t*=(long double)(s32)increase;
    converted=(long long)t; /* __ftol 00599dc1. */
    amount=(u32)converted;
    first=D(self+0x15c);second=D(self+0xbc);c=D(self);
    first+=amount;second+=amount;D(self+0x15c)=first;
    a=c+amount;D(self+0xbc)=second;D(self)=a;
    a=D(self+4);a+=((s32)amount<0?0xffffffffu:0u)+(c+amount<amount);
    D(self+4)=a;
    return a;
}
