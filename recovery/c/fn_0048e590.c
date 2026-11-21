#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,const char *,u32,u32);
extern u32 CM_CDECL cm_fn_004bfa30(u32 *,u32,u32,char,char *,s32);
extern u32 CM_CDECL cm_fn_004bff70(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_0090c310(u32,u32);
void CM_THISCALL cm_fn_0048e590(u32 self,u32 input,u32 mode,u32 output) {
 volatile struct { void *prev; u32 handler,state; } frame;
 u8 scratch[0x154];
 u32 a,b,c,d,e,f,g,base,rec,other,obj,t; s32 x,y,z; unsigned long long v;
 frame.prev=CM_FS(void *,0); frame.handler=0x956096; frame.state=~0u; CM_FS(void *,0)=(void *)&frame;
 a=(u32)cm_fn_00785830((void *)input,1);
 b=(u32)cm_fn_00785830((void *)input,2);
 if(mode) {
 c=(u32)cm_fn_00785830((void *)input,3);
 d=(u32)cm_fn_00785830((void *)input,4);
 e=(u32)cm_fn_00785830((void *)input,5);
 f=(u32)cm_fn_00785830((void *)input,6);
 g=(u32)cm_fn_00785830((void *)input,7);
 } else c=d=e=f=g=0;
 *(u32 *)(scratch+(mode?0x10:0x0c))=a;
 frame.state=mode?0:1;
 v=cm_fn_0043f460((int *)(scratch+(mode?0x10:0x0c)),6);
 cm_fn_00441c10((u32)(scratch+0x18),0x1e,(u32)v,(u32)(v>>32),6);
 rec=b*0x245;
 if(!mode) {
 base=CM_GLOBAL(u32,0xae23b4); x=*(s8 *)(base+rec+0x52);
 cm_fn_0066f4e0(0xde1f64,0x99b844,x,x,base+rec+0x38,scratch+0x18);
 cm_fn_004ae660((void *)output,0xde1f64);
 base=CM_GLOBAL(u32,0xae23b4)+rec;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base);
 } else if ((s8)c>0) {
 base=CM_GLOBAL(u32,0xae23b4); obj=*(u32 *)(self+4); y=*(s8 *)(obj+0x37); x=*(s8 *)(base+rec+0x52);
 t=cm_fn_0090c310((s8)c,~0u);
 cm_fn_0066f4e0(0xde1f64,0x99bc18,x,x,y,y,base+rec+0x38,scratch+0x18,t,obj+4);
 cm_fn_004ae660((void *)output,0xde1f64);
 base=CM_GLOBAL(u32,0xae23b4)+rec;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base);
 obj=*(u32 *)(self+4); cm_fn_004ae8a0(output,(char *)(obj+4),0x7d0,obj);
 } else if ((u16)d) {
 obj=*(u32 *)(self+4); cm_fn_004bfa30((u32 *)obj,0,d,1,(char *)(scratch+0x38),300);
 obj=*(u32 *)(self+4); t=cm_fn_004bff70(obj,0,d,1);
 base=CM_GLOBAL(u32,0xae23b4); x=*(s8 *)(base+rec+0x52); y=(s8)t;
 cm_fn_0066f4e0(0xde1f64,0x99bb78,x,x,y,y,base+rec+0x38,scratch+0x18,scratch+0x38);
 cm_fn_004ae660((void *)output,0xde1f64); base=CM_GLOBAL(u32,0xae23b4)+rec;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base);
 obj=*(u32 *)(self+4); cm_fn_004ae8a0(output,(char *)(scratch+0x38),0x7d0,obj);
 } else if ((u8)g==1 || (u8)g==2) {
 obj=*(u32 *)(self+4); base=CM_GLOBAL(u32,0xae23b4); y=*(s8 *)(obj+0x37); x=*(s8 *)(base+rec+0x52);
 cm_fn_0066f4e0(0xde1f64,(u8)g==1?0x99bad8:0x99ba2c,x,x,y,y,base+rec+0x38,scratch+0x18,obj+4);
 cm_fn_004ae660((void *)output,0xde1f64); base=CM_GLOBAL(u32,0xae23b4)+rec;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base);
 obj=*(u32 *)(self+4); cm_fn_004ae8a0(output,(char *)(obj+4),0x7d0,obj);
 } else if ((s32)e>=0) {
 obj=*(u32 *)(self+4); other=e*0x245; base=CM_GLOBAL(u32,0xae23b4);
 z=*(s8 *)(base+other+0x52); y=*(s8 *)(obj+0x37); x=*(s8 *)(base+rec+0x52);
 cm_fn_0066f4e0(0xde1f64,(s8)f?0x99b95c:0x99b88c,x,x,z,z,y,y,base+rec+0x38,scratch+0x18,base+other+0x38,obj+4);
 cm_fn_004ae660((void *)output,0xde1f64); base=CM_GLOBAL(u32,0xae23b4)+rec;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base); base=CM_GLOBAL(u32,0xae23b4)+other;
 cm_fn_004ae8a0(output,(char *)(base+0x38),0x7d5,base); obj=*(u32 *)(self+4); cm_fn_004ae8a0(output,(char *)(obj+4),0x7d0,obj);
 }
 CM_FS(void *,0)=frame.prev;
}
