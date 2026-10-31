/* Preferred-image VA 008a9120, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008a9120(u32 self, volatile s8 first, volatile s8 second)
{
    u8 work[0x200];
    s8 slot, value;
    u32 selected, source, destination, i, version, line;
    void *name, *extension;
    slot = first;
    if (slot < 0 || slot > 10) {
        name = work;
        extension = work + 0x100;
        line = 0x16d0u;
        goto error;
    }
    value = second;
    if (value < 0 || value > 19) {
        name = work + 0x100;
        extension = work;
        line = 0x16d7u;
        goto error;
    }
    if (D(self + 0xe40u) != 0)
        return 0;
    selected = self + 0x4a54u + (u32)(s32)slot;
    if (B(selected) != (u8)value) {
        source = self + 0xe44u;
        destination = self + 0x4a63u;
        for (i = 0; i < 0xf07u; ++i) {
            u32 word = D(source);
            D(destination) = word;
            source += 4u;
            destination += 4u;
        }
        {
            u16 word = W(source);
            W(destination) = word;
            u8 byte = B(source + 2u);
            B(destination + 2u) = byte;
        }
        D(self + 0xc4deu) = 1;
        for (i = 0; i < 11u; ++i) {
            u32 address = self + 0x4a54u + i;
            if (B(address) == (u8)value) {
                u8 old = B(selected);
                B(address) = old;
            }
        }
        B(selected) = (u8)value;
        D(self + 0x4a5fu) = 1;
    }
    return 1;
error:
    cm_fn_00944cff(0x00a8b94cu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
