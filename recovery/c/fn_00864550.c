#include "cmre.h"
#include "cmre_x87.h"
/* Preferred-image VAs in this file refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry: 00864550 overwrites ECX; 008665b4 RET 18; return FLD at
   008665a9. Calls use the pushes/ECX at their listed call sites.
   00865601/02 push two arguments, 00865603 sets ECX for 0054cdb0.
   0086562e/33 push two cdecl arguments for 00547770. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00547770(u32,u32);
extern u32 CM_CDECL cm_fn_00549e50(u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern s32 CM_THISCALL cm_fn_005968d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32,u32,u32,u32);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32,u32,u32,u32);
extern s32 CM_CDECL cm_fn_00946a04(void);
#define U(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile s8,(u32)(a))
#define W(a) CM_GLOBAL(volatile s16,(u32)(a))
typedef float cm_float_view __attribute__((may_alias));
#define F(a) CM_GLOBAL(volatile cm_float_view,(u32)(a))
#define D(a) CM_GLOBAL(double,(u32)(a))
#define G(a) U(a)
/* The original uses the caller-owned sixth DWORD as both integer scratch and
   rounded float storage. Every v access is that home, never a cached local.
   x/y/z separately retain values which remain live across an original FST. */
#define v F((u32)&arg6)
#define SOFT(t,m) do { long double live=v; if (live>D(t)) { \
  live=v; live-=D(t); live*=D(m); live+=D(t); v=(float)live; } } while(0)
/* FCOM C3 is set for both equal and unordered. */
#define X87_EQ(a,b) (!((a)<(b) || (a)>(b)))

float CM_STDCALL cm_fn_00864550(volatile u32 p,volatile u32 club, volatile u32 flag,
                               volatile u32 mode,volatile u32 arg5,volatile u32 arg6)
{
 u32 q,r,other,persondata,nation,home,idx,target,cb,pp,tail_flag,tail_mode;
 s32 level,age,kind,tmp,rep,rr,cap,id;
 volatile float ratio; /* 008645c5 FSTP m32: required rounding, not allocation. */
 long double x,y;
 volatile double z; /* Original m64 spills, with x87 live values kept in x/y. */
 unsigned long long wide;
 s16 rb,rs,rp;
 s8 special;
 pp=p; /* EBX is captured before the first lookup, not reloaded afterwards. */
 q=cm_fn_004cf5a0(G(0xae1050),pp);
 U(0xae1080)=q;
 if(!q || (B(q+0x35)&0x3f)==5) other=0;
 else { r=U(q+4); other=G(0xae23b4)+r*0x245u; }
 tmp=(s32)U(pp); id=tmp; age=B(pp+0x18); persondata=U(pp+0x61);
 ratio=(float)((long double)tmp/(s32)G(0xae2364));
 if(club) {
   level=(s8)(W(club+0x80)/500+1);
   if(other && B(club+0x64)==2) {
     --level; if(level<1)level=1; else if(level>4)level=4;
   }
 } else {
   if(persondata)tmp=W(persondata+0xd)/500;
    else { q=U(pp+0x69); tmp=q?W(q+0xc)/500:1; }
   if(tmp<1)tmp=1;
   level=(s8)tmp;
 }
   if(other && (kind=B(pp+0x3d))) {}
  else {
    q=U(pp+0x24);
    if(q && (kind=B(pp+0x28))) {}
    else if((kind=B(pp+0x3d))) {}
   else if(persondata)kind=11;
    else { kind=B(pp+0x28); if(!kind)kind=9; }
 }
 idx=(u32)(kind-5);
 if(idx<=10) {
   target=U(0x8665b8+idx*4);
   switch(target) {
   case 0x8646ba: case 0x864789: case 0x8648c8:
   case 0x8649fd: case 0x864b3b: case 0x864c0a:
     if(other && (tmp=W(other+0x80)/500+1)>level) level=tmp;
     else {
        q=U(pp+0x69);
       if(q) {
          q=U(pp+0x24);
         if(q) {
           if(target==0x8646ba) {
              if(B(pp+0x28)!=11) { tmp=W(q+0x80)/500+1; if(tmp>level)level=tmp; }
           } else {
             tmp=W(q+0x80)/500+1;
              if(tmp>level && B(pp+0x28)!=11)level=tmp;
           }
         }
       }
     }
      q=U(pp+0x69);
     if(q) {
         tmp=W(q+0xa);
         if(target==0x8646ba || target==0x864b3b || target==0x864c0a)
           { arg5=(u32)tmp; x=(s32)arg5; }
        else {
          /* 008647f2/00864931/00864a66 MOV and following FILD. */
           arg6=(u32)tmp;
          x=(s32)arg6;
        }
       x=x*x*x;
       x*=D(target==0x8646ba?0x96ff38:
             (target==0x864789 || target==0x8648c8)?0x968eb8:
             target==0x8649fd?0x96ff28:0x96ff18);
     } else {
       x=(long double)ratio*F(0x96d228)+F(0x968e70);
       x*=F(target==0x8646ba?0x96ff34:
             (target==0x864789 || target==0x8648c8)?0x96d1b0:
             target==0x8649fd?0x968db0:0x96d2dc);
       x*=D(0x96a5c0);
     }
     v=(float)((long double)level*x*D(0x9689e0));
     if(target==0x8646ba) {
       if((long double)F(0x96ff30)>v)v=750;
       else if((long double)v>F(0x96ff34))v=150000;
     } else if(target==0x864789 || target==0x8648c8) {
       if((long double)F(0x96d1a8)>v)v=500;
       if(other && other!=club) {
         if(target==0x864789) {
            special=B((u32)&arg5);
            if(special!=11 && special!=6) {v=150000;break;}
           if(W(other+0x80)/500+4>level){v=150000;break;}
            if(!((long double)v>=F(0x968dac)))v=(float)((long double)v*D(0x969588));
           break;
         }
         if(W(other+0x80)/500+4>level){v=150000;break;}
       }
        special=B((u32)&arg5);
        if(special!=11 && special!=6 && (long double)v>F(0x96ff34))v=150000;
     } else if(target==0x8649fd) {
       if((long double)F(0x96ff20)>v)v=275;
       /* 00864acb fcomp / 00864ad1 test ah,1 / 00864ad4 je: clamp on unordered too. */
        else if(!((long double)v<=F(0x968db0)))v=5000;
        if(other && other!=club) {
          special=B((u32)&arg5);
          if(special!=6 && special!=11 && W(other+0x80)/500+4>level)v=150000;
        }
     } else {
       if((long double)F(target==0x864b3b?0x96d210:0x96d834)>v)
         v=target==0x864b3b?250:200;
       /* 00864cd8 fcomp / 00864cde test ah,1 / 00864ce1 je: clamp on unordered too. */
        else if(!((long double)v<=F(0x96d2dc)))v=1000;
     }
     break;
   case 0x864d34:
     if(!persondata) {
         r=U(pp+8); q=U(pp+4);
        cm_fn_00944c9f(0xde1f64,0xa83968,q,r,id);
       tmp=cm_fn_00946a04();
       wide=cm_fn_00945e40((u32)tmp,(u32)(tmp>>31),4,0);
        wide=cm_fn_00945e80((u32)wide,(u32)(wide>>32),0x8000,0);
        arg5=(u32)wide;
        v=(float)((long double)(s32)arg5*F(0x96fed8));
       break;
     }
      if(B((u32)&mode)==-1 && club) {
       rb=W(club+0x80); rs=W(persondata+0xb);
       if(rs>rb && age>=24 && age<34) *(volatile s8 *)&mode=1;
       else {
          z=(double)((long double)rb*D(0x969040)); y=z; /* 00864d9e m64. */
         if((long double)rs>y && age>=21) *(volatile s8 *)&mode=2;
         else if(((long double)rs>(long double)rb*D(0x968968) && age>=17) || age>24)
           *(volatile s8 *)&mode=4;
         else if((long double)(W(persondata+7)*50)>y) *(volatile s8 *)&mode=5;
         else *(volatile s8 *)&mode=6;
       }
     }
      special=B((u32)&arg6); /* 00864e04: byte read, cached in BL. */
     if(special==3)x=F(0x96feb0);
     else if(special==1) {
       x=F((B(p+0x59)<15 && B(p+0x5a)>15)?0x96ff6c:0x96fe94);
     } else if(special==2)x=F((age>22 && age<32)?0x96fe94:0x96ff68);
      else if(((tmp=B((u32)&arg5))==13 || tmp==15) && age>35)x=F(0x96d1b0);
      else if(((tmp=B((u32)&mode))==6 || tmp==5) && W(persondata+0xb)<2500)x=F(0x96ff64);
      else if((tmp==6 || tmp==5) && W(persondata+0xb)<6250)x=F(0x96feec);
     else if(B(persondata+0x15)>=15) {
       if(W(persondata+0xb)>6750 && age<31)x=F(B(p+0x59)<15?0x96ff68:0x96ff60);
       else x=F(0x96ff08);
     } else if(B(persondata+0x12)>=15 || B(persondata+0x14)>=15 || B(persondata+0x13)>=15) {
       if(W(persondata+0xb)>7250 && age<31)x=F(B(p+0x59)<14?0x96ff50:0x96ff4c);
       else x=F(0x96ff48);
     } else if(B(persondata+0xf)==20) {
       x=F((W(persondata+0xd)>7250 && B(p+0x59)<3 && age<34)?0x96ff5c:0x96ff58);
     } else if((B(persondata+0x10)>=15 || B(persondata+0x11)>=15) &&
               W(persondata+0xb)>7500 && B(p+0x59)<6 && age<33)x=F(0x96ff04);
     else x=F(0x96ff54);
      v=(float)x; /* 00864e1c/44/68/7b/a2 or 00864fc6 FST; x stays live. */
     tmp=1;
      cb=club; /* ESI at 00864fca, after the selected factor's FST. */
      if(cb) {
        q=U(cb+0x53);
        if(q && (W(cb+0x80)<5750 || B(q+0x85)<12)) {
          r=U(p+0x39);
          if(r==cb || (r && U(r+0x53)==q))tmp=0;
       }
     }
      if(age>18 && age<32 && tmp) {
        rp=W(persondata+0xd);
        tmp=B(p+0x59);
        tmp=(tmp+230)*25-B(p+0x57)*10;
        if(rp>tmp) {
          /* 008650c9 stores only when an x87 value was replaced. */
          if(B(persondata+0xf)==20) {
            if(!(x>=F(0x96fe94))) { x=F(0x96fe94); v=(float)x; }
           } else if(X87_EQ(x,F(0x96ff6c))) { x=F(0x96feac); v=(float)x; }
           else if(X87_EQ(x,F(0x96fe94))) { x=F(rp>7250?0x96feac:0x96ff6c); v=(float)x; }
          else if(rp>7250) {
            if(!(x>=F(0x96ff6c))) { x=F(0x96ff6c); v=(float)x; }
          } else if(!(x>=F(0x96fe94))) { x=F(0x96fe94); v=(float)x; }
        }
     }
      if(B(0xae233c)==1 && x>F(0x96feec) && cb) {
        q=cm_fn_00549e50(cb);
        if((u8)q) {
          rb=W(persondata+0xb);
          if(rb>6750) {
            tmp=B(p+0x59);
            x=v; /* 0086510b/17 reload after the call and field reads. */
            x*=D(tmp==1?0x968980:tmp<6?0x96a5a8:0x9689c0);
          } else { x=v; if(rb>4250)x*=D(0x968fa0); }
        } else x=v; /* 00865141 */
     }
     rb=W(persondata+0xb); rs=W(persondata+9);
     rr=rb; if(rs>rr)rr=rs;
     rp=W(persondata+0xd); if(rp>rr)rr=rp;
      if(rp<1750 && rr>3250) {
        arg5=(u32)(s32)(s16)rr;
        /* __ftol at preferred VA 0086518a */
        tmp=(s32)(long long)(((long double)(s32)arg5-D(0x96fec0))*D(0x968f30));
       rr=(s32)(3250u-(u32)tmp);
     }
     if(special==1 && (s16)rr<7250)rr=7250;
     else if(special==2 && (s16)rr<6750)rr=6750;
      if((X87_EQ(x,F(0x96fe94)) || X87_EQ(x,F(0x96ff6c)) ||
          X87_EQ(x,F(0x96feac))) && (s16)rr<7250)rr=7250;
      tmp=W(persondata+7); cap=W(persondata+5);
      arg5=(u32)(s32)rp; /* 00865227; retained for 00865359 FILD. */
     tmp=(s32)((u32)(s32)rb+(u32)(s32)rs+(u32)(s32)rp+
               (u32)(s32)(s16)rr*4u+((u32)tmp+(u32)cap)*50u);
     /* __ftol at preferred VA 00865258 */
     rep=(s32)((u32)(tmp/9)+(u32)(long long)((long double)ratio*F(0x96d784)-F(0x96d228)));
      cb=club; /* EBX reload at 00865261, before country and cap reads. */
      nation=U(p+0x1a);
       if(nation && (q=U(cb+0x53))!=0 &&
         (special=B(q+0x76), B(nation+0x76)!=special)) {
        r=other?U(other+0x53):0;
        if(!other || (r && B(r+0x76)!=special)) {
         if((u32)rep<1500)rep=1500;
         else {
           /* __ftol at preferred VA 008652b9 */
           rep=(s32)(long long)((long double)(unsigned long long)(u32)rep*D(0x9689b8));
           if((u32)rep<=1)rep=1;
         }
       } else if((u32)rep<=1)rep=1;
     } else if((u32)rep<=1)rep=1;
     x=(long double)(unsigned long long)((u32)rep*(u32)rep)*x*D(0x968eb8);
      v=(float)x; if(!(x>=F(0x96fee8)))v=175;
     q=other?U(other+0x53):0;
     tmp=q?B(q+0x7e):rp>4250?1:nation?B(nation+0x7e):3;
     if(tmp!=1)v=(float)((long double)v*F(tmp==2?0x96d248:0x968f18));
       if(cb && (rb=W(cb+0x80))!=0) {
        y=(s32)arg5;
        if(rb<2500)y*=F(0x96fee4);
       else { arg5=(u32)level*500; y/=(s32)arg5; }
       if(y<D(0x969520))y=F(0x96d364);
       if(y>D(0x968980))y=F(0x96d390);
       v=(float)(y*v);
     }
     if(level<15) {
       cap=(s32)((u32)level*(level<10?2500u:2750u));
       arg5=(u32)cap;
       if((long double)v>cap)v=(float)cap;
      } else {
         q=U(cb+0x53);
        if(q && (r=U(q), r==G(0x9cf2e4) || r==G(0x9cf488) || r==G(0x9cf360)))
          cap=(s32)((u32)level*(B(cb+0x82)?5250u:7500u));
       else cap=(s32)((u32)level*3250u);
       arg5=(u32)cap;
       if((long double)v>cap)v=(float)(((long double)v-cap)*D(0x967880)+cap);
     }
     y= B(persondata+0x15)==20?150000:
         (B(persondata+0x14)==20 || B(persondata+0x13)==20 || B(persondata+0x12)==20)?125000:
         B(persondata+0x11)==20?100000:80000;
     F((u32)&arg5)=(float)y;
      if(B(0xae233c)==1 && cb && (u8)cm_fn_00549e50(cb) && W(persondata+0xb)>7250)
       F((u32)&arg5)=(float)((long double)F((u32)&arg5)*D(0x9689c0));
     y=F((u32)&arg5);
     if((long double)v>y)v=(float)(((long double)v-y)*D(0x968918)+y);
     q=U(p+0x39);
     if(!q) {
       tmp=1;
       if(age>=33 && B(p+0x22) && B(persondata+0x44)+B(p+0x58)<=30 &&
           (s32)U(p)%15!=0 && W(cb+0x80)<=4500)tmp=0;
        if(tmp) {
          r=U(p+0x42); q=U(p+0x3e);
          tmp=cm_fn_0054cdb0(0xae2c90,q,r);
          arg5=(u32)tmp;
          y=(s32)arg5;
          F((u32)&arg5)=(float)y; /* 00865615 FST, comparison retains y. */
          if(y>F(0x96d210))F((u32)&arg5)=250;
          q=cm_fn_00547770(cb,0xae2c90);
         y=F((u32)&arg5);
         if(q)y+=F(0x96d7b0);
         if(y>F(0x96d880))y=(y-F(0x96d880))*D(0x968918)+D(0x968dd8);
         y=F(0x96fedc)-y*F(0x96fee0);
         if((long double)F(0x96b028)>y)y=F(0x96b028);
         v=(float)(y*v);
       }
     }
     break;
   default:v=1000;break;
   }
 } else v=1000;
  cb=club; /* 00864ceb; subsequent EBX is not the mutable club home. */
  if(!cb)return F((u32)&arg6);
 if(persondata) {
    /* 00864d03 reads type before reputation and the arg5 scratch write. */
    special=B(cb+0x64);
    rep=W(cb+0x80); arg5=(u32)rep;
    x=(long double)(s32)arg5*D(special==1?0x96fed0:rep>2250?0x96ff10:0x96a5c0);
   z=(double)x;
   /* _CIpow at preferred-image VA 00865715 */
   x=cm_x87_CIpow((long double)(double)z,(long double)D(0x969000));
   z=(double)x;
   if(x>D(0x967890))z=1;
    idx=(u32)((s32)B((u32)&mode)-1);
   if(idx<=5) {
     target=U(0x8665e4+idx*4);
      switch(target) {
      case 0x865753: case 0x8657c8: case 0x8658c8:
        if(age<20) {
          tmp=rep*3; arg5=(u32)tmp;
          y=(long double)(s32)arg5*z*D(target==0x865753?0x968e48:target==0x8657c8?0x968e80:0x967880);
        }
        else {
          q=age>=24?cm_fn_00549e50(cb):0;
          tmp=W(cb+0x80)*3;arg5=(u32)tmp;
          y=(long double)(s32)arg5*z;
         if(target==0x865753)y*=D((u8)q?0x968940:0x968928);
         else if(target==0x8657c8) {if(!(u8)q)y*=D(0x968e48);}
         else y*=D((u8)q?0x968e80:0x968f20);
       }
       break;
      case 0x865837:
        if(age<20) {
          tmp=rep*3; arg5=(u32)tmp;
          y=(long double)(s32)arg5*z*D(0x9689e0);
          if(!((long double)v>=y))v=(float)(((long double)v+y)*D(0x968968));
        }
        q=age>=24?cm_fn_00549e50(cb):0;
        tmp=W(cb+0x80)*3;arg5=(u32)tmp;
        y=(long double)(s32)arg5*z*D((u8)q?0x968e48:0x968e80);
        break;
      case 0x865934:
        arg5=(u32)(rep*3); y=(long double)(s32)arg5*z*D(0x967888);break;
      case 0x865958:
        arg5=(u32)(rep*3); y=(long double)(s32)arg5*z*D(0x968a98);break;
     default:y=v;break;
     }
      if(!((long double)v>=y))v=(float)(((long double)v+y)*D(0x968968));
    }
   q=cm_fn_004cf5a0(G(0xae1050),p);
   if(q && (B(q+0x45)&0x10) && idx<=5) {
     target=U(0x8665fc+idx*4);
     v=(float)((long double)v*D(target==0x8659b2?0x968f80:
          target==0x8659be?0x969520:target==0x8659ca?0x968e18:
          target==0x8659d6?0x968e38:0x968eb0));
   }
 }
  q=U(cb+0x53);
 if(q) {
   home=U(p+0x39);
   tmp=0;
   if(home) {
     if(U(home+0x53)==q) {
       nation=U(p+0x1a);
       if(nation==q || B(p+0x59)>3 || B(p+0x57)<15)tmp=1;
     }
   } else { nation=U(p+0x1a); if(nation==q)tmp=1; }
   if(tmp) {
      rep=W(cb+0x80)/50;
      rr=G(0x9cf36c); r=U(q);
     if(r==(u32)rr) {
       cap=(s32)((u32)level*(u32)level*10u);arg5=(u32)cap;
       /* 00865a7f FILD cap / 00865a83 FLD v / 00865a87 FCOMP ST(1) /
          00865a8b TEST AH,1 / 00865a8e JE / 00865a90 FSTP: store the finite
          cap on unordered as well as v<cap. */
       if(!((long double)v>=cap))v=(float)cap;
       SOFT(0x9690b8,0x9689e0);
     } else if(r==G(0x9cf2e4) || r==G(0x9cf360) || r==G(0x9cf488)) {
     } else if(r==G(0x9cf4f8)) {
        x=(long double)v*D(0x969040);
        home=U(cb+0x57); /* 00865b02 precedes the FSTP at 00865b05. */
        v=(float)x;
       if(home && U(home)==G(0x9cf594)) {SOFT(0x968898,0x968f20);}
       else if(nation!=q) {SOFT(0x96fec0,0x9689e0);}
        else {
          special=B(p+0x18);
          if(special>31 || (special<24 && (home=U(p+0x61)) && W(home+0xd)<3750)) {
            SOFT(0x96a5e8,0x968918);
          } else {SOFT(0x96fec8,0x968918);}
        }
     } else {
       u32 base=0,threshold=0,mult=0x968918;
       s32 category=0;
       if(r==G(0x9cf484)){base=0x9689e0;category=1;}
       else if(r==G(0x9cf45c)){base=0x9689e0;category=2;}
       else if(r==G(0x9cf314) || r==G(0x9cf300) || r==G(0x9cf338)){base=0x969040;category=3;}
       else if(r==G(0x9cf4a8)){base=0x9689e0;category=4;}
       else if(r==G(0x9cf510) || r==G(0x9cf2fc) || r==G(0x9cf2dc) ||
               r==G(0x9cf3e4) || r==G(0x9cf224)){category=5;}
       else if(r==G(0x9cf2c0) || r==G(0x9cf408)){base=0x968968;category=6;}
       else if(r==G(0x9cf244)){base=0x96a6a8;category=7;}
       else if(r==G(0x9cf434) || r==G(0x9cf2ac) || r==G(0x9cf430)){base=0x968968;category=7;}
       else if(r==G(0x9cf260) || r==G(0x9cf218)){base=0x96a6a8;category=8;}
       if(base) {
         tmp=B(q+0x85); cap=tmp*10;
         arg5=(u32)(s32)(s16)rep;
         y=D(base);
         if((s16)rep>cap) {
            club=1;
            if(tmp>=1)club=(u32)tmp;
            y=(long double)(s32)arg5/(s32)club*D(0x967880)*D(base);
           if(y>D(0x967890))y=D(0x967890);
         }
           x=v; /* FLD precedes club+64 read and the rounded home write. */
           if(category==4 || category==6 || category==7) {
             special=B(cb+0x64);
             if(category==4)threshold=special==1?0x968878:0x968f50;
             else if(category==6)threshold=special==1?0x96f870:0x968898;
             else threshold=special==1?0x968878:0x968898;
           }
          v=(float)(x*y);
       }
       switch(category) {
       case 1:SOFT(0x968878,0x968968);SOFT(0x96feb8,0x968968);break;
       case 2:
          if((s16)rep>cap)threshold=0x968910;
          else if(!persondata)threshold=0x96ff40;
          else {
            rp=W(persondata+0xd);
            if(rp<1250)threshold=0x96a5e8;
            else {
              special=B(p+0x18);
              threshold=(special<20 || special>32) && rp<2750?0x96fec0:0x96ff40;
            }
          }
         mult=0x9689e0;break;
       case 3:threshold=0x968888;mult=0x9689e0;break;
        case 4:break; /* threshold from the original pre-store type read */
       case 5:threshold=B(cb+0x64)==1?0x96a648:0x9688a8;mult=0x968f20;break;
        case 6:mult=0x967880;break;
        case 7:break;
       default:break;
       }
       /* EBX retains the original club even where its stack home is overwritten. */
       if(threshold)SOFT(threshold,mult);
        if(!category) {
          x=(long double)v*D(0x968e80);
          v=(float)x; /* 0086600f FST; compare the still-live x at 00866013. */
          if(x>D(0x96a728))v=(float)(((long double)v-D(0x96a728))*D(0x9689e0)+D(0x96a728));
        }
       if(category==8)SOFT(0x9688d0,0x9689e0);
     }
   }
 }
 /* Remaining boundaries use EBX, not the possibly overwritten argument home. */
 if(persondata) {
   if(other && other!=cb) {
     q=cm_fn_004cf5f0(G(0xae1050),p,other);
     if(q && (B(q+0x4f)&0xf0)==0x70) {
       tmp=(s32)U(q+0xc);
       if((long double)v>tmp)v=(float)tmp;
     }
   }
    tail_flag=flag; tail_mode=mode; pp=p; /* 008663af/b3 and 008663bf/d8. */
    if(tail_flag || !other) {
       if(tail_flag || (q=U(pp+0x61), W(q+0xb)<2250)) {
        q=cm_fn_005a0590(G(0xae2a38),cb);
        if(!q)return v;
        tmp=cm_fn_005968d0(q,pp,tail_mode);arg5=(u32)tmp;
        y=(s32)arg5;
       /* 00866401 fcom / 00866407 test ah,1 / 0086640a je: the signed helper
          result is stored into v on unordered as well as less-than. */
        if(!(y>=v))v=(float)y;
        if(!other || tail_flag)goto finish;
     } else goto finish;
   }
    if(B(pp+0x57)>10 && B(pp+0x59)<6 && W(persondata+0xb)>6750 &&
      (B(persondata+0x15)==20 || B(persondata+0x14)==20 || B(persondata+0x13)==20) &&
       B(pp+0x18)>20) {
     q=cm_fn_005a0590(G(0xae2a38),cb);
     if(!q)return v;
      tmp=cm_fn_005968d0(q,pp,tail_mode);arg5=(u32)tmp;
      special=B(persondata+0x14);
      y=(s32)arg5;
      if(special==20) {
        special=B(pp+0x59);
        x=v;
        v=(float)(x*D(special==1?0x968980:0x9689c0));
      } else {
        x=(long double)v*D(0x968928);
        special=B(persondata+0x13); /* Read before FCOMPP/branch. */
        if(!(x>=y)) {
          tmp=B(pp+0x59);
          x=v;
          v=(float)(x*D(special==20?(tmp==1?0x9689c0:0x9689b8):
                                                   (tmp==1?0x968978:0x968980)));
        }
      }
   }
 }
finish:
 /* FCOMP C0 includes unordered: preferred-image VAs 00866518-00866523,
    0086653f-0086654a, 0086656d-00866578 and 00866594-0086659f
    (pinned 3.9.68 SHA-256 above). Negated >= retains that NaN branch. */
 if(B(cb+0x64)!=1) {
    if(W(cb+0x80)>2250 && !(v>=F(0x96d750))) { F((u32)&arg6)=75.0f; return F((u32)&arg6); }
    if(!(v>=F(0x969560))) { F((u32)&arg6)=50.0f; return F((u32)&arg6); }
 } else {
    if(age<20 && !(v>=F(0x96d750))) { F((u32)&arg6)=75.0f; return F((u32)&arg6); }
   if(!(v>=F(0x968960)))v=100;
 }
  return F((u32)&arg6);
}
#undef v
#undef X87_EQ
