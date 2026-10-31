/* Preferred-image VA 004ca300, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004ca300(u32 self, volatile u32 target, volatile u32 output)
{
    u8 work[0x208];
    u32 list, row, record, id, array = 0, size, destination, i, value, version, result, pointer;
    u16 word;
    u8 count = 0, capacity, kind, byte;
    D((u32)work + 4u) = self;
    list = D(self + 0x34u);
    B((u32)work + 3u) = 0;
    if (list != 0) {
        value = D(list + 2u);
        D(list + 10u) = value;
        list = D(self + 0x34u);
        row = cm_fn_0068d1b0(list);
        while (row != 0) {
            record = target;
            id = D(row + 4u);
            if (id == D(record)) {
                kind = B(row + 0x35u) & 0x3fu;
                if (kind != 5 && kind != 6) {
                    capacity = B((u32)work + 3u);
                    if (capacity == count) {
                        capacity = (u8)(capacity + 20u);
                        B((u32)work + 3u) = capacity;
                        size = (u32)(s32)(s8)capacity * 111u;
                        array = (u32)cm_fn_00945667(array, size);
                        /* Native code tests the list field here, not the allocation. */
                        list = D(self + 0x34u);
                        if (list == 0)
                            goto allocation_error;
                    }
                    destination = array + (u32)(s32)(s8)count * 111u;
                    count = (u8)(count + 1u);
                    for (i = 0; i < 27; ++i) {
                        value = D(row + i * 4u);
                        D(destination + i * 4u) = value;
                    }
                    word = W(row + 108u);
                    W(destination + 108u) = word;
                    byte = B(row + 110u);
                    B(destination + 110u) = byte;
                    self = D((u32)work + 4u);
                }
            }
            list = D(self + 0x34u);
            row = cm_fn_0068d1b0(list);
        }
    }
    pointer = output;
    D(pointer) = array;
    return count;
allocation_error:
    cm_fn_00944cff(0x009a8190u, 0, 0, work + 0x108, work + 8);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009a8120u, version, work + 0x108, work + 8, 0x64cu, 0x009a8220u);
    cm_fn_0066f4e0(0x00ae28bcu, 0x009a8108u);
    cm_fn_0066f4e0(0x00ae26c4u, 0x009a7ff8u, 0x00ae24d0u);
    result = cm_fn_005e8290(0x00ae28bcu, 0x00ae26c4u, 0);
    pointer = output;
    result &= 0xffffff00u;
    D(pointer) = 0;
    return result;
}
