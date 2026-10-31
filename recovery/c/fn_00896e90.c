/* Preferred-image VA 00896e90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32 self, u32 record, u32 type);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00896e90(u32 self, u32 record, volatile s8 second, volatile u32 third)
{
    u8 work[0x200];
    s8 slot;
    u32 type, index, base, row, i, version, line;
    u8 count;
    void *name, *extension;
    if (record == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x3a8u;
        goto error;
    }
    slot = second;
    if (slot < 0 || slot > 19) {
        name = work;
        extension = work + 0x100;
        line = 0x3b0u;
        goto error;
    }
    type = third;
    index = cm_fn_008982b0(self, record, type);
    if (index == 0xffffffffu)
        return 0;
    base = D(self + 0x601u);
    D(base + (u32)(s32)slot * 4u + index * 145u + 2u) = 0xffffffffu;
    if (B(self + 0x5f0u) != 0) {
        row = self + 0x38u;
        for (i = 0; i < 16u; ++i) {
            if (D(row) != 0 && D(row - 8u) == record && D(row - 4u) == type) {
                D(row) = 0;
                count = B(self + 0x5f0u);
                B(self + 0x5f0u) = (u8)(count - 1u);
            }
            row += 0x5cu;
        }
    }
    return 1;
error:
    cm_fn_00944cff(0x00a8b908u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
