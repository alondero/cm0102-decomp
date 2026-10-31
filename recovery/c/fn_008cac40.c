/* Preferred-image VA 008cac40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller VA008caa8e overwrites full EAX immediately at008caa93. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern void CM_CDECL cm_fn_00945333(u32 table, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_008cac40(u32 self, volatile u32 argument)
{
    u8 work[0x200];
    u32 capacity, count, base, allocation, destination, source, value, version;
    u8 byte;
    capacity = D(self + 0x890u);
    count = D(self + 0x894u);
    if (capacity == count) {
        base = D(self + 0x88cu);
        capacity += 50u;
        D(self + 0x890u) = capacity;
        if (base == 0)
            allocation = (u32)cm_fn_00944e46(capacity * 13u);
        else
            allocation = (u32)cm_fn_00945667(base, capacity * 13u);
        D(self + 0x88cu) = allocation;
        if (allocation == 0) {
            cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x100, work);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009a8120u, version, work + 0x100, work, 0x2581u, 0x00a8eb94u);
            cm_fn_0066f4e0(0x00ae28bcu, 0x009a8108u);
            cm_fn_0066f4e0(0x00ae26c4u, 0x009a7ff8u, 0x00ae24d0u);
            cm_fn_005e8290(0x00ae28bcu, 0x00ae26c4u, 0);
            return;
        }
    }
    count = D(self + 0x894u);
    base = D(self + 0x88cu);
    destination = base + count * 13u;
    source = argument;
    value = D(source);
    D(destination) = value;
    value = D(source + 4u);
    D(destination + 4u) = value;
    value = D(source + 8u);
    D(destination + 8u) = value;
    byte = B(source + 12u);
    B(destination + 12u) = byte;
    count = D(self + 0x894u);
    base = D(self + 0x88cu);
    ++count;
    D(self + 0x894u) = count;
    cm_fn_00945333(base, count, 13, 0x008b4e00u);
}
