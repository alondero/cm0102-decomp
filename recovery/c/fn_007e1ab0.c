#include "cmre.h"
/* Preferred-image VA 007e1ab0/base00400000, CM0102 3.9.68 SHA256
 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 L is ESP after sub esp,a0: amount1/mode2/sign3, final record4/ctor8,
 saved lookup18/self1c, date20/28, record30/ctor34/44, copied data50.
 Native callees remain boundaries. Scratch bytes remain uninitialized except
 original stores. EBX is fully set to -1 before use; no incoming EBX input.
 Full incidental EAX is retained because there are no indexed direct callers. */
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

u32 CM_THISCALL cm_fn_007e1ab0(u32 self,u32 input,u32 second)
{
 volatile struct {u32 prev,handler,state;} frame;
 u8 scratch[0xa0];
 u32 l=(u32)scratch,eax,lookup,key,receiver,base,src,dst,i,edx;
 u8 value,amount,mode,flag;
 frame.state=0xffffffffu;frame.handler=0x00960426;
 eax=CM_FS(volatile u32,0);frame.prev=eax;
 CM_FS(volatile u32,0)=(u32)&frame;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 D(l+0x1c)=self;B(l+1)=0;B(l+2)=0xff;
 if(value==0){value=B(input+0x2d);eax=(eax&0xffffff00u)|value;if(value==0)goto done;}
 key=D(input);eax=cm_fn_007e82e0(self,key,1);lookup=eax;
 value=B(input+0x2d);eax=(eax&0xffffff00u)|value;
 D(l+0x18)=lookup;
 if(value==1){
  eax=cm_fn_0054a110(l+0x34);frame.state=0;
  eax=cm_fn_0054a110(l+0x44);frame.state=1;
  eax=cm_fn_007e8e10(self,l+0x30,input,1);
  if(eax!=0){
   eax=cm_fn_0054c5b0(0x00ae2c90,l+0x28,1);
   *(volatile u8 *)&frame.state=2;
   eax=(u32)cm_fn_00417c10(l+0x44,l+0x20,eax);
   src=second;dst=l+0x50;
   B(l+0x3c)=0x17;B(l+0x3d)=0xff;B(l+0x3e)=0xff;B(l+0x3f)=0xff;
   edx=D(l+0x1c);
   for(i=0;i<0x13;i++){D(dst)=D(src);dst+=4;src+=4;}
   W(dst)=W(src);dst+=2;src+=2;
   receiver=D(edx+7);*(volatile u8 *)&frame.state=1;B(dst)=B(src);
   eax=(u32)cm_fn_0068d0d0(receiver,edx,l+0x30,0x70);
   lookup=D(l+0x18);
  }
  B(l+1)=1;B(l+2)=2;B(l+3)=1;frame.state=0xffffffffu;
 }
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 if(value!=0){
  value=(u8)(B(lookup+4)+1u);B(lookup+4)=value;
  eax=(u32)value/5u;
  if((u32)value%5u==0){
   amount=B(l+1);B(l+2)=1;amount++;B(l+3)=5;B(l+1)=amount;
  }
 }
 value=B(l+1);eax=(eax&0xffffff00u)|value;
 if(value==0)goto done;
 eax=cm_fn_0054a110(l+8);
 amount=B(l+1);frame.state=3;
 B(l+4)=amount;W(l+6)=0xffff;B(l+5)=0;
 eax=(u32)cm_fn_00417c10(l+8,l+0x20,0x00ae2c90);
 mode=B(l+2);flag=B(l+0x17);value=B(l+3);
 B(l+0x14)=mode;mode=B(input+0x48);flag&=0xfd;
 W(l+0x15)=(u16)(s16)(s8)value;
 flag=(u8)((flag&0xfb)|((mode==0)?4:0));
 key=D(input);B(l+0x17)=flag;receiver=D(0x00ae28f0);
 B(l+0x10)=0x17;B(l+0x11)=0xff;B(l+0x12)=0xff;B(l+0x13)=0xff;
 eax=cm_fn_00556940(receiver,key,l+4);
 key=D(input);base=D(0x00ae23bc);receiver=D(0x00ae28f0);
 eax=cm_fn_00557280(base+key*110u,l+4,second);
done:
 receiver=frame.prev;CM_FS(volatile u32,0)=receiver;return eax;
}
