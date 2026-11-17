#include "cmre.h"
#include "cmre_x87.h"
/* All addresses below are preferred-image VAs in CM0102 3.9.68,
 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 Boundary mappings follow the ECX setups and reversed PUSH sequences in
 006c6a30--006c8759. __ftol sites are expressed by FTO below. */
#define U(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define C(a) CM_GLOBAL(volatile s8,(u32)(a))
#define W(a) CM_GLOBAL(volatile s16,(u32)(a))
#define F(a) CM_GLOBAL(volatile float,(u32)(a))
#define D(a) CM_GLOBAL(volatile double,(u32)(a))
#define FTO(a) ((u32)(long long)(long double)(a))
/* 006c6abc PUSH 500, CALL 0090cfc0; ADD ESP,8 after two calls.
   006c6c05 MOV ECX,EDI; CALL 00717410, no pushes.
   006c6f20 MOV ECX,[ESI+f582]; CALL 007163c0, EDX consumed at
   006c6f2f and EAX consumed at 006c7192. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern unsigned long long CM_THISCALL cm_fn_007163c0(u32);
extern u32 CM_THISCALL cm_fn_00717410(u32);
extern u8 CM_THISCALL cm_fn_0070dfa0(u32);
extern u8 CM_THISCALL cm_fn_0070df40(u32);
extern u8 CM_THISCALL cm_fn_0070df20(u32);
extern u8 CM_THISCALL cm_fn_0070def0(u32);
extern u8 CM_THISCALL cm_fn_006cb780(u32,u32);
extern u8 CM_THISCALL cm_fn_006f4650(volatile u8 *);
extern float CM_THISCALL cm_fn_006f09b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006f9f90(u32,u32,u32,u32,float,float,u8 *);
extern u32 CM_THISCALL cm_fn_00897dc0(u32,u32,s8,u32);
extern u32 CM_THISCALL cm_fn_006e1cd0(u32,u32);
extern u32 CM_THISCALL cm_fn_006f5b20(u32,u32);
extern u8 CM_THISCALL cm_fn_006bce80(u32,u32,u32,u32,u32);
extern u8 CM_THISCALL cm_fn_00713e90(u32,u32,u32,u32,u32,u32,float,u32);
extern float CM_THISCALL cm_fn_006fa1e0(u32);
extern u8 CM_THISCALL cm_fn_006cb9b0(u32,volatile u8 *,s32,s32);
extern u8 CM_THISCALL cm_fn_006cbad0(u32,u32);
extern u8 CM_THISCALL cm_fn_006bd5b0(u32,u32,u8);
extern u32 CM_CDECL cm_fn_00547840(u32,u32);
extern u32 CM_CDECL cm_fn_00547810(u32,u32);
#define RNG(n) cm_fn_0090cfc0((u32)(n))
#define FLAGS(p) cm_fn_007163c0(p)
#define TEST(p) cm_fn_00717410(p)
#define ACT U(self+0xf582)
#define SIDE C(self+0x8eae)
#define SBYTE(off) (*(s8 *)(scratch+(off)))
#define BYTE(off) (*(u8 *)(scratch+(off)))
#define WORD(off) (*(u32 *)(scratch+(off)))
static int zone(s8 x,s8 y,int side) {
 return x>=2 && x<=6 && (side ? (y>=0 && y<=1):(y>=10 && y<=11));
}
u32 CM_THISCALL cm_fn_006c6a30(u32 self,u32 x,u32 y,u8 *a,u8 *b,
 u8 *c,u8 *d,u32 *score,u32 mode)
{
 u8 scratch[0x8c];
 u32 actor=ACT, person,item,id,first,second,p,q,i,j,result=0;
 float scale=0.2f,base=5000.0f,dist,mind;
 u8 z0,z1,z2,z3,changed,eligible;
 s8 xx,yy,step,count,side;
 u32 rating,v0,v1,v2,v3,v4,v5,role,club,flag,t,n;
 long double h,k,l;
 B(self+0x8eb2)=0;
 person=U(actor+0x69); item=U(person+0x1a);
 if(item) {
  id=U(item);
  if(id==U(0x9cf2e4)||id==U(0x9cf45c)||id==U(0x9cf510)||
     id==U(0x9cf3e4)||id==U(0x9cf2dc)||id==U(0x9cf408)) scale=1;
  else if(id==U(0x9cf4a8)||id==U(0x9cf2c0)||id==U(0x9cf314)) scale=0.6f;
 }
 first=RNG(500); actor=ACT;
 person=((u32)(s32)W(actor+0x29)-1000)*3;
 second=cm_fn_0090cfc0(500);
 U(score)=person-first+second;
 if(!cm_fn_0070dfa0(ACT)&&cm_fn_006cb780(self,ACT)==1&&
    cm_fn_006f4650((volatile u8 *)ACT)) return 0;
 side=SIDE;
 z0=zone((s8)x,(s8)y,side!=0);
 z1=zone((s8)x,(s8)y,side!=1);
 actor=ACT;
 BYTE(0x60)=B(actor+0x103);
 BYTE(0x64)=B(actor+0x102);
 t=WORD(0x64);
 q=self+0x4796+(side==0)*0x22d8;
 for(i=0;i<20;i++,q+=0x1be) {
  if(C(q+0x19)<0) continue;
  if(TEST(q)&1) continue;
  h=cm_fn_006f09b0(q,t,WORD(0x60));
  if(h==D(0x9689a0)||h!=h) {base=0;break;}
  if(h<base||h!=h) base=(float)h;
 }
 p=self+0x4796+(u32)(s32)SIDE*0x22d8;
 for(j=0;j<20;j++,p+=0x1be) {
  if(C(p+0x19)<0) continue;
  actor=ACT; if(p==actor) continue;
  n=(u32)(s32)C(actor+0x19);
  if((s32)n>=0&&(s32)n<=10) {
   t=U(self+0x8ebc+(u32)(s32)C(actor+0x27)*4);
   if(W(t+n*2)==1) {
    side=SIDE;
    if(side==0&&(s8)y>C(p+0x103)) continue;
    if(side==1&&(s8)y<C(p+0x103)) continue;
   }
  }
  side=SIDE;
  BYTE(0x70)=B(p+0x103); BYTE(0x74)=B(p+0x102);
  mind=5000;
  q=self+0x4796+(side==0)*0x22d8;
  for(i=0;i<20;i++,q+=0x1be) {
   if(C(q+0x19)<0) continue;
   if(TEST(q)&1) continue;
   h=cm_fn_006f09b0(q,WORD(0x74),WORD(0x70));
   if(h==D(0x9689a0)||h!=h) {mind=0;break;}
   if(h<mind||h!=h) mind=(float)h;
  }
  BYTE(0x18)=0;
  if((u8)mode!=0x36&&(u8)mode!=0x37) {
   BYTE(0x78)=B(p+0x103); BYTE(0x7c)=B(p+0x102);
   if(!(u8)cm_fn_006f9f90(ACT,WORD(0x7c),WORD(0x78),1,base,mind,
      scratch+0x18)) continue;
  }
  n=(u32)(s32)C(p+0x19); role=0;
  if((s32)n>=0&&(s32)n<=10) {
   t=U(self+0x8ebc+(u32)(s32)C(p+0x27)*4);
   role=(u16)W(t+n*2);
  }
  /* __ftol 006c6e5b, 006c6ecd. */
  h=(long double)mind*mind; h*=F(0x969580);
  h=(long double)F(0x968db0)-h; rating=FTO(h);
  if(cm_fn_0070df40(p)) rating-=1500;
  else if(cm_fn_0070dfa0(p)) {
   side=SIDE; yy=C(p+0x103);
   rating+=((side==0&&yy<3)||(side!=0&&yy>8))?10000:3000;
  }
  v0=0;
  if((u16)role!=1) {
   WORD(0x64)=(u32)(s32)W(p+0x35);
   h=(long double)F(p+0x4d)+(s32)WORD(0x64);
   h-=F(0x968da8);h*=F(0x96d22c);v0=FTO(h);
  }
  if(!cm_fn_0070dfa0(p)) {
   eligible=cm_fn_006cb780(self,p);
   n=eligible==1?2000:eligible==2?4000:0;
   rating-=n;v0+=n;
   if((FLAGS(ACT)>>32)&1) {if(!n)n=500;v0+=n;rating-=n;}
  }
  q=U(self+0x4792);v3=v4=v5=0;
  t=U(q+0x14);v1=0;
  if(t) v1=((u32)(s32)W(t+0x69)-12)*25;
  v2=0-v1*4; n=0;
  side=C(p+0x27);
  flag=B(q+(side?0x41:0x40))==0;
  club=U(q+(side?0x20:0x1c));
  t=cm_fn_00897dc0(U(0xde1d08),club,0,flag);
  if(U(p+0x69)==t) v0+=1000;
  else {
   side=C(p+0x27);q=U(self+0x4792);
   flag=B(q+(side?0x41:0x40))==0;club=U(q+(side?0x20:0x1c));
   t=cm_fn_00897dc0(U(0xde1d08),club,1,flag);
   if(U(p+0x69)==t) {
    side=C(p+0x27);q=U(self+0x4792);
    flag=B(q+(side?0x41:0x40))==0;club=U(q+(side?0x20:0x1c));
    t=cm_fn_00897dc0(U(0xde1d08),club,0,flag);
    actor=ACT;
    q=self+0x9032+(u32)(s32)C(actor+0x27)*208;
    t=cm_fn_006e1cd0(q,t);
    if(!t||C(t+0x19)<0) v0+=1000;
   } else {
    /* __ftol 006c7107 */
    h=F(p+0xf1);h*=h;h*=D(0x96d410);
    t=FTO(h);v1-=t;n=0-t;
   }
  }
  if((u8)cm_fn_006f5b20(p,C(p+0x27)==0)) {
   /* __ftol 006c7146,006c7157 */
    k=F(p+0x99);
    h=(long double)F(p+0xe5)*F(p+0xc1);h*=D(0x96d410);
    t=FTO(h);h=k;h*=h;h*=D(0x9688a0);
   t=FTO(h)-t;v1+=t;n+=t;
  }
  if(!cm_fn_0070df40(ACT)) {
   t=(u32)FLAGS(ACT);
   if(t&1) {
    t=U(self+0x9766+(u32)(s32)SIDE*0x18e3);
    if(t&8) {v1-=1000;v2+=1000;}
    else if(t&4) {v1-=750;v2+=500;}
    else if(t&2) {v1+=250;v2-=250;}
   } else if((u32)FLAGS(ACT)&16) {v1-=1000;v2+=1000;}
   else if((u32)FLAGS(ACT)&8) {v1-=750;v2+=500;}
   else if((u32)FLAGS(ACT)&4) {v1+=250;v2-=250;}
  }
  changed=0;
  if((u16)role!=1) {
    t=(u32)(s32)C(p+0x104);actor=ACT;
    t*= (u32)(s32)C(p+0x105);
    t*= (u32)(s32)C(actor+0x10b);t*=38;
    /* 006c728c rounds the threshold to a qword before RNG. */
    k=(double)((long double)(s32)t*D(0x967880));
   t=RNG(rating*6);
   WORD(0x64)=t;
    if(!((long double)(s32)t>k) &&
      cm_fn_006bce80(self,p,(u32)(scratch+0x14),(u32)(scratch+0x15),0)) {
    if(SBYTE(0x14)<0) BYTE(0x14)=0;
    else if(SBYTE(0x14)>8) BYTE(0x14)=8;
    if(SBYTE(0x15)<0) BYTE(0x15)=0;
    else if(SBYTE(0x15)>11) BYTE(0x15)=11;
    xx=C(p+0x102);
    if(SBYTE(0x14)!=xx||SBYTE(0x15)!=C(p+0x103)) changed=1;
    BYTE(0x40)=BYTE(0x14);BYTE(0x2c)=BYTE(0x15);
    if(changed) {
     mind=5000;q=self+0x4796+(SIDE==0)*0x22d8;
     for(i=0;i<20;i++,q+=0x1be) {
      if(C(q+0x19)<0)continue;
      if(TEST(q)&1)continue;
      h=cm_fn_006f09b0(q,WORD(0x40),WORD(0x2c));
      if(h==D(0x9689a0)||h!=h) {mind=0;break;}
      if(h<mind||h!=h)mind=(float)h;
     }
     if(TEST(p)&0x40)
      v0+=(u32)(s32)C(p+0x104)*(u32)(s32)C(p+0x105)*50;
    }
   } else goto unchanged;
  } else {
unchanged:
   BYTE(0x40)=B(p+0x102);BYTE(0x2c)=B(p+0x103);
  }
  side=C(p+0x27);yy=C(p+0x103);
  step=(s8)((side==1 ? B(self+0x8eb0)-yy : yy-B(self+0x8eaf))+1);
  if(step>-2) {
   t=U(self+0x9766+(side==0)*0x18e3);
   if(t&0x400) {
    actor=ACT;
    if(B(actor+0x106)) {xx=C(p+0x107);if((s32)xx<(s32)RNG(50))step++;}
   }
  }
  if((u8)mode==0x36)step=-4;
  else if(step>=0) {
   step=(s8)(step*step+1);xx=C(p+0x107);
   if((s32)xx<(s32)RNG(((s32)step*5+5)*2)) {
    actor=ACT;
    t=(u32)(s32)C(actor+0x10b)*400+500;
    v0-=t*(u32)(s32)step*(u32)(s32)step;
   }
  }
  side=SIDE;xx=SBYTE(0x40);yy=SBYTE(0x2c);
  z2=zone(xx,yy,side!=0);z3=zone(xx,yy,side!=1);
  {s32 dx=(s8)x-xx,dy=(s8)y-yy;
   s8 ax=(s8)(dx<0?-dx:dx),ay=(s8)(dy<0?-dy:dy);
   if(ax>=0&&ax<=8&&ay>=0&&ay<=11)dist=F(0xa15e44+((u32)ay*9+(u32)ax)*4);
   else dist=1000;
   WORD(0x64)=(u32)(s32)(s8)y;
   WORD(0x60)=(u32)dy;
  }
  count=0;q=self+0x4796+(SIDE==0)*0x22d8;
  for(i=0;i<20;i++,q+=0x1be) {
   if(C(q+0x19)<0)continue;
   if((u16)TEST(q)==1)continue;
   h=cm_fn_006f09b0(q,WORD(0x40),WORD(0x2c));
   if(h==D(0x9689a0)||h!=h)count+=3;
   else if(h<=D(0x968940))count++;
  }
  v0+=(5-(u32)(s32)count)*400;
  if(z2&&count<3&&step<1) {
   /* _CIpow 006c76ae, __ftol 006c76b9 */
   h=cm_x87_CIpow((long double)(3-count),(long double)D(0x968930));
   h*=D(0x9688a8);v0+=FTO(h);
  }
  actor=ACT;
  eligible=cm_fn_00713e90(actor,0x5d,x,y,WORD(0x40),WORD(0x2c),dist,(u32)(scratch+0x1a));
  if(B(actor+0x2c)!=eligible)v0-=RNG((u32)(s32)C(actor+0x2d)*400);
  if((u8)mode==0x34||(u8)mode==0x37) {
   if(z2) {
    h=cm_fn_006fa1e0(p);h*=F(0x96d2dc);h=(long double)F(0x96d40c)-h;
    h+=(long double)F(p+0xad)*F(0x968960);
    v0+=FTO(h);
    } else if(!(dist>=D(0x968930))&&(count==0||count<D(0x968930))) {
    h=(long double)F(p+0x9d)*F(0x968960);
    h+=(long double)F(p+0x99)*F(0x969560);
    h+=(long double)F(p+0x8d)*F(0x969560);
    h+=F(count==0?0x96d408:0x96d404);v0+=FTO(h);
   }
  }
  if(mind>D(0x968940)&&!z2) {
   actor=ACT;t=(u32)(s32)C(actor+0x106)+5;
   h=(long double)(s32)(t*(0u-700))/scale;v2+=FTO(h);
   h=(long double)(s32)(t*(0u-500))/scale;v3=FTO(h);
  } else if(!changed) {
   actor=ACT;
   h=(long double)(s32)(30-(u32)(s32)C(actor+0x10b));
   h*= (long double)F(0x968a78)-F(p+0xad);h*=scale;
   k=h*F(0x96d400);v2+=FTO(k);h*=F(0x96d3fc);v3=FTO(h);
  }
  if((u16)role==1) {
   if(!cm_fn_0070dfa0(ACT)&&RNG(100000)!=0)continue;
   if(!cm_fn_0070def0(ACT)) {
    actor=ACT;h=(s32)((u32)(s32)C(actor+0x106)*(0u-1000));
    h*=(long double)mind+D(0x967880);v0+=FTO(h);
   }
   actor=ACT;t=(u32)(s32)C(actor+0x106);
   v3+=0u-1000-t*2200;v2+=0u-2000-t*5000;n-=t*1500;
  }
  actor=ACT;t=(u32)(s32)C(actor+0x19);
  if((s32)t>=0&&(s32)t<=10) {
   q=U(self+0x8ebc+(u32)(s32)C(actor+0x27)*4);
   if(W(q+t*2)==1)v0-=1000;
  }
  if(z1) {
    if(!(mind>=D(0x968950))&&(u16)TEST(actor)==1&&cm_fn_0070dfa0(p))v0-=2000;
   if(z3) {
    actor=ACT;h=C(actor+0x106);h*=mind;h*=mind;h*=F(0x96d3f8);v0+=FTO(h);
   }
  }
  if(z3) {
   h=(s32)((u32)(s32)C(actor+0x106)*32+200);h*=mind;v0-=FTO(h);
  }
  /* Pinned image/hash above, preferred-image VAs: 006c6a34 saves ECX
     in ESI; 006c7aae loads ACT; 006c7ab4--006c7ab6 push y, x, ACT;
     006c7ab7 restores ECX=self before CALL 006c7ab9 to 006cb9b0.
     Preserve the receiver as well as the three stack arguments. */
  eligible=cm_fn_006cb9b0(self,(volatile u8 *)ACT,(s32)WORD(0x40),(s32)WORD(0x2c));
  if(eligible) {
   actor=ACT;h=C(actor+0x106);
   k=(long double)F(p+0xad)*F(0x968960);
   l=(long double)F(actor+0x8d)*F(0x968960);
   v4=FTO((k+F(0x96d1a8))*h*F(0x96afc4)+l);
   v5=FTO(((long double)F(0x96d408)-k)*h*F(0x96afc4)+l);
   if((FLAGS(actor)>>32)&4) {
    actor=ACT;h=(long double)F(actor+0xe1)*F(0x968960)+F(0x96d1a8);
    t=FTO(h);v4+=t;v5+=t;
   }
   v2-=2000;n-=2000;v1-=2000;
  }
  if((u32)FLAGS(ACT)&0x40) {
   side=SIDE;
   if((side==0&&C(p+0x102)<=2)||(side==1&&C(p+0x102)>=6))v0+=1000;
  }
  if((u32)FLAGS(ACT)&0x100) {
   side=SIDE;
   if((side==0&&C(p+0x102)>=6)||(side==1&&C(p+0x102)<=2))v0+=1000;
  }
  if(((u32)FLAGS(ACT)&0x80)&&!cm_fn_0070df20(p)&&!eligible)v0+=1000;
  if(z0&&!z2) {mind=0;v0-=4000;}
  else if(!z0&&z2)mind=(float)((long double)mind*mind+D(0x967890));
  else {
   side=SIDE;
   if((((s8)x>2)&&(z0||(side==1&&(s8)y<3)||(side==0&&(s8)y>8))&&
       ((s8)x>=6||z0)&&((side==1&&yy<3)||(side==0&&yy>8))&&xx<2)||
      (((s8)x<6)&&(z0||(side==1&&(s8)y<3)||(side==0&&(s8)y>8))&&
       ((s8)x<=2||z0)&&((side==1&&yy<3)||(side==0&&yy>8))&&xx>6)) {
    v0-=2000;mind=0;
   }
  }
  side=SIDE;
  if(((side==0&&(s8)y>=yy)||(side==1&&(s8)y<=yy))&&cm_fn_0070dfa0(p)) {
   v2-=5000;n-=3000;v3-=3000;
  }
  side=SIDE;
  if((side==0&&(s8)y>yy)||(side==1&&(s8)y<yy)) {
   s32 delta=side==0?(s32)WORD(0x60):(s32)yy-(s8)y;
   h=(long double)mind-((long double)delta-D(0x968968));
   mind=(float)h;
   if(h<D(0x9689a0)||h!=h) {
    h=(long double)mind*mind*F(0x96d3f4);v0+=FTO(h);mind=0;
   }
  } else {
   count=0;q=self+0x4796+(u32)(s32)side*0x22d8;
   for(i=0;i<20;i++,q+=0x1be)
    if(C(q+0x19)>=0&&((side==1&&C(q+0x103)<yy)||(side==0&&C(q+0x103)>yy)))count++;
   if(!count)mind=(float)((long double)mind*2+D(0x967890));
   else if((xx<2||xx>6)&&!cm_fn_0070dfa0(p)&&((FLAGS(p)>>32)&4))
    mind=(float)((long double)mind*D(0x968940)+D(0x967890));
  }
  if(step>=0) {
   h=(long double)mind-step;mind=(float)h;
   if(h<D(0x9689a0)||h!=h) {
    h=(long double)mind*mind*F(0x96d3f0);v0+=FTO(h);mind=0;
   }
  }
  h=(long double)mind*mind*F(0x968960);v0+=FTO(h);
  side=SIDE;
   if(((side==0&&(s8)y>=yy)||(side==1&&(s8)y<=yy))&&!(mind>=D(0x96d3e8))&&
     !((side==1&&yy<3)||(side==0&&yy>8))&&
     (actor=ACT,(B(actor+0x106)||B(actor+0x10b)))) {
   h=mind;
   if(cm_fn_0070def0(p)){h*=F(0x96d208);h=(long double)F(0x96d3e4)-h;}
   else {h*=F(0x96d210);h=(long double)F(0x96d2dc)-h;}
   t=(u32)(s32)(s16)FTO(h);side=SIDE;
   if(side==0)v0-=(WORD(0x60)+1)*t;
   else if(side==1)v0-=((u32)(s32)yy-WORD(0x64)+1)*t;
   } else if(!(mind>=F(0x9689a8))&&cm_fn_0070dfa0(p))v0-=1500;
  else if((u16)role!=1) {
   actor=ACT;h=(s32)((u32)(s32)C(actor+0x10b)+10);h*=mind;h*=mind;v0+=FTO(h);
   side=SIDE;
   if((side==1&&yy<3)||(side==0&&yy>8)) {
    if(cm_fn_006cbad0(self,p)) {actor=ACT;v0+=((u32)(s32)C(actor+0x10b)+10)*20;}
    else if(cm_fn_006bd5b0(self,p,0)) {actor=ACT;v0+=((u32)(s32)C(actor+0x10b)+10)*50;}
   } else if(!cm_fn_0070dfa0(p)) {
    if(cm_fn_006cbad0(self,p)) {actor=ACT;v0+=80+(u32)(s32)C(actor+0x10b)*8;}
    else if(cm_fn_006bd5b0(self,p,0)) {actor=ACT;v0+=((u32)(s32)C(actor+0x10b)+10)*20;}
   }
   side=SIDE;
   if((side==0&&yy>(s8)y)||(side==1&&yy<(s8)y)) {
    actor=ACT;h=(s32)((u32)(s32)C(actor+0x10b)+10);h*=mind;h*=mind;v0+=FTO(h);
   }
  }
  t=(u32)(s32)C(p+0x2b);
  if((s32)t>1){actor=ACT;v0-=((u32)(s32)C(actor+0x10b)+4)*t*t*50;}
  t=RNG((u32)(s32)SBYTE(0x18));BYTE(0x18)=(u8)t;
  t=(u32)(s32)(s8)t;t*=t;
  v3-=t*155;v1-=t*500;n-=t*155;v2-=t*25;
  actor=ACT;q=U(actor+0x69);t=RNG((u32)(s32)C(q+0x5b)+1);
  if(!t) {
   actor=ACT;q=U(actor+0x69);club=U(p+0x69);
   if(cm_fn_00547840(club,q))continue;
   actor=ACT;q=U(actor+0x69);club=U(p+0x69);
   if(cm_fn_00547810(club,q))v0+=1000;
  }
  {s8 command=-1;u32 value=0xfffe7960;
   if((s8)mode<=-1) {
    s8 m=C(self+0x8ea9);
    if(m==4) {if(dist>F(0x9689a8))continue;value=v0;command=0x3d;goto update;}
    actor=ACT;h=(long double)F(0x96d3e0)-F(actor+0xd9);
    h*=D(0x96a7a8);h=(long double)F(0x968990)-h;
    if(h>=dist&&dist>=F(0x96d228)){value=v2;command=0x1e;}
    if(eligible&&dist<=F(0x968990)&&dist>=F(0x968e70)) {
     if((s32)v4>(s32)value){value=v4;command=8;}
     if((s32)v5>(s32)value){value=v5;command=7;}
    }
    if(!(dist>F(0x9689a8))&&(s32)v1>(s32)value&&m<3){value=v1;command=11;}
    if(dist<=F(0x96d228)&&dist>=F(0x969584)&&(s32)n>(s32)value){value=n;command=60;}
    if(dist<=F(0x969584)&&dist>=D(0x968940)&&(s32)v3>(s32)value){value=v3;command=31;}
    value+=v0;
   } else {
    /* Original byte-map and target-table reads at 006c8342/006c8348. */
    u32 target=0x6c858a;
    if((u32)(s32)(s8)mode-6<=0x54) {
     t=B(0x6c8788+(u32)(s32)(s8)mode-6);target=U(0x6c875c+t*4);
    }
    command=(s8)mode;
    switch(target) {
     case 0x6c8510:if(dist<=F(0x968990)&&dist>=F(0x968e70))value=v4+v0;break;
     case 0x6c8544:if(dist<=F(0x968990)&&dist>=F(0x968e70))value=v5+v0;break;
      case 0x6c84ec:if(!(dist>F(0x9689a8)))value=v1+v0;break;
     case 0x6c8384:case 0x6c83d5:
      actor=ACT;h=(long double)F(0x96d3e0)-F(actor+0xd9);
      h*=D(0x96a7a8);h=(long double)F(0x968990)-h;
      if(h>=dist&&dist>=F(0x96d228))value=v2+v0;
      else if(target==0x6c83d5)value=v1+v0;
      break;
     case 0x6c8426:if(dist<=F(0x969584)&&dist>=D(0x968940))value=v3+v0;break;
     case 0x6c8462:if(dist<=F(0x96d228)&&dist>=F(0x969584))value=n+v0;break;
     case 0x6c849f:
      value=v1+v0;
      if(dist<=D(0x96d298)&&dist>=D(0x968930)&&xx>1&&xx<7&&(yy<3||yy>8))value=n+v0;
      break;
     case 0x6c834f:
      actor=ACT;h=(long double)F(actor+0xe9)*D(0x967880)+F(0x968e70);
      if(h>=dist)value=v0;
      break;
      case 0x6c8579:if(!(dist>F(0x9689a8)))value=v0;break;
     default:value=v0;break;
    }
   }
   if(command<=-1)continue;
update:
   if((s32)value>(s32)U(score)) {
    U(score)=value;B(a)=(u8)command;result=p;B(b)=changed;
    B(c)=BYTE(0x40);B(d)=BYTE(0x2c);
   }
  }
 }
 return result;
}
