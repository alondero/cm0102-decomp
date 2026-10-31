#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_009443a0(u32, u32);
extern u32 CM_CDECL cm_fn_009445d0(u32, u32);

u32 CM_CDECL cm_fn_0093fb70(u32 self)
{
    u32 width, old_stride, rows, size, value, second, allocator, allocated;
    u32 source, destination, remaining, index, sample, edge, middle, address;
    width = D(self + 0xce);
    old_stride = D(self + 0xd6);
    rows = D(self + 0xd2);
    size = width + 4u;
    value = D(self + 0xc4);
    D(self + 0xd6) = size;
    D(self + 0xce) = size;
    size *= rows;
    second = D(self + 0xa8);
    D(self + 0xc4) = value - 0x80u;
    allocator = D(self + 4);
    D(self + 0xa8) = second - 0x80u;
    allocated = cm_fn_009443a0(allocator, size);
    if ((s32)size > 0) {
        address = allocated;
        for (index = size >> 2; index != 0; --index) {
            D(address) = 0;
            address += 4;
        }
        for (index = size & 3u; index != 0; --index) {
            B(address) = 0;
            ++address;
        }
    }
    source = D(self + 0xda);
    if ((s32)rows > 0) {
        destination = allocated;
        remaining = rows;
        do {
            if ((s32)width > 0) {
                index = 0;
                do {
                    sample = B(source + index);
                    if (sample != 0) {
                        edge = (sample >> 3) - (sample >> 6);
                        middle = ((sample + 1u) >> 1) - ((sample + 15u) >> 5)
                            - ((sample + 4u) >> 3) - (sample >> 6);
                        address = destination + index;
                        value = B(address);
                        B(address) = (u8)(value + edge);
                        value = B(address + 1);
                        B(address + 1) = (u8)(value + middle - edge);
                        value = B(address + 2);
                        B(address + 2) = (u8)(value + middle);
                        value = B(address + 3);
                        B(address + 3) = (u8)(value + middle - edge);
                        value = B(address + 4);
                        B(address + 4) = (u8)(value + edge);
                    }
                    ++index;
                } while ((s32)index < (s32)width);
            }
            source += old_stride;
            destination += width + 4u;
            --remaining;
        } while (remaining != 0);
    }
    allocator = D(self + 4);
    source = D(self + 0xda);
    if (source == D(allocator + 0x64)) {
        D(allocator + 0x94) = 1;
        D(self + 0xda) = allocated;
        return allocator;
    }
    cm_fn_009445d0(allocator, source);
    D(self + 0xda) = allocated;
    return allocated;
}
