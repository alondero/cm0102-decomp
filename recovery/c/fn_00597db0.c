#include "cmre.h"
/* Preferred-image VAs, base00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4; five direct callers consume full EAX. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u8 CM_THISCALL cm_fn_005986b0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define F(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00597db0(u32 self,u32 kind)
{
    u32 index,record,group,a,base,value,region,factor,minimum,maximum;
    s32 rating,level,division,denominator;
    s8 category;
    long double live,weight;
    volatile long long converted;
    a=D(self+8);record=D(0x00ae23b4)+a*581u;
    if(B(record+0x64)==3)return 0;
    rating=(s32)W(record+0x80)/50;
    index=(u32)(s32)(s8)kind-5u;
    if(index<=5) {
        /* Original immutable DWORD dispatch table, exact guarded read. */
        (void)D(0x005982f0+index*4u);
        base=index<=2?35000u:index==3?20000u:index==4?1500u:1000u;
    } else base=750;
    if(cm_fn_0053b900((u32 *)record))return base;
    group=D(record+0x53);
    value=base;
    if(group) {
        category=B(group+0x7e);
        if(category<1)category=1;
        else if(category>3)category=3;
        level=B(group+0x85);
        live=(long double)level;
        denominator=(s32)category*20;
        live/=(long double)denominator;
        live*=(long double)(s32)base;
        converted=(long long)live; /* __ftol CALL00597ea1, ADR0006. */
        value=(u32)converted;
        if(category==1) {
            region=D(group);
            a=D(0x009cf2e4);
            if(region==a || region==D(0x009cf360) || region==D(0x009cf488)) {
                if(W(record+0x80)>7250) {
                    live=(long double)(s32)value;live*=F(0x009689b8);
                    converted=(long long)live; /* __ftol CALL0059813c. */
                    value=(u32)converted;
                }
            } else if(region!=D(0x009cf36c)) {
                if(region==D(0x009cf4f8)) {
                    if((s32)value>1750) {
                        value-=1750u;live=(long double)(s32)value;
                        live*=F(0x00969040);live+=F(0x0096a5f8);
                        converted=(long long)live; /* __ftol CALL00597f19. */
                        value=(u32)converted;
                    }
                } else {
                    if(region==D(0x009cf314) || region==D(0x009cf300) ||
                       region==D(0x009cf45c) || region==D(0x009cf338)) {
                        factor=0x00969040;minimum=2500;
                    } else if(region==D(0x009cf4a8) || region==D(0x009cf2c0) ||
                              region==D(0x009cf244) || region==D(0x009cf434)) {
                        factor=0x00969040;minimum=1250;
                    } else {factor=0x00968e80;minimum=750;}
                    /* The native low-WORD sign extension overwrites L+4. */
                    division=(s16)rating;
                    weight=(long double)F(factor);
                    if(division>level*10) {
                        denominator=level<1?1:level;
                        weight=(long double)division;
                        weight/=(long double)denominator;
                        weight*=F(0x00967880);weight*=F(factor);
                    }
                    if((s32)value>(s32)minimum) {
                        if(weight>(long double)F(0x00967890))
                            weight=(long double)F(0x00967890);
                        value-=minimum;live=(long double)(s32)value;
                        live*=weight;
                        if(minimum==750) {
                            live+=F(0x0096a5f0);
                            converted=(long long)live; /* __ftol CALL00598002. */
                        } else if(minimum==1250) {
                            live+=F(0x0096a5e8);
                            converted=(long long)live; /* __ftol CALL0059808d. */
                        } else {
                            live+=F(0x009690b0);
                            converted=(long long)live; /* __ftol CALL00598114. */
                        }
                        value=(u32)converted;
                    }
                }
            }
        }
        a=D(record+0x57);
        if(a) {
            live=(long double)(s32)W(a+0x69);
            live*=(long double)(s32)value;live*=F(0x00968918);
            converted=(long long)live; /* __ftol CALL00598178, linked path. */
            value=(u32)converted;
        }
    } else {
        live=(long double)(s32)base;live*=F(0x009689e0);
        converted=(long long)live; /* __ftol CALL00598178, unlinked path. */
        value=(u32)converted;
    }
    a=(u32)(s32)(s8)cm_fn_005986b0(self)+2u;
    if(a<=4) {
        (void)D(0x00598308+a*4u);
        if(a==4)value+=value;
        else if(a!=2) {
            factor=a==0?0x00969040:a==1?0x009689d0:0x00968940;
            live=(long double)(s32)value;live*=F(factor);
            converted=(long long)live; /* __ftol CALL005981c7. */
            value=(u32)converted;
        }
    }
    a=D(self+0x14);if((s32)a<(s32)value)value=a;
    if(index<=5) {
        (void)D(0x0059831c+index*4u);
        if(index==0){maximum=35000;minimum=1750;}
        else if(index<=2){maximum=20000;minimum=500;}
        else if(index==3){maximum=7500;minimum=250;}
        else if(index==4){maximum=2000;minimum=125;}
        else {maximum=1500;minimum=125;}
    } else {maximum=1000;minimum=125;}
    if((s32)value>(s32)maximum)return maximum;
    if((s32)value<(s32)minimum)return minimum;
    if((s32)value>250000)value=250000;
    return value;
}
