/* Preferred-image VA 00696570, CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Sole native caller 69791b overwrites full EAX at 697924.
   Original callees remain boundaries. ADR0006 conversions are named below. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00547790(u32,u32);
extern u32 CM_CDECL cm_fn_005477d0(u32,u32);
extern u32 CM_CDECL cm_fn_00547810(u32,u32);
extern u32 CM_CDECL cm_fn_00547840(u32,u32);

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_00696570(u32 self,u32 event,u32 out_first,u32 out_second)
{
    u32 id, person, club, other, result, active, chosen, destination;
    s32 first, cost, second, square;
    long double x, ratio;
    if (B(event+0x1fu)==1) return;
    W(out_second)=0;
    W(out_first)=0;
    id=D(event+0xcu);
    person=D(0xae23bcu)+id*110u;
    if (D(person+0x61u)==0) return;
    id=D(event+8u);
    club=D(0xae23b4u)+id*581u;
    active=cm_fn_00600a50(club,0,1,0,0);
    other=D(person+0x39u);
    if (active!=0) { chosen=other!=0?other:club; result=1; }
    else if (other!=0) { chosen=other; result=1; }
    else { chosen=club; result=0; }
    first=(s32)cm_fn_004d0c60(D(0xae1050u),person,chosen,0,0xffffffffu,result);
    if (first==0) first=1;
    cost=(s32)cm_fn_008e7a70(event,0,0,0,1);
    if (club!=0) {
        id=D(club);
        destination=D(self)+id*73u+0x38u;
        x=(long double)first;
        x=x+(long double)Q(destination);
        Q(destination)=(double)x;
        x=(long double)(s32)(s16)W(club+0x80u);
        x=x*(long double)Q(0x968918u);
        /* Native 69667d: duplicate extended x, multiply, then __ftol. */
        square=(s32)(u32)(long long)(x*x);
        if (first==0) ratio=(long double)CM_GLOBAL(volatile float,0x968f18u);
        else ratio=(long double)cost/(long double)first;
        if (cost<=square) B(out_first)|=4;
        else if (first>250000) {
            /* Native C0 tests include unordered values. */
            if (!(ratio>=(long double)Q(0x968918u))) B(out_first)|=0x10;
            else if (!(ratio>=(long double)Q(0x968968u))) B(out_first)|=8;
            else if (!(ratio>=(long double)Q(0x968950u))) B(out_first)|=4;
            else if (!(ratio>=(long double)Q(0x969000u))) B(out_first)|=2;
            else B(out_first)|=1;
        } else {
            if (!(ratio>=(long double)Q(0x968fd0u))) B(out_first)|=8;
            else if (!(ratio>=(long double)Q(0x968f10u))) B(out_first)|=4;
            else B(out_first)|=2;
        }
        if (cm_fn_00547790(person,club)!=0) B(out_first)|=0x40;
        else if (cm_fn_005477d0(person,club)!=0) B(out_first+1u)|=1;
        other=D(club+0xbfu);
        if (cm_fn_00547810(person,other)!=0) B(out_first)|=0x40;
        else {
            other=D(club+0xbfu);
            if (cm_fn_00547840(person,other)!=0) B(out_first+1u)|=1;
        }
    }
    other=D(person+0x39u);
    if (other==0) return;
    active=cm_fn_00600a50(other,0,1,0,0);
    chosen=D(person+0x39u);
    if (active!=0) { chosen=chosen!=0?chosen:other; result=1; }
    else if (chosen!=0) result=1;
    else { chosen=other; result=0; }
    second=(s32)cm_fn_004d0c60(D(0xae1050u),person,chosen,0,0xffffffffu,result);
    id=D(other);
    destination=D(self)+id*73u+0x38u;
    x=(long double)second;
    x=(long double)Q(destination)-x;
    Q(destination)=(double)x;
    x=(long double)(s32)(s16)W(other+0x80u);
    x=x*(long double)Q(0x968918u);
    /* Native 69680b: signed64 conversion, lowDWORD used by signed CMP. */
    square=(s32)(u32)(long long)(x*x);
    if (second==0) ratio=(long double)CM_GLOBAL(volatile float,0x968f18u);
    else ratio=(long double)cost/(long double)second;
    if (cost<=square) B(out_second)|=4;
    else if (second>250000) {
        if (!(ratio>=(long double)Q(0x9689e0u))) B(out_second)|=1;
        else if (!(ratio>=(long double)Q(0x968968u))) B(out_second)|=2;
        else if (!(ratio>=(long double)Q(0x968950u))) B(out_second)|=4;
        else if (!(ratio>=(long double)Q(0x968938u))) B(out_second)|=8;
        else B(out_second)|=0x10;
    } else {
        if (!(ratio>=(long double)Q(0x968fd0u))) B(out_second)|=2;
        else if (!(ratio>=(long double)Q(0x968f10u))) B(out_second)|=4;
        else B(out_second)|=8;
    }
    if (cm_fn_00547790(person,other)!=0) { B(out_second)|=0x20; return; }
    if (cm_fn_005477d0(person,other)!=0) { B(out_second)|=0x80; return; }
    chosen=D(other+0xbfu);
    if (cm_fn_00547810(person,chosen)!=0) { B(out_second)|=0x20; return; }
    chosen=D(other+0xbfu);
    if (cm_fn_00547840(person,chosen)!=0) B(out_second)|=0x80;
}
