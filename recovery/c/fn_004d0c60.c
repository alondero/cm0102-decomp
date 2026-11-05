#include "cmre.h"
/* Preferred-image VA004d0c60/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX is forwarded unchanged to THIS2 004cf5f0: root is THIS5/RET20.
   Fifth stack DWORD is physically popped but never read. */
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern s16 CM_CDECL cm_fn_005407e0(u32,u32,u32);
extern u16 CM_CDECL cm_fn_006e86d0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00867b50(u32);
extern u32 CM_STDCALL cm_fn_008d0a70(u32);
extern u32 CM_STDCALL cm_fn_008d0780(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define H(a) CM_GLOBAL(volatile u16,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_004d0c60(u32 self,u32 person,u32 club,u32 mode,u32 rating,u32 unused)
{
    /* L is native aligned ESP after SUB34; all related scalar/scratch slots
       share this allocation. No native unwritten bytes are initialized. */
    volatile u8 scratch[0x34];
    u32 l=(u32)scratch,a,b,c,derived,stats,contract,result;
    s32 value,other,age,baseline,delta,high;
    s16 si;
    s8 byte;
    long double live,x,y,ratio;
    volatile long long converted;
    (void)unused;
    a=0;
    if(person) {
        a=D(person);b=D(0x00ae2364);
        if((s32)a<(s32)b) {
            b=a*79u;a=D(0x00ae2d0c);a=D(b+a);
            if((s32)a<0)a=0;
            else {b=D(0x00ae1050);a=a*80u+D(b);}
        } else a=0;
    }
    D(0x00ae1080)=a;
    if(a){a=D(a+4);b=D(0x00ae23b4);derived=b+a*581u;}
    else derived=0;
    D(l+0x14)=derived;
    contract=cm_fn_004cf5f0(self,person,derived);
    stats=D(person+0x61);D(l+0x10)=contract;
    if(!stats) {
        if(derived){a=D(0x00de1f54);(void)a;return cm_fn_008d0780(person,derived,1);}
        return 0;
    }
    if(B(stats+0x15)>=15){D(l+0x2c)=0;D(l+0x30)=0x4168cba8;B(l+6)=28;}
    else if(B(stats+0x14)>=15){D(l+0x2c)=0;D(l+0x30)=0x4164fb18;B(l+6)=29;}
    else if(B(stats+0x13)>=15){D(l+0x2c)=0;D(l+0x30)=0x416312d0;B(l+6)=29;}
    else if(B(stats+0x12)>=15){D(l+0x2c)=0;D(l+0x30)=0x41612a88;B(l+6)=29;}
    else if(B(stats+0xf)==20){D(l+0x2c)=0;D(l+0x30)=0x4156e360;B(l+6)=33;}
    else {
        if(B(stats+0x10)<15)byte=B(stats+0x11);
        D(l+0x2c)=0;D(l+0x30)=0x415e8480;B(l+6)=29;
    }
    byte=B(person+0x18);baseline=B(l+6);B(l+7)=byte;
    D(l+0x20)=(u32)baseline;age=byte;delta=baseline-age;
    D(l+0x24)=(u32)age;D(l+0xc)=(u32)delta;
    live=(long double)(s32)D(l+0xc);
    if(live>(long double)Q(0x009688a0)){live*=F(0x00968f18);live=(long double)Q(0x00968f10)-live;}
    x=live;
    converted=(long long)live; /* ADR0006: __ftol CALL004d0dc9, site1; duplicate ST0 remains live. */
    a=(u32)converted;b=(u32)((s32)a>>31);a=(a^b)-b;D(l+8)=a;
    byte=B(stats+0xf);live=x*(long double)(s32)D(l+8);
    live*=F(0x00968f08);live*=Q(0x00968f00);live+=Q(0x00967890);
    if(byte<15) {
        value=B(stats+0x28);other=B(stats+0x27);value-=15;
        D(l+8)=(u32)value;x=(long double)(s32)D(l+8);D(l+8)=(u32)other;
        value=B(stats+0x43);x*=Q(0x00968ef8);other=B(stats+0x36);x+=Q(0x00967890);
        y=(long double)(s32)D(l+8);D(l+8)=(u32)value;other-=15;value=B(stats+0x2f);
        y*=Q(0x00968ef0);value-=15;y+=Q(0x00967890);y*=live;x*=y;
        y=(long double)(s32)D(l+8);y*=Q(0x00968e60);y+=Q(0x00967890);
        F(l+8)=(float)y;D(l+8)=(u32)other;x*=y;
        y=(long double)(s32)D(l+8);y*=Q(0x00968ef8);y+=Q(0x00967890);
        F(l+8)=(float)y;D(l+8)=(u32)value;x*=y;
        y=(long double)(s32)D(l+8);y*=Q(0x00968ef8);y+=Q(0x00967890);F(l+8)=(float)y;
        live=x*y;
    }
    value=B(stats+0x2d);D(l+8)=(u32)(value*value);x=(long double)(s32)D(l+8);
    value=W(stats+9);x*=Q(0x00967888);D(l+0x18)=(u32)value;x*=Q(0x00968ee8);
    x=(long double)Q(0x00968ee0)-x;x*=live;F(l+8)=(float)x;
    live=(long double)(s32)D(l+0x18);live*=Q(0x00968ed8);live+=Q(0x00968ed0);
    if(!(live>=(long double)Q(0x009689c0)))live=(long double)Q(0x009689c0);
    x=(long double)F(l+8);
    if(!(x>=(long double)Q(0x00967898)))live=(long double)Q(0x00967898);
    else {x=(long double)F(l+8);if(live>=x)live=(long double)F(l+8);}
    F(l+8)=(float)live;
    if(!club)a=(u32)(s32)W(stats+0xb);
    else {
        c=D(club+0xcf);si=cm_fn_005407e0(person,c,0);value=si;a=rating;
        if((u16)a==0xffff){c=D(club+0xcf);a=cm_fn_006e86d0(c,person,club,1,(u32)value);}
        a*=50u;if((s16)a<0)a=0;
        b=(u32)value;if(b>=5000u)b=5000;
        a=(u32)(s32)(s16)a+(u32)value;D(l+0x18)=a;D(l+0x1c)=0;
        live=(long double)(long long)(unsigned long long)a;
        D(l+0x18)=b;D(l+0x1c)=0;x=(long double)(long long)(unsigned long long)b;
        x*=F(l+8);live+=x;live*=F(0x00968f08);
        converted=(long long)live; /* ADR0006: __ftol CALL004d0fb2, site2. */
        a=(u32)converted;if(a<=1)a=1;else if(a>12000)a=12000;
    }
    si=W(stats+0xd);
    if(si>5000) {
        b=D(l+0x20);c=D(l+0x24);b+=4;
        if((s32)c<(s32)b) {
            D(l+0x24)=(u32)(s32)si;b=(a>>1)+(u32)(s32)si-2500u;
            if(b>a)a=b;
            if(si>8000) {
                byte=B(l+7);
                if(byte<=B(l+6)) {
                    live=(long double)(s32)D(l+0x24);b=a>>1;live*=Q(0x00968ec8);
                    D(l+0x24)=b;D(l+0x28)=0;x=(long double)(long long)(unsigned long long)b;
                    D(l+0x24)=a;D(l+0x28)=0;live+=x;live-=Q(0x00968ec0);
                    x=(long double)(long long)(unsigned long long)a;Q(l+0x24)=(double)x;
                    if(!(live>(long double)Q(l+0x24)))live=(long double)Q(l+0x24);
                    converted=(long long)live; /* ADR0006: __ftol CALL004d105d, site3. */
                    a=(u32)converted;
                }
            }
        }
    }
    b=a*a;byte=B(person+0x22);D(l+0x24)=b;D(l+0x28)=0;
    live=(long double)(long long)(unsigned long long)b;live*=Q(l+0x2c);live*=Q(0x00968eb8);
    if(byte) {
        c=D(person+0x1a);
        if(c) {
            value=W(c+0x8e);a=(u32)byte&255u;value*=a;
            high=(s32)(((long long)value*0x38e38e39LL)>>32);high>>=2;high+=(u32)high>>31;
            value=(s16)high;other=(s32)D(l+0xc);delta=value<other?value:other;
            if(delta<0)value=0;else if(value>=other)value=other;
            U(l+7)=(u8)(U(l+7)+(u8)value);
        }
    }
    byte=(s8)(U(l+6)-U(l+7)+10u);
    if(byte<0)byte=0;else if(byte>=20)byte=19;
    value=B(0x009a7f30+(u32)(s32)byte);D(l+0x24)=(u32)value;
    x=(long double)(s32)D(l+0x24);x*=live;x*=Q(0x00967898);Q(l+0x2c)=(double)x;
    if(!club){a=cm_fn_0090cfc0(3);a++;D(l+0x24)=a;ratio=(long double)(s32)D(l+0x24);}
    else {
        c=D(club+0x57);
        if(c) {
            a=D(club+0x53);
            if(!a)ratio=(long double)Q(0x00967890);
            else {
                D(l+0x24)=(u32)(s32)B(a+0x7e);ratio=(long double)(s32)D(l+0x24);
                if(!(ratio>=(long double)Q(0x00967890)) || ratio>(long double)Q(0x00968930))ratio=(long double)Q(0x00968930);
                ratio+=Q(0x00967890);value=B(club+0x64);ratio*=Q(0x00968968);
                if(value==2)ratio+=ratio;else if(value==3)ratio*=Q(0x00968eb0);
                value=W(c+0x69);b=(u32)value*(u32)value;b*=25u;value=W(club+0x80);b+=(u32)value;
                D(l+0x24)=b;x=(long double)(s32)D(l+0x24);ratio*=Q(0x00968908);ratio=x/ratio;
            }
        } else {
            c=D(club+0x53);
            if(!c)ratio=(long double)Q(0x00967890);
            else {
                D(l+0x24)=(u32)(s32)B(c+0x7e);ratio=(long double)(s32)D(l+0x24);
                if(!(ratio>=(long double)Q(0x00967890)) || ratio>(long double)Q(0x00968930))ratio=(long double)Q(0x00968930);
                value=B(club+0x64);if(value==2)ratio+=ratio;else if(value==3)ratio*=Q(0x00968eb0);
                value=B(c+0x85);b=(u32)(value*value);b*=25;value=W(club+0x80);b+=(u32)value;
                D(l+0x24)=b;x=(long double)(s32)D(l+0x24);ratio*=Q(0x00968908);ratio=x/ratio;
            }
        }
    }
    live=(long double)Q(0x00968ea8)-ratio;live*=live;live*=Q(0x00968918);live*=Q(0x00968a98);live=(long double)Q(0x00967890)-live;
    if(!(live>=(long double)Q(0x00967880)))live=(long double)Q(0x00967880);
    else if(live>(long double)Q(0x009689c0))live=(long double)Q(0x009689c0);
    live*=Q(l+0x2c);Q(l+0x2c)=(double)live;
    x=ratio*ratio;y=x;y*=Q(0x00968ea0);
    if(!(y>=(long double)Q(l+0x2c))) {
        y=(long double)Q(l+0x2c);y*=Q(0x009689e0);x*=Q(0x00968e98);y+=x;Q(l+0x2c)=(double)y;
    }
    x=ratio;x*=Q(0x009688e8);y=(long double)Q(l+0x2c);
    if(!(y>=x))Q(l+0x2c)=(double)x;
    if((u8)mode) {
        if(club) {
            club=D(club+0x53);
            if(club) {
                a=D(club);b=D(0x009cf434);
                if(a==b) {
                    x=(long double)Q(l+0x2c);y=(long double)Q(0x00968e90);live=(long double)Q(l+0x2c);
                    if(!(x>=y)) {
                        live*=Q(0x00968e88);live*=Q(0x00968e48);live+=Q(0x00968e80);live*=Q(l+0x2c);
                        if((long double)Q(0x009688e8)>live){D(l+0x2c)=0;D(l+0x30)=0x40b38800;goto mode_return;}
                    } else live*=Q(0x009689d8);
                    Q(l+0x2c)=(double)live;
                }
            }
        }
mode_return:
        contract=D(l+0x10);
        if(contract && cm_fn_00867b50(contract)) {
            live=(long double)(s32)D(contract+0x21);x=(long double)Q(l+0x2c);
            if(!(x>=live))live=(long double)Q(l+0x2c);
            converted=(long long)live; /* ADR0006: __ftol CALL004d1382, site4. */
            return (u32)converted;
        }
        live=(long double)Q(l+0x2c);goto final_cast;
    }
    contract=D(l+0x10);
    if(!contract){live=(long double)Q(0x009689a0);Q(l+0x2c)=(double)live;goto country_adjust;}
    a=D(0x00de1f54);(void)a;a=cm_fn_008d0a70(person);
    if(!a)live=(long double)Q(l+0x2c);
    else {
        a=H(contract+0x2f);b=H(0x00ae2c92);a-=b;value=W(contract+0x2d);
        a=a*365u+(u32)value;b=(u32)(s32)W(0x00ae2c90);a-=b;D(l+8)=a;
        if((s32)a<1)return 0;
        if(club==D(l+0x14)) {
            byte=B(contract+0x3d);
            if(byte>0) {
                b=U(contract+0x35)&0x3fu;
                if(b!=2){live=(long double)Q(l+0x2c);live*=Q(0x00968968);goto duration;}
                a+=(u32)(s32)byte*7u+200u;D(l+8)=a;
            }
        }
        live=(long double)Q(l+0x2c);
duration:
        x=(long double)(s32)D(l+8);x*=Q(0x00968e78);
        if((s32)a<350){live*=x;Q(l+0x2c)=(double)live;}
        else {
            x=(long double)Q(0x00968e18)-x;
            if(!(x>=(long double)Q(0x00967890)))x=(long double)F(0x00968e70);
            if(x>(long double)Q(0x00968950))x=(long double)F(0x009689a8);
            live/=x;Q(l+0x2c)=(double)live;
        }
    }
    x=(long double)Q(0x009689a0);
    if(live<x || live>x) { /* Native AH40 treats equality and unordered alike. */
        if(!club)goto final_contract;
        a=D(club+0x53);
        if(a) {
            a=D(a);b=D(0x009cf434);
            if(a==b) {
                if(!(live>=(long double)Q(0x00968e90))) {
                    x=live;x*=Q(0x00968e88);x*=Q(0x00968e48);x+=Q(0x00968e80);live*=x;
                    if((long double)Q(0x009688e8)>live)live=(long double)Q(0x009688e8);
                } else live*=Q(0x009689d8);
                Q(l+0x2c)=(double)live;goto final_contract;
            }
        }
    }
country_adjust:
    if(club) {
        club=D(club+0x53);
        if(club) {
            a=D(club);b=D(0x009cf300);
            if(a==b){live*=Q(0x009689b0);Q(l+0x2c)=(double)live;}
            else {
                a=D(club);b=D(0x009cf484);
                if(a==b) {
                    live*=Q(0x009689e0);x=(long double)Q(0x00968e68);Q(l+0x2c)=(double)live;
                    if(live>x) {
                        live-=Q(0x00968e68);live*=Q(0x00968968);live+=Q(0x00968e68);
                        converted=(long long)live; /* ADR0006: __ftol CALL004d1555, site5. */
                        D(l+0x24)=(u32)converted;live=(long double)(s32)D(l+0x24);Q(l+0x2c)=(double)live;
                    }
                    if(live>(long double)Q(0x009688e0)) {
                        live-=Q(0x009688e0);live*=Q(0x00968968);live+=Q(0x009688e0);
                        converted=(long long)live; /* ADR0006: __ftol CALL004d1585, site6. */
                        D(l+0x24)=(u32)converted;live=(long double)(s32)D(l+0x24);Q(l+0x2c)=(double)live;
                    }
                }
            }
        }
    }
final_contract:
    if(contract) {
        if(cm_fn_00867b50(contract)) {
            live=(long double)(s32)D(contract+0x21);x=(long double)Q(l+0x2c);
            if(x>=live)goto final_cast;
        }
        live=(long double)Q(l+0x2c);
    }
final_cast:
    converted=(long long)live; /* ADR0006: __ftol CALL004d15bd, site7. */
    result=(u32)converted;return result;
}
