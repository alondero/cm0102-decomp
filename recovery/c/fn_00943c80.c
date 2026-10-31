/* Preferred-image VA 00943c80, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_009443a0(u32 allocator, u32 size);
extern u32 CM_CDECL cm_fn_009415c0(u32 stream);

u32 CM_CDECL cm_fn_00943c80(u32 allocator, volatile u32 stream,
                            volatile u32 capacity, volatile u32 initial)
{
    u32 object, limit, loaded, allocation, index, value, pointer;
    u16 last;
    object = cm_fn_009443a0(allocator, 0x14);
    limit = capacity;
    loaded = initial;
    D(object) = allocator;
    D(object + 4u) = limit;
    D(object + 8u) = loaded;
    allocation = cm_fn_009443a0(allocator, limit + limit);
    D(object + 0xcu) = allocation;
    allocation = cm_fn_009443a0(allocator, limit + limit);
    D(object + 0x10u) = allocation;
    index = 0;
    if ((s32)loaded > 0) {
        do {
            value = cm_fn_009415c0(stream);
            pointer = D(object + 0x10u);
            W(pointer + index * 2u) = (u16)value;
            value = cm_fn_009415c0(stream);
            pointer = D(object + 0xcu);
            W(pointer + index * 2u) = (u16)value;
            ++index;
        } while ((s32)index < (s32)loaded);
    }
    pointer = D(object + 0x10u);
    /* The original reads before the array even when no pair was loaded. */
    last = W(pointer + index * 2u - 2u);
    while ((s32)index < (s32)limit) {
        pointer = D(object + 0x10u);
        W(pointer + index * 2u) = last;
        value = cm_fn_009415c0(stream);
        pointer = D(object + 0xcu);
        W(pointer + index * 2u) = (u16)value;
        ++index;
    }
    return object;
}
