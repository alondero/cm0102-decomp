#include "cmre.h"
/* Preferred-image VA 0x00804100 (base 0x00400000), CM0102 3.9.68.
   ECX is read before written: the object pointer. RET 0x24 pops nine
   stack words. The single callee takes EAX/EDX/ECX plus eighteen stack
   words (the callee is regparm(3) stdcall, RET 0x48).
   0x00804153 calls the callee and 0x00804158 returns: the wrapper's last
   EDX write is 0x00804145 'shl edx, 8' (= scaled), the call at 0x00804153
   overwrites EDX and the wrapper performs no further EDX write, so it
   forwards the callee's EDX:EAX unchanged. The counter word at
   [callee_self + 0x12e9a0] is callee-private state (read/written only
   inside the callee) and is never read here. Declaring the callee boundary
   return as unsigned long long is how that unchanged EDX:EAX forward is
   expressed; the high dword is not fabricated from `scaled` and `count`. */
extern unsigned long long CM_STDCALL __attribute__((regparm(3)))
    cm_fn_0055f990(u32 incoming_eax, u32 incoming_edx, u32 self,
        volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
        volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
        volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
        volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
        volatile u32 a16, volatile u32 a17);

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))

unsigned long long CM_THISCALL cm_fn_00804100(u32 self, u32 p1, u32 p2,
                                u32 p3, u32 p4, u32 p5, u32 p6, u32 p7,
                                u32 p8, u32 p9)
{
    s16 index = (s16)W(self + 0x382au);
    u32 scaled = (u32)(s32)index * 3u;
    u32 callee_self;
    scaled <<= 8;
    callee_self = D(self + 0x7fcu + scaled);
    return cm_fn_0055f990((u32)(s32)index, scaled, callee_self,
                          1u, 0u, 0u, 0u, 0u, p2, p3, p4, p5, 0u,
                          p6, p7, p8, p9, 0u, 0u, 0u, p1);
}
