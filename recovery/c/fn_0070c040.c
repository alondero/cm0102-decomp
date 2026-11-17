#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry ECX is saved at 0070c057; argument is FLD at 0070c046;
   all exits RET 4. Receiver/stack boundaries follow the listed MOV ECX
   and reverse PUSH sequences, not callee source. */
#define B(p,o) CM_GLOBAL(volatile u8,(u32)(p)+(o))
#define W(p,o) CM_GLOBAL(volatile u16,(u32)(p)+(o))
#define D(p,o) CM_GLOBAL(volatile u32,(u32)(p)+(o))
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_006ce110(volatile u8 *,s8);
extern u32 CM_THISCALL cm_fn_00700420(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006ce310(u8 *,s8,u8 *,u8 *,u8 *,u8 *,u8 *,s8);
extern void CM_THISCALL cm_fn_006d6f70(char *,u16,char *,char,u8,char,char,u32);
extern u32 CM_THISCALL cm_fn_006c38c0(u32,u32);
extern void CM_THISCALL cm_fn_006cd770(u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007172d0(u32,u32);
extern u32 CM_THISCALL cm_fn_006eecb0(u32,u32);
extern u8 CM_THISCALL cm_fn_006bba50(u32);
extern u32 CM_THISCALL cm_fn_006d4250(u32);
extern u32 CM_THISCALL cm_fn_006bac40(u32);
extern void CM_THISCALL cm_fn_006bb8d0(void *,void *,float);
extern void CM_THISCALL cm_fn_00716e10(int,u32);
extern u32 CM_THISCALL cm_fn_006bb950(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_006cbbb0(u32,u32);
extern u32 CM_THISCALL cm_fn_00717470(int,short);
extern unsigned long long CM_FASTCALL cm_fn_006f0630(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void cm_fn_005e8290(const char *,const char *,int);
extern unsigned long long cm_fn_005cc7d0(void);
extern void cm_fn_005a8f60(void);
extern void cm_fn_00945934(u32);

static void message(u32 m,u16 id,u32 obj,u8 who,u8 side,u32 last)
{
    u32 receiver=D(m,0x8f7a);
    cm_fn_006d6f70((char *)receiver,id,(char *)obj,(char)who,side,-1,-1,last);
}

void CM_THISCALL cm_fn_0070c040(u32 self,float factor)
{
    /* Scratch offsets are relative to ESP after four saved registers:
       10..27, then adjacent 256-byte buffers 28 and 128. */
    u8 scratch[0x218];
    u32 a,d=0,m,other,v,r,t,receiver;
    unsigned long long rr;
    s32 p,q,n,diff,minute;
    u8 bl,side,who;
    u16 ids[6]={0x219f,0x21a0,0x21b0,0x21b3,0x21b4,0x21b5};
    int i;
#define S(o) scratch[(o)-0x10]
#define RNG(bound) (rr=cm_fn_0090cfc0((u32)(bound)),d=(u32)(rr>>32),a=(u32)rr)
#define VALID() (receiver=D(other,0x19e),receiver && (a=cm_fn_006c38c0(receiver,other),(s16)a==1))
#define MSG(id,obj,w,s,l) message(D(self,0x19e),(id),(obj),(w),(s),(l))
    *(u32 *)&S(0x24)=0;
    S(0x18)=0; S(0x12)=0; S(0x14)=0;
    if ((long double)factor==CM_GLOBAL(double,0x9689a0) || factor!=factor) factor=1.0f;
    a=D(self,0x19e); bl=B(a,0x8eaa);
    v=RNG(2000); RNG(1000);
    n=(s32)((a-v)*(u32)((s32)(s8)bl+1));
    /* 0070c0bf __ftol: FILD signed DWORD, FDIV float argument. */
    rr=(long long)((long double)n/(long double)factor);
    p=(s32)(u32)rr; d=(u32)(rr>>32);
    other=cm_fn_006ce110((volatile u8 *)D(self,0x19e),(s8)B(self,0x27));
    a=other;
    if (!other) {
        cm_fn_00944cff(0xa16058,0,0,&S(0x128),&S(0x28));
        t=(u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,t,&S(0x128),&S(0x28),0x422b);
        cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
        CM_GLOBAL(u32,0xb67a34)=0;
        cm_fn_005cc7d0(); cm_fn_005a8f60(); cm_fn_00945934(-1);
    }
    m=D(self,0x19e);
    if ((s8)B(m,0x8ea9)>3) S(0x18)=1;
    bl=S(0x18);
    if (other!=self) {
        a=(u32)&S(0x16); a=(a&0xffffff00)|B(self,0x102);
        d=(u32)&S(0x13); d=(d&0xffffff00)|B(self,0x103);
        a=cm_fn_00700420(other,a,d,*(u32 *)&S(0x18),-1,(u32)&S(0x16),(u32)&S(0x13));
        *(u32 *)&S(0x24)=a;
    }
    if (!S(0x13)) {
        if ((s16)a>375) S(0x12)=1;
        else if ((s16)a>260) S(0x14)=1;
    }
    if ((s16)p>240) bl=1;
    else if ((s16)p<10) bl=bl ? (RNG(4)!=0 ? 3:4):2;
    else {
        q=240-(s16)p;
        if(q>50) bl=bl ? (RNG(4)!=0?6:7):5;
        else if(q>10) bl=(bl!=0)+8;
        else if(q>6) bl=(bl!=0)+10;
        else bl=(bl!=0)+12;
    }
    a=cm_fn_006ce310((u8 *)D(self,0x19e),B(self,0x27)==0,
        &S(0x10),&S(0x11),&S(0x15),&S(0x23),&S(0x17),1);
    if (bl!=1) {
        if(bl==12 || bl==13) MSG(bl==12?0x1fca:0x1fcb,D(self,0x19e),B(self,0x2e),B(self,0x27)==0,0);
        if((s8)bl<12 || (s8)bl>13) return;
        if(S(0x13) && !S(0x16) && VALID()) MSG(0x20d5,D(self,0x19e),B(other,0x2e),B(other,0x27),0);
        MSG(0x211b,D(self,0x19e),B(self,0x2e),B(self,0x27),0);
        m=D(self,0x19e); side=B(self,0x27);
        if(bl==12) {who=B(m,0x8ea8); side=B(self,0x27); t=B(m,0x8ea7); cm_fn_006cd770(m,t,who,2,side,1,1,0);}
        else {who=B(m,0x8ea8); t=B(m,0x8ea7); cm_fn_006cd770(m,t,who,2,side,1,1,0);}
        if(VALID()) {RNG(3); B(other,0x2b)+=(u8)a;}
        B(D(self,0x19e),0x1cd)=1; return;
    }
    n=(s16)*(u32 *)&S(0x24)+(s16)p;
    p=n ? (s32)((s16)p*100)/n:0;
    if((s16)p<=71) {
        if(S(0x14) && VALID()) {
            t=(s32)(s8)B(other,0x17)*20+50;
            W(other,0x35)+=350;
            if(!RNG(t)) {RNG(4); a=cm_fn_007172d0(other,a); if((u8)a) a=cm_fn_006eecb0(other,0);}
        } else if(S(0x12) && VALID()) {
            t=((s32)(s8)B(other,0x17)+1)*20;
            W(other,0x35)+=600;
            if(!RNG(t)) {RNG(7); a=cm_fn_007172d0(other,a); if((u8)a) a=cm_fn_006eecb0(other,0);}
        }
        bl=S(0x12);
        if((s16)p<=51) {RNG(3); t=B(other,0x2b); W(other,0x35)+=200; B(other,0x2b)=t+(u8)a;}
        if(bl) {
            MSG((s16)p<=51?0x2112:0x2111,D(self,0x19e),B(other,0x2e),B(other,0x27),0);
            for(i=0;i<6;i++) if(cm_fn_00717470(other,ids[i])) {
                MSG(ids[i],D(self,0x19e),B(other,0x2e),B(other,0x27),0); break;
            }
        } else MSG(S(0x14)?((s16)p<=51?0x2110:0x210f):((s16)p>61?0x1fe6:(s16)p>51?0x1fe4:0x1fe5),D(self,0x19e),B(other,0x2e),B(other,0x27),0);
        if((s16)p>61) {
            W(other,0x35)+=140; RNG(3); B(other,0x2b)+=(u8)(a+1);
            m=D(self,0x19e); side=B(self,0x27); who=B(m,0x8ea8); t=B(m,0x8ea7);
            cm_fn_006cd770(m,t,who,0,side,0,1,0);
            MSG(0x211b,D(self,0x19e),B(self,0x2e),B(self,0x27),0);
            B(D(self,0x19e),0x1cd)=1;
        } else if((s16)p>51) {
            side=B(self,0x27);
            if(side==1) B(D(self,0x19e),0x8ea8)=255;
            else if(!side) B(D(self,0x19e),0x8ea8)=12;
            side=B(self,0x27); B(D(self,0x19e),0x8eae)=side==0;
            D(D(self,0x19e),0xf582)=0;
            MSG(0x211b,D(self,0x19e),B(self,0x2e),B(self,0x27),0);
            B(D(self,0x19e),0x8eb2)=3;
            MSG(0x1f75,D(self,0x19e),255,B(self,0x27)==0,0);
        } else {
            MSG(0x211b,D(self,0x19e),B(self,0x2e),B(self,0x27),0);
            cm_fn_006f0630(other,d);
        }
        return;
    }
    a=cm_fn_006bba50(D(self,0x19e));
    if((u8)a==255) {
        a=cm_fn_006d4250(D(self,0x19e));
        if((u8)a) B(D(self,0x19e),0x1a8)=1;
        if(VALID()) {
            t=B(other,0xf); m=D(other,0x69); B(other,0xf)=t+1;
            t=(s32)(s8)B(m,0x58)+1;
            if(RNG(t)) {
                t=(s32)(s8)B(D(other,0x69),0x5a)+1;
                if(RNG(t)) {
                    a=B(other,0xf); d=a*a; RNG(d);
                    t=B(self,0xf); if((s8)a<(s32)t) a=(a&0xffffff00)|t;
                    B(other,0x100)+=(u8)a; a=cm_fn_006eecb0(other,0);
                }
            }
            W(other,0x35)-=500;
            if(S(0x13)) W(other,0x35)-=600;
        }
    }
    if(VALID() && other!=self && !S(0x13) && !RNG(50) && !S(0x16))
        MSG(0x20d6,D(self,0x19e),B(other,0x2e),B(other,0x27),0);
    MSG(0x1fff,D(self,0x19e),B(self,0x2e),B(self,0x27)==0,0);
    RNG(3);
    if(a && (s8)cm_fn_006bba50(D(self,0x19e))>-1) {
        side=B(self,0x27)==0;
        goto special;
    }
    a=cm_fn_006bba50(D(self,0x19e));
    MSG((s8)a>-1?0x20fa:0x1fcd,D(self,0x19e),B(self,0x2e),B(self,0x27),0);
    a=cm_fn_006bba50(D(self,0x19e));
    if((s8)a>-1) {side=B(self,0x27); goto special;}
    if(VALID() && other!=self && S(0x16))
        MSG(0x1fe1,D(self,0x19e),B(other,0x2e),B(other,0x27),(s32)(s8)S(0x16));
    ++S(0x10);
    m=D(self,0x19e);
    if(B(m,0x1a8)) MSG(0x1fd2,m,255,B(self,0x27)==0,0);
    else {
        if(!S(0x17)) {
            bl=S(0x11); diff=(s8)S(0x10)-(s8)bl;
            minute=(s16)W(m,0x8ed0)/11;
            if(diff==2 && ((minute>=70 && W(m,0x8ed4)==990)||minute>=106)) {
                MSG(0x1fd0,m,255,B(self,0x27)==0,0); t=0x3fc66666; goto scale;
            }
            if((diff==1 || S(0x15)) && ((minute>=80 && W(m,0x8ed4)==990)||minute>=110)) {
                MSG(0x1fd1,m,255,B(self,0x27)==0,0); t=0x3fb9999a; goto scale;
            }
            if(S(0x10)==bl && !S(0x15) && minute>=70) {
                S(0x12)=255;
                bl=cm_fn_006cbbb0(m,B(self,0x27)==0);
                who=cm_fn_006cbbb0(D(self,0x19e),B(self,0x27));
                if((s8)bl<(s8)who) bl=B(D(self,0x19e),4+(B(self,0x27)==0)); else bl=S(0x12);
                m=D(self,0x19e); minute=(s16)W(m,0x8ed0)/11;
                n=(minute>=85 && minute<90)||minute>=115;
                r=bl==255?(n?0x1fd8:0x1fd6):(n?0x1fd9:0x1fd7);
                t=bl==255?(n?0x3fb9999a:0x3faccccd):(n?0x3fd33333:0x3fb33333);
                cm_fn_006bb950(m,B(self,0x27),t);
                MSG(r,D(self,0x19e),255,B(self,0x27)==0,(s32)(s8)bl); goto after_scale;
            }
            if(diff<0 && diff>-3 && (minute>=80 || (minute>=70 && S(0x10)==1) || (minute>=60 && S(0x10)==1 && (s8)S(0x11)==3))) {
                MSG(((minute>=85 && minute<90)||minute>=115)?0x1fdb:0x1fda,m,255,B(self,0x27)==0,0); goto after_scale;
            }
        }
        if(S(0x15) && (s16)W(m,0x8ed0)/11<80) {
            MSG(0x1fdc,m,255,B(self,0x27)==0,0); t=0x3fb9999a;
        } else t=0x3fa00000;
scale:
        cm_fn_006bb950(D(self,0x19e),B(self,0x27),t);
    }
after_scale:
    if(VALID() && other!=self && S(0x13) && !S(0x16))
        MSG(0x20d5,D(self,0x19e),B(other,0x2e),B(other,0x27),0);
    cm_fn_006bb8d0((void *)D(self,0x19e),(void *)self,1.7f);
    side=B(self,0x27); m=D(self,0x19e); ++B(m,0xf5bd+(side==0)*54);
    RNG(5);
    /* CL/AL remain the score bytes when the first comparison branches directly. */
    who=S(0x10); side=S(0x11);
    if((s8)who-(s8)side>(s32)(a+2)) {
        if(!RNG(7)) {
            m=D(self,0x19e); side=B(self,0x27); t=D(m,0x4792);
            t=D(t,side?0x1c:0x20); v=D(t,0x53);
            if(v) {
                t=D(m,0x4792); t=D(t,side?0x1c:0x20); t=D(t,0x53);
                if(B(t,0x77)==8) {MSG(0x2117,m,255,side==0,0); goto finish;}
            }
        }
        side=S(0x11); who=S(0x10);
    }
    if(who==side || (s8)who-1==(s8)side || S(0x15)) {
        t=(s16)W(D(self,0x19e),0x8ed0)/11; RNG(t); v=a; RNG(120);
        if((s32)v>(s32)a) {
            m=D(self,0x19e); side=B(self,0x27); t=D(m,0x4792);
            t=D(t,side?0x1c:0x20); v=D(t,0x53);
            if(v) {t=D(m,0x4792); t=D(t,side?0x1c:0x20); t=D(t,0x53);
                if(B(t,0x77)==8) MSG(0x2116,m,255,side==0,0);}
        }
    }
finish:
    m=D(self,0x19e); side=B(self,0x27); B(m,0x8eae)=side;
    D(D(self,0x19e),0xf582)=0; B(D(self,0x19e),0x8eb2)=8;
    B(D(self,0x19e),0x1ca)=0; return;
special:
    MSG(0x20ed,D(self,0x19e),255,side,0);
    a=cm_fn_006bba50(D(self,0x19e)); bl=(u8)a;
    a=cm_fn_006bac40(D(self,0x19e));
    if(bl==B(self,0x27)) {
        MSG(0x20df,D(self,0x19e),255,B(self,0x27),0);
        RNG((s8)S(0x11)-(s8)S(0x10)+3);
        if((s32)a>0) MSG(0x20d9,D(self,0x19e),255,B(self,0x27),0);
        cm_fn_006bb8d0((void *)D(self,0x19e),(void *)self,3.0f);
    } else {
        if((S(0x10)==S(0x11) || (s8)S(0x10)-1==(s8)S(0x11)) && !RNG(40))
            MSG(0x20d9,D(self,0x19e),255,B(self,0x27),0);
        cm_fn_006bb8d0((void *)D(self,0x19e),(void *)self,1.45f);
    }
    cm_fn_00716e10(self,0);
}
