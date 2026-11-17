#include "cmre.h"
/* Original-instruction authority: preferred-image VAs 007a87d0-007a8b5e,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Entry ECX and RET 4 establish thiscall. At 007a89d1 one stack
 * argument is pushed for 0090cfc0. At 007a8b1b-007a8b48 the three
 * earlier pushes survive 0090d130 and belong to variadic 00944c9f.
 */
extern void CM_CDECL cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00945667(u32,u32);
extern void CM_CDECL cm_fn_00945934(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))

static void report(u8 *a, u8 *b, u32 line)
{
    char *s;
    cm_fn_00944cff(0xa6bf9c,0,0,a,b);
    s=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,s,a,b,line);
    cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    D(0xb67a34)=0;
}
static void terminate(void)
{
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffff);
}

void CM_THISCALL cm_fn_007a87d0(u32 self,u32 input)
{
    u8 scratch[512];
    u32 eax,ecx,edx,record;
    eax=D(input);
    if ((s32)eax<0 || (s32)eax>(s32)D(0xae2364)) {
        report(scratch,scratch+256,0xa9d);
        return;
    }
    record=D(0xae23bc)+eax*110u;
    eax=D(self+0x18);
    edx=D(self+0x14);
    if(edx==eax-1u) {
        eax=cm_fn_00945667(D(self+0x10),(eax*3u+0x2eeu)*4u);
        edx=D(self+0x18)+250u;
        D(self+0x10)=eax;
        D(self+0x18)=edx;
        if(!eax) {
            report(scratch+256,scratch,0xaac);
            terminate();
        }
    }
    eax=D(self+0x14); ecx=D(input);
    edx=eax*3u; eax=D(self+0x10);
    D(eax+edx*4u)=ecx;
    eax=D(self+0x14); ecx=W(input+4);
    edx=eax*3u; eax=D(self+0x10);
    W(eax+edx*4u+4)=(u16)ecx;
    eax=D(self+0x14); ecx=W(input+6);
    edx=eax*3u; eax=D(self+0x10);
    W(eax+edx*4u+6)=(u16)ecx;
    ecx=D(self+0x14); D(self+0x14)=ecx+1u;
    eax=D(record+0x69);
    if(eax) {
        eax=D(self+0x24); ecx=D(self+0x20);
        if(ecx==eax-1u) {
            eax=cm_fn_00945667(D(self+0x1c),(eax+250u)*16u);
            edx=D(self+0x24)+250u;
            D(self+0x1c)=eax; D(self+0x24)=edx;
            if(!eax) {
                report(scratch,scratch+256,0xac5);
                terminate();
            }
        }
        edx=D(record+0x69); eax=D(self+0x20); ecx=D(self+0x1c);
        edx=D(edx); D(eax*16u+ecx)=edx;
        eax=D(record+0x1a);
        if(eax) {
            ecx=D(self+0x20); edx=D(self+0x1c); eax=D(eax);
            D(ecx*16u+edx+4)=eax;
        } else {
            ecx=D(0xae2350);
            eax=cm_fn_0090cfc0(ecx);
            edx=D(self+0x20); ecx=D(self+0x1c);
            D(edx*16u+ecx+4)=eax;
        }
        edx=D(self+0x20); eax=D(self+0x1c); ecx=D(record);
        D(edx*16u+eax+12)=ecx;
        eax=D(self+0x20); D(self+0x20)=eax+1u;
    }
    if(!D(record+0x61)) return;
    ecx=D(self+0x44); eax=D(self+0x40);
    if(eax==ecx-1u) {
        eax=D(self+0x3c);
        eax=cm_fn_00945667(eax,(ecx+250u)*16u);
        edx=D(self+0x44);
        D(self+0x3c)=eax;
        eax=D(self+0x40);
        edx+=250u;
        D(self+0x44)=edx;
        /* Original bug: tests the count, not the allocation result. */
        if(!eax) {
            report(scratch,scratch+256,0xae9);
            terminate();
        }
    }
    edx=D(record+0x61); ecx=D(self+0x3c);
    edx=D(edx); D(eax*16u+ecx)=edx;
    eax=D(record+0x1a); ecx=D(self+0x40); edx=D(self+0x3c);
    eax=D(eax); D(ecx*16u+edx+4)=eax;
    ecx=D(self+0x40); edx=D(self+0x3c); eax=D(record);
    D(ecx*16u+edx+12)=eax;
    eax=D(self+0x40); D(self+0x40)=eax+1u;
}
