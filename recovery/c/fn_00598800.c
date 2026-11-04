/* WIP: case 3 x87 precision mismatch: original __ftol at 0x00598D1F
   converts 117724152; compiled long-double producer converts 117724151.
   Native earlier __ftol restores CW 0; compiled casts use FISTTP without
   FLDCW. Incoming precision behavior needs independent resolution. */
/* Preferred-image VA 0x00598800; image base 0x00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5 / RET 0x14, EDX:EAX result. Original callees remain boundaries.
   ADR 0006 casts name each original __ftol call. Arithmetic uses unsigned
   representations for the original wrapping integer operations. */
#include "cmre.h"
typedef unsigned long long U64;
typedef long long I64;
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define Q(a) CM_GLOBAL(volatile double, (a))
#define F(a) CM_GLOBAL(volatile float, (a))
#define SX(a) ((U64)(I64)(s32)(a))
extern u32 CM_CDECL cm_fn_005444a0(u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_008ccb80(u32,u32,u32 *);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ca300(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern U64 CM_CDECL cm_fn_009452ca(u32);
extern U64 CM_STDCALL cm_fn_00945e80(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005afde0(u32,u32 *);
extern u8 CM_THISCALL cm_fn_005b02f0(u32,u32);

long long CM_THISCALL cm_fn_00598800(u32 self,u32 person,u32 include,u32 output,u32 adjust,u32 extra)
{
  u32 index = D(self+8), low = D(self), base = D(0xae23b4);
  u32 high = D(self+4), club = base + index*0x245u;
  U64 total = ((U64)high<<32)|low;
  u32 accumulated=0, offsets=0, list=0, list2=0;
  u32 a,b,c,n,j,k,rec,competition,owner,rank,kind,threshold;
  s32 factor, rating, count;
  s8 factor_byte, code;
  long double retained,t,ratio;
  double factor_q, tenth;
  U64 converted, delta, sum;
  if (include) {
    rec=club+0xd7;
    for (n=50;n;n--,rec+=4) {
      a=D(rec);
      if (a && (s32)D(a)<(s32)D(0xae2364) && cm_fn_005444a0(a,0))
        total+=SX(D(a+0x52));
    }
  }
  if (output) D(output)=0;
  if (extra && cm_fn_00600a50(club,1,1,0,0)) {
    a=D(0xde1f54);
    count=(s8)cm_fn_008ccb80(a,club,&list);
    if(count>0) {
      rec=list;
      for(n=0;n<(u32)count;n++,rec+=0x1b) {
        if(D(rec+0x10)) {
          code=(s8)B(rec+0x19);
          if(code==3||code==4||code==6||code==5) {
            j=0; b=0;
            if((s32)(n*0x1bu)>0) {
              a=list+8;
              do {
                if(b) break;
                c=D(a);
                if(c==D(rec+8)) b=1;
                j++; a+=0x1b;
              } while((s32)j<(s32)n);
            }
            if(!b) accumulated-=D(rec+0x14);
            base=D(0xae23b4); index=D(self+8);
            a=base+index*0x245u;
            b=D(list+j*0x1bu+8);
            c=D(0xae23bc)+b*0x6eu;
            a=cm_fn_004cf5f0(D(0xae1050),c,a);
            if(a) offsets-=D(a+0xc);
            rec=list+n*0x1bu;
          }
        } else if(!person || D(rec+8)!=D(person)) {
          accumulated+=D(rec+0x14);
        }
      }
    }
    if(list) cm_fn_009452ca(list);
    count=(s8)cm_fn_004ca300(D(0xae1050),club,(u32)&list2);
    if(count>0) for(n=0;n<(u32)count;n++) {
      rec=list2+n*0x6fu;
      if(B(rec+0x50)!=4) {
        a=D(rec);
        b=D(0xae23bc)+a*0x6eu;
        a=cm_fn_004cf5a0(D(0xae1050),b);
        base=D(0xae23b4);
        D(0xae1080)=a;
        b=a ? base+D(a+4)*0x245u : 0;
        index=D(self+8); c=base+index*0x245u;
        if(b==c) {
          a=D(rec); b=D(0xae23bc)+a*0x6eu;
          a=cm_fn_004cf5f0(D(0xae1050),b,c);
          if(a) offsets-=D(a+0xc);
        }
        offsets+=D(rec+0xc);
        accumulated+=D(rec+8);
      }
    }
    if(list2) cm_fn_009452ca(list2);
  }
  competition=D(club+0x57);
  if(!competition) return (I64)total;
  a=D(competition); b=D(0xadadfc);
  if(!D(b+a*4u)) return (I64)total;
  a=D(club+0xbf);
  if(a && (b=D(a+0x69))!=0) {
    factor_byte=(s8)(B(b+0xf)+6u);
    code=(s8)B(a+0x57);
    if(code>15) factor_byte=(s8)((u8)factor_byte+15u-(u8)code);
  } else factor_byte=16;
  owner=D(club+0x53);
  if(owner && D(owner)==D(0x9cf314)) factor_byte=(s8)((u8)factor_byte+5u);
  factor=factor_byte;
  /* 598B67 FILD survives every intervening cast until 598D4C FSTP. */
  retained=(long double)factor;
  factor_q=(double)retained; /* 598B70 FST qword; does not pop. */
  a=D(self+0xc)+offsets;
  t=(long double)(s32)a;
  t-=(long double)(s32)D(self+0x1c)*(long double)Q(0x96a670);
  t*=retained;
  converted=(U64)(I64)t; /* __ftol 0x00598B89 */
  total-=converted;
  a=D(club+0x69);
  if(!a) total-=SX((u32)factor*2500u);
  else if(!B(club+0x6d)) total-=SX(D(a+0x3c)*(u32)factor*2u);
  else {
    rating=(s16)W(club+0x80);
    if(rating>0x1676) {
      b=D(a+0x3c)*(u32)factor;
      t=(long double)(s32)b*(long double)Q(0x96a5c8);
      total+=(U64)(I64)t; /* __ftol 0x00598BCE */
    } else if(rating>0x109a) {
      b=D(a+0x3c)*(u32)factor;
      t=(long double)(s32)b*(long double)Q(0x96a668);
      total+=(U64)(I64)t; /* __ftol 0x00598BF4 */
    } else if(rating>0x4e2 && B(club+0x64)==1) {
      b=D(a+0x3c)*(u32)factor;
      t=(long double)(s32)b*(long double)Q(0x96a660);
      total+=(U64)(I64)t; /* __ftol 0x00598C20 */
    } else total-=SX(D(a+0x3c)*(u32)factor);
  }
  code=(s8)B(club+0x7f);
  if(code<6) a=(u32)(s32)code*(u32)factor*200u;
  else if(code<16) a=((u32)(s32)code-2u)*(u32)factor*500u;
  else a=((u32)(s32)code-12u)*(u32)factor*2000u;
  t=(long double)(s32)a*(long double)Q(0x9689e0);
  converted=(U64)(I64)t; /* __ftol 0x00598CB4; 598CBD CDQ replaces EDX. */
  rank=W(club+0x80); rating=(s16)rank;
  total-=SX((u32)converted);
  total-=SX((u32)rating*(u32)factor);
  a=D(club+0x73)*(u32)rating;
  t=(long double)(s32)a*retained;
  t*=(long double)Q(rating<4000 ? 0x96a658 : 0x96a350);
  total+=(U64)(I64)t; /* __ftol 0x00598D1F; retained popped at 598D4C. */
  a=D(0x9cf2e4); b=D(0x9cf360); c=D(0x9cf300);
  if(owner) {
    kind=D(owner);
    j=(u32)(s32)(s16)W(competition+0x69);
    j*=j;
    if(kind==a||kind==b||kind==D(0x9cf488)) j*=5000u;
    else if(kind==D(0x9cf314)||kind==c) j*=2500u;
    else j*=750u;
    total+=SX(j);
  }
  /* 598DEC/598E07 FILD and FLD ST0 leave the square below the cast. */
  code=(s8)B(club+0x6d);
  j=(u32)rating*(u32)rating;
  retained=(long double)(s32)j;
  t=retained*(long double)factor_q;
  t*=(long double)Q(code ? 0x96a350 : 0x96a658);
  total+=(U64)(I64)t; /* __ftol 0x00598E17 */
  if(D(club+0x1db)!=0xffffffffu && B(club+0x64)==1) {
    j=(u32)rating*300u;
    if(!owner) j=0;
    else {
      kind=D(owner);
      if(kind!=a && kind!=D(0x9cf314) && kind!=b && kind!=c &&
         kind!=D(0x9cf45c) && kind!=D(0x9cf488)) {
        if(kind==D(0x9cf338)||kind==D(0x9cf4a8)||kind==D(0x9cf244)||
           kind==D(0x9cf434)||kind==D(0x9cf4dc)) {
          t=(long double)(s32)j*(long double)Q(0x96a5d0);
          j=(u32)(I64)t; /* __ftol 0x00598EFD; 598F0A CDQ. */
        } else if(kind==D(0x9cf2c0)||kind==D(0x9cf408)) {
          t=(long double)(s32)j*(long double)Q(0x969510);
          j=(u32)(I64)t; /* __ftol 0x00598EEC; 598F0A CDQ. */
        } else {
          t=(long double)(s32)j*(long double)Q(0x968968);
          j=(u32)(I64)t; /* __ftol 0x00598EDB; 598F0A CDQ. */
        }
      }
    }
    total+=SX(j);
  }
  a=D(self+0x110); b=D(self+0x118); c=D(self+0x114); j=D(self+0x11c);
  delta=(((U64)c<<32)|a)-(((U64)j<<32)|b);
  /* JS/JG inspect SBB flags; the sign branch precedes the signed JG. */
  if((s32)(delta>>32)<0 || delta==0) a=0; else a=(u32)delta;
  delta=SX(a)-8000000u;
  a=(I64)delta>=0 ? (u32)delta : 0;
  b=(s32)accumulated>0 ? accumulated : 0;
  sum=SX(b)+SX(a);
  total-=SX((u32)sum); /* 598F92 CDQ discards sum's computed high word. */
  if((I64)total>0) {
    a=D(self); b=D(self+4);
    delta=((U64)b<<32)|a;
    t=(long double)(I64)delta*(long double)Q(0x967880);
    tenth=(double)t; /* 598FD0 FSTP qword rounds before comparison. */
    if(!((long double)(I64)total >= (long double)tenth)) {
      /* 598FE3 discards retained square. C0 includes unordered. */
      if(!((long double)tenth >= (long double)Q(0x96a650))) {
        total=(U64)(I64)(long double)tenth; /* __ftol 0x00598FFA */
      } else {
        total=(U64)(I64)(long double)Q(0x96a650); /* __ftol 0x00599012 */
      }
    } else {
      t=retained*(long double)Q(0x968968);
      threshold=(u32)(I64)t; /* __ftol 0x0059902A */
      c=D(club+0xcf);
      if(c && (s16)rank!=0 && (c=D(c+0x69))!=0) {
        ratio=(long double)(s16)W(c+0xa)/(long double)rating;
        if(!(ratio >= (long double)Q(0x967890))) {
          if(!(ratio >= (long double)Q(0x968968))) ratio=(long double)F(0x968f18);
          t=(long double)(s32)threshold*ratio;
          t*=ratio;
          threshold=(u32)(I64)t; /* __ftol 0x0059908C */
        }
      }
      k=0;
      if(B(club+0x82)||!owner) k=1;
      else {
        kind=D(owner);
        if(kind==D(0x9cf4a8)||kind==D(0x9cf408)||kind==D(0x9cf4f8)||
           kind==D(0x9cf434)||kind==D(0x9cf2c0)||kind==D(0x9cf2fc)||
           kind==D(0x9cf45c)||kind==D(0x9cf4dc)||kind==D(0x9cf314)) k=1;
      }
      if(owner && D(owner)!=D(0x9cf314)) {
        if((I64)SX(threshold)<(I64)(delta-10000000u)) threshold=D(self)-10000000u;
        if(k && (I64)SX(threshold)>(I64)((U64)40000000u-sum)) threshold=40000000u-(u32)sum;
      } else {
        if((I64)SX(threshold)<(I64)(delta-15000000u)) threshold=D(self)-15000000u;
        if(k && (I64)SX(threshold)>(I64)((U64)35000000u-sum)) threshold=35000000u-(u32)sum;
      }
      if((s32)threshold<50000) threshold=50000;
      if((I64)total>(I64)SX(threshold)) {
        t=(long double)(I64)(total-SX(threshold))*(long double)Q(0x967880);
        t+=(long double)(s32)threshold;
        total=(U64)(I64)t; /* __ftol 0x00599205 */
      }
    }
  } /* 599214 discards retained square when total is not positive. */
  a=D(0xb67a44);
  if(a && club) {
    b=D(club); c=D(a);
    total+=SX(D(c+b*0x49u+0x28));
  }
  if((s16)rank>5000) {
    t=(long double)rating;
    a=(u32)(s32)(s16)W(competition+0x69);
    t*=(long double)Q(0x96a648);
    t-=(long double)Q(0x9688d8);
    t*=(long double)(s32)a;
    t*=(long double)Q(0x96a640);
    converted=(U64)(I64)t; /* __ftol 0x00599291 */
    if((I64)total<(I64)converted) {
      total=converted;
      if((I64)total<50000) total=50000;
    }
  }
  if((s16)rank<3500) {
    a=B(club+0x64)==1 ? (u32)rating*500u : (u32)rating*250u;
    if((I64)total>(I64)SX(a)) total=SX(a);
  }
  if((I64)total<0) total=0;
  if(owner) {
    kind=D(owner);
    if(kind==D(0x9cf4a8)||kind==D(0x9cf408)||kind==D(0x9cf2c0)||
       kind==D(0x9cf510)||kind==D(0x9cf4f8)||kind==D(0x9cf2fc)||
       kind==D(0x9cf2dc)||kind==D(0x9cf3e4)||kind==D(0x9cf218)) {
      total=cm_fn_00945e80((u32)total,(u32)(total>>32),2,0);
      if((I64)total>2500000) {
        t=(long double)(I64)(total-2500000u)*(long double)Q(0x967880);
        t+=(long double)Q(0x96a650);
        total=(U64)(I64)t; /* __ftol 0x0059949F */
      }
    } else if(kind==D(0x9cf244)||kind==D(0x9cf434)) {
      if((I64)total>15000000) {
        t=(long double)(I64)(total-15000000u)*(long double)Q(0x9689e0);
        t+=(long double)Q(0x96a630);
        total=(U64)(I64)t; /* __ftol 0x0059949F */
      }
    } else if(kind==D(0x9cf260)) {
      if((I64)total>2500000) {
        t=(long double)(I64)(total-2500000u)*(long double)Q(0x9689e0);
        t+=(long double)Q(0x96a650);
        total=(U64)(I64)t; /* __ftol 0x0059949F */
      }
    } else if(kind==D(0x9cf314)||kind==D(0x9cf300)||kind==D(0x9cf45c)||kind==D(0x9cf338)) {
      if((I64)total>17500000) {
        t=(long double)(I64)(total-17500000u)*(long double)Q(0x9689e0);
        t+=(long double)Q(0x96a638);
        total=(U64)(I64)t; /* __ftol 0x0059949F */
      }
    }
  }
  if(person && adjust && B(0x9cc270) && D(0xae2a60) && cm_fn_00600a50(club,0,1,0,0)) {
    a=D(club+0xcf); b=D(0xae2a60);
    a=cm_fn_005afde0(b,(u32 *)a);
    if(a) {
      code=(s8)cm_fn_005b02f0(a,person);
      if(code==3) threshold=(u32)total;
      else {
        if(code==2) {
          a=(u32)(s32)(s16)W(club+0x80);
          ratio=(long double)Q(0x968a90);
        } else {
          a=D(club+0x53);
          if(a && (s8)B(a+0x85)<9 && (s16)(b=W(club+0x80))<4250) {
            a=(u32)(s32)(s16)b;
            ratio=(long double)Q(code==1 ? 0x967898 : 0x968ef0);
          } else {
            a=(u32)(s32)(s16)W(club+0x80);
            ratio=(long double)Q(code==1 ? 0x968a98 : 0x967898);
          }
        }
        t=(long double)(s32)a;
        t*=t; t*=ratio;
        threshold=(u32)(I64)t; /* __ftol 0x005995FD; duplicate popped 599602. */
      }
      a=D(club+0xcf);
      if(a && (a=D(a+0x69)) && (b=W(club+0x80))!=0) {
        ratio=(long double)(s16)W(a+0xa)/(long double)(s16)b;
        if(ratio>(long double)Q(0x967890)) ratio=(long double)Q(0x967890);
        else if(!(ratio >= (long double)Q(0x967880))) ratio=(long double)Q(0x967880);
        t=(long double)(s32)threshold*ratio;
        threshold=(u32)(I64)t; /* __ftol 0x00599676 */
      }
      if(D(club+0x57)) {
        a=D(club+0x53);
        t=(long double)(s8)B(a+0x85)*(long double)Q(0x967888);
        if(!(t >= (long double)Q(0x9689e0))) t=(long double)Q(0x9689e0);
        else if(t>(long double)Q(0x967890)) t=(long double)Q(0x967890);
        t=(long double)(s32)threshold*t;
        threshold=(u32)(I64)t; /* __ftol 0x005996D8 */
      }
      a=D(self+4);
      if((s32)a<0 && (b=W(club+0x80))!=0) {
        a=D(person+0x61);
        if(a) {
          t=(long double)(s16)W(a+0xb)/(long double)(s16)b;
          if(!(t >= (long double)Q(0x968f20))) t=(long double)Q(0x968f20);
          else if(t>(long double)Q(0x969520)) t=(long double)Q(0x969520);
        } else t=(long double)(s32)threshold;
        t*=(long double)Q(0x96a5d0);
        threshold=(u32)(I64)t; /* __ftol 0x00599763 */
      }
      if(code!=3 && (I64)total>(I64)SX(threshold)) {
        t=(long double)(I64)(total-SX(threshold))*(long double)Q(0x96a628);
        a=(u32)(I64)t; /* __ftol 0x00599796 */
        b=threshold-a; total=SX(b);
        if((I64)total>125000) {
          t=(long double)(s32)threshold*(long double)Q(0x968f98);
          a=(u32)(I64)t; /* __ftol 0x005997BC; 5997C1 CDQ. */
          if((I64)total>(I64)SX(a)) total=SX(a);
        }
        if(output) D(output)=1;
      }
    }
  }
  return (I64)total;
}
