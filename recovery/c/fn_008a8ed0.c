/* Preferred-image VA 008a8ed0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008a8ed0(u32 self, volatile s8 first, volatile s8 second, volatile u32 third)
{
    u8 work[0x200];
    s8 group, slot;
    u32 source, destination, i, row, chosen, value, candidate, version;
    group = first;
    if (group < 0 || group > 4)
        goto error;
    slot = second;
    if (slot < 0 || slot > 1)
        goto error;
    if (D(self + 0xe40u) != 0)
        return 0;
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
    value = third;
    row = (u32)(s32)group * 2u;
    chosen = self + 0x4a2cu + (row + (u32)(s32)slot) * 4u;
    D(self + 0xc4deu) = 1;
    D(chosen) = value;
    if (value == 0 && slot < 1) {
        slot = (s8)(slot + 1);
        while (slot < 2) {
            candidate = self + 0x4a2cu + (row + (u32)(s32)slot) * 4u;
            value = D(candidate);
            if (value != 0) {
                value = D(candidate);
                D(chosen) = value;
                D(candidate) = 0;
                break;
            }
            slot = (s8)(slot + 1);
        }
    }
    D(self + 0x4a5fu) = 1;
    return 1;
error:
    cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1630u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
