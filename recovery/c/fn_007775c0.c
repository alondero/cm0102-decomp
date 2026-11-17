#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
/* Preferred-image VAs below refer to CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   77769f-7776a8 pushes three words and sets ECX: the boundary
   inputs are the three pushed arguments and ECX receiver.
   7776ba/77778f set ECX for 68d1b0; 7776da/77773d set ECX
   for 5c6710 and each site pushes one word. 777662/7777d6 ret 4. */
extern u32 CM_THISCALL cm_fn_005ae6a0(u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void *CM_CDECL cm_fn_00945501(u32,u32);

void CM_THISCALL cm_fn_007775c0(u32 self,u32 source)
{
    u8 scratch[0x20c];
    u32 n = D(0xae2350);
    u32 limit = D(0xae235c) - n*2u;
    u32 eax = D(self+0x24);
    u32 row, left, iter, node, p, q, idx, ecx, arg, active;
    u8 mark;
    if (!eax) {
        eax = (u32)cm_fn_00945501(limit,1);
        D(self+0x24) = eax;
        if (!eax) {
            cm_fn_00944cff(0xa5b2a4,0,0,scratch+0x10c,scratch+0xc);
            eax = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,eax,
                          scratch+0x10c,scratch+0xc,0x2229);
            cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
            D(0xb67a34)=0;
            return;
        }
    }
    row=0xb63df0;
    left=0x21;
    do {
        eax=D(row);
        if (!eax || B(eax+0x11c)) {
            scratch[0]=0;
            do {
                eax=(eax&0xffffff00u)|B(row+0x14);
                ecx=*(u32 *)(scratch);
                arg=D(0xae2a58);
                iter=cm_fn_005ae6a0(arg,source,ecx,eax);
                if (iter) {
                    eax=D(iter+2);
                    D(iter+0xa)=eax;
                    node=cm_fn_0068d1b0(iter);
                    while (node) {
                        active=D(row);
                        arg=D(node+0x14);
                        ecx=D(0xae2c74);
                        eax=(u32)cm_fn_005c6710((void *)ecx,arg);
                        if (!eax) {
                            if (!active) {
                                p=D(node+0x1c);
                                if (p) {
                                    q=D(p+0x53);
                                    if (q && B(q+0x11c)) {
                                        idx=D(p);
                                        if ((s32)idx<(s32)limit)
                                            B(D(self+0x24)+idx)=3;
                                    }
                                }
                                p=D(node+0x20);
                                if (p) {
                                    q=D(p+0x53);
                                    if (q && B(q+0x11c)) {
                                        idx=D(p);
                                        if ((s32)idx<(s32)limit)
                                            B(D(self+0x24)+idx)=3;
                                    }
                                }
                            } else {
                                p=D(node+0x1c);
                                mark=2;
                                if (p) {
                                    ecx=D(node+0x14);
                                    q=D(p+0x57);
                                    if (q==ecx) mark=1;
                                    else {
                                        q=D(node+0x20);
                                        if (D(q+0x60)==ecx) mark=1;
                                    }
                                    idx=D(p);
                                    if ((s32)idx<(s32)limit)
                                        B(D(self+0x24)+idx)=mark;
                                }
                                p=D(node+0x20);
                                if (p) {
                                    idx=D(p);
                                    if ((s32)idx<(s32)limit)
                                        B(D(self+0x24)+idx)=mark;
                                }
                            }
                        }
                        node=cm_fn_0068d1b0(iter);
                    }
                }
                eax=0;
                scratch[0]++;
            } while ((s8)scratch[0]<=2);
        }
        row+=0x48;
    } while (--left);
}
