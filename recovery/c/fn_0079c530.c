/* Preferred-image VA 0x0079C530, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall with ECX as the destination and one stack argument (the
   source), ret 4. Copies dwords +0x00, +0x04, +0x08; then calls 00417C10 with
   ECX = destination + 0x0c, first stack argument = an 8-byte local buffer and
   second = source + 0x0c. It then copies, in order: word +0x14, dwords +0x16
   and +0x1a, words +0x1e, +0x20, +0x22, and bytes +0x24 through +0x2a.
   Returns the destination pointer in EAX. Field meanings remain unknown;
   00417C10 is an explicit boundary. */
#include "cmre.h"
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u8 *CM_THISCALL cm_fn_0079c530(u8 *dst, u8 *src)
{
    u32 local[2];

    *(u32 *)(dst + 0x00) = *(u32 *)(src + 0x00);
    *(u32 *)(dst + 0x04) = *(u32 *)(src + 0x04);
    *(u32 *)(dst + 0x08) = *(u32 *)(src + 0x08);
    cm_fn_00417c10((u32 *)(dst + 0x0c), local, (u16 *)(src + 0x0c));
    *(u16 *)(dst + 0x14) = *(u16 *)(src + 0x14);
    *(u32 *)(dst + 0x16) = *(u32 *)(src + 0x16);
    *(u32 *)(dst + 0x1a) = *(u32 *)(src + 0x1a);
    *(u16 *)(dst + 0x1e) = *(u16 *)(src + 0x1e);
    *(u16 *)(dst + 0x20) = *(u16 *)(src + 0x20);
    *(u16 *)(dst + 0x22) = *(u16 *)(src + 0x22);
    dst[0x24] = src[0x24];
    dst[0x25] = src[0x25];
    dst[0x26] = src[0x26];
    dst[0x27] = src[0x27];
    dst[0x28] = src[0x28];
    dst[0x29] = src[0x29];
    dst[0x2a] = src[0x2a];
    return dst;
}
