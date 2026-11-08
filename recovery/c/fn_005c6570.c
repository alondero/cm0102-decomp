#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_THISCALL cm_fn_005abc30(u32,u32);
extern int CM_THISCALL cm_fn_005c6740(int,u32,u32,int,int);
extern void CM_THISCALL cm_fn_005c8260(int,int *,int);
/* Original entry overwrites ECX before any read or forwarding; RET4.
   EAX after its last 0068d040 call is returned unchanged (005cbae3..baf3).
   This declaration transports that actual EAX across an original boundary. */
extern u32 CM_STDCALL cm_fn_005cb9e0(void *);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);

/* Preferred-image VA 005c6570, base 00400000; SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THISCALL, two stack words / RET8; actual full EAX transport.
   Scratch L=entryESP-0xec; all scratch-object arguments are L+0.
   No scratch initialization is inferred. Original callees remain boundaries. */
u32 CM_THISCALL cm_fn_005c6570(u32 self,u32 record,u32 excluded)
{
    volatile struct { u32 prev,handler,state; } seh;
    u8 scratch[0xe0];
    u32 previous, eax, edx, ecx, value;
    previous = CM_FS(volatile u32,0);
    seh.state = 0xffffffffu;
    seh.handler = 0x0095a0dbu;
    seh.prev = previous;
    CM_FS(volatile u32,0) = (u32)&seh;
    eax = D(record+0x14u);
    if (eax != D(self+4u)) {
        ecx = self+0xb1u;
        if (ecx != 0) {
            value = B(eax);
            eax = (eax & 0xffffff00u) | B(self+0xafu);
            ecx = (u32)(s32)(s8)(u8)(value-(u8)eax);
            eax = self+ecx*105u+0xb1u;
            if (D(eax) != 0 && eax != 0)
                cm_fn_005c8260((int)self,(int *)eax,0);
        }
    } else {
        cm_fn_0077bbe0(scratch,8,0);
        edx = D(record+0xcu);
        seh.state = 0;
        cm_fn_00785790(scratch,0,edx);
        eax = D(record+0x10u);
        cm_fn_00785790(scratch,1,eax);
        ecx = W(record+0x2au);
        cm_fn_00785790(scratch,2,ecx);
        edx = W(record+0x28u);
        cm_fn_00785790(scratch,3,edx);
        eax = D(record+0x1cu);
        if (excluded != eax) {
            ecx = D(0x00b7431cu);
            cm_fn_007861e0((void *)ecx,(u32)scratch,eax);
        }
        eax = D(record+0x20u);
        if (excluded != eax) {
            ecx = D(0x00b7431cu);
            cm_fn_007861e0((void *)ecx,(u32)scratch,eax);
        }
        seh.state = 0xffffffffu;
    }
    edx = D(record+0x14u);
    eax = D(self+4u);
    if (edx == eax) {
        eax = D(self+0x34ccu);
        if (eax != 0) {
            eax = D(record+0x1cu);
            ecx = D(record+0x20u);
            edx = (edx & 0xffff0000u) | W(record+0x28u);
            value = eax;
            eax = (eax & 0xffff0000u) | W(record+0x2au);
            eax = (u32)cm_fn_005c6740((int)self,eax,edx,(int)ecx,(int)value);
            if (eax != 0) {
                D(eax+0x10u) = 0;
                B(eax+0x18u) = 0;
            }
        }
    }
    if (B(record+0x43u) == 0xffu) {
        ecx = D(record+0x1cu);
        eax = cm_fn_0053b900((u32 *)ecx);
        if (eax != 0)
            eax = cm_fn_005cb9e0((void *)record);
        else {
            ecx = D(0x00ae2a58u);
            eax = cm_fn_005abc30(ecx,record);
        }
    }
    previous = seh.prev;
    CM_FS(volatile u32,0) = previous;
    return eax;
}
