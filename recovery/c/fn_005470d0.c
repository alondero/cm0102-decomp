#include "cmre.h"
/* Preferred-image VA005470d0/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2/RET0; both indexed callers consume full EAX bitmask. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u32 CM_THISCALL cm_fn_0062c3d0(u32,u32);
extern u8 CM_THISCALL cm_fn_008972c0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00896660(u32,u32,u32);
extern unsigned long long CM_STDCALL __attribute__((regparm(3)))
    cm_fn_005b2ae0(u32,u32,u32,u32,u32);
extern u32 CM_CDECL __attribute__((regparm(3)))
    cm_fn_006e6340(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u8 CM_CDECL cm_fn_00543540(u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

/* Materialize the original m32 rounding without aliasing a DWORD formal
   through a float pointer. The resulting DWORD store has the same bytes. */
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
u32 CM_CDECL cm_fn_005470d0(volatile u32 person_arg,volatile u32 club_arg)
{
    /* Native L=ESP after SUB0c. L3 caches BYTE11; L4 holds the initial
       mask, then the late saved m64. Both incoming formals are rewritten
       as native BYTE/DWORD/float scratch after their pointers are cached. */
    volatile u8 scratch[12];
    u32 l=(u32)scratch,person,club,stats,a,b,c,mask=0,scale,offset;
    s32 total,value,high;
    s8 byte13,byte14,byte15,remembered;
    long double live,x;
    volatile long long converted;
    unsigned long long physical;
    a=club_arg;
    D(l+4)=0;
    if(!a)return 0;
    person=person_arg;
    if(!person)return 0;
    stats=D(person+0x61);
    a=(u32)(s32)B(stats+0x11);B(l+3)=(s8)a;
    if((s8)a==20) {
        if(B(stats+0x19)==(s8)a){mask=0x20;D(l+4)=mask;}
        if((s8)a==20 && (B(stats+0x17)==20 || B(stats+0x18)==20)) {
            mask+=0x10;D(l+4)=mask;
        }
    }
    a=(u32)(s32)B(stats+0x12);B((u32)&person_arg)=(s8)a;
    if((s8)a==20 && B(stats+0x18)<15 && B(stats+0x17)<15){mask+=0x40;D(l+4)=mask;}
    byte13=B(stats+0x13);
    if(byte13==20 && B(stats+0x18)<15 && B(stats+0x17)<15){mask+=0x80;D(l+4)=mask;}
    byte14=B(stats+0x14);
    if(byte14==20){mask+=0x100;D(l+4)=mask;}
    byte15=B(stats+0x15);
    if(byte15==20){mask+=0x200;D(l+4)=mask;}
    if(stats) {
        total=B(stats+0x19);total+=B(stats+0x18);total+=B(stats+0x17);
        total+=B(stats+0x16);total+=B(stats+0x10);
        mask=D(l+4);
        total+=byte15;total+=byte14;
        value=B((u32)&person_arg);
        total+=byte13;total+=value;total+=B(l+3);
        if(total>110)mask+=0x800000;
    }
    a=cm_fn_0090cfc0(4);value=B(person+0x5d);a+=5;
    if(value<(s32)a) {
        stats=D(person+0x61);
        if(B(stats+0x1c)>15 || B(stats+0x25)<6) {
            c=D(0x00b74350);
            live=cm_fn_007c1e30(c,stats,2,11,6.5f);
            live*=F(0x00969584);
            a=D(person+0x61);c=D(0x00b74350);
            person_arg=float_word(live);
            live=cm_fn_007c1e30(c,a,2,10,6.5f);
            live+=(long double)word_float(person_arg);
            a=D(person+0x61);c=D(0x00b74350);
            person_arg=float_word(live);
            live=cm_fn_007c1e30(c,a,2,1,6.5f);
            live*=F(0x00968f18);
            x=(long double)word_float(person_arg);
            if(!(live>=x))++mask; /* native C0 includes unordered */
        }
    }
    a=D(person);c=D(0x00b63d28);
    b=cm_fn_0062c3d0(c,a);scale=4;
    a=cm_fn_0090cfc0(1000);a+=500;
    if((s32)(s16)b>(s32)a)scale=8;
    a=cm_fn_0090cfc0(12);stats=D(person+0x61);value=B(stats+0x34);
    if(value<(s32)a)scale+=3;
    stats=D(person+0x61);a=cm_fn_0090cfc0(scale);value=B(stats+0x3c);a+=8;
    if(value<(s32)a && B(stats+0xf)<20)mask+=2;
    club=club_arg;c=D(0x00de1d08);
    a=cm_fn_008972c0(c,club,person,0);
    B((u32)&club_arg)=(s8)a;
    if((s8)a>-1 && (s8)a<11) {
        a=club_arg;c=D(0x00de1d08);
        b=cm_fn_00896660(c,club,0);
        /* Two earlier native pushes belong to THIS2/RET8 5b2ae0.
           Its valid arg2=0 path ignores incoming EDX, retains ECX, and
           writes only AL/AX of EAX. Carry both actual result registers. */
        physical=cm_fn_005b2ae0(b,0,b,a,0);
        a=(u32)physical;c=(u32)(physical>>32);
        /* Native 6e6340 receives these physical EAX/EDX/ECX values plus
           six DWORDs. Arg5 is rawFFFFFFFF, not a numeric float -1.0. */
        a=cm_fn_006e6340(a,c,b,person,a,0x800,0,0xffffffffu,0);
        b=a;a=cm_fn_0090cfc0(40);a+=150;
        if((s32)b<(s32)a)mask+=4;
    }
    a=D(person+0x61);c=D(0x00b74350);offset=0;
    live=cm_fn_007c1e30(c,a,2,22,6.5f);
    if(live>(long double)F(0x00969570)) {
        a=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,a,2,13,6.5f);
        converted=(long long)live; /* ADR0006: __ftol CALL00547371, site1. */
        if((s8)(u32)converted>10)offset=70u-(u32)(s32)(s8)(u32)converted;
        a=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,a,2,5,6.5f);
        live*=F(0x00969580);
        a=D(person+0x61);c=D(0x00b74350);
        club_arg=float_word(live);
        live=cm_fn_007c1e30(c,a,2,1,6.5f);
        live=(long double)word_float(club_arg)/live;
        converted=(long long)live; /* ADR0006: __ftol CALL005473c2, site2. */
        offset+=(u32)converted;
    }
    a=cm_fn_0090cfc0(0xffffffce);stats=D(person+0x61);a+=offset;value=B(stats+0x37);
    if(value<(s32)a)mask+=0x400;
    a=cm_fn_0090cfc0(0xffffffce);stats=D(person+0x61);value=B(stats+0x39);
    if(value<(s32)a) {
        a=cm_fn_0090cfc0(0xffffffce);value=B(stats+0x3e);
        if(value<(s32)a) {
            c=D(0x00b74350);
            live=cm_fn_007c1e30(c,stats,2,14,6.5f);
            club_arg=float_word(live); /* native FSTm32 does not pop */
            if(live>(long double)Q(0x009689a0)) {
                a=cm_fn_0090cfc0(10);
                live=(long double)word_float(club_arg);
                converted=(long long)live; /* ADR0006: __ftol CALL00547445, site3. */
                if((s32)(s8)(u32)converted<(s32)a)mask+=0x800;
            }
        }
    }
    c=D(0x00b63d28);
    if(c) {
        a=D(person);b=cm_fn_0062c3d0(c,a);
        a=cm_fn_0090cfc0(500);a+=750;
        if((s32)(s16)b>(s32)a)mask+=8;
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(15);value=B(stats+0x2e);
    if(value<(s32)a)mask+=0x1000;
    if(B(stats+0xf)<20) {
        stats=D(person+0x61);a=cm_fn_0090cfc0(10);value=B(stats+0x40);
        if(value<(s32)a)mask+=0x2000;
        if(B(stats+0xf)<20) {
            a=cm_fn_0090cfc0(12);stats=D(person+0x61);value=B(stats+0x44);
            if(value<(s32)a)mask+=0x4000;
        }
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(0xffffffce);value=B(stats+0x1e);
    if(value<(s32)a)mask+=0x8000;
    if(B(stats+0xf)<20) {
        stats=D(person+0x61);a=cm_fn_0090cfc0(20);
        value=B(stats+0x25);total=B(stats+0x1c);total+=value;a+=25;
        if(total>(s32)a)mask+=0x10000;
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(10);b=12u-a;value=B(stats+0x21);
    if(value<(s32)b)mask+=0x20000;
    if(B(stats+0x11)<15) {
        a=cm_fn_00543540(person,stats,11);remembered=(s8)a;
        a=cm_fn_0090cfc0(4);a+=14;
        if((s32)remembered>(s32)a) {
            a=D(person+0x61);c=D(0x00b74350);
            live=cm_fn_007c1e30(c,a,2,15,6.5f);
            club_arg=float_word(live);
            if(live>(long double)Q(0x009689a0)) {
                stats=D(person+0x61);a=cm_fn_00543540(person,stats,11);
                live=(long double)word_float(club_arg);
                club_arg=(u32)(s32)(s8)a;
                x=(long double)(s32)club_arg;x*=Q(0x00969578);
                if(x>live)mask+=0x40000;
            }
        }
    }
    stats=D(person+0x61);a=cm_fn_0090cfc0(5);value=B(stats+0x28);a+=13;
    if(value>(s32)a)mask+=0x80000;
    c=D(0x00b74350);
    live=cm_fn_007c1e30(c,stats,2,22,6.5f);
    if(live>(long double)F(0x00969570)) {
        stats=D(person+0x61);a=cm_fn_00543540(person,stats,12);remembered=(s8)a;
        stats=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,stats,2,16,6.5f);
        club_arg=(u32)(s32)remembered;
        live+=(long double)(s32)club_arg;live-=F(0x00968a78);
        person_arg=float_word(live);
        a=cm_fn_0090cfc0(20);a+=20;club_arg=a;
        x=(long double)(s32)club_arg;
        if(x>(long double)word_float(person_arg)) {
            a=D(person+0x61);c=D(0x00b74350);
            live=cm_fn_007c1e30(c,a,6,2,6.5f);
            a=D(person+0x61);c=D(0x00b74350);
            Q(l+4)=(double)live; /* native late FSTPm64 overlaps old mask */
            live=cm_fn_007c1e30(c,a,6,1,6.5f);live*=Q(0x00969510);
            if(live>(long double)Q(l+4))mask+=0x100000;
        }
    }
    a=D(person+0x61);c=D(0x00b74350);
    live=cm_fn_007c1e30(c,a,2,21,6.5f);
    if(!(live>=(long double)Q(0x00968930))) {
        a=D(person+0x61);c=D(0x00b74350);
        live=cm_fn_007c1e30(c,a,2,16,6.5f);
        if(live>(long double)Q(0x00968ff8))mask+=0x200000;
    }
    stats=D(person+0x61);a=cm_fn_00543540(person,stats,20);
    if((s8)a<7)mask+=0x400000;
    else {
        stats=D(person+0x61);a=cm_fn_00543540(person,stats,20);
        stats=D(person+0x61);value=W(stats+5);
        high=(s32)(((long long)value*(-2004318071LL))>>32);
        high=(s32)((u32)high+(u32)value);high>>=3;high+=(u32)high>>31;
        if((s32)(s8)a<high)mask+=0x400000;
    }
    return mask;
}
