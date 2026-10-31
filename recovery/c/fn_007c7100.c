/* Preferred-image VA 007c7100, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00952f5b(u32 string);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007c7100(u32 self, u32 size, volatile u16 tag, volatile u32 title)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 index, capacity, allocation, base, row, value, version, result, line;
    index = D(self + 0x14u);
    capacity = D(self + 0x18u);
    if ((s32)index >= (s32)capacity) {
        name = work + 0x100;
        extension = work;
        line = 0x56u;
        goto error;
    }
    allocation = (u32)cm_fn_00944e46(size * 8u);
    index = D(self + 0x14u);
    base = D(self);
    D(base + index * 18u) = allocation;
    index = D(self + 0x14u);
    base = D(self);
    row = base + index * 18u;
    if (D(row) == 0 && (s32)size > 0) {
        name = work;
        extension = work + 0x100;
        line = 0x5eu;
        goto error;
    }
    D(row + 4u) = size;
    index = D(self + 0x14u);
    base = D(self);
    D(base + index * 18u + 8u) = 0;
    index = D(self + 0x14u);
    value = tag;
    base = D(self);
    W(base + index * 18u + 12u) = (u16)value;
    value = cm_fn_00952f5b(title);
    index = D(self + 0x14u);
    base = D(self);
    D(base + index * 18u + 14u) = value;
    result = D(self + 0x14u) + 1u;
    D(self + 0x14u) = result;
    return result;
error:
    cm_fn_00944cff(0x00a7853cu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
