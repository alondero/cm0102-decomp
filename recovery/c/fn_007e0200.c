#include "cmre.h"
/* Preferred-image VA 0x007e0200, image base 0x00400000; CM0102 3.9.68
 SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 Incoming ECX is forwarded unchanged to 7e82e0. RET8. Full incidental EAX.
 L is the original base after sub esp,20h: amount L+2, mode L+3,
 date-copy L+4, record L+c, constructor L+10. Unwritten bytes stay unknown.
 Original callees remain explicit boundaries. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00556940(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00557280(u32,u32,u32);

u32 CM_THISCALL cm_fn_007e0200(u32 self, u32 input, u32 second)
{
 volatile struct {u32 prev,handler,state;} frame;
 u8 scratch[0x20];
 u32 l=(u32)scratch, eax, p, key, receiver, base;
 u8 delta, value, bl, flags, mode;
 frame.state=0xffffffffu; frame.handler=0x009602c8;
 eax=CM_FS(volatile u32,0); frame.prev=eax;
 CM_FS(volatile u32,0)=(u32)&frame;
 B(l+2)=0;B(l+3)=0xff;
 value=B(input+0x2c);eax=(eax&0xffffff00u)|value;
 if(value==0){value=B(input+0x2d);eax=(eax&0xffffff00u)|value;if(value==0)goto done;}
 key=D(input);eax=cm_fn_007e82e0(self,key,1);
 delta=B(input+0x2c);
 if(delta!=0){
  p=eax;value=(u8)(B(p+4)+delta);B(p+4)=value;
  eax=value;
  eax&=1u;
  if(value%2u==0){B(l+2)=1;B(l+3)=1;bl=2;}
  else bl=(u8)input;
 }else bl=(u8)input;
 if(B(input+0x2d)==1){
  value=B(input+0x2c);
  if(value==0){value=B(input+0x2e);if(value>=9 && value<=12)B(l+2)=(u8)(B(l+2)+value-8);}
  else if(value==1){value=B(input+0x2f);if(value>=9 && value<=12)B(l+2)=(u8)(B(l+2)+value-8);}
  
  B(l+3)=2;bl=1;
 }
 value=B(l+2);eax=(eax&0xffffff00u)|value;
 if(value==0)goto done;
 eax=cm_fn_0054a110(l+0x10);
 B(l+0xc)=B(l+2);
 frame.state=0;
 
 W(l+0xe)=0xffff;B(l+0xd)=0;
 eax=cm_fn_00417c10(l+0x10,l+4,0x00ae2c90);
 mode=B(l+3);value=B(input+0x48);B(l+0x1c)=mode;flags=B(l+0x1f);flags&=0xfd;
 B(l+0x18)=0xff;flags=(u8)((flags&0xfb)|((value==0)?4:0));
 W(l+0x1d)=(u16)(s16)(s8)bl;
 key=D(input);p=l+0xc;
 receiver=D(0x00ae28f0);
 B(l+0x19)=0x14;B(l+0x1a)=0xff;B(l+0x1b)=0xff;B(l+0x1f)=flags;
 eax=cm_fn_00556940(receiver,key,p);
 key=D(input);base=D(0x00ae23bc);receiver=D(0x00ae28f0);
 eax=cm_fn_00557280(base+key*110u,l+0xc,second);
done:
 receiver=frame.prev;CM_FS(volatile u32,0)=receiver;
 return eax;
}
