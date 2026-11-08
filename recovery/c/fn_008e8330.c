/* Native routine recovery; original helpers remain explicit boundaries.
   Preferred-image VA 0x008e8330, base 0x00400000, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#pragma GCC target("no-sse3")
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern long long CM_THISCALL cm_fn_00598800(u32,u32,u32,u32,u32,u32);
u32 CM_THISCALL cm_fn_008e8330(u32 self,u32 person,u32 unused,volatile u32 club)
{
 u32 eax=D(person+0x61u), old,financial,result,original_club;
 s32 upper,lower,rep,skill,q,diff;
 long long total;
 long double retained,t;
 (void)unused;
 if(!eax)return eax;
 upper=(s16)W(eax+7u);lower=(s16)W(eax+5u);
 if(upper-lower<25)return eax;
 eax=(eax&0xffffff00u)|B(self+0x3fu);
 if((u8)eax)return eax;
 eax=D(self+0x3au);if(eax)return eax;
 eax=D(self+0x35u);if(eax)return eax;
 original_club=club;
 financial=D(0xae2a38u);
 financial=cm_fn_005a0590(financial,original_club);
 old=D(self+0x1bu);
 /* 0x008e8398: after four saves and five pushes, ESP+0x30 is
    entry ESP+12, the actual third argument slot; EBX keeps the club. */
 club=old;
 total=cm_fn_00598800(financial,0,0,0,1,0);
 eax=old+old;
 if(total>(long long)(s32)eax)return eax;
 rep=(s16)W(original_club+0x80u);
 if(rep<15){
  eax=D(person+0x61u);skill=(s16)W(eax+0xbu);q=skill/500;
  eax=(u32)rep;diff=q-rep;
  if(diff>2){
   t=(long double)(s32)club*(long double)Q(0x9689b0u);
   result=(u32)(long long)t; /* original __ftol 0x008e83f8 */
   D(self+0x1bu)=result;B(self+0x3fu)=20;return result;
  }
 }
 eax=D(person+0x1au);
 if(eax && (s16)W(eax+0x8eu)>5000){
  eax=D(person+0x61u);
  if(eax && (s16)W(eax+0xbu)>4250){
   eax=(eax&0xffffff00u)|B(person+0x22u);
   if(!(u8)eax){
    retained=(long double)(s32)club;
    t=retained*(long double)Q(0x968e80u);
    result=(u32)(long long)t; /* original __ftol 0x008e843f */
    D(self+0x3au)=result;B(self+0x3eu)=10;
    goto final_conversion;
   }
  }
 }
 eax=D(person+0x61u);skill=(s16)W(eax+0xbu);
 /* Native signed multiply-high, SAR5 and sign correction implement /500.
    Native EAX at the no-store return is the pre-correction sign bit. */
 q=skill/500;eax=skill<0?1u:0u;
 diff=q-rep;
 if(diff>=-3)return eax;
 retained=(long double)(s32)club;
 t=retained*(long double)Q(0x968e80u);
 result=(u32)(long long)t; /* original __ftol 0x008e847b */
 D(self+0x35u)=result;B(self+0x39u)=20;
final_conversion:
 t=retained*(long double)Q(0x9689b0u);
 result=(u32)(long long)t; /* original __ftol 0x008e848d */
 D(self+0x1bu)=result;return result;
}
