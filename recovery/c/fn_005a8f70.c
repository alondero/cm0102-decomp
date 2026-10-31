/* Preferred-image VA 0x005a8f70; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

typedef struct {
    u32 prev;
    u32 handler;
    s32 state;
} cm_seh_005a8f70;

extern void cm_fn_005e8290(const char *message, const char *destination,
                           int zero);
extern void CM_FASTCALL cm_fn_00768290(u32 self);
extern u32 CM_THISCALL cm_fn_00767af0(u32 allocation, u32 zero);
extern u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 input);
extern void CM_THISCALL cm_fn_005a90b0(u32 self, u32 input);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);

u32 CM_THISCALL cm_fn_005a8f70(u32 self, u32 input)
{
    volatile cm_seh_005a8f70 frame;
    volatile u8 scratch[0x208];
    u32 allocation;
    u32 identifier;
    u32 base;
    u32 offset;
    u32 result;

    frame.state = -1;
    frame.handler = 0x00959459u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)&scratch[4] = self;
    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x0096a748u;
    identifier = CM_GLOBAL(volatile u32, 0x009cf2fc);
    base = CM_GLOBAL(volatile u32, 0x00ae23a8);
    frame.state = 0;
    *(volatile u32 *)(self + 0x13) = 8;
    offset = identifier * 0x122u;
    *(volatile u8 *)(self + 0x12) = 1;
    *(volatile u32 *)(self + 0x0e) = base + offset;

    if (input != 0) {
        result = cm_fn_0076a280(self, input);
        if (result != 0)
            goto done;
    }

    allocation = cm_fn_00944cf1(0x0f);
    *(volatile u32 *)&scratch[0] = allocation;
    *(volatile u8 *)&frame.state = 1;
    if (allocation != 0)
        result = cm_fn_00767af0(allocation, 0);
    else
        result = 0;
    *(volatile u8 *)&frame.state = 0;
    *(volatile u32 *)(self + 0x0a) = result;

    if (result == 0) {
        (void)cm_fn_00944cff(0x009cc0fcu, 0, 0,
                             (void *)&scratch[0x108],
                             (void *)&scratch[8]);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             (u32)cm_fn_0090d130(),
                             (u32)&scratch[0x108],
                             (u32)&scratch[8], 0x51);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        cm_fn_005a90b0(self, input);
    }

done:
    result = self;
    CM_FS(u32, 0) = frame.prev;
    return result;
}
