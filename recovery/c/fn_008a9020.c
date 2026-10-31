#include "cmre.h"
/* Preferred-image VA 0x008a9020, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_008a9020(u32 self, s8 index, u32 value)
{
    u8 scratch[512];
    u32 source, destination, i, word, other;
    u16 half;
    u8 byte;
    char *version;
    if (index < 0 || index > 1) {
        cm_fn_00944cff(0x00a8b94c, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x1674);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if (D(self + 0xe40)) return 0;
    source = self + 0xe44;
    destination = self + 0x4a63;
    for (i = 0; i < 0xf07; ++i) {
        word = D(source);
        D(destination) = word;
        source += 4;
        destination += 4;
    }
    half = W(source);
    W(destination) = half;
    byte = B(source + 2);
    B(destination + 2) = byte;
    D(self + 0xc4de) = 1;
    D(self + (u32)(s32)index * 4u + 0x4a24) = value;
    if (!value && !index) {
        other = D(self + 0x4a28);
        if (other) {
            D(self + 0x4a24) = other;
            D(self + 0x4a28) = 0;
        }
    }
    D(self + 0x4a5f) = 1;
    return 1;
}
