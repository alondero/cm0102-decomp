/* Preferred-image VA 005af390, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_0068d080(u32 self);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32 self, const void *source, s16 count);
extern u32 CM_CDECL cm_fn_0090bde0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_005af390(void)
{
    u8 work[0x214];
    u32 path, stream, row, result, text;
    s16 count;
    cm_fn_0068d080(0x00ae2a48u);
    path = cm_fn_0090bde0(0x00987da0u, 0x009cc258u);
    stream = cm_fn_00945b00(path, 0x0098f928u);
    if (stream == 0)
        return 0;
    cm_fn_00945bfe((u32)(work + 2), 2, 1, stream);
    row = 0;
    count = *(volatile s16 *)(work + 2);
    if (count > 0) {
        do {
            result = cm_fn_00945bfe((u32)(work + 4), 15, 1, stream);
            if (result != 1) {
                cm_fn_00944cff(0x009cc208u, 0, 0, work + 0x114, work + 0x14);
                text = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x114, work + 0x14, 0x10b9u);
                cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
                D(0x00b67a34u) = 0;
            }
            /* A short read still falls through to this original copy call. */
            cm_fn_0068d0d0(0x00ae2a48u, work + 4, 15);
            row += 1u;
            count = *(volatile s16 *)(work + 2);
        } while ((s16)row < count);
    }
    return cm_fn_00945a09(stream);
}
