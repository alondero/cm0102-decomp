/* Preferred-image VA 0x00933080, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   cdecl, three stack arguments. Signed comparisons of wrapping DWORD sums.
   The byte-read loop writes through the original first argument slot;
   the receiver remains the cached entry value in ESI. */
#include "cmre.h"

extern u32 cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern s32 cm_fn_00946a22(volatile u8 *file, u32 position, u32 mode);
extern s32 cm_fn_00946aae(volatile u8 *file);

u32 cm_fn_00933080(u32 arg1, u32 offset, u32 mode)
{
    u32 self = arg1;
    u32 position;
    u32 row;
    u32 begin;
    u32 end;
    u32 length;
    u32 method;
    u32 file;
    u32 index = 0;
    if (self == 0 || *(volatile u32 *)(self + 4u) != 0)
        return 0xffffffffu;
    if (mode == 0) {
        row = *(volatile u32 *)(self + 0xcu);
        position = *(volatile u32 *)(self + row * 0x10cu + 0x25u);
        position += offset;
    } else if (mode == 1) {
        file = *(volatile u32 *)self;
        position = (u32)cm_fn_00946aae((volatile u8 *)file);
        position += offset;
    } else if (mode == 2) {
        row = *(volatile u32 *)(self + 0xcu);
        length = *(volatile u32 *)(self + row * 0x10cu + 0x29u);
        begin = *(volatile u32 *)(self + row * 0x10cu + 0x25u);
        position = length + begin + offset;
    } else {
        position = 0;
    }
    row = self + *(volatile u32 *)(self + 0xcu) * 0x10cu;
    begin = *(volatile u32 *)(row + 0x25u);
    length = *(volatile u32 *)(row + 0x29u);
    end = length + begin;
    if ((s32)position >= (s32)end || (s32)position < (s32)begin)
        return 0xffffffffu;
    method = *(volatile u32 *)(self + 0x19u);
    *(volatile u32 *)(self + 8u) = position - begin;
    if (method != 4) {
        file = *(volatile u32 *)self;
        return (u32)cm_fn_00946a22((volatile u8 *)file, position, 0);
    }
    begin = *(volatile u32 *)(row + 0x25u);
    file = *(volatile u32 *)self;
    cm_fn_00946a22((volatile u8 *)file, begin, 0);
    position = *(volatile u32 *)(self + 8u);
    *(volatile u32 *)(self + 0x14u) = 0;
    *(volatile u32 *)(self + 0x10u) = 0;
    if ((s32)position > 0) {
        do {
            cm_fn_00932c30((u32)&arg1, 1, 1, self);
            position = *(volatile u32 *)(self + 8u);
            index += 1u;
        } while ((s32)index < (s32)position);
    }
    return 0;
}
