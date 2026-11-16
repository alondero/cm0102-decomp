#include "cmre.h"
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00684230(u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
#define R32(p) (*(volatile u32 *)(p))
#define R16(p) (*(volatile u16 *)(p))
#define R8(p) (*(volatile u8 *)(p))
u32 CM_THISCALL cm_fn_00797e80(u32 self) {
 volatile struct { u32 prev, handler, state; } frame;
 u8 scratch[0x40c];
 u32 a,b,vt,mem,obj,result,i,edx,ecx,line; void *p,*q;
 frame.prev=CM_FS(u32,0); frame.handler=0x95eb2e; frame.state=~0u; CM_FS(u32,0)=(u32)&frame;
 mem=cm_fn_00944e46(16);
 if (!mem) { p=scratch+0x20c; q=scratch+0xc; line=0x296; }
 else {
  for(i=0;i<4;i++) { u32 base=R32(self+0xee); R32(mem+i*4)=R32(base+16+i*4); }
  vt=R32(self);
  /* Pinned 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
     VA 00797eea saves EBX, 00797f0f/14/15/16 push four arguments,
     00797f19 calls with ECX=self, and 00797fbb restores EBX. */
  b=((u32 (CM_THISCALL *)(u32,u32,void *,void *,u32))R32(vt+0x3c))(self,0,scratch,scratch+4,0);
  obj=cm_fn_00944cf1(0xee); frame.state=0;
  if (!obj) result=0;
  else {
   u32 arg19=(obj & 0xffff0000u)|R16(self+0xd9);
   ecx=(self & 0xffffff00u)|R8(self+0x51);
   a=*(u32 *)(scratch+4);
   { u32 arg16=ecx; ecx=(ecx&0xffff0000u)|R16(self+0x40);
    u32 arg13=ecx; edx=self+0xbe; ecx=self+0xc5;
    u32 arg11=edx, arg10=ecx;
    edx=(edx&0xffffff00u)|R8(self+0xc4);
    ecx=(ecx&0xffffff00u)|R8(self+0xc3);
    u32 arg9=edx, arg8=ecx;
    edx=(edx&0xffffff00u)|R8(self+0xc2);
    result=cm_fn_0068b100(obj,self,4,mem,1,R32(self+4),b,*(u32 *)scratch,edx,arg8,arg9,arg10,arg11,arg13,0,a,arg16,2,0,arg19,~0u,0,2);
   }
  }
  /* Pinned CM0102 3.9.68 SHA-256
     0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
     preferred-image VA 00797f97 reads the destination before the SEH state
     reset at 00797f9b; 00797fa6 stores through that captured pointer. */
  ecx=R32(self+12);
  frame.state=~0u;
  R32(ecx)=result;
  cm_fn_009452ca(mem); cm_fn_009452ca(b);
  a=R32(self+12); ecx=R32(a);
  if(ecx) { R32(self+0x30)=0; cm_fn_00684230(R32(a)); CM_FS(u32,0)=frame.prev; return 1; }
  p=scratch+0x30c; q=scratch+0x10c; line=0x2ad;
 }
 cm_fn_00944cff(0xa6a068,0,0,p,q);
 a=cm_fn_0090d130(); cm_fn_00944c9f(0xae24d0,0x9870e8,a,p,q,line);
 cm_fn_005e8290(0x9870e0,0xae24d0,0); CM_GLOBAL(u32,0xb67a34)=0;
 CM_FS(u32,0)=frame.prev; return 0;
}
