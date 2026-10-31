/* Preferred-image VA 007f7a20, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00804670(u32, u32);
extern u32 CM_THISCALL cm_fn_007f6cd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007b1260(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007f7a20(u32 self, u32 record, u32 owner)
{
    u8 scratch[0x204];
    u32 line;
    u8 *name = scratch + 0x104;
    u8 *extension = scratch + 4;
    if (!record) { line = 0x31e; goto error; }
    if (!owner) {
        u32 index = (u32)(s32)(s16)W(0x00b77c7a);
        owner = D(0x00b74c26u + index * 768u);
        if (!owner) {
            line = 0x328;
            name = scratch + 4;
            extension = scratch + 0x104;
            goto error;
        }
    }
    u8 value = B(owner);
    u8 base = B(0x00ae2364);
    u8 index = (u8)(value - base + 16);
    if ((s8)index < 0 || (s8)index > 15) {
        line = 0x332;
        name = scratch + 4;
        extension = scratch + 0x104;
        goto error;
    }
    u32 list = D(self + (u32)index * 4 + 0xc);
    if (list) return cm_fn_00804670(list, record);
    B((u32)scratch) = 0;
    for (u32 i = 0; i < 10; ++i) {
        /* The native local has only its low byte initialized before PUSH. */
        u32 kind = D((u32)scratch);
        u32 candidate = cm_fn_007f6cd0(self, kind, owner);
        if (candidate && B(candidate + 0xa) == 1) {
            cm_fn_007b1260(candidate, record);
            return 1;
        }
        B((u32)scratch) = (u8)(i + 1);
    }
    return 0;
error:
    cm_fn_00944cff(0x00a7b094, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
