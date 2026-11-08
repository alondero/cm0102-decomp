#include "cmre.h"
/* Preferred-image VA007de330/base00400000, CM0102 3.9.68 SHA256
 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 THIS2/RET8/full incidental EAX; L is ESP after SUB ESP,9c.
 amount L2/sign L3, record L4/ctor L8, saved self L18, dates L1c/L24,
 record L2c/ctors L30/L40 and copied bytes L4c. Unknown bytes stay unknown.
 OR BL,ff defines the only consumed byte; EBX high bits are unused then restored.
 Native68d0d0 receives EDX=L2c from the original LEA, same as its source.
 Native callees and SEH delivery remain explicit boundaries. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007e8e10(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u8);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007de330(u32 self,u32 input,u32 second)
{
 volatile struct {u32 prev,handler,state;} frame;
 u8 scratch[0x9c];
 u32 l=(u32)scratch,eax,key,receiver,base,src,dst,i,edx;
 u8 value,mode=0xff,flag,cl;
 frame.state=0xffffffffu;frame.handler=0x009600c6;
 eax=CM_FS(volatile u32,0);frame.prev=eax;
 CM_FS(volatile u32,0)=(u32)&frame;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 D(l+0x18)=self;B(l+2)=0;
 if(value==0){value=B(input+0x2d);eax=(eax&0xffffff00u)|value;if(value==0)goto done;}
 key=D(input);eax=cm_fn_007e82e0(self,key,1);
 cl=B(input+0x2c);
 if(cl!=0){
  value=(u8)(B(eax+4)+1u);B(eax+4)=value;
  eax=(u32)value/5u;
  if(value%5u==0){B(l+2)=1;mode=1;B(l+3)=5;}
 }
 if(B(input+0x2d)==1){
  eax=cm_fn_0054a110(l+0x30);frame.state=0;
  eax=cm_fn_0054a110(l+0x40);frame.state=1;
  eax=cm_fn_007e8e10(self,l+0x2c,input,1);mode=2;
  if(eax!=0){
   eax=cm_fn_0054c770(0x00ae2c90,l+0x24,1);
   *(volatile u8 *)&frame.state=mode;
   eax=(u32)cm_fn_00417c10(l+0x40,l+0x1c,eax);
   src=second;dst=l+0x4c;
   B(l+0x38)=0xff;B(l+0x39)=0x0e;B(l+0x3a)=0xff;B(l+0x3b)=0xff;
   eax=D(l+0x18);
   for(i=0;i<0x13;i++){D(dst)=D(src);dst+=4;src+=4;}
   receiver=D(eax+7);edx=l+0x2c;W(dst)=W(src);dst+=2;src+=2;
   *(volatile u8 *)&frame.state=1;B(dst)=B(src);
   eax=(u32)cm_fn_0068d0d0(receiver,edx,l+0x2c,0x70);
  }
  value=B(l+2);eax=(eax&0xffffff00u)|value;B(l+3)=1;
  value++;eax=(eax&0xffffff00u)|value;frame.state=0xffffffffu;B(l+2)=value;
 }
 value=B(l+2);eax=(eax&0xffffff00u)|value;
 if(value==0)goto done;
 eax=cm_fn_0054a110(l+8);
 value=B(l+2);B(l+4)=value;frame.state=3;W(l+6)=0xffff;B(l+5)=0;
 eax=(u32)cm_fn_00417c10(l+8,l+0x1c,0x00ae2c90);
 value=B(l+3);cl=B(input+0x2d);W(l+0x15)=(u16)(s16)(s8)value;
 flag=B(l+0x17);B(l+0x14)=mode;flag&=0xfd;
 if(cl!=0){cl=B(input+0x2c);B(l+0x10)=0x0d;if(cl!=0)B(l+0x10)=0xff;}
 else B(l+0x10)=0xff;
 cl=B(input+0x48);flag=(u8)((flag&0xfb)|((cl==0)?4:0));
 key=D(input);B(l+0x17)=flag;receiver=D(0x00ae28f0);
 B(l+0x11)=0x0e;B(l+0x12)=0xff;B(l+0x13)=0xff;
 eax=cm_fn_00556940(receiver,key,l+4);
 key=D(input);base=D(0x00ae23bc);receiver=D(0x00ae28f0);
 eax=cm_fn_00557280(base+key*110u,l+4,second);
done:
 receiver=frame.prev;CM_FS(volatile u32,0)=receiver;return eax;
}
