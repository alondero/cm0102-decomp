/* Preferred-image VA 008c7560, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053d740(u8 *person, u32 *output);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00946464(void *key, u32 table, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008c7560(u32 self, u32 person, u32 club, u32 flag)
{
    u8 work[0x210];
    u32 pointer, version, receiver, id, count, flags, table;
    if (club == 0)
        return 0;
    pointer = D(club + 0x53u);
    if (pointer == 0)
        return 0;
    if (person == 0) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x110, work + 0x10);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x110, work + 0x10, 0x1c23u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if (B(pointer + 0x76u) != 0x0eu)
        return 0;
    if (cm_fn_0053d740((u8 *)person, (u32 *)0) != 0)
        return 0;
    if (flag != 0)
        return 1;
    receiver = D(0x00ae1050u);
    pointer = cm_fn_004cf5a0(receiver, person);
    if (pointer == 0)
        return 1;
    if ((B(pointer + 0x35u) & 0x3fu) != 2u)
        return 1;
    id = D(person);
    count = D(0x00ae2364u);
    if ((s32)id >= (s32)count)
        return 1;
    flags = D(self + 0x86eu);
    if ((B(flags + id * 8u) & 0x20u) == 0)
        return 1;
    table = D(self + 0x93au);
    D((u32)work) = id;
    count = D(self + 0x93eu);
    return cm_fn_00946464(work, table, count, 14, 0x008e55c0u) == 0;
}
