/* Preferred-image VA 007858c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007858c0(u32 self, s8 kind, volatile u32 second, volatile u32 third)
{
    u8 work[0x200];
    u32 text, optional, offset, remaining, cursor, value, line, version;
    u8 byte, end;
    void *name, *extension;
    if (kind < 0 || kind >= 50) {
        name = work;
        extension = work + 0x100;
        line = 0x1e8du;
        goto error;
    }
    text = second;
    if (text == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x1e95u;
        goto error;
    }
    offset = (u32)(s32)kind * 4u;
    cm_fn_009466d0(self + offset + 5u, text, 200u - offset);
    B(self + 0xccu) = 0;
    optional = third;
    if (optional != 0) {
        remaining = 0xffffffffu;
        cursor = text;
        while (remaining != 0) {
            byte = B(cursor);
            ++cursor;
            --remaining;
            if (byte == 0)
                break;
        }
        value = ~remaining;
        while ((value & 3u) != 0)
            ++value;
        value = (u32)((s32)(value + ((value & 0x80000000u) != 0 ? 3u : 0u)) >> 2);
        end = (u8)(value + (u32)(s32)kind);
        B(optional) = end;
        if ((s8)end > 50)
            B(optional) = 50;
    }
    return 1;
error:
    cm_fn_00944cff(0x00a693ecu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
