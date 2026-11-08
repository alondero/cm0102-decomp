#include "cmre.h"
/* Preferred-image VA007e24c0/base00400000, CM0102 3.9.68 SHA256
 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 ECX forwarded to native7e82e0; RET8. Only BL is observed before saved EBX
 restoration. L is ESP after sub esp,28: amount3, date4/c, record14/ctor18.
 Scratch remains uninitialized except original writes. Native callees remain
 boundaries. Full incidental EAX is retained. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e24c0(u32 self,u32 input,u32 second)
{
 volatile struct {u32 prev,handler,state;} frame;
 u8 scratch[0x28];
 u32 l=(u32)scratch,eax,key,receiver,base,esi;
 u8 value,cl,dl,bl=0xff,low,flags;
 u16 word;
 frame.state=0xffffffffu;frame.handler=0x009604b0;
 eax=CM_FS(volatile u32,0);frame.prev=eax;
 CM_FS(volatile u32,0)=(u32)&frame;
 B(l+3)=0;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 if(value==0){value=B(input+0x2d);eax=(eax&0xffffff00u)|value;if(value==0)goto done;}
 key=D(input);eax=cm_fn_007e82e0(self,key,1);
 if(B(input+0x2d)==1){
  cl=B(input+0x2c);
  if(cl==0)cl=B(input+0x2e);
  else if(cl==1)cl=B(input+0x2f);
  else cl=0xff;
  B(l+3)=1;bl=2;esi=1;
  if(cl!=0xff){
   if(cl==9)W(eax+8)=(u16)(W(eax+8)+5u);
   else if((s8)cl<12)W(eax+8)=(u16)(W(eax+8)+12u);
   else W(eax+8)=(u16)(W(eax+8)+32u);
  }
 }else esi=input;
 cl=B(input+0x2c);dl=0;
 if(cl!=0){
  do{
   cl=B(input+((dl==0)?0x2e:0x2f));
   if((s8)cl<=3)W(eax+8)=(u16)(W(eax+8)+2u);
   else if((s8)cl<=8)W(eax+8)=(u16)(W(eax+8)+3u);
   else if(cl==9)W(eax+8)=(u16)(W(eax+8)+5u);
   else if(cl==10)W(eax+8)=(u16)(W(eax+8)+12u);
   else W(eax+8)=(u16)(W(eax+8)+32u);
   ++dl;cl=B(input+0x2c);
  }while((s32)(s8)dl<(s32)cl);
 }
 if(W(eax+8)>=16){
  cl=B(eax+0xa);esi=0;dl=(cl>>3)&7;low=cl&7;
  do{
   cl=B(eax+0xb);bl=cl&7;
   if(bl<7){bl=(u8)((((u8)(cl+1u)^cl)&7)^cl);B(eax+0xb)=bl;}
   cl=B(eax+0xb);bl=low;
   W(eax+8)=(u16)(W(eax+8)-16u);
   cl=(u8)((cl&7)+bl);bl=B(l+3);cl=(u8)(cl+dl);esi+=16u;
   bl=(u8)(bl+cl);word=W(eax+8);B(l+3)=bl;
  }while(word>=16);
  bl=0;
 }
 value=B(l+3);eax=(eax&0xffffff00u)|value;
 if(value==0)goto done;
 eax=cm_fn_0054a110(l+0x18);
 value=B(input+0x2d);eax=(eax&0xffffff00u)|value;
 dl=B(l+3);frame.state=0;B(l+0x14)=dl;
 W(l+0x16)=0xffff;B(l+0x15)=0;
 if(value!=0)eax=cm_fn_00417c10(l+0x18,l+4,0x00ae2c90);
 else{
  eax=cm_fn_0054c5b0(0x00ae2c90,l+4,14);
  *(volatile u8 *)&frame.state=1;
  eax=cm_fn_00417c10(l+0x18,l+0xc,eax);
  *(volatile u8 *)&frame.state=0;
 }
 flags=B(l+0x27);value=B(input+0x48);flags&=0xfd;
 flags=(u8)((flags&0xfb)|((value==0)?4:0));
 key=D(input);B(l+0x27)=flags;receiver=D(0x00ae28f0);
 B(l+0x24)=bl;W(l+0x25)=(u16)esi;
 B(l+0x20)=0x18;B(l+0x21)=0xff;B(l+0x22)=0xff;B(l+0x23)=0xff;
 eax=cm_fn_00556940(receiver,key,l+0x14);
 key=D(input);base=D(0x00ae23bc);receiver=D(0x00ae28f0);
 eax=cm_fn_00557280(base+key*110u,l+0x14,second);
done:
 receiver=frame.prev;CM_FS(volatile u32,0)=receiver;return eax;
}
