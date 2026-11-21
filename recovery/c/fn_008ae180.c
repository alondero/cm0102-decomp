/* Preferred-image VAs refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   008ae1d0/008ae1d4 compute L and L+0x100, recomputed at 008ae1ee/008ae1f2;
   one 0x200 scratch keeps both 256-byte buffers adjacent without inventing
   initialization.  HELD-CONCERN RESOLUTION: the first unresolved obligation
   was the error-path argument map.  From the listing, 008ae1f9/008ae1fb/
   008ae1fc push 0x7c/L/L+0x100 before the void 0090d130 call (008ae1fd);
   008ae202/008ae203/008ae208 push its result, 0x9870e8 and 0xae24d0 before
   00944c9f (008ae20d); 008ae212/008ae214/008ae219 push 0/0xae24d0/0x9870e0
   before 005e8290 (008ae21e); the single 008ae223 add esp,0x24 discards all
   nine pushes.  So 0090d130 takes no arguments, 00944c9f takes six, and
   005e8290 takes three - the map in this source follows those VAs exactly,
   fixing the earlier mis-attribution rather than guessing.  The residual
   uninitialized-scratch concern is an explicit UNKNOWN BOUNDARY, not a
   defect: no listing VA shows a consumer reading those specific byte values
   (the buffers only cross original-code callee boundaries), and adding
   initialization or frame tuning to match incidental stack bytes would
   invent behavior.  Per Adam 2026-11-19 unproven-consumption it is recorded
   as a boundary.  Current canonical gate: -O0 pass and -O2 pass, cov 1.0. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(u32 message, u32 buffer, u32 mode);

u32 CM_THISCALL cm_fn_008ae180(u32 self, u16 count_arg)
{
    u8 scratch[0x200];
    *(volatile u16 *)(self + 4u) = count_arg;
    *(volatile u32 *)(self + 0x4b6u) = 0u;

    u32 i;
    for (i = 0u; i < 0x14u; ++i) {
        *(volatile u32 *)(self + 0x2d8u + i * 4u) = 0xffffffffu;
        *(volatile u32 *)(self + 0x288u + i * 4u) = 0xffffffffu;
    }

    s32 signed_count = *(volatile s16 *)(self + 4u);
    u32 allocation = (u32)cm_fn_00944cf1(((u32)signed_count << 2));
    *(volatile u32 *)self = allocation;
    if (allocation == 0u) {
        cm_fn_00944cff(0x00a8c6d4u, 0u, 0u, (u32)&scratch[0x100], (u32)&scratch[0]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0x100], (u32)&scratch[0], 0x7cu);
        }
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    }

    /* 008ae230 clears all of EAX; 008ae23d increments all 32 bits,
       while 008ae23a/008ae245 use only AX. Keep wrap unsigned. */
    u32 index = 0u;
    if ((s16)*(volatile u16 *)(self + 4u) > 0) {
        for (;;) {
            u32 data = *(volatile u32 *)self;
            s32 signed_index = (s16)(u16)index;
            ++index;
            *(volatile u32 *)(data + (u32)signed_index * 4u) = 0xffffffffu;
            if ((s16)(u16)index >= *(volatile s16 *)(self + 4u))
                break;
        }
    }
    return self;
}
