#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry ECX saved at 007abace, argument at 007abac4, RET 4 at 007acfad.
   All receiver/stack argument maps follow the MOV ECX/PUSH sequences in
   007abb00..007abc5d, 007abddd..007abe55 and 007acea0..007acf5c. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define C(a) CM_GLOBAL(volatile s8,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define S(a) CM_GLOBAL(volatile s16,(u32)(a))
#define I(a) CM_GLOBAL(volatile s32,(u32)(a))
#define F(a) CM_GLOBAL(double,a)
extern void CM_THISCALL cm_fn_00775ba0(u32,u32);
extern void CM_THISCALL cm_fn_007a8470(u32,u32);
extern void CM_THISCALL cm_fn_005afd00(u32,u32);
extern u32 CM_THISCALL cm_fn_007c6650(u32,u32);
extern void CM_THISCALL cm_fn_0086fb80(u32,u32);
extern void CM_THISCALL cm_fn_0055e1d0(u32,u32);
extern u32 CM_THISCALL cm_fn_00631f90(u32,u32);
extern void CM_THISCALL cm_fn_0056a090(u32,u32);
extern void CM_THISCALL cm_fn_00556830(u32,u32);
extern u32 CM_THISCALL cm_fn_0062bfa0(u32,u32);
extern u32 CM_THISCALL cm_fn_004d0b50(u32,u32);
extern u32 CM_THISCALL cm_fn_004d0bd0(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
extern u32 CM_CDECL cm_fn_0053a5b0(u32);
extern void CM_THISCALL cm_fn_005fff30(u32,u32);
extern void CM_THISCALL cm_fn_007c6bf0(u32,u32,u8);
extern u16 CM_STDCALL cm_fn_007c6930(u32,u32);
extern void CM_THISCALL cm_fn_007c6a30(u32,u32,u8,s8,u32);
extern u32 CM_CDECL cm_fn_00549ea0(u32);
extern u32 CM_CDECL cm_fn_0053ab90(u32,u32);
extern u32 CM_THISCALL cm_fn_007ad0e0(u32,u32);
extern u32 CM_STDCALL cm_fn_007a6f10(u32,u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern void CM_STDCALL cm_fn_007a8ba0(u32,u32);
extern void CM_CDECL cm_fn_00547a10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ad590(u32,u32);
extern void CM_CDECL cm_fn_00538ec0(u32);
extern void CM_CDECL cm_fn_00538bc0(u32);
#define R(n) cm_fn_0090cfc0((u32)(n))
#define P D(p+0x61)
#define Q (D(p)*79u+D(0xae2d0c))
/* Each conversion is the original x87 __ftol at the named call sites:
   007abf0a/4d/93/cb/ff5, 007ac06f/1d7/1fe/21f/254/280/2ae/
   336/35d/37b/3a9/3fa/450/486/4bf/4e9/732.
   Signed 64-bit truncation retains the x87 extended intermediate. */
static u32 ft(long double x) { return (u32)(long long)x; }
void CM_THISCALL cm_fn_007abac0(u32 self, volatile u32 home)
{
 u32 p=home,a,b,e,t,v,k,j,base,lo,hi; s32 n; s16 threshold,z; s8 age,c;
 long double f,g;
 base=D(0xae2d0c); a=D(p); a=a*79u+base+11;
 c=B(a);
 if (!(c&0x30) || D(p+0x39)) { B(a)=(u8)c&0xcf; return; }
 cm_fn_00775ba0(D(0xb74318),p);
 a=D(self+0x3c); t=D(a); b=D(a+4); e=D(a+8);
 cm_fn_007a8470(self,t);
 base=D(0xae23c4); B(p+0x5f)=2; D(p+0x61)=base+t*70u;
 a=Q+11; B(a)|=0x40;
 B(Q+13)=0; a=Q+12; B(a)&=0xfd;
 a=Q+11; B(a)&=0x7f; a=Q+12; B(a)&=0xfe; B(Q+9)=0x9c;
 a=D(0xae2a60); if(a && B(0x9cc270)) cm_fn_005afd00(a,p);
 cm_fn_007c6650(D(0xb74350),p);
 cm_fn_0086fb80(D(0xdd7f00),p);
 cm_fn_0055e1d0(D(0xae28f0),p);
 cm_fn_00631f90(D(0xb63d28),p);
 cm_fn_0056a090(D(0xae2a30),p);
 a=D(p); cm_fn_00556830(D(0xae28f0),a);
 a=D(p); cm_fn_0062bfa0(D(0xb63d28),a);
 cm_fn_004d0b50(D(0xae1050),p);
 cm_fn_004d0bd0(D(0xae1050),p);
 a=D(p+0x69); B(p+0x22)=0; B(p+0x23)=0; if(a) D(p+0x69)=0;
 a=Q+11; B(a)&=0xf0; B(Q+8)=0; B(Q+9)=0x9c;
 a=D(0xae23a8)+b*290u; D(p+0x1a)=a;
 if(I(a+0x88)<100 && R(4)==0) {
  t=R(D(0xae2350)); a=D(0xae23a8)+t*290u;
  if(D(a+0x71) && I(a+0x88)<100) D(p+0x1a)=a;
 }
 if((s32)e<=0 || (s32)e>=I(0xae2350)) D(p+0x1e)=0;
 else if(b!=D(0x9cf488) || R(100)!=0) {
  if(e==D(0x9cf424) || R(3)==0) D(p+0x1e)=D(0xae23a8)+e*290u;
  else D(p+0x1e)=0;
 }
 v=cm_fn_00539c60(D(p+0x1a)); D(p+4)=v;
 v=cm_fn_0053a1b0(D(p+0x1a)); D(p+8)=v;
 v=cm_fn_0053a5b0(D(p+0x1a)); D(p+12)=v;
 cm_fn_005fff30(D(0xb63c98),p);
 W(p+0x40)=W(0xae2c92); W(p+0x3e)=W(0xae2c90);
 D(p+0x39)=0; D(p+0x24)=0; D(p+0x4e)=0; D(p+0x52)=0;
 for(j=1;j<=64;j++){ home=(home&0xffffff00u)|j; cm_fn_007c6bf0(D(0xb74350),p,(u8)home); }
 home=(home&0xffffff00u)|65;
 threshold=(s16)((20-(s16)C(p+0x56))*10);
 for(j=1;j<=64;j++) {
  home=(home&0xffffff00u)|j;
  z=(s16)cm_fn_007c6930(p,home);
  if(z && z>=threshold) {
   n=((s32)z-threshold)/75; c=n>10?10:(s8)n;
   cm_fn_007c6a30(D(0xb74350),p,(u8)home,c,1);
  }
 }
 home=(home&0xffffff00u)|65;
 B(p+0x3d)=11; B(p+0x28)=11; a=Q+11; B(a)&=0xcf;
 a=D(p+0x1a);
 if(a && !B(a+0x11c) && I(a+0x88)<350) {
  v=R(D(a+0x88)); k=R(1000);
  if((s32)v<(s32)k) age=(s8)(R(5)+23); else age=(s8)(R(7)+17);
  home=(home&0xffffff00u)|(u8)age;
  a=P;
  if(age>20){lo=0x96a5d8;hi=0x9689e0;}
  else if(age>26){lo=0x968918;hi=0x968e48;}
  else if(age>23){lo=0x968fb0;hi=0x96a688;}
  else {lo=0x968e48;hi=0x9689e0;}
  f=(long double)S(a+7)*(long double)F(lo); v=R(ft(f));
  g=(long double)(s32)v; f=(long double)S(a+7)*(long double)F(hi);
  W(a+5)=ft(g+f);
  a=P; v=R(750); W(a+9)=v+(u16)(W(a+5)*30u)+250;
  a=P;
  if(S(a+7)>140 && S(a+5)>100 && S(a+9)<6500 && age>18 && age<22 && R(4)) {
   a=P; W(a+9)=ft((long double)S(a+9)*(long double)F(0x968978));
  }
  if(age>18) {
   a=P; z=S(a+5);
   if(z<120) {v=R(200); W(a+13)=v+(u16)(W(a+5)*15u)+1;}
   else if(age>21 && z>120) {a=P;v=R(200);W(a+13)=v+(u16)(W(a+5)+10)*25u;}
   else {a=P;v=R(200);W(a+13)=v+(u16)(W(a+5)*20u)+(z<140?1:250);}
  } else {
   a=P; z=S(a+5); v=R(200);
   if(z<140) W(a+13)=v+(u16)(W(a+5)*5u)+1;
   else W(a+13)=v+(u16)(W(a+5)+25)*10u;
  }
  if(S(P+9)<2250){v=R(5);W(P+13)=v+1;}
  a=P; z=S(a+13); if(S(a+9)<z){W(a+9)=(u16)z+10;W(P+13)=1;}
  a=P;v=R(50);W(a+11)=v+W(a+9);
  if(age>23) {
   a=P; if(S(a+9)<6000) W(a+9)=ft((long double)S(a+9)*F(0x969588));
   a=P; if(S(a+11)<6000) W(a+11)=ft((long double)S(a+11)*F(0x969588));
   a=P; z=S(a+5); v=ft((long double)z*F(0x96a5d0));
   if(z<(s16)v && R(4)){a=P;W(a+5)=ft((long double)S(a+5)*F(0x96a5d0));}
  } else if(age<20) {
   a=P;v=ft((long double)S(a+7)*F(0x968968));
   if(S(a+5)<(s16)v && R(4)){a=P;W(a+5)=ft((long double)S(a+7)*F(0x968968));}
   if(S(P+5)>120 && R(age<18?150:50)) W(P+5)=120;
  } else {
   a=P;if(S(a+9)<5750)W(a+9)=ft((long double)S(a+9)*F(0x968928));
   a=P;if(S(a+11)<5750)W(a+11)=ft((long double)S(a+11)*F(0x968928));
   a=P;v=ft((long double)S(a+7)*F(0x968e48));
   if(S(a+5)<(s16)v && R(4)){a=P;W(a+5)=ft((long double)S(a+7)*F(0x968e48));}
   if(S(P+5)>140 && R(50)) W(P+5)=140;
  }
 } else {
  a=P;
  if(B(a+0x15)==20){lo=0x968e48;hi=0x968e80;}
  else if(B(a+0x11)==20 || B(a+0x10)==20){a=P;lo=0x96a5d8;hi=0x9689e0;}
  else {c=B(a+15);a=P;lo=c==20?0x968968:0x968e48;hi=0x9689e0;}
  v=R(ft((long double)S(a+7)*F(lo)));
  W(a+5)=ft((long double)(s32)v+(long double)S(a+7)*F(hi));
  v=R(250);a=P;
  if(v){v=R(750);W(a+9)=v+(u16)(W(a+5)*20u)+250;a=P;v=R(20);W(a+13)=v+(u16)(W(a+5)*5u)+1;}
  else {v=R(750);W(a+9)=v+(u16)(W(a+5)*15u)+125;W(P+13)=1;}
  a=P;z=S(a+13);if(S(a+9)<z){W(a+9)=(u16)z+10;W(P+13)=1;}
  a=P;v=R(50);W(a+11)=v+W(a+9);
  v=R(10000);k=0;
  if(!v){age=(s8)(R(4)+15); k=age==15?80:100;}
  else if((s32)v<5000){age=(s8)(R(3)+16);k=age<18?80:100;}
  else if((s32)v>9975){age=(s8)(R(6)+21);}
  else {age=(s8)(R(4)+17);k=age==17?90:110;}
  home=(home&0xffffff00u)|(u8)age;
  if(k && S(P+5)>(s16)k && R(k==80||k==90?250:150)) W(P+5)=k;
 }
 if(!(u8)cm_fn_00549ea0(D(p+0x1a))) {
  if(age<25 && S(P+5)<120) W(P+13)=1;
  else {a=P;W(a+13)=ft((long double)S(a+13)*F(0x96a5d0));}
 }
 W(p+18)=W(0xae2c92)-(s16)age;
 v=R(365); W(p+16)=v;
 v=cm_fn_0053ab90(p,0xae2c90); W(p+24)=(s16)(s8)v;
 a=P;
 if(S(a+7)>100 && (C(a+21)>=15 || C(a+20)>=15)) {
  if(R(3)) v=R(11)+10;
  else if(!R(3)) v=R(7)+14;
  else v=R(16)+5;
 } else {if(R(4))v=R(10)+8;else v=R(16)+5;}
 B(P+27)=v;
 a=P;c=C(a+27);
 if(c>15){v=R(5);B(a+27)-=(u8)v;}
 else if(c<5){a=P;v=R(10);B(a+27)+=(u8)v;}
 else if(c<10){a=P;v=R(8);B(a+27)+=(u8)v;}
 c=C(P+27);
 if(c>15)v=R(11)+10;
 else if(c>10)v=R(10)+8;
 else if(c<5)v=R(10)+1;
 else if(R(4))v=R(16)+5;
 else v=R(10)+5;
 B(P+54)=v;
 if(B(P+15)==20 && R(5)) {
  v=R(20);if(S(P+7)>(s32)(v+100))v=R(11)+10;else v=R(16)+5;
  B(P+29)=v;
 } else {
  v=R(20);B(P+29)=v+1;a=P;c=C(a+29);
  if(c>15){v=R(6);B(a+29)-=(u8)v;}
  else if(c<5){a=P;v=R(6);B(a+29)+=(u8)v;}
 }
 a=P;c=-5;
 if(C(a+17)>=15){c=0;if(C(a+25)>=15)c=5;}
 else if(((C(a+19)>=15||C(a+18)>=15)&&C(a+25)>=15)||C(a+15)>=15||C(a+21)>=15)c=0;
 v=R((s32)c*3);k=R(20);B(P+46)=v+k;
 a=P;c=C(a+46);B(a+46)=c<1?1:c>20?20:c;
 v=R(20);B(P+52)=v+1;c=C(P+52);
 if(c>15){a=P;v=R(6);B(a+52)-=(u8)v;}
 else if(c<8){a=P;v=R(10);B(a+52)+=(u8)v;}
 v=R(20);B(P+68)=v+1;
 if(C(P+68)>15 && R(3)){a=P;v=R(5);B(a+68)-=(u8)v;}
 else {c=C(P+68);if(c<5){a=P;v=R(10);B(a+68)+=(u8)v;}else if(c<10){a=P;v=R(5);B(a+68)+=(u8)v;}}
 if(age<18){v=R(20);B(P+60)=v+1;a=P;c=C(a+60);if(c>15){v=R(8);B(a+60)-=(u8)v;}else if(c<10){a=P;v=R(c<5?8:6);B(a+60)+=(u8)v;}}
 else if(age<24){v=R(18);B(P+60)=v+3;a=P;c=C(a+60);if(c>15){v=R(5);B(a+60)-=(u8)v;}else if(c<10){a=P;v=R(c<5?10:5);B(a+60)+=(u8)v;}}
 else {v=R(15);B(P+60)=v+6;a=P;c=C(a+60);if(c>15){v=R(3);B(a+60)-=(u8)v;}else if(c<10){a=P;v=R(8);B(a+60)+=(u8)v;}}
 a=P;if(C(a+60)<10 && C(a+68)>15){v=R(5);B(a+60)+=(u8)v;}
 v=R(20);B(P+61)=v+1;a=P;c=C(a+61);
 if(c>15){v=R(age<18?10:age<24?8:5);B(a+61)-=(u8)v;}
 else if(c<5){a=P;v=R(age<18?6:age<24?8:10);B(a+61)+=(u8)v;}
 a=P;if((B(a+17)==20||B(a+18)==20)&&C(a+61)<10){a=P;v=R(5);B(a+61)+=(u8)v;}
 a=P;if(C(a+46)>15&&C(a+61)<10){v=R(5);B(a+61)+=(u8)v;}
 if(age<24) {
  a=P;v=R(age<18?40:20);B(a+30)-=(u8)v;
  a=P;v=R(age<18?40:20);B(a+36)-=(u8)v;
  c=C(P+44);
  if(c>15 || (age<18&&c>10)){a=P;v=R(age<18?(c>15?8:5):5);B(a+44)-=(u8)v;}
  a=P;c=C(a+47);
  if(c>10){if(c<=15)a=P;v=R(age<18?(c>15?10:8):(c>15?8:5));B(a+47)-=(u8)v;}
  a=P;if(C(a+47)>15){v=R(8);B(a+47)-=(u8)v;}
 } else if(age>25 && age<29) {
  a=P;if(C(a+61)<5){v=R(8);B(a+61)+=(u8)v;}
  a=P;if(C(a+33)<5){v=R(8);B(a+33)+=(u8)v;}
  a=P;if(C(a+60)<5){v=R(8);B(a+60)+=(u8)v;}
  a=P;v=R(10);B(a+30)-=(u8)v;
  a=P;v=R(10);B(a+36)-=(u8)v;
  a=P;if(C(a+44)<5){v=R(8);B(a+44)+=(u8)v;}
  c=C(P+47);if(c>10){a=P;v=R(c>15?5:3);B(a+47)-=(u8)v;}
 }
 a=D(p+0x1a);
 if(age<19 && a && B(a+0x11c) && R(4)) {
  a=cm_fn_007ad0e0(self,p);
  if(a && (u8)cm_fn_007a6f10(p,a)){
   if(cm_fn_007875e0(a))cm_fn_007a8ba0(p,a);
   if(S(P+7)>80){a=D(a+0xbf);if(a&&D(a+0x69)){v=R(100);t=D(a+0x69);if(C(t+22)>(s32)v)cm_fn_00547a10(a,p,0);}}
  }
 } else {
  a=D(p+0x1a);
  if((a&&!B(a+0x11c)&&R(15)) || !R(10)){
   a=cm_fn_007ad590(self,p);if(a)cm_fn_007a6f10(p,a);
  }
 }
 if(!D(p+0x39)&&(s8)home<20){
  a=P;if(S(a+11)>1250)W(a+11)=1250;
  a=P;if(S(a+9)>1250)W(a+9)=1250;
  W(P+13)=1;
 }
 cm_fn_00538ec0(p);cm_fn_00538bc0(p);
}
