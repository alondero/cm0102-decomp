#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern void CM_THISCALL cm_fn_00417c10(u32 *,u32 *,u16 *);
extern u32 CM_CDECL cm_fn_0053d980(u32,u32,u32);
extern void CM_THISCALL cm_fn_0058e340(u32 *,int *,int *,u32);
extern void cm_fn_005e8290(const char *,const char *,int);
extern u32 CM_STDCALL cm_fn_0076d940(u32);
extern u32 CM_THISCALL cm_fn_00774f90(volatile u32 *,volatile u32 *,volatile u32 *);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile s16 *)(a))
static void error_at(u8 *a,u8 *b,u32 n) {
 char *p;
 cm_fn_00944cff(0xa83b78,0,0,a,b);
 p=cm_fn_0090d130();
 cm_fn_00944c9f(0xae24d0,0x9870e8,p,a,b,n);
 cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
 CM_GLOBAL(u32,0xb67a34)=0;
}
void CM_THISCALL cm_fn_0086b710(u32 self,u32 s,u32 m,u32 r) {
 volatile struct { u32 prev,handler,state; } seh;
 u8 scratch[0x480];
 u32 x,y,z,i,base,p; s32 v,t,k,q;
 seh.prev=CM_FS(u32,0); seh.handler=0x962c66; seh.state=~0u; CM_FS(u32,0)=(u32)&seh;
 if (!s || !m || !r) { error_at(scratch+0x380,scratch+0x280,0x67b); goto out; }
 x=D(m+0x14); v=W(x+0x69)*500; t=W(s+0x80);
 if(t<v) { v-=t; if(v>200)v=200; W(s+0x80)=(s16)(t+v); t=W(s+0x80); if(t<1)t=1; else if(t>10000)t=10000; W(s+0x80)=(s16)t; }
 cm_fn_0054a110((u16 *)(scratch+0x14)); seh.state=0;
 cm_fn_00417c10((u32 *)(scratch+0x14),(u32 *)(scratch+8),(u16 *)0xae2c90);
 D((u32)scratch+0x1c)=D(s); scratch[0x20]=2; D((u32)scratch+0x21)=D(D(m+0x14));
 y=D(s+0xcf);
 if(y) {
  if(D(y+0x69)) {
   z=D(CM_GLOBAL(u32,0xadadfc)+D(m)*4);
   if(*(volatile s8 *)(z+0x42)==1) { q=100; k=cm_fn_0053d980(y,D(s+0x53),1)?100:200; }
   else { q=200; k=200; }
   base=r+(D(m+0x20)==s)*0x625;
   /* __ftol at 0086b89c, 0086b933, 0086b9c7: x87 extended product and 64-bit cast. */
   for(i=0;i<3;i++) {
    s32 factor=i==0?200:(i==1?k:q);
    p=base+0x188+i*2;
    v=W(x+0x69)*factor;
    { long double f=(long double)v*(long double)CM_GLOBAL(double,0x96ff88); W(p)=(s16)(W(p)+(u16)(long long)f); }
    p=r+(D(m+0x20)==s)*0x625+0x188+i*2;
    t=(s16)(W(x+0x69)*factor); v=W(p);
    if(t>v) v=t; else if(v>10000)v=10000;
    W(p)=(s16)v;
   }
  }
  y=D(s+0xcf);
  if(y) { p=D(self+0xc)+D(y)*8; cm_fn_0058e340((u32 *)D(self),(int *)p,(int *)(p+4),(u32)(scratch+0x14)); }
  else error_at(scratch+0x180,scratch+0x80,0x1010);
 }
 y=D(s+0xbf);
 if(y) { p=D(self+0xc)+D(y)*8; cm_fn_0058e340((u32 *)D(self),(int *)p,(int *)(p+4),(u32)(scratch+0x14)); }
 p=r+(D(m+0x1c)!=s)*0x625+0x190;
 for(i=0;i<20;i++,p+=0x49) {
  y=D(p); if(y==~0u)continue;
  z=CM_GLOBAL(u32,0xae23bc)+y*110;
  if(z && (s32)D(z)>=(s32)CM_GLOBAL(u32,0xae2364))continue;
  if(z==D(s+0xcf))continue;
  if(!z) error_at(scratch+0x180,scratch+0x80,0x1010);
  else { y=D(self+0xc)+D(z)*8; cm_fn_0058e340((u32 *)D(self),(int *)y,(int *)(y+4),(u32)(scratch+0x14)); }
 }
 if(cm_fn_0076d940(D(m+0x14))) {
  cm_fn_00774f90((u32 *)CM_GLOBAL(u32,0xb74318),(u32 *)D(m+0x1c),(u32 *)D(m+0x14));
  cm_fn_00774f90((u32 *)CM_GLOBAL(u32,0xb74318),(u32 *)D(m+0x20),(u32 *)D(m+0x14));
 }
 cm_fn_0054a110((u16 *)(scratch+0x4c)); *(volatile u8 *)&seh.state=1;
 cm_fn_00417c10((u32 *)(scratch+0x4c),(u32 *)(scratch+8),(u16 *)0xae2c90);
 scratch[0x54]=2; D((u32)scratch+0x55)=D(x);
 p=D(self+0x10)+D(s)*8;
 cm_fn_0058e340((u32 *)D(self+4),(int *)p,(int *)(p+4),(u32)(scratch+0x4c));
out: CM_FS(u32,0)=seh.prev;
}
