/* Preferred-image VA 008cb810, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern u32 CM_THISCALL cm_fn_008cb940(u32 self, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008cb810(u32 self, u32 value)
{
    u8 work[0x200];
    u32 capacity, count, pointer, size, allocation, version, receiver;
    capacity = D(self + 0x866u);
    count = D(self + 0x86au);
    if (capacity == count) {
        pointer = D(self + 0x862u);
        capacity += 100;
        D(self + 0x866u) = capacity;
        size = capacity * 37u;
        if (pointer == 0)
            allocation = (u32)cm_fn_00944e46(size);
        else
            allocation = (u32)cm_fn_00945667(pointer, size);
        D(self + 0x862u) = allocation;
        if (allocation == 0) {
            cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x100, work);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009a8120u, version, work + 0x100, work, 0x2794u, 0x00a8ec34u);
            cm_fn_0066f4e0(0x00ae28bcu, 0x009a8108u);
            cm_fn_0066f4e0(0x00ae26c4u, 0x009a7ff8u, 0x00ae24d0u);
            return cm_fn_005e8290(0x00ae28bcu, 0x00ae26c4u, 0);
        }
    }
    count = D(self + 0x86au);
    pointer = D(self + 0x862u);
    receiver = pointer + count * 37u;
    D(self + 0x86au) = count + 1u;
    return cm_fn_008cb940(receiver, value);
}
