/* WIP: native memory-read/fault-order audit and 44 conversion controls incomplete.
 * Preferred-image VA 006e6340, base 00400000; CM 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Six physical cdecl DWORD arguments, full EAX. Original callees stay explicit.
 */
#include "cmre.h"
#define U(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define S(a) ((s32)(s16)W(a))
#define C(a) ((s32)(s8)B(a))
#define D(a) CM_GLOBAL(volatile double,(u32)(a))
#define F(a) CM_GLOBAL(volatile float,(u32)(a))
#define L(a) (*(volatile u32 *)(frame+(a)-0x10))
#define LD(a) (*(volatile double *)(frame+(a)-0x10))
#define LF(a) (*(volatile float *)(frame+(a)-0x10))
#define LB(a) (*(volatile u8 *)(frame+(a)-0x10))
#define CNT ((s32)(s8)LB(0x11))
#define AGE ((s32)(s8)LB(0x12))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005407e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005444a0(u32,u32);
extern u32 CM_CDECL cm_fn_00547810(u32,u32);
extern u32 CM_CDECL cm_fn_00547840(u32,u32);
extern u32 CM_CDECL cm_fn_00547d30(u32,u32);
extern u32 CM_CDECL cm_fn_00547d60(u32,u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u16 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_THISCALL cm_fn_0062c3d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0062c590(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern u32 CM_CDECL cm_fn_006e86d0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007c1950(u32,u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u8 CM_THISCALL cm_fn_007c6830(u32,u32,u32,u8,u32);
extern u32 CM_THISCALL cm_fn_008c7260(u32,u32);
extern u32 CM_THISCALL cm_fn_008c7560(u32,u32,u32,u32);

/* Keep __ftol's FISTP qword and temporary chopping control word. */
__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_006e6340(u32 person,u32 position,u32 flags,u32 club,s32 threshold,u32 optional)
{
    u8 frame[0x50];
    u32 player, selected, a,b,c,d, rating, sum, pos, count, object;
    s32 i,j,k,n, v, w, z;
    register long double x;
    long double y;
    s8 byte_value;
    L(0x14)=0; L(0x20)=0; LB(0x11)=0; L(0x18)=0;
    L(0x2c)=0xffffffff; LB(0x13)=0; L(0x3c)=10000; L(0x38)=100;
    if (!person) return (u32)-10000;
    player=U(person+0x61);
    if (!player) return (u32)-10000;
    L(0x44)=flags&0x800;
    if (!L(0x44) || (flags&0x210000)) {
        a=cm_fn_0062c590(U(0xb63d28),person,0);
        L(0x3c)=a; L(0x38)=(s32)(s16)a/100;
    }
    LB(0x12)=B(person+0x18);
    a=U(0xae1050);
    if (a) L(0x34)=cm_fn_004cf5f0(a,person,club);
    else L(0x34)=0;
    if (threshold>-1) LB(0x13)=1;
    selected=optional;
    if (selected) L(0x1c)=U(selected+0x69);
    else if (club && (selected=U(club+0xcf))!=0) L(0x1c)=U(selected+0x69);
    else { L(0x1c)=0; selected=0; }
    L(0x28)=selected;
    L(0x24)=cm_fn_005407e0(person,selected,0);
    a=cm_fn_005b7d20(player); L(0x40)=a;
    pos=0;
    if (!(u16)a) {pos=(u32)-100; L(0x20)=pos;}
    if (flags&0x400) pos+=100;
    else if (position&1) {i=C(player+0xf); pos+=(u32)(i*i-300);}
    else if (a&1) pos-=200;
    else if (position&2) pos+=U(0xa15d50+4*(u32)C(player+0x10));
    else if (position&4) pos+=U(0xa15d50+4*(u32)C(player+0x11));
    else if ((u16)position==8) {
        i=C(player+0x16); j=C(player+0x11); if(j>i)i=j;
        j=C(player+0x12); if(j>i)i=j; j=C(player+0x13); if(j>i)i=j;
        pos+=U(0xa15d50+4*(u32)i);
    } else if (position&8) {
        i=C(player+((position&0x880)?0x16:0x12)); pos+=U(0xa15d50+4*(u32)i);
    } else if (position&0x10) {
        if (position&0x880) {i=C(player+0x14); j=C(player+0x13); if(j>=i)i=j;}
        else if ((u16)position==0x10) {
            i=C(player+0x14); j=C(player+0x12); if(j>=i)i=j;
            j=C(player+0x13); if(j>=i)i=j;
        } else {
            s32 cached13;
            i=C(player+0x14)-5; cached13=C(player+0x13); k=i; if(cached13>=k)k=cached13;
            j=C(player+0x12)-5;
            if(k>j) {if(i<=cached13)i=cached13;} else i=j;
            i=(s8)i;
        }
        pos+=U(0xa15d50+4*(u32)i);
    } else if (position&0x20) {
        i=C(player+0x14);
        if (!(position&0x880)) {
            j=C(player+0x1a); L(0x20)=i+j;
            LD(0x58)=(long double)(s32)L(0x20)*D(0x968968);
            x=i; if(!(x>LD(0x58)))x=LD(0x58);
            i=(u32)(long long)x; /* __ftol 006e662e -> 00945640 */
        }
        pos+=U(0xa15d50+4*(u32)i);
    } else if(position&0x40) pos+=U(0xa15d50+4*(u32)C(player+0x15));
    else pos+=100;
    /* 006e665e: secondary position component, with the native priority. */
    if(flags&0x200) pos+=100;
    else if(position&0x1000) pos+=(u32)((C(player+0x1a)-10)*10);
    else if(position&0x80) pos+=(u32)((C(player+0x18)-10)*10);
    else if(position&0x100) {
        i=C(player+0x18)/2; j=C(player+0x19); if(i<=j)i=j;
        pos+=(u32)(C(player+0x30)+(i*5-60)*2);
    } else if(position&0x200) pos+=(u32)((C(player+0x19)-10)*10);
    else if(position&0x400) {
        i=C(player+0x17)/2; j=C(player+0x19); if(i<=j)i=j;
        pos+=(u32)(C(player+0x3b)+(i*5-60)*2);
    } else if(position&0x800) pos+=(u32)((C(player+0x17)-10)*10);
    else pos+=100;
    L(0x20)=pos;
    sum=0;
    if(flags&0x40) {
        LB(0x11)=1; sum=S(player+0xb)/50; L(0x18)=sum;
        a=U(person+0x39);
        if(club==a && optional) {
            a=U(optional+0x3e); b=U(optional+0x42); L(0x58)=a; L(0x5c)=b;
            c=W(person+0x40);
            if ((u16)(a>>16)<c || ((u16)(a>>16)==c && (s16)a<S(person+0x3e))) {
                a=cm_fn_0054c8e0(0xae2c90,(u32)(frame+0x40),365);
                b=U(a); d=U(a+4); c=W(person+0x40); L(0x58)=b; L(0x5c)=d;
                if((u16)(b>>16)<c || ((u16)(b>>16)==c && (s16)b<S(person+0x3e))) {
                    {
                        u32 person_year, global_year;
                        s32 person_day, global_day;
                        person_year=W(person+0x40);
                        global_year=W(0xae2c92);
                        person_day=S(person+0x3e);
                        global_day=S(0xae2c90);
                        a=(global_year-person_year)*365;
                        a-=(u32)person_day; a+=(u32)global_day;
                    }
                    L(0x48)=a;
                    x=(long double)(s32)a*D(0x96a350); x=D(0x968fa8)-x;
                    x*=(s32)L(0x18);
                    sum=(u32)(long long)x; /* __ftol 006e684c */
                    L(0x18)=sum;
                }
            }
        }
        if(LB(0x13) && (s32)(sum*200)<threshold)return (u32)-10000;
    }
    if(flags&0x10) {
        i=S(player+9); LB(0x11)++; L(0x48)=i;
        x=i; a=U(person+0x1a); i=S(a+0x8e); L(0x48)=i;
        y=(long double)i*D(0x96a5c0); y=D(0x96d640)-y; y*=D(0x968ff8); x/=y;
        a=(u32)(long long)x; /* __ftol 006e68c6 */
        sum+=a; L(0x18)=sum;
        if(LB(0x13) && (s32)(sum*200)/CNT<threshold)return (u32)-10000;
    }
    if(flags&0x20) {
        LB(0x11)++; sum+=(u32)(S(player+0xd)/50); L(0x18)=sum;
        if(LB(0x13) && (s32)(sum*200)/CNT<threshold)return (u32)-10000;
    }
    L(0x58)=flags&4;
    if((flags&4) && (s32)U(person)<(s32)U(0xae2364)) {
        x=cm_fn_007c1e30(U(0xb74350),player,5,0x12,6.5f); x*=D(0x968ea8);
        a=(u32)(long long)x; /* __ftol 006e696f */
        if(a){LB(0x11)++; sum+=a; L(0x18)=sum;}
    }
    b=L(0x24);
    if(flags&0x80) {
        if(CNT){b=(s32)(sum*50)/CNT; L(0x24)=b;}
        a=cm_fn_006e86d0(L(0x28),person,club,2,b);
        LB(0x11)++; L(0x2c)=a; sum+=(s32)(s16)a; L(0x18)=sum;
        if(LB(0x13) && (s32)(sum*200)/CNT<threshold)return (u32)-10000;
    }
    L(0x50)=flags&0x100;
    if(flags&0x100) {
        if(CNT) {
            a=cm_fn_004cf5a0(U(0xae1050),person); U(0xae1080)=a;
            if(a){a=U(a+4); a=U(0xae23b4)+a*0x245;}
            if(a==club) {
                a=(s32)(sum*25)/CNT; b=a;
                c=U(person+0x61); a=(a&0xffff0000)|(u16)(W(c+7)*25); b+=a;
            } else b=(s32)(sum*50)/CNT;
            L(0x24)=b;
        }
        a=cm_fn_006e86d0(L(0x28),person,club,1,b);
        if((s16)L(0x2c)>-1 && (s16)a<(s16)L(0x2c))a=L(0x2c);
        LB(0x11)++; sum+=(s32)(s16)a; L(0x18)=sum;
        if(LB(0x13) && (s32)(sum*200)/CNT<threshold)return (u32)-10000;
    }
    if((flags&0x40000) && (a=U(0xae2364), b=U(person), (s32)b<(s32)a)) {
        volatile float rounded40000;
        a=cm_fn_007c1950(U(0xb74350),player);
        x=(s8)a;
        a=U(person+0x39);
        x*=D(0x968918); rounded40000=(float)x;
        if(club==a && optional) {
            a=U(optional+0x3e); b=U(optional+0x42); L(0x48)=a; L(0x4c)=b; c=W(person+0x40);
            if((u16)(a>>16)<c || ((u16)(a>>16)==c && (s16)a<S(person+0x3e))) {
                a=cm_fn_0054c8e0(0xae2c90,(u32)(frame+0x1c),365);
                b=U(a); d=U(a+4); c=W(person+0x40); L(0x48)=b; L(0x4c)=d;
                if((u16)(b>>16)<c || ((u16)(b>>16)==c && (s16)b<S(person+0x3e))) {
                    {
                        u32 person_year, global_year;
                        s32 person_day, global_day;
                        person_year=W(person+0x40);
                        global_year=W(0xae2c92);
                        person_day=S(person+0x3e);
                        global_day=S(0xae2c90);
                        a=(global_year-person_year)*365-(u32)person_day+(u32)global_day;
                    }
                    L(0x48)=a; y=(D(0x969070)-(long double)(s32)a)*D(0x968eb0);
                    y+=(long double)(s32)a*rounded40000; y*=D(0x968e10);
                    rounded40000=(float)y;
                }
            }
        }
        x=rounded40000;
        if(x>D(0x968998)) {
            x*=D(0x96d638); x-=D(0x96d630); x*=CNT;
            a=(u32)(long long)x; /* __ftol 006e6c3a */
            sum+=(s32)(s8)a; L(0x18)=sum;
        } else if(x>D(0x9689a0)) {
            x*=D(0x96d628); x-=D(0x96d620); x*=CNT;
            a=(u32)(long long)x; /* __ftol 006e6c78 */
            sum+=(s32)(s8)a; L(0x18)=sum;
        }
    }
    if((flags&2) && (a=U(0xae2364), b=U(person), (s32)b<(s32)a)) {
        volatile float f;
        union {u32 u; float f;} bits;
        bits.u=0x40d9999a; f=bits.f;
        if(CNT && (s32)L(0x18)/CNT>150) {
            i=(s32)L(0x18)/CNT-150; x=(long double)i*F(0x96d61c)+F(0x96d618);
            if(x>D(0x96d610)){x=D(0x96d610);} f=(float)x;
        } else if((s16)L(0x24)>150) {
            i=(s16)L(0x24)-150; x=(long double)i*F(0x96d61c)+F(0x96d618);
            if(x>D(0x96d610)){x=D(0x96d610);} f=(float)x;
        }
        x=cm_fn_007c1e30(U(0xb74350),player,6,0x12,f);
        a=U(person+0x39);
        f=(float)x;
        if(club==a && optional) {
            a=U(optional+0x3e); b=U(optional+0x42); L(0x48)=a; L(0x4c)=b; c=W(person+0x40);
            if((u16)(a>>16)<c || ((u16)(a>>16)==c && (s16)a<S(person+0x3e))) {
                a=cm_fn_0054c8e0(0xae2c90,(u32)(frame+0x1c),365);
                b=U(a); d=U(a+4); c=W(person+0x40); L(0x48)=b; L(0x4c)=d;
                if((u16)(b>>16)<c || ((u16)(b>>16)==c && (s16)b<S(person+0x3e))) {
                    {
                        u32 person_year, global_year;
                        s32 person_day, global_day;
                        person_year=W(person+0x40);
                        global_year=W(0xae2c92);
                        person_day=S(person+0x3e);
                        global_day=S(0xae2c90);
                        a=(global_year-person_year)*365-(u32)person_day+(u32)global_day;
                    }
                    L(0x48)=a; b=U(person+0x61); i=C(b+0xf)/10+34;
                    x=(D(0x969070)-(long double)(s32)a)*D(0x968eb0);
                    x+=(long double)(s32)a*f; x*=D(0x968e10); f=(float)x;
                    if(AGE>i) {
                        object=U(0xb74350); a=object; x=D(a+0x18);
                        if(x!=D(0x9689a0) && x==x)x=(long double)D(a+0x10)/D(a+0x18);
                        else x=D(0x968998);
                        if((long double)f>x) {
                            y=D(a+0x18);
                            if(y!=D(0x9689a0) && y==y)y=(long double)D(a+0x10)/D(a+0x18);
                            else y=D(0x968998);
                            y+=f; y*=F(0x968f18); f=(float)y;
                        }
                        goto object_ready_006e6ec8;
                    }
                }
            }
        }
        object=U(0xb74350);
object_ready_006e6ec8:
        if(f>D(0x968f10)) {
            x=(long double)f*F(0x96af88); x-=F(0x96d608); x*=CNT;
            a=(u32)(long long)x; /* __ftol 006e6ef4 */ L(0x18)+=(s32)(s8)a;
        } else if(f>D(0x968998)) {
            x=(long double)f*D(0x96afc8); x-=D(0x96d510); x*=CNT;
            a=(u32)(long long)x; /* __ftol 006e6f30 */ L(0x18)+=(s32)(s8)a;
        } else if(f>D(0x9689a0)) {
            x=(long double)f*F(0x96af88); x-=F(0x96d608); x*=CNT;
            a=(u32)(long long)x; /* __ftol 006e6f66 */ L(0x18)+=(s32)(s8)a;
        }
        if(club) {
            u32 club_matches;
            a=U(person+0x39);
            club_matches=(club==a);
            if(!club_matches && a) {
                u32 club_nation, person_club_nation;
                club_nation=U(club+0x53);
                person_club_nation=U(a+0x53);
                club_matches=(club_nation==person_club_nation);
            }
            if(club_matches) {
                f=(float)((long double)f*D(0x9689d8));
                x=cm_fn_007c1e30(object,player,1,0x12,f);
                if(x>D(0x968998)) {
                    x*=D(0x96d628); x-=D(0x96d600); x*=CNT;
                    a=(u32)(long long)x; /* __ftol 006e6fe6 */ L(0x18)+=(s32)(s8)a;
                } else if(!(x>=(long double)f)) {
                    x*=F(0x96d5f8); x/=f; x-=F(0x96d5f8); x*=CNT;
                    a=(u32)(long long)x; /* __ftol 006e701a */ L(0x18)+=(s32)(s8)a;
                }
                bits.u=0x3fb66666;
                x=cm_fn_007c1e30(U(0xb74350),player,6,0x14,bits.f);
                x*=D(0x968e30); x-=D(0x96d5f0); x*=CNT;
                a=(u32)(long long)x; /* __ftol 006e705c */ L(0x18)+=a;
            }
        }
    }
    rating=L(0x14);
    if(CNT) {
        rating=(s32)L(0x18)/CNT; L(0x14)=rating;
        if((s32)rating>150) {
            x=(s32)(rating-150); x*=D(0x968940); x+=D(0x968f50);
            rating=(u32)(long long)x; /* __ftol 006e709d */ L(0x14)=rating;
        }
    }
    if(flags&8) {
        u32 person_limit, person_id;
        person_limit=U(0xae2364);
        person_id=U(person);
        if((s32)person_id<(s32)person_limit) {
            x=C(player+0x45); x*=D(0x968940); x-=D(0x968e08);
            a=(u32)(long long)x; /* __ftol 006e70da */ rating+=a; L(0x14)=rating;
        }
    }
    if(flags&0x4000) {
        i=(7000-S(club+0x80))/100; if(i<10)i=(i&~255)|10;
        a=U(person+0x61);
        if(B(a+0xf)==20) j=36;
        else if(B(a+0x15)==20) j=34;
        else { k=C(a+0x14); j=(k==20)?34:35; }
        if(AGE>j) {
            x=AGE-j; y=(s8)i; x/=y; x=D(0x967890)-x; x*=(s32)L(0x14);
            rating-=5; LD(0x48)=(s32)rating;
            if(x>=LD(0x48))x=LD(0x48);
            rating=(u32)(long long)x; /* __ftol 006e718a */
        }
        a=cm_fn_0062ed70(U(0xb63d28),person);
        if(a)rating-=S(a+0x2c);
        object=L(0x50);
        if(object) {
            j=(L(0x40)&1)?29:25;
            x=CNT; x*=D(0x968f80); x=D(0x968e38)-x;
            if(AGE>j && x>D(0x9689a0)) {
                y=(long double)(AGE-j)*x;
                a=(u32)(long long)y; /* __ftol 006e71fd */ rating-=a;
            }
        }
        if(!(position&1)) {
            i=C(player+0x19)/2; i+=C(player+0x18)/2; i+=C(player+0x17)/2;
            i+=C(player+0x16)/2; i+=C(player+0x15)/2; i+=C(player+0x14)/2;
            i+=C(player+0x13)/2; i+=C(player+0x12)/2; i+=C(player+0x11)/2; i+=C(player+0x10)/2;
            if(i>35){i-=20; rating+=(u32)(i*i/250);}
        }
        x=C(player+0x2f)-10; x*=D(0x96a670);
        a=(u32)(long long)x; /* __ftol 006e72b6 */ rating+=a; L(0x14)=rating;
        a=U(club+0x53);
        if(a) {
            b=U(person+0x1a); c=B(a+0x77);
            if(c!=B(b+0x77)) {
                byte_value=(s8)cm_fn_007c6830(U(0xb74350),person,club,(u8)c,1);
                if(object) {
                    i=C(person+0x56)/2;
                    j=(s8)(AGE-i+5);
                    x=byte_value;
                    {
                        long double age_product;
                        age_product=(long double)j*D(0x96d0e8);
                        LD(0x50)=D(0x96d5d0)-age_product;
                    }
                    if(!(x>LD(0x50)))x=LD(0x50);
                    a=(u32)(long long)x; /* __ftol 006e733f */ byte_value=(s8)a;
                } else {
                    x=(s32)L(0x14); x*=D(0x96d1e0); x-=D(0x9688a0);
                    if(!((long double)byte_value>=x)) {
                        a=(u32)(long long)x; /* __ftol 006e736c */ byte_value=(s8)a;
                    }
                }
                a=B(person+0x22);
                if(a) {
                    b=U(person+0x1a); i=S(b+0x8e);
                    if((s32)(a*a+(u32)i)>5000 && byte_value<=5)byte_value=5;
                }
                a=(s32)byte_value;
                if(a<=9) {
                    b=U(0x6e866c+4*a); c=U(person+0x1a);
                    x=(s32)L(0x14); d=B(c+0x76);
                    switch(b) {
                    case 0x6e748a: x*=D(d==14?0x96a6a8:0x96d588); break;
                    case 0x6e7475: x*=D(d==14?0x96d588:0x96d590); break;
                    case 0x6e7460: x*=D(d==14?0x96d590:0x96d598); break;
                    case 0x6e744b: x*=D(d==14?0x96d598:0x96d5a0); break;
                    case 0x6e7436: x*=D(d==14?0x96d5a0:0x96d5a8); break;
                    case 0x6e7421: x*=D(d==14?0x96d5a8:0x96d5b0); break;
                    case 0x6e7409: x*=D(d==14?0x96d5b0:0x96d5b8); break;
                    case 0x6e73f1: x*=D(d==14?0x96d5b8:0x96d5c0); break;
                    case 0x6e73d9: x*=D(d==14?0x96d5c0:0x96d5c8); break;
                    case 0x6e73b6: x*=D(d==14?0x96d5c8:0x968e50); break;
                    }
                    rating=(u32)(long long)x; /* __ftol 006e74a5 */ L(0x14)=rating;
                }
            }
        }
    }
    object=L(0x34);
    if(object && club && (flags&0x2000000)) {
        u32 person_limit, person_id;
        person_limit=U(0xae2364);
        person_id=U(person);
        if((s32)person_id>=(s32)person_limit)goto skip_006e783d;
        a=U(person+0x39);
        if(!a) {
            b=U(person+0x42); c=U(person+0x3e);
            a=cm_fn_0054cdb0(0xae2c90,c,b);
            if((s32)a>60) {
                x=(s32)L(0x14); x*=D(0x969040);
                goto convert_006e76b6;
            }
        } else {
            i=S(a+0x80)-1250; j=S(club+0x80); k=C(object+0x3d);
            L(0x50)=j;
            if(j>i) {
                k=(s16)(k+100);
                if(k<125) {
                    x=F(0x96d248);
                    if((s16)j) {
                        x=i; x/=j; x*=k; x*=F(0x96d5d8);
                        if(x>D(0x967890))x=F(0x968e70);
                        else if(!(x>=D(0x969040)))x=F(0x96d248);
                    }
                    if(AGE>34)x*=D(0x969040);
                    else if(AGE>30)x*=D(0x969520);
                    else if(AGE>27)x*=D(0x9689d0);
                    if(B(player+0xf)==20) {
                        if(AGE<18)x*=D(0x969040);
                        else if(AGE<20)x*=D(0x969520);
                        else x*=D(0x9689d0);
                    }
                    goto convert_006e7690;
                }
            } else if(k<25 && AGE>30) {
                x=F(AGE>37?0x968f18:(AGE>34?0x96d248:0x96d364));
                /* 006e762b's young-age branch is unreachable after AGE>30;
                 * keep its native predicates visible. */
                if((s16)j>5250 && AGE<20) {
                    if(k< -75)x=F(0x968f18);
                    else if(k< -25)x=F(0x96d320);
                    else if(k<0)x=F(0x96d364);
                }
                if(B(player+0xf)==20) {
                    if(AGE<18)x*=D(0x969040);
                    else if(AGE<21)x*=D(0x969520);
                    else x*=D(0x9689d0);
                }
convert_006e7690:
                y=(long double)(s32)L(0x14)*x;
                rating=(u32)(long long)y; /* __ftol 006e7690 */ L(0x14)=rating;
            }
            a=cm_fn_005444a0(person,0);
            if(a) {
                x=(s32)L(0x14); x*=D(0x9689d0);
convert_006e76b6:
                rating=(u32)(long long)x; /* __ftol 006e76b6 */ L(0x14)=rating;
            }
        }
        if(AGE>34) {
            a=B(person+0x22);
            if(!a) {
                x=(s32)L(0x14); x*=D(0x9689e0);
                rating=(u32)(long long)x; /* __ftol 006e76db */ goto store_006e7839;
            }
            if(a<8) {
                x=(s32)L(0x14); x*=D(0x968968);
                rating=(u32)(long long)x; /* __ftol 006e76f5 */ goto store_006e7839;
            }
            if(AGE>35 && a<25) {
                x=(s32)L(0x14); x*=D(0x96a5d0);
                rating=(u32)(long long)x; /* __ftol 006e7714 */ goto store_006e7839;
            }
            if(a<15) {
                x=(s32)L(0x14); x*=D(0x969040);
                rating=(u32)(long long)x; /* __ftol 006e772e */ goto store_006e7839;
            }
            if(a<35)goto convert_006e77f3;
        } else if(AGE>27) {
            a=B(person+0x22);
            if(!a) {
                x=(s32)L(0x14); x*=D(0x968968);
                rating=(u32)(long long)x; /* __ftol 006e775d */ goto store_006e7839;
            }
            if(a<8) {
                x=(s32)L(0x14); x*=D(0x96a5d0);
                rating=(u32)(long long)x; /* __ftol 006e7777 */ goto store_006e7839;
            }
            if(a<15) {
                x=(s32)L(0x14); x*=D(0x9689b0);
                rating=(u32)(long long)x; /* __ftol 006e7795 */ goto store_006e7839;
            }
        } else if(AGE>24) {
            a=B(person+0x22);
            if(!a) {
                x=(s32)L(0x14); x*=D(0x96a5d0);
                rating=(u32)(long long)x; /* __ftol 006e77b7 */ goto store_006e7839;
            }
            if(a<8) {
                x=(s32)L(0x14); x*=D(0x969040);
                rating=(u32)(long long)x; /* __ftol 006e77ce */ goto store_006e7839;
            }
            if(a<15)goto convert_006e77f3;
            if(a>20)goto five_006e7834;
        } else {
            a=B(person+0x22);
            if(AGE>21) {
                if(!a) {
convert_006e77f3:
                    x=(s32)L(0x14); x*=D(0x969520);
                    rating=(u32)(long long)x; /* __ftol 006e77f3 */ goto store_006e7839;
                }
                if(a<6) {
                    x=(s32)L(0x14); x*=D(0x9689d8);
                    rating=(u32)(long long)x; /* __ftol 006e780a */ goto store_006e7839;
                }
                if(a<10) {
                    x=(s32)L(0x14); x*=D(0x9689d0);
                    rating=(u32)(long long)x; /* __ftol 006e7821 */ goto store_006e7839;
                }
                if(a>20)goto five_006e7834;
            } else if(a>10) {
five_006e7834: rating=5;
store_006e7839: L(0x14)=rating;
            }
        }
    }
skip_006e783d:
    if(!L(0x44)) {
        i=(s16)L(0x3c);
        if(!(flags&0x200000)) {
            u32 person_id, receiver;
            person_id=U(person);
            receiver=U(0xb63d28);
            a=cm_fn_0062c3d0(receiver,person_id);
            if((s16)a>1000) {
                person_id=U(person);
                receiver=U(0xb63d28);
                a=cm_fn_0062c3d0(receiver,person_id); i=(s32)((u32)i+500-(s32)(s16)a);
            }
        }
        if(i<5000)return (u32)i+(L(0x20)*5-10000)*20;
        if(i<9000)rating+=(s32)((u32)i*3)/100-280;
        else {
            /* Native signed high-half multiply by -0x51eb851f, SAR5. */
            j=(s32)((unsigned long long)((long long)(s32)(10000-(u32)i)*(s32)0xae147ae1)>>32);
            j>>=5; rating+=(u32)j+((u32)j>>31);
        }
        L(0x14)=rating;
        a=cm_fn_0062ed70(U(0xb63d28),person);
        if(a){i=S(a+0x28); rating+=(i<=10)?(u32)(-i*2):(u32)(-20-i); L(0x14)=rating;}
    }
    if(flags&0x200000) {
        u32 apply_penalty=0;
        i=(s16)L(0x38);
        if(i>=99 && C(player+0x3c)>14 && C(player+0x2d)<8) {
            u32 person_id, receiver;
            person_id=U(person);
            receiver=U(0xb63d28);
            apply_penalty=((s16)cm_fn_0062c3d0(receiver,person_id)<500);
        }
        if(apply_penalty)rating-=200;
        else rating+=100-(u32)i;
        L(0x14)=rating;
    }
    if((flags&0x1000) && L(0x1c) && club && (s32)L(0x20)>=100) {
        if(AGE<=23) {
            a=L(0x1c); x=C(a+0x22)-10; x*=D(0x96a670);
            a=(u32)(long long)x; /* __ftol 006e79bb */ rating=L(0x14)+a; L(0x14)=rating;
        }
        a=cm_fn_00547d30(person,club);
        if(a) {
            x=(s32)(rating-10); LD(0x50)=(long double)(s32)L(0x14)*D(0x969040);
            if(x>=LD(0x50))x=LD(0x50);
            goto convert_006e7a4a;
        } else if(cm_fn_00547d60(person,club)) {
            x=(s32)(rating+10); LD(0x50)=(long double)(s32)L(0x14)*D(0x9689c0);
            if(!(x>LD(0x50)))x=LD(0x50);
convert_006e7a4a:
            rating=(u32)(long long)x; /* __ftol 006e7a4a */ L(0x14)=rating;
        }
        if(cm_fn_00547840(person,optional)) {rating-=50; L(0x14)=rating;}
        else if(cm_fn_00547810(person,optional)) {
            x=(s32)L(0x14); x*=D(0x968928); LD(0x50)=(s32)(rating+20);
            if(!(x>LD(0x50)))x=LD(0x50);
            rating=(u32)(long long)x; /* __ftol 006e7aa8 */ L(0x14)=rating;
        }
        a=L(0x1c); i=C(player+0x44); j=C(a+0x14); L(0x2c)=i;
        n=i-10; v=n*j/3; if(v>=10)v=10;
        w=(C(person+0x5b)-10)*j/3; if(w>=10)w=10;
        b=L(0x28);
        {
            s32 person59, selected59;
            /* 006e7b0f, 006e7b13: person read precedes selected read. */
            person59=C(person+0x59);
            selected59=C(b+0x59);
            z=(person59-10)*selected59/3;
        }
        if(z>=10)z=10;
        k=v+w+z;
        if(!(position&1)) {
            count=(position&0x880)?0:1;
            if(position&6) {
                s32 p28, p26, p27, p43, selected1f;
                j=C(a+0xe); v=C(a+0x1a); w=C(player+0x32);
                /* 006e8038..006e80ac: retain the native byte-load order. */
                p28=C(player+0x28);
                p26=C(player+0x26);
                p27=C(player+0x27);
                p43=C(player+0x43);
                selected1f=C(a+0x1f);
                k+=v*w/40+(p28-10)*j/10+p26*j/500+
                    p27*j/350+p43*j/250+selected1f*n/8-10;
                if(count)k+=C(player+0x2e)-15;
            } else if(position&8) {
                s32 p28, p26, p43, selected1f, p27;
                j=C(a+0xe); v=C(a+0x1a); w=C(player+0x32);
                /* 006e7b90..006e7c05. */
                p28=C(player+0x28);
                p26=C(player+0x26);
                p43=C(player+0x43);
                selected1f=C(a+0x1f);
                p27=C(player+0x27);
                k+=v*w/40+(p28-10)*j/8+p26*j/325+
                    p43*j/150+selected1f*n/6+p27/15-10;
                if(count)k+=C(player+0x3e)/15+(s32)L(0x2c)-15;
            } else if(position&0x10) {
                s32 p28, p26, p43, selected1f, p27;
                v=C(player+0x32); w=C(a+0x1a); j=C(a+0xe);
                /* 006e7c92..006e7d0c. */
                p28=C(player+0x28);
                p26=C(player+0x26);
                p43=C(player+0x43);
                selected1f=C(a+0x1f);
                p27=C(player+0x27);
                k+=v*w/60+(p28-10)*j/5+p26*j/150+
                    p43*j/100+selected1f*((s32)L(0x2c)-10)/6+p27/10-10;
                if(!count)k+=C(player+0x23)/15;
                else {
                    s32 p3e, p37;
                    p3e=C(player+0x3e);
                    p37=C(player+0x37);
                    k+=p3e/15+p37/15+(s32)L(0x2c)-15;
                }
            } else if(position&0x20) {
                s32 p28, selected1f, p43, p26, p27;
                j=C(a+0xe); v=C(a+0x1a); w=C(player+0x32);
                /* 006e7da7..006e7e17. */
                p28=C(player+0x28);
                selected1f=C(a+0x1f);
                p43=C(player+0x43);
                p26=C(player+0x26);
                p27=C(player+0x27);
                k+=v*w/150+(p28-10)*j/4+selected1f*n/8+
                    p43*j/75+p26*j/150+p27/8-10;
                if(!count)k+=C(player+0x23)/15;
                else {
                    s32 p43reload, p1a;
                    p43reload=C(player+0x43);
                    p1a=C(player+0x1a);
                    k+=p43reload/15+p1a-15;
                }
                if(C(player+0x26)>30) {
                    /* 006e7e7e saves BL; 006e7ea3 consumes that same byte. */
                    s32 saved36;
                    saved36=C(player+0x36);
                    if(saved36>15) {
                        v=C(player+0x26)/3; w=C(player+0x1b);
                        k+=v*w*saved36/1500;
                    }
                }
            } else if(position&0x40) {
                s32 p28, p43, selected1f, p26, p27;
                j=C(a+0xe); v=C(a+0x1a); w=C(player+0x32);
                /* 006e7ee1..006e7f59. */
                p28=C(player+0x28);
                p43=C(player+0x43);
                selected1f=C(a+0x1f);
                p26=C(player+0x26);
                p27=C(player+0x27);
                k+=v*w/400+(p28-10)*j/3+p43*j/200+
                    selected1f*n/10+p26*j/175+p27/5-10;
                if(count) {
                    /* 006e7f7a..006e8006: retain the 27 reload and cached EBX. */
                    {
                        s32 q33, q27hi, q27reload, q36, q40;
                        q33=C(player+0x33);
                        q27hi=C(player+0x27);
                        q27hi=(s32)((unsigned long long)((long long)q27hi*(s32)0x88888889)>>32);
                        q27reload=C(player+0x27);
                        q36=C(player+0x36);
                        q40=C(player+0x40);
                        q27hi=(s32)((u32)q27hi+(u32)q27reload)>>3;
                        q27hi=(s32)((u32)q27hi+((u32)q27hi>>31));
                        k+=q33/15+q27hi+q36+q40-20;
                        if(C(player+0x26)>30 && C(player+0x36)>15) {
                            v=C(player+0x26)/3; w=C(player+0x1b);
                            k+=v*w*q36/1500;
                        }
                    }
                }
            }
        }
        if((L(0x40)&0x40) && (position&0x40)) {
            v=C(player+0x36); w=C(player+0x2e); z=C(a+0x13); k+=(v*w-200)*z/500;
        }
        if(k) {
            x=(s32)L(0x14); x*=D(0x96d580); x=D(0x967890)-x; x*=k;
            i=(u32)(long long)x; /* __ftol 006e8131 */
            if(i< -80)i=-80; else if(i>80)i=80;
            L(0x14)+=(u32)i;
        }
    }
    if((flags&0x1000000) && club) {
        a=U(club+0x53);
        if(U(person+0x1a)!=a && U(person+0x1e)!=a)L(0x14)-=50;
    }
    if(flags&0x10000) {
        object=L(0x34); byte_value=0; count=0;
        if(object) {
            a=U(object+0x45);
            if(a&0x2808) {
                x=C(object+0x44); x*=D(0x96a670);
                if(!(D(0x96d578)>=x))x=D(0x96d578);
                goto convert_006e8204;
            } else if(a) {
                x=C(object+0x44); x*=D(0x968e00);
                if(!(D(0x96d410)>=x))x=D(0x96d410);
convert_006e8204:
                count=(u32)(long long)x; /* __ftol 006e8204 */
            }
            a=B(object+0x4f)>>4; a--;
            if(a<=6) {
                b=U(0x6e8694+4*a);
                switch(b) {
                case 0x6e8227: count+=25; if(!(flags&0x800000))goto appearances_006e8241; break;
                case 0x6e828b: count+=15; if(!(flags&0x800000))goto appearances_006e8241; break;
                case 0x6e82e8:
                    if(flags&0x800000)count+=10;
                    else {
                        i=C(object+0x3f); j=C(object+0x3e); x=(j-i)*5;
                        y=(s16)L(0x38); y*=D(0x968e18); y-=D(0x96d570); x+=y;
                        a=(u32)(long long)x; /* __ftol 006e8332 */ count+=a;
appearances_006e8241:
                        a=cm_fn_008c7260(U(0xde1f54),person);
                        if(a) {
                            b=cm_fn_004cf5a0(U(0xae1050),person); U(0xae1080)=b;
                            if(b){b=U(b+4); b=U(0xae23b4)+b*0x245;}
                            c=cm_fn_008c7560(U(0xde1f54),person,b,1);
                            if(c) {
                                b=W(a+4); c=W(a+6); c+=b;
                                if(c)byte_value=(s8)(b*100/c);
                                if(!c || byte_value<70)count+=50-byte_value/2;
                            }
                        }
                    }
                    break;
                case 0x6e8398: count-=20; break;
                }
            }
            L(0x14)+=count;
        }
    } else if(flags&0x20000) {
        a=cm_fn_004cf5f0(U(0xae1050),person,club); byte_value=0;
        if(a) {
            b=B(a+0x4f)>>4; b--; i=0;
            if(b<=7) {
                b=U(0x6e86b0+4*b);
                switch(b) {
                case 0x6e83e7:
                    i=-25;
                    if(!(flags&0x800000)) {
                        a=cm_fn_008c7260(U(0xde1f54),person);
                        if(a) {
                            b=cm_fn_004cf5a0(U(0xae1050),person); U(0xae1080)=b;
                            if(b){b=U(b+4); b=U(0xae23b4)+b*0x245;}
                            c=cm_fn_008c7560(U(0xde1f54),person,b,1);
                            if(c) {
                                b=W(a+4); c=W(a+6); c+=b;
                                if(c)byte_value=(s8)(b*100/c);
                                if(!c || byte_value<70)i=byte_value/2-75;
                            }
                        }
                    }
                    break;
                case 0x6e8493: i=25; break;
                }
            }
            L(0x14)+=(u32)i;
        }
        if(!L(0x44)) {
            u32 person_id, receiver;
            /* 006e84b6, 006e84b9: argument read precedes receiver read. */
            person_id=U(person);
            receiver=U(0xb63d28);
            a=cm_fn_0062c3d0(receiver,person_id);
            if((s16)a< -1000) {
                person_id=U(person);
                receiver=U(0xb63d28);
                a=cm_fn_0062c3d0(receiver,person_id); x=(s16)a;
                x*=D(0x96a740); x+=D(0x96afc8);
                a=(u32)(long long)x; /* __ftol 006e84f1 */ L(0x14)-=a;
            } else {
                person_id=U(person);
                receiver=U(0xb63d28);
                a=cm_fn_0062c3d0(receiver,person_id);
                if((s16)a>500) {
                    person_id=U(person);
                    receiver=U(0xb63d28);
                    a=cm_fn_0062c3d0(receiver,person_id); x=(s16)a;
                    x*=D(0x967888); x=D(0x968890)-x;
                    a=(u32)(long long)x; /* __ftol 006e8539 */ L(0x14)+=a;
                }
            }
        }
    }
    if(flags&0x100000) {
        a=B(person+0x22)*5; if((s32)a>=50)a=50; L(0x14)-=a;
    } else if(L(0x58)) {
        u32 person_id, cutoff;
        /* 006e8574, 006e8577: retain this comparison's read order. */
        person_id=U(person);
        cutoff=U(0xae2364);
        if((s32)person_id<(s32)cutoff) {
            a=U(person+0x39);
            if(!a) {if(S(player+9)<5000)L(0x14)-=30;}
            else {
                a=U(a+0x57);
                if(a) {
                    x=S(a+0x69)-16; x*=D(0x9688a0);
                    a=(u32)(long long)x; /* __ftol 006e85b6 */ L(0x14)+=a;
                }
            }
        }
    }
    a=U(0xae2364); b=U(person);
    if((s32)b>=(s32)a)L(0x14)-=30;
    a=U(person+0x39);
    if(a==club) {
        x=(s32)(L(0x14)+5); LD(0x58)=(long double)(s32)L(0x14)*D(0x9689b8);
        if(!(x>LD(0x58)))x=LD(0x58);
        a=(u32)(long long)x; /* __ftol 006e8610 */ L(0x14)=a;
    }
    pos=L(0x20); rating=L(0x14);
    if(flags==0x800)return pos;
    if((s32)pos<0)return (500-rating)*pos;
    if((s32)rating<1)return rating+pos;
    return rating*pos;
}


