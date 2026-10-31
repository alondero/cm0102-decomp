/* Preferred-image VA 004c79a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004c79a0(u32 self, u32 force_append)
{
    u8 work[0x200];
    u32 count, pointer, index, capacity, size, allocation, version;
    if (force_append == 0) {
        count = D(self + 8u);
        pointer = D(self);
        index = 0;
        if ((s32)count > 0) {
            do {
                if (D(pointer) == 0xffffffffu)
                    return index;
                ++index;
                pointer += 80;
            } while ((s32)index < (s32)count);
        }
    }
    capacity = D(self + 4u);
    count = D(self + 8u);
    if (count == capacity) {
        pointer = D(self);
        capacity += 200;
        D(self + 4u) = capacity;
        size = capacity * 80u;
        if (pointer != 0)
            allocation = (u32)cm_fn_00945667(pointer, size);
        else
            allocation = (u32)cm_fn_00944e46(size);
        D(self) = allocation;
        if (allocation == 0) {
            cm_fn_00944cff(0x009a8190u, 0, 0, work + 0x100, work);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009a8120u, version, work + 0x100, work, 0x175u, 0x009a81c4u);
            cm_fn_0066f4e0(0x00ae28bcu, 0x009a8108u);
            cm_fn_0066f4e0(0x00ae26c4u, 0x009a7ff8u, 0x00ae24d0u);
            cm_fn_005e8290(0x00ae28bcu, 0x00ae26c4u, 0);
            return 0xffffffffu;
        }
    }
    count = D(self + 8u);
    D(self + 8u) = count + 1u;
    return count;
}
