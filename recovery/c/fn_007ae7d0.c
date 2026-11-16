#include "cmre.h"

/* Preferred-image VAs 007ae7fa-007ae81b, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   string instructions depend on DF. These forward loops require DF clear;
   plain C cannot represent the incoming flag when it is set. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_THISCALL cm_fn_005fde40(u32 self, u32 value);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                    void *local100, void *local4);

u32 CM_THISCALL cm_fn_007ae7d0(u32 self, u32 value)
{
    u8 scratch[0x200];
    u32 result = cm_fn_005fde40(CM_GLOBAL(volatile u32, 0x00b63c98u), value);
    if (result == 0 || *(volatile u8 *)result == 0) {
        (void)cm_fn_00944cff(0x00a701b0u, 0, 0,
                             (void *)(scratch + 0x100), (void *)scratch);
        u32 converted = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, converted,
                             (u32)(scratch + 0x100), (u32)scratch, 0x131u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    u32 length = 0;
    while (*(volatile u8 *)(result + length) != 0)
        length++;
    length++;
    u32 source = result;
    u32 destination = self + 0x139u;
    u32 dwords = length >> 2;
    while (dwords != 0) {
        u32 word = *(volatile u32 *)source;
        *(volatile u32 *)destination = word;
        source += 4u;
        destination += 4u;
        dwords--;
    }
    u32 bytes = length & 3u;
    while (bytes != 0) {
        u8 byte = *(volatile u8 *)source;
        *(volatile u8 *)destination = byte;
        source++;
        destination++;
        bytes--;
    }
    return 1;
}
