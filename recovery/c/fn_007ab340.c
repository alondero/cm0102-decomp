#include "cmre.h"
/* Preferred-image VA007ab340/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/RET4. The sole indexed caller's next original7a8690 replaces EAX
   before any read; all root return bits are unobserved there. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern void CM_THISCALL cm_fn_0086fb80(u32,u32);
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
extern u32 CM_CDECL cm_fn_0053a5b0(void);
extern void CM_THISCALL cm_fn_005fff30(u32,u32);
extern void CM_THISCALL cm_fn_007c6bf0(u32,u32,u8);
extern void CM_THISCALL cm_fn_004ca1c0(u32,u32,u32,u32);
extern u16 CM_STDCALL cm_fn_007c6930(u32,u32);
extern void CM_THISCALL cm_fn_007c6a30(u32,u32,u8,s8,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u8 CM_CDECL cm_fn_0053ab90(u32,u32);
extern u32 CM_STDCALL cm_fn_004cb990(s8,u32);
extern u32 CM_THISCALL cm_fn_004d0b50(u32,u32);
extern u32 CM_STDCALL cm_fn_004d45f0(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern void CM_THISCALL cm_fn_0085e960(u32,u32,u32);
extern void CM_CDECL cm_fn_00539490(u32);

__attribute__((target("no-sse3")))
void CM_STDCALL cm_fn_007ab340(volatile u32 argument)
{
    /* L=original ESP after SUB10. L0 is a BYTE scratch then a date;
       L8 is the adjacent output date. The formal itself is reused. */
    volatile u8 scratch[16];
    u32 l=(u32)scratch,person=argument,a,b,c,p,target,index;
    u32 first,second,year,month;
    u8 flag,age,other;
    s16 threshold,score;
    long double live;
    volatile long long converted;
    a=D(0x00ae2d0c);b=D(person);c=b*79u+a+11u;
    flag=B(c);B(c)=(u8)(flag&0xcfu);B(person+0x5f)=1;
    c=D(0x00dd7f00);cm_fn_0086fb80(c,person);
    b=D(person);B(person+0x22)=0;B(person+0x23)=0;
    a=D(0x00ae2d0c);c=b*79u+a+11u;flag=B(c);B(c)=(u8)(flag&0xf0u);
    b=D(person);a=D(0x00ae2d0c);B(b*79u+a+8u)=0;
    b=D(person);a=D(0x00ae2d0c);B(b*79u+a+9u)=0x9c;
    a=D(person+0x1a);a=cm_fn_00539c60(a);b=D(person+0x1a);D(person+4)=a;
    a=cm_fn_0053a1b0(b);b=D(person+0x1a);D(person+8)=a;
    (void)b; /* Original pushed but callee53a5b0 takes no argument. */
    a=cm_fn_0053a5b0();D(person+12)=a;
    c=D(0x00b63c98);cm_fn_005fff30(c,person);
    a=W(0x00ae2c92);W(person+0x40)=(u16)a;
    a=W(0x00ae2c90);W(person+0x3e)=(u16)a;
    for(flag=1;flag<=64;++flag) {
        argument=(argument&0xffffff00u)|flag;a=argument;c=D(0x00b74350);
        cm_fn_007c6bf0(c,person,(u8)a);
    }
    /* The last native loop update also writes65 to the formal. */
    argument=(argument&0xffffff00u)|65u;
    c=D(0x00ae1050);cm_fn_004ca1c0(c,person,0,0);
    /* Original4ca1c0 PUSH/POP ECX preserves its receiver. MOVSXCX
       leaves that high16, but only resulting DI enters later arithmetic. */
    a=(u32)(s32)(s8)B(person+0x56);
    threshold=(s16)(u16)((20u-a)*10u);
    for(flag=1;flag<=64;++flag) {
        argument=(argument&0xffffff00u)|flag;b=argument;c=D(0x00b74350);
        score=(s16)cm_fn_007c6930(person,b);
        if(score!=0 && score>=threshold) {
            a=(u32)(((s32)score-(s32)threshold)/75);B(l)=10;
            if((s32)a<=10)B(l)=(u8)a;
            c=D(0x00b74350);cm_fn_007c6a30(c,person,(u8)b,(s8)B(l),1);
        }
    }
    argument=(argument&0xffffff00u)|65u;
    p=D(person+0x69);argument=(u32)(s32)(s16)W(p+6);
    live=(long double)(s32)argument;live*=Q(0x00969040);
    converted=(long long)live; /* __ftol CALL007ab4ce, site1. */
    a=cm_fn_0090cfc0((u32)converted);p=D(person+0x69);++a;W(p+4)=(u16)a;
    p=D(person+0x69);a=(u32)(s32)(s16)W(p+6);b=(u32)(s32)(s16)W(p+4);
    argument=a;live=(long double)(s32)argument;argument=b;live*=F(0x00968f18);
    if(!((long double)(s32)argument>=live)) {
        converted=(long long)live; /* __ftol CALL007ab50e, site2. */
        W(p+4)=(u16)(u32)converted;
    }
    p=D(person+0x69);a=cm_fn_0090cfc0(750);b=W(p+4);
    W(p+8)=(u16)(a+(u16)(b*25u)+1u);
    p=D(person+0x69);a=cm_fn_0090cfc0(200);b=W(p+4);
    W(p+12)=(u16)(a+(u16)(b*15u)+1u);
    p=D(person+0x69);b=W(p+12);
    if((s16)W(p+8)<(s16)b) {W(p+8)=(u16)(b+10u);p=D(person+0x69);W(p+12)=1;}
    p=D(person+0x69);a=cm_fn_0090cfc0(50);b=W(p+8);W(p+10)=(u16)(a+b);
    a=D(person+0x39);
    if(a)age=B(person+0x3d);
    else {a=D(person+0x24);age=a?B(person+0x28):(u8)argument;}
    index=(u32)((s32)(s8)age-1);
    if(index<=9) {target=D(0x007aba98+index*4u);(void)target;}
    else goto default_age;
    switch(index) {
    case 0:
        a=cm_fn_0090cfc0(20);c=D(person+0x39);age=(u8)(a+36u);
        argument=(argument&0xffffff00u)|age;
        if(!c)goto age_ready;
        p=D(person+0x69);if(!p)goto age_ready;
        score=(s16)W(c+0x80);
        if(score>6250) {
            other=B(c+0x82);
            a=cm_fn_0090cfc0(other?11:20);p=D(person+0x69);B(p+15)=(u8)(a+(other?10:1));
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+19)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+20)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+22)=(u8)(a+1);
            first=cm_fn_0090cfc0(100);second=cm_fn_0090cfc0(100);p=D(person+0x69);B(p+25)=(u8)(first-second);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+29)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+27)=(u8)(a+1);
            a=cm_fn_0090cfc0(other?15:11);p=D(person+0x69);B(p+32)=(u8)(a+(other?6:10));
        } else {
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+15)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+19)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+20)=(u8)(a+1);
            a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+22)=(u8)(a+1);
            first=cm_fn_0090cfc0(100);second=cm_fn_0090cfc0(100);p=D(person+0x69);B(p+25)=(u8)(first-second);
            if(score>3250) {
                a=cm_fn_0090cfc0(5);
                if(a) {a=cm_fn_0090cfc0(16);p=D(person+0x69);B(p+29)=(u8)(a+5);}
                else {a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+29)=(u8)(a+1);}
                a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+27)=(u8)(a+1);
                a=cm_fn_0090cfc0(10);
                if(a) {a=cm_fn_0090cfc0(15);p=D(person+0x69);B(p+32)=(u8)(a+1);}
                else {a=cm_fn_0090cfc0(15);p=D(person+0x69);B(p+32)=(u8)(a+6);}
            } else {
                a=cm_fn_0090cfc0(10);
                if(a) {a=cm_fn_0090cfc0(15);p=D(person+0x69);B(p+29)=(u8)(a+6);}
                else {a=cm_fn_0090cfc0(20);p=D(person+0x69);B(p+29)=(u8)(a+1);}
                first=cm_fn_0090cfc0(100);second=cm_fn_0090cfc0(100);p=D(person+0x69);B(p+27)=(u8)(first-second);
                a=cm_fn_0090cfc0(100);
                if(a) {a=cm_fn_0090cfc0(15);p=D(person+0x69);B(p+32)=(u8)(a+6);}
                else {a=cm_fn_0090cfc0(10);p=D(person+0x69);B(p+32)=(u8)(a+1);}
            }
        }
        age=(u8)argument;break;
    case 1:case 2:a=cm_fn_0090cfc0(25);age=(u8)(a+40);break;
    case 3:a=cm_fn_0090cfc0(10);age=(u8)(a+37);break;
    case 5:case 6:a=cm_fn_0090cfc0(15);age=(u8)(a+35);break;
    case 7:a=cm_fn_0090cfc0(15);age=(u8)(a+32);break;
    case 9:a=cm_fn_0090cfc0(25);age=(u8)(a+35);break;
    default:goto default_age;
    }
    goto age_ready;
default_age:
    a=cm_fn_0090cfc0(15);age=(u8)(a+39);
age_ready:
    a=W(0x00ae2c92);W(person+0x12)=(u16)(a-(u32)(s32)(s8)age);
    a=cm_fn_0090cfc0(365);W(person+0x10)=(u16)a;
    a=cm_fn_0053ab90(person,0x00ae2c90);W(person+0x18)=(u16)(s16)(s8)a;
    a=cm_fn_0090cfc0(125);other=(u8)a;
    if((s8)other<34)flag=0;else if((s8)other<40)flag=1;else if((s8)other<60)flag=2;
    else if((s8)other<90)flag=3;else if((s8)other<94)flag=4;else if((s8)other<96)flag=5;
    else if((s8)other<108)flag=6;else if((s8)other<111)flag=7;else if((s8)other<121)flag=8;else flag=9;
    p=D(person+0x69);B(p+0x43)=flag;
    a=D(person+0x39);
    if(a) {p=D(person+0x69);if(p) {
        other=B(person+0x3d);c=D(0x00ae1050);(void)c;a=cm_fn_004cb990((s8)other,0);
        if(a) {
            c=D(0x00ae1050);(void)cm_fn_004d0b50(c,person);D(person+0x4e)=0;
            a=W(0x00ae2c92);argument=(argument&0xffff0000u)|(u16)a;
            a=cm_fn_0090cfc0(6);year=argument;year+=a+1u;
            b=D(person+0x39);c=D(0x00ae1050);(void)c;month=cm_fn_004d45f0(b);
            a=cm_fn_0090cfc0(28);a=(u32)(s32)(s16)(s8)(u8)(a+1u);
            b=cm_fn_00549f70(l,a,month,year,0xffffffffu);
            (void)cm_fn_00417c10(person+0x46,l+8,b);
            c=D(0x00ae1050);a=cm_fn_004cf5a0(c,person);
            if(a)cm_fn_0085e960(a,person,0);
        }
    }}
    cm_fn_00539490(person);
}
