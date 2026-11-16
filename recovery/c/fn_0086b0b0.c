#include "cmre.h"
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern void CM_THISCALL cm_fn_00417c10(u32 *,u32 *,u16 *);
extern u32 CM_CDECL cm_fn_0053d980(u32,u32,u32);
extern void CM_THISCALL cm_fn_0058e340(u32 *,int *,int *,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern char *cm_fn_0090d130(void);
extern void cm_fn_005e8290(const char *,const char *,int);
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile s16 *)(a))
#define B(a) (*(volatile u8 *)(a))

static void err(u8 *a,u8 *b,int line) {
 char *s;
 cm_fn_00944cff(0xa83b78,0,0,a,b);
 s=cm_fn_0090d130();
 cm_fn_00944c9f(0xae24d0,0x9870e8,s,a,b,line);
 cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
 CM_GLOBAL(u32,0xb67a34)=0;
}
static void insert(u32 self,u32 rec,u8 *obj,u32 off,u32 table) {
 u32 p=D(self+table)+D(rec)*8;
 cm_fn_0058e340((u32 *)D(self+off),(int *)p,(int *)(p+4),(u32)obj);
}
void CM_THISCALL cm_fn_0086b0b0(u32 self,u32 staff,u32 team,u32 records) {
 volatile struct { u32 prev,handler,state; } frame;
 u8 buf[0x488];
 u32 base,rec,p,q,i,amount,other;
 s32 x,y;
 s16 v,low;
 long double t;
 frame.state=~0u; frame.handler=0x962c46;
 frame.prev=CM_FS(u32,0); CM_FS(u32,0)=(u32)&frame;
 if (!staff || !team || !records) {
  err(buf+0x388,buf+0x288,0x5f6);
  CM_FS(u32,0)=frame.prev; return;
 }
 base=D(team+0x14);
 x=W(base+0x69)*500; y=W(staff+0x80);
 if(y<x) {
  x-=y; if(x>350)x=350;
  W(staff+0x80)=(s16)(y+x);
  v=W(staff+0x80); if(v<1)v=1; else if(v>10000)v=10000;
  W(staff+0x80)=v;
 }
 cm_fn_0054a110((u16 *)(buf+0x1c));
 frame.state=0;
 cm_fn_00417c10((u32 *)(buf+0x1c),(u32 *)(buf+0xc),(u16 *)0xae2c90);
 D(buf+0x24)=D(staff);
 B(buf+0x28)=1;
 D(buf+0x29)=D(D(team+0x14));
 rec=D(staff+0xcf);
 if(rec) {
  if(D(rec+0x69)) {
   if((s8)B(D(CM_GLOBAL(u32,0xadadfc)+D(team)*4)+0x42)==1) {
    other=150;
    amount=cm_fn_0053d980(rec,D(staff+0x53),1)?150:300;
   } else { amount=300; other=300; }
   p=records+(D(team+0x20)==staff?0x625:0)+0x188;
   /* The original __ftol at 0086b244 consumes x87 ST0: C long double cast lowers it. */
   t=(long double)(W(base+0x69)*300)*CM_GLOBAL(double,0x96ff88);
   W(p)=(s16)(W(p)+(s16)(long long)t);
   p=records+(D(team+0x20)==staff?0x625:0)+0x188;
   low=(s16)(W(base+0x69)*300); v=W(p);
   W(p)=low>v?low:v>10000?10000:v;
   p=records+(D(team+0x20)==staff?0x625:0)+0x18a;
   /* __ftol at 0086b2dc. */
   t=(long double)(W(base+0x69)*(s16)amount)*CM_GLOBAL(double,0x96ff88);
   W(p)=(s16)(W(p)+(s16)(long long)t);
   p=records+(D(team+0x20)==staff?0x625:0)+0x18a;
   low=(s16)(W(base+0x69)*(s16)amount); v=W(p);
   W(p)=low>v?low:v>10000?10000:v;
   p=records+(D(team+0x20)==staff?0x625:0)+0x18c;
   /* __ftol at 0086b36e. */
   t=(long double)(W(base+0x69)*(s16)other)*CM_GLOBAL(double,0x96ff88);
   W(p)=(s16)(W(p)+(s16)(long long)t);
   p=records+(D(team+0x20)==staff?0x625:0)+0x18c;
   low=(s16)(W(base+0x69)*(s16)other); v=W(p);
   W(p)=low>v?low:v>10000?10000:v;
  }
  rec=D(staff+0xcf);
  if(rec) insert(self,rec,buf+0x1c,0,0xc);
  else err(buf+0x88,buf+0x188,0x1010);
 }
 rec=D(staff+0xbf);
 if(rec) insert(self,rec,buf+0x1c,0,0xc);
 p=records+(staff!=D(team+0x1c)?0x625:0)+0x190;
 for(i=0;i<20;i++,p+=0x49) {
  q=D(p); if(q==~0u)continue;
  rec=CM_GLOBAL(u32,0xae23bc)+q*110;
  if(rec && (s32)D(rec)>=CM_GLOBAL(s32,0xae2364))continue;
  if(rec==D(staff+0xcf))continue;
  if(rec)insert(self,rec,buf+0x1c,0,0xc);
  else err(buf+0x188,buf+0x88,0x1010);
 }
 cm_fn_0054a110((u16 *)(buf+0x54));
 *(volatile u8 *)&frame.state=1;
 cm_fn_00417c10((u32 *)(buf+0x54),(u32 *)(buf+0x14),(u16 *)0xae2c90);
 B(buf+0x5c)=1; D(buf+0x5d)=D(base);
 q=D(self+0x10)+D(staff)*8;
 cm_fn_0058e340((u32 *)D(self+4),(int *)q,(int *)(q+4),(u32)(buf+0x54));
 CM_FS(u32,0)=frame.prev;
}
