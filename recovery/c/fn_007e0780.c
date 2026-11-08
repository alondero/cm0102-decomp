#include "cmre.h"
/* Preferred-image VA 007e0780, base 00400000, CM0102 3.9.68.
 SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 L is ESP after sub esp,9c: amount2/mode3, final record4/ctor8,
 saved lookup18, date copy1c/24, record2c/ctor30/40, copied data4c.
 Scratch remains uninitialized except explicit original stores. Native callees
 remain boundaries; full incidental EAX is returned (no indexed callers). */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e0780(u32 self,u32 input,u32 second)
{
 volatile struct {u32 prev,handler,state;} frame;
 u8 scratch[0x9c];
 u32 l=(u32)scratch,eax,lookup,key,p,receiver,base,src,dst,i,edx;
 unsigned long long pair;
 u8 value,bl,mode,flag;
 frame.state=0xffffffffu;frame.handler=0x00960326;
 eax=CM_FS(volatile u32,0);frame.prev=eax;
 CM_FS(volatile u32,0)=(u32)&frame;
 B(l+2)=0;B(l+3)=0xff;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 if(value==0){value=B(input+0x2d);eax=(eax&0xffffff00u)|value;if(value==0)goto done;}
 key=D(input);eax=cm_fn_007e82e0(self,key,1);lookup=eax;
 value=B(input+0x2d);eax=(eax&0xffffff00u)|value;
 D(l+0x18)=lookup;
 if(value==1){
  eax=cm_fn_0054a110(l+0x30);frame.state=0;
  eax=cm_fn_0054a110(l+0x40);frame.state=1;
  eax=cm_fn_007e8e10(self,l+0x2c,input,1);
  if(eax!=0){
   eax=cm_fn_0054c5b0(0x00ae2c90,l+0x24,1);
   *(volatile u8 *)&frame.state=2;
   pair=cm_fn_00417c10(l+0x40,l+0x1c,eax);eax=(u32)pair;edx=(u32)(pair>>32);
   src=second;dst=l+0x4c;
   B(l+0x38)=0x2a;B(l+0x39)=0xff;B(l+0x3a)=0xff;B(l+0x3b)=0xff;
   for(i=0;i<0x13;i++){D(dst)=D(src);dst+=4;src+=4;}
   W(dst)=W(src);dst+=2;src+=2;
   *(volatile u8 *)&frame.state=1;
   receiver=D(self+7);B(dst)=B(src);
   eax=(u32)cm_fn_0068d0d0(receiver,edx,l+0x2c,0x70);
   lookup=D(l+0x18);
  }
  B(l+2)=1;B(l+3)=2;bl=1;frame.state=0xffffffffu;
 }else bl=0xff;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 if(value!=0){
  value=(u8)(B(lookup+4)+value);eax=(eax&0xffffff00u)|value;
  B(lookup+4)=value;
  if(value>=3 && ((value-3u)&1u)==0){
   mode=B(l+2);B(l+3)=1;mode++;
   bl=(u8)((value==3)+2);B(l+2)=mode;
  }
 }
 value=B(l+2);eax=(eax&0xffffff00u)|value;
 if(value==0)goto done;
 eax=cm_fn_0054a110(l+8);
 value=B(l+2);frame.state=3;
 B(l+4)=value;W(l+6)=0xffff;B(l+5)=0;
 eax=(u32)cm_fn_00417c10(l+8,l+0x1c,0x00ae2c90);
 value=B(input+0x48);mode=B(l+3);W(l+0x15)=(u16)(s16)(s8)bl;
 flag=B(l+0x17);flag&=0xfd;B(l+0x14)=mode;
 flag=(u8)((flag&0xfb)|((value==0)?4:0));
 key=D(input);p=l+4;B(l+0x17)=flag;
 receiver=D(0x00ae28f0);
 B(l+0x10)=0x2a;B(l+0x11)=0xff;B(l+0x12)=0xff;B(l+0x13)=0xff;
 eax=cm_fn_00556940(receiver,key,p);
 key=D(input);base=D(0x00ae23bc);receiver=D(0x00ae28f0);
 eax=cm_fn_00557280(base+key*110u,l+4,second);
done:
 receiver=frame.prev;CM_FS(volatile u32,0)=receiver;return eax;
}
