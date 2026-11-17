#include "cmre.h"
/* Original-instruction authority: CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VAs 0084cd10..0084cef3: ECX receiver, ret 8.
   0084ce3d..58, 0084ce61..7e and 0084ceab..b6 push explicit
   object arguments; ECX/EDX contain vtables, not thiscall receivers. */
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32, volatile s8 *);
extern u32 CM_CDECL cm_fn_00945bfe(u32,u32,u32,u32);
extern s32 CM_CDECL cm_fn_00946fa0(u32,u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
typedef u32 (CM_STDCALL *Create)(u32,u32,u32,u32);
typedef u32 (CM_STDCALL *Lock)(u32,u32,u32,u32,u32,u32,u32,u32);
typedef u32 (CM_STDCALL *Unlock)(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_0084cd10(u32 self, s8 index, volatile u32 path)
{
    /* One allocation, L = ESP immediately after the original sub esp,58. */
    u32 storage[0x58/4];
    u32 l=(u32)storage, stream, a, vt, slot, c, n, r;
    D(l)=0;
    if (!D(self+0x28)) return 0;
    stream=(u32)cm_fn_00945b00(path,(volatile s8 *)0x0098f928);
    if (!stream) return 0;
    cm_fn_00945bfe(l+0x2c,0x2c,1,stream);
    if (cm_fn_00946fa0(l+0x2c,0x00a82808,4)) goto close;
    if (cm_fn_00946fa0(l+0x34,0x00a82800,4)) goto close;
    if (cm_fn_00946fa0(l+0x38,0x00a827fc,3)) goto close;
    n=(1u<W(l+0x42))+1u;
    D(l+4)=0;
    D(l+8)=0;
    c=D(l+0x4c);
    D(l+0xc)=0;
    a=W(l+0x40);
    D(l+0x10)=0;
    W(l+4)=(u16)a;
    W(l+0x14)=0;
    a=D(l+0x44);
    W(l+0x10)=(u16)c;
    D(l+8)=a;
    W(l+6)=(u16)n;
    n=(c&0xffff)*a;
    a=W(l+0x4e);
    D(l+0x18)=0;
    W(l+0x12)=(u16)a;
    a=D(l+0x54);
    D(l+0x1c)=0;
    path=a;
    D(l+0x20)=0;
    D(l+0x20)=a;
    D(l+0xc)=n;
    D(l+0x24)=0;
    D(l+0x28)=0;
    slot=self+(u32)(s32)index*4u;
    a=D(self+0x28);
    D(l+0x28)=l+4;
    D(l+0x18)=0x14;
    D(l+0x1c)=0xe2;
    vt=D(a);
    r=((Create)D(vt+0xc))(a,l+0x18,slot,0);
    if (r) goto clear;
    a=D(slot);
    vt=D(a);
    r=((Lock)D(vt+0x2c))(a,0,path,l,(u32)&path,0,0,0);
    if (r) goto clear;
    n=path;
    c=D(l);
    cm_fn_00945bfe(c,1,n,stream);
    cm_fn_00945a09(stream);
    n=path;
    a=D(slot);
    vt=D(a);
    c=D(l);
    r=((Unlock)D(vt+0x4c))(a,c,n,0,0);
    if (r) { D(slot)=0; return 0; }
    return 1;
clear:
    D(slot)=0;
close:
    cm_fn_00945a09(stream);
    return 0;
}
