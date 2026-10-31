/* Preferred-image VA 008a9e90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005b3560(u32 self, u32 index, u32 flag, u32 third, u32 fourth);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008a9e90(u32 self, u32 index, volatile u32 fourth)
{
    u8 work[0x200];
    u32 source, destination, original_source, i, value, third, flag, text, fourth_value;
    u16 word;
    u8 byte;
    if ((s8)index < 0 || (s8)index > 10) {
        cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x19e7u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if (D(self + 0xe40u) != 0)
        return 0;
    if (B(self + 0x2727u) == 1)
        return 0;
    original_source = self + 0xe44u;
    source = original_source;
    destination = self + 0x4a63u;
    for (i = 0; i < 0xf07u; ++i) {
        value = D(source);
        D(destination) = value;
        source += 4u;
        destination += 4u;
    }
    /* Native snapshots the second argument between MOVSD and MOVSW/MOVSB. */
    fourth_value = fourth;
    word = W(source);
    W(destination) = word;
    byte = B(source + 2u);
    B(destination + 2u) = byte;
    third = D(self + 0x2729u);
    byte = B(self + 0x2727u);
    D(self + 0xc4deu) = 1;
    /* SETE CL preserves the upper three bytes of the prior DWORD load. */
    flag = (third & 0xffffff00u) | (byte == 2);
    cm_fn_005b3560(original_source, index, flag, third, fourth_value);
    D(self + 0x4a5fu) = 1;
    return 1;
}
