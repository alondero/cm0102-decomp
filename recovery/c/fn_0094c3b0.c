#include "cmre.h"

u32 *CM_CDECL cm_fn_0094c3b0(u8 *dest, u8 *source)
{
    volatile u8 *d = (volatile u8 *)dest;
    volatile u8 *s = (volatile u8 *)source;
    volatile u8 *original_dest = (volatile u8 *)dest;
    u32 word;
    u32 sum;
    u32 mark;

    if (((u32)d & 3) != 0) {
        for (;;) {
            u8 byte = *d;
            ++d;
            if (byte == 0) {
                --d;
                goto destination_found;
            }
            if (((u32)d & 3) == 0)
                break;
        }
    }
    for (;;) {
        word = *(volatile u32 *)d;
        sum = 0x7efefeff + word;
        mark = (word ^ 0xffffffff) ^ sum;
        d += 4;
        if ((mark & 0x81010100) != 0) {
            word = *(volatile u32 *)(d - 4);
            if ((word & 0xff) == 0) { d -= 4; break; }
            if ((word & 0xff00) == 0) { d -= 3; break; }
            if ((word & 0xff0000) == 0) { d -= 2; break; }
            if ((word & 0xff000000) == 0) { d -= 1; break; }
        }
    }
destination_found:

    if (((u32)s & 3) != 0) {
        for (;;) {
            u8 byte = *s;
            ++s;
            if (byte == 0) {
                *d = byte;
                return (u32 *)original_dest;
            }
            *d = byte;
            ++d;
            if (((u32)s & 3) == 0)
                break;
        }
    }
    for (;;) {
        word = *(volatile u32 *)s;
        sum = 0x7efefeff + word;
        mark = (word ^ 0xffffffff) ^ sum;
        word = *(volatile u32 *)s;
        s += 4;
        if ((mark & 0x81010100) != 0) {
            if ((word & 0xff) == 0) {
                *(volatile u8 *)d = (u8)word;
                return (u32 *)original_dest;
            }
            if ((word & 0xff00) == 0) {
                *(volatile u16 *)d = (u16)word;
                return (u32 *)original_dest;
            }
            if ((word & 0xff0000) == 0) {
                *(volatile u16 *)d = (u16)word;
                *(volatile u8 *)(d + 2) = 0;
                return (u32 *)original_dest;
            }
            if ((word & 0xff000000) == 0) {
                *(volatile u32 *)d = word;
                return (u32 *)original_dest;
            }
        }
        *(volatile u32 *)d = word;
        d += 4;
    }
}
