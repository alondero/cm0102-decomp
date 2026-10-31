/* Preferred-image VA 005c4b60, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00945667(u32 old, u32 size);
extern void CM_THISCALL cm_fn_00417c10(u32 self, void *output, u32 other);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005c4b60(u32 self, volatile u32 first, volatile u32 second, volatile u32 third, volatile u32 date)
{
    u8 work[0x208];
    u32 capacity, count, base, pointer, value, allocation, version, result;
    capacity = D(self + 0x34e4u);
    count = D(self + 0x34e0u);
    if (count == capacity) {
        pointer = D(self + 0x34d8u);
        capacity += 100u;
        D(self + 0x34e4u) = capacity;
        allocation = (u32)cm_fn_00945667(pointer, capacity * 20u);
        D(self + 0x34d8u) = allocation;
        if (allocation == 0) {
            cm_fn_00944cff(0x009cd0a0u, 0, 0, work + 0x108, work + 8);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x797u);
            result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
            return result;
        }
    }
    count = D(self + 0x34e0u);
    pointer = first;
    base = D(self + 0x34d8u);
    value = D(pointer);
    D(base + count * 20u + 8u) = value;
    count = D(self + 0x34e0u);
    pointer = second;
    base = D(self + 0x34d8u);
    value = D(pointer);
    D(base + count * 20u + 12u) = value;
    count = D(self + 0x34e0u);
    pointer = third;
    base = D(self + 0x34d8u);
    value = D(pointer);
    D(base + count * 20u + 16u) = value;
    pointer = date;
    count = D(self + 0x34e0u);
    base = D(self + 0x34d8u);
    cm_fn_00417c10(base + count * 20u, work, pointer);
    result = D(self + 0x34e0u) + 1u;
    D(self + 0x34e0u) = result;
    return result;
}
