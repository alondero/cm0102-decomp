#include "cmre.h"
/* Preferred-image VA00546ab0/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2/RET0; all 13 indexed callers pass the full EAX bitmask onward. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u32 CM_THISCALL cm_fn_008cca10(u32,u32,u32);
extern u8 CM_CDECL cm_fn_00543540(u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

static u32 float_word(long double value)
{
    union { float f;u32 u; } word;
    word.f=(float)value;
    return word.u;
}
static float word_float(u32 value)
{
    union { float f;u32 u; } word;
    word.u=value;
    return word.f;
}

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_00546ab0(volatile u32 person_arg,volatile u32 club_arg)
{
    /* Native L=ESP after SUB0c: L0 is the mask until cached in EBP;
       L4..11 is reused m64 scratch. Incoming formals become DWORD/m32
       scratch; keep their stores and reloads after caching the person. */
    volatile u8 scratch[12];
    u32 l=(u32)scratch,person,stats,a,b,c,mask=0,offset=0;
    s32 value,total,high;
    s8 remembered,byte;
    long double live,x;
    volatile long long converted;
    a=club_arg;D(l)=0;
    if(!a)return 0;
    person=person_arg;if(!person)return 0;
    stats=D(person+0x61);
    if(B(stats+0xf)==20){mask=1;D(l)=mask;}
    byte=B(stats+0x11);
    if(byte==20) {
        if(B(stats+0x19)==byte){mask+=4;D(l)=mask;}
        if(byte==20 && (B(stats+0x17)==20 || B(stats+0x18)==20)){mask+=2;D(l)=mask;}
    }
    if(B(stats+0x12)==20 && B(stats+0x18)<15 && B(stats+0x17)<15){mask+=8;D(l)=mask;}
    if(B(stats+0x13)==20 && B(stats+0x18)<15 && B(stats+0x17)<15){mask+=0x10;D(l)=mask;}
    if(B(stats+0x14)==20){mask+=0x20;D(l)=mask;}
    if(B(stats+0x15)==20){mask+=0x40;D(l)=mask;}
    a=cm_fn_0090cfc0(3);
    b=D(person);b=b*79u;c=D(0x00ae2d0c);b=U(b+c+0xb)&15u;
    if((s32)b>(s32)a) {
        c=D(0x00de1f54);
        if(c) {
            a=cm_fn_008cca10(c,person,0);
            if(!a){mask+=0x1000000;D(l)=mask;}
        }
    }
    stats=D(person+0x61);
    if(stats) {
        total=B(stats+0x19);total+=B(stats+0x18);total+=B(stats+0x17);
        total+=B(stats+0x16);total+=B(stats+0x15);total+=B(stats+0x14);
        total+=B(stats+0x13);total+=B(stats+0x12);
        value=B(stats+0x11);total+=B(stats+0x10);total+=value;
        if(total>110){mask+=0x80;D(l)=mask;}
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(3);value=B(stats+0x3c);a+=15;
    if(value>(s32)a){mask+=0x100;D(l)=mask;}
    c=D(0x00b74350);live=cm_fn_007c1e30(c,stats,2,22,6.5f);
    if(live>(long double)F(0x00969570)) {
        a=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,a,2,13,6.5f);
        converted=(long long)live; /* ADR0006: __ftol CALL00546c3b, site1. */
        if((s8)(u32)converted>10) {
            club_arg=(u32)(s32)(s8)(u32)converted;
            live=(long double)(s32)club_arg;live-=Q(0x00969568);
            converted=(long long)live; /* ADR0006: __ftol CALL00546c55, site2. */
            offset=(u32)converted;
        }
        a=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,a,2,5,6.5f);live*=F(0x00969560);
        a=D(person+0x61);c=D(0x00b74350);
        club_arg=float_word(live);
        live=cm_fn_007c1e30(c,a,2,1,6.5f);
        live=(long double)word_float(club_arg)/live;
        converted=(long long)live; /* ADR0006: __ftol CALL00546c9a, site3. */
        offset+=(u32)converted;
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(50);value=B(stats+0x37);
    b=(u32)value+offset;
    if((s32)b>(s32)a){mask+=0x200;D(l)=mask;}
    c=D(0x00b74350);live=cm_fn_007c1e30(c,stats,2,14,6.5f);
    club_arg=float_word(live); /* Native FSTm32 retains extended ST0. */
    if(live>(long double)Q(0x00968f10)){mask+=0x800000;D(l)=mask;}
    a=cm_fn_0090cfc0(50);stats=D(person+0x61);value=B(stats+0x39);
    if(value>(s32)a) {
        a=cm_fn_0090cfc0(50);stats=D(person+0x61);value=B(stats+0x3e);
        if(value>(s32)a) {
            live=(long double)word_float(club_arg);
            if(live>(long double)Q(0x009689a0)) {
                a=cm_fn_0090cfc0(10);live=(long double)word_float(club_arg);b=a+5;
                converted=(long long)live; /* ADR0006: __ftol CALL00546d45, site4. */
                if((s32)(s8)(u32)converted>(s32)b)goto add_400;
            }
            a=D(person+0x61);a=cm_fn_00543540(person,a,23);remembered=(s8)a;
            a=cm_fn_0090cfc0(5);a+=15;
            if((s32)remembered>(s32)a) {
                a=D(person+0x61);c=D(0x00b74350);
                live=cm_fn_007c1e30(c,a,2,7,6.5f);live*=Q(0x00968f50);
                a=D(person+0x61);c=D(0x00b74350);Q(l+4)=(double)live;
                live=cm_fn_007c1e30(c,a,2,22,6.5f);
                if(live>(long double)Q(l+4)) {
add_400:
                    mask+=0x400;D(l)=mask;
                }
            }
        }
    }
    a=cm_fn_0090cfc0(5);stats=D(person+0x61);a+=15;value=B(stats+0x2e);
    if(value>(s32)a){mask+=0x800;D(l)=mask;}
    a=cm_fn_0090cfc0(5);stats=D(person+0x61);a+=15;value=B(stats+0x40);
    if(value>(s32)a){mask+=0x1000;D(l)=mask;}
    stats=D(person+0x61);a=cm_fn_0090cfc0(5);value=B(stats+0x44);a+=15;
    if(value>(s32)a){mask+=0x2000;D(l)=mask;}
    a=cm_fn_00543540(person,stats,4);remembered=(s8)a;a=cm_fn_0090cfc0(5);a+=15;
    if((s32)remembered>(s32)a){mask+=0x4000;D(l)=mask;}
    stats=D(person+0x61);a=cm_fn_00543540(person,stats,13);remembered=(s8)a;a=cm_fn_0090cfc0(5);a+=15;
    if((s32)remembered>(s32)a){mask+=0x20000;D(l)=mask;}
    stats=D(person+0x61);a=cm_fn_00543540(person,stats,20);remembered=(s8)a;a=cm_fn_0090cfc0(5);a+=15;
    if((s32)remembered>(s32)a){mask+=0x100000;D(l)=mask;}
    stats=D(person+0x61);a=cm_fn_00543540(person,stats,21);remembered=(s8)a;a=cm_fn_0090cfc0(5);a+=15;
    if((s32)remembered>(s32)a){mask+=0x200000;D(l)=mask;}
    stats=D(person+0x61);a=cm_fn_00543540(person,stats,1);remembered=(s8)a;a=cm_fn_0090cfc0(5);
    mask=D(l);a+=15;
    if((s32)remembered>(s32)a)mask+=0x400000;
    stats=D(person+0x61);a=cm_fn_0090cfc0(3);value=B(stats+0x21);a+=16;
    if(value>(s32)a)mask+=0x8000;
    a=cm_fn_00543540(person,stats,11);remembered=(s8)a;a=cm_fn_0090cfc0(4);a+=14;
    if((s32)remembered>(s32)a) {
        a=D(person+0x61);c=D(0x00b74350);live=cm_fn_007c1e30(c,a,2,15,6.5f);
        Q(l+4)=(double)live;a=cm_fn_0090cfc0(20);club_arg=a;
        live=(long double)(s32)club_arg;live*=Q(0x00967880);live+=Q(0x00968eb0);
        if(!(live>=(long double)Q(l+4)))mask+=0x10000; /* Native C0 includes unordered. */
    }
    stats=D(person+0x61);c=D(0x00b74350);live=cm_fn_007c1e30(c,stats,2,22,6.5f);
    value=W(stats+0xb);high=(s32)(((long long)value*0x66666667LL)>>32);
    high>>=2;high+=(u32)high>>31;club_arg=1400u-(u32)high;
    x=(long double)(s32)club_arg;
    if(!(x>=live)) {
        c=D(0x00b74350);live=cm_fn_007c1e30(c,stats,6,2,6.5f);
        a=D(person+0x61);c=D(0x00b74350);F(l)=(float)live;
        live=cm_fn_007c1e30(c,a,6,1,6.5f);person_arg=float_word(live);
        a=cm_fn_0090cfc0(3);club_arg=a;
        live=(long double)(s32)club_arg;live*=Q(0x00967880);live+=Q(0x00968968);
        live*=(long double)word_float(person_arg);
        if(!(live>=(long double)F(l)))mask+=0x40000;
    }
    a=D(person+0x61);c=D(0x00b74350);live=cm_fn_007c1e30(c,a,2,21,6.5f);
    if(!(live>=(long double)Q(0x00968930))) {
        a=D(person+0x61);c=D(0x00b74350);live=cm_fn_007c1e30(c,a,2,16,6.5f);
        if(live>(long double)Q(0x00968ff8)) {
            a=D(person+0x61);c=D(0x00b74350);live=cm_fn_007c1e30(c,a,2,2,6.5f);live*=Q(0x00968930);
            a=D(person+0x61);c=D(0x00b74350);Q(l+4)=(double)live;
            live=cm_fn_007c1e30(c,a,2,1,6.5f);
            if(live>(long double)Q(l+4))mask+=0x80000;
        }
    }
    return mask;
}
