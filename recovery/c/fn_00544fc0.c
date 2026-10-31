/* Preferred-image VA 00544fc0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define SWAP(v) (((v) >> 24) | (((v) >> 8) & 0xff00u) | \
                 (((v) << 8) & 0xff0000u) | ((v) << 24))

extern u32 CM_CDECL cm_fn_0090bde0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_0090c140(u32 stream);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 size, u32 count, u32 stream);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                   void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 text, u32 flags);

u32 CM_THISCALL cm_fn_00544fc0(u32 self)
{
    u8 scratch[0x200];
    void *name, *extension;
    u32 path, file, magic, value, next, left, right, part, count, allocation;
    u32 version, line;

    path = cm_fn_0090bde0(0x00987da0u, 0x009b632cu);
    file = cm_fn_00945b00(path, 0x0098f928u);
    if (file == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x100, scratch);
        name = scratch + 0x100;
        extension = scratch;
        line = 0x4dd2u;
        goto error;
    }

    magic = cm_fn_0090c140(file);
    (void)cm_fn_00945bfe(self, 0x20u, 1, file);
    if (magic == 0xf1fffff1u) {
        value = D(self);
        next = D(self + 4u);
        D(self) = SWAP(value);
        value = next;
        next = D(self + 8u);
        D(self + 4u) = SWAP(value);
        value = next;
        next = D(self + 0xcu);
        D(self + 8u) = SWAP(value);
        value = next;
        next = D(self + 0x10u);
        D(self + 0xcu) = SWAP(value);
        value = next;
        next = D(self + 0x14u);
        D(self + 0x10u) = SWAP(value);
        value = next;
        next = D(self + 0x18u);
        D(self + 0x14u) = SWAP(value);
        value = next;
        next = D(self + 0x1cu);
        D(self + 0x18u) = SWAP(value);
        D(self + 0x1cu) = SWAP(next);
    }

    left = D(self + 4u);
    right = D(0x00ae234cu);
    if (left != right)
        goto bad_header;
    left = D(self + 0xcu);
    right = D(0x00ae2350u);
    if (left != right)
        goto bad_header;
    left = D(self + 0x10u);
    right = D(0x00ae2354u);
    if (left != right)
        goto bad_header;
    left = D(self + 8u);
    part = D(self + 0x14u);
    right = D(0x00ae2378u);
    left += part;
    right -= 0x7fu;
    if (left != right)
        goto bad_header;
    left = D(self + 0x18u);
    right = D(0x00ae2358u);
    if (left != right)
        goto bad_header;
    left = D(self + 0x1cu);
    right = D(0x00ae2388u);
    if (left != right)
        goto bad_header;

    count = D(self);
    allocation = (u32)cm_fn_00944e46(count * 79u);
    D(self + 0x20u) = allocation;
    if (allocation != 0) {
        count = D(self);
        (void)cm_fn_00945bfe(allocation, 79, count, file);
    }
    count = D(self + 4u);
    allocation = (u32)cm_fn_00944e46(count * 186u);
    D(self + 0x24u) = allocation;
    if (allocation != 0) {
        count = D(self + 4u);
        (void)cm_fn_00945bfe(allocation, 186, count, file);
    }
    count = D(self + 8u);
    allocation = (u32)cm_fn_00944e46(count * 83u);
    D(self + 0x28u) = allocation;
    if (allocation != 0) {
        count = D(self + 8u);
        (void)cm_fn_00945bfe(allocation, 83, count, file);
    }
    count = D(self + 0xcu);
    allocation = (u32)cm_fn_00944e46(count * 109u);
    D(self + 0x2cu) = allocation;
    if (allocation != 0) {
        count = D(self + 0xcu);
        (void)cm_fn_00945bfe(allocation, 109, count, file);
    }
    count = D(self + 0x18u);
    allocation = (u32)cm_fn_00944e46(count * 52u);
    D(self + 0x34u) = allocation;
    if (allocation != 0) {
        count = D(self + 0x18u);
        (void)cm_fn_00945bfe(allocation, 52, count, file);
    }
    count = D(self + 0x10u);
    allocation = (u32)cm_fn_00944e46(count * 52u);
    D(self + 0x30u) = allocation;
    if (allocation != 0) {
        count = D(self + 0x10u);
        (void)cm_fn_00945bfe(allocation, 52, count, file);
    }
    count = D(self + 0x1cu);
    allocation = (u32)cm_fn_00944e46(count * 79u);
    D(self + 0x3cu) = allocation;
    if (allocation != 0) {
        count = D(self + 0x1cu);
        (void)cm_fn_00945bfe(allocation, 79, count, file);
    }
    count = D(self + 0x14u);
    allocation = (u32)cm_fn_00944e46(count * 83u);
    D(self + 0x38u) = allocation;
    if (allocation != 0) {
        count = D(self + 0x14u);
        (void)cm_fn_00945bfe(allocation, 83, count, file);
    }
    (void)cm_fn_00945a09(file);
    return 1;

bad_header:
    (void)cm_fn_00945a09(file);
    cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 0x100);
    name = scratch;
    extension = scratch + 0x100;
    line = 0x4df1u;
error:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
