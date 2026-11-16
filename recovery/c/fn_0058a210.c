/* Preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   58a210/215 requests a 0x16f8 stack allocation through 945610.
   945632..94563e relocates the physical helper return word.
   That transport and original page-touch/fault order remain UNRESOLVED:
   this local array is not a claim to implement that machine boundary.
   No ordinary helper call is substituted.
   With Y the post-helper ESP, 58a21a reads Y+1700 (argument 2);
   58a22f/58a404 read Y+16fc (argument 1); 58a39c/58a46f
   read Y+1704 (argument 3). 58a4c9 is CDECL RET.
   Scratch offsets below are relative to Y, not the saved-register ESP.
   Getter calls push one index and the following store executes with the
   next index pushed: 58a257 writes Y+4, not Y+8. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern char * CM_CDECL cm_fn_0066f4e0(char *, const char *, ...);
#define G(a) CM_GLOBAL(volatile u32,a)
#define W(o) (*(u32 *)(scratch+(o)))
#define P(o) ((u32)(scratch+(o)))
#define GET(i) ((u32)cm_fn_00785830((void *)a1,(i)))

void CM_CDECL cm_fn_0058a210(u32 a1,u32 a2,u32 a3)
{
    u8 scratch[0x16f8];
    u32 b,p,d,s,t,r,x,y,z;
    if (!a2) {
        s=GET(0); b=GET(1); p=GET(2); d=GET(3);
        cm_fn_0053e140(b,p,d,P(0x50),0x190,5,s);
        cm_fn_0066f4e0((char *)scratch+0x370,(const char *)0x9c6bc0,P(0x50));
        cm_fn_004ae660((void *)a3,P(0x370));
        x=G(0xae23bc);
        t=s*110u;
        r=cm_fn_0053e990(x+t,b,p,d,0);
        if (r) {
            x=G(0xae23bc);
            cm_fn_004ae8a0(a3,(const char *)scratch+0x50,0x7da,t+x);
        }
        return;
    }
    b=GET(0); p=GET(1);
    W(4)=GET(2); W(8)=GET(3);
    d=GET(4); W(0)=GET(5);
    W(0x10)=GET(6); W(0xc)=GET(7);
    W(0x14)=GET(8); W(0x18)=GET(9); z=GET(10);
    x=G(0xae23b4);
    d=x+d*581u;
    x=G(0xae23e0);
    s=x+z*101u;
    x=W(8); y=W(4);
    cm_fn_0053e140(p,y,x,P(0x50),0x190,1,b);
    x=W(0x10); y=W(0x18); z=W(0x14);
    t=W(0xc);
    cm_fn_0053e140(t,z,y,P(0x1e0),0x190,1,x);
    x=W(0);
    cm_fn_0066f4e0((char *)scratch+0x1c,
                  (const char *)(x==1 ? 0x9c6b8c : 0x9c6b50),0xad9c64u);
    x=(u32)(s32)*(volatile s8 *)(s+0x37);
    s+=4;
    y=(u32)(s32)*(volatile s8 *)(d+0x37);
    W(0)=d+4;
    cm_fn_0066f4e0((char *)scratch+0x370,(const char *)0x9c6c0c,
                  y,y,0u,0u,x,x,d+4,P(0x50),P(0x1c),P(0x1e0),s);
    cm_fn_004ae660((void *)a3,P(0x370));
    x=W(8); y=W(4);
    z=G(0xae23bc);
    t=b*110u;
    r=cm_fn_0053e990(z+t,p,y,x,0);
    if (r) {
        z=G(0xae23bc);
        cm_fn_004ae8a0(a3,(const char *)scratch+0x50,0x7da,t+z);
    }
    x=W(0);
    cm_fn_004ae8a0(a3,(const char *)x,0x7d5,d);
}
