/* Preferred-image VA 0x00550380, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   cdecl, four stack arguments. Both tails normalize EAX to 0 or 1.
   Scratch L is entry ESP-0x200, with buffers L and L+0x100.
   Error path calls 0x00945934 after both shutdown boundaries. */
#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern void cm_fn_00945934(u32 status);
extern u32 cm_fn_00945bfe(u32 buffer, u32 a, u32 b, u32 stream);

u32 CM_CDECL cm_fn_00550380(u32 stream, u32 reading, u32 buffer, u32 record)
{
    u8 scratch[0x200];
    u32 count = *(volatile u32 *)(record + 0x37u);
    u32 result;
    if (*(volatile u32 *)(record + 0x3fu) != 2) {
        char *text;
        cm_fn_00944cff(0x009b8dc0u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch + 0x100, scratch, 0x638u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }
    if (reading != 0)
        result = cm_fn_00932c30(buffer, 0x65u, count, stream);
    else
        result = cm_fn_00945bfe(buffer, 0x65u, count, stream);
    return (u32)(result == count);
}
