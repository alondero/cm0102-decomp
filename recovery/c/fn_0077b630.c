/* Preferred-image VA 0x0077b630; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern void cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *message, const char *destination,
                           int zero);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);
extern void cm_fn_00945934(u32 code);

u32 CM_THISCALL cm_fn_0077b630(u32 self, u32 input)
{
    volatile u8 scratch[0x200];
    u32 allocation;

    *(volatile u32 *)(self + 0x04u) = 0;
    *(volatile u32 *)(self + 0x08u) = input;
    allocation = (u32)cm_fn_00944cf1(input);
    *(volatile u32 *)self = allocation;

    if (allocation == 0) {
        (void)cm_fn_00944cff(0x00a649a8u, 0, 0,
                             (void *)&scratch[0x100],
                             (void *)&scratch[0]);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             (u32)cm_fn_0090d130(),
                             (u32)&scratch[0x100],
                             (u32)&scratch[0], 0x232u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }
    return self;
}
