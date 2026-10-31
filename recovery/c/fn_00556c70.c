/* Preferred-image VA 0x00556C70, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall with ECX as the destination and one stack argument (the
   source), ret 4. Copies bytes +0x00 and +0x01 and word +0x02; calls 00417C10
   with ECX = destination + 0x04, first stack argument = an 8-byte local buffer
   and second = source + 0x04. Then copies bytes +0x0c..+0x0f one at a time in a
   loop, byte +0x10 and word +0x11. Byte +0x13 then takes bits 0, 1 and 2 from
   the source in three separate read-merge-store steps (d ^ ((s ^ d) & mask)),
   re-reading the source byte each time. Returns the destination in EAX.
   Field meanings remain unknown; 00417C10 is an explicit boundary. */
#include "cmre.h"
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u8 *CM_THISCALL cm_fn_00556c70(u8 *dst, u8 *src)
{
    u32 local[2];
    u32 i;
    u8 bits;

    dst[0x00] = src[0x00];
    dst[0x01] = src[0x01];
    *(u16 *)(dst + 0x02) = *(u16 *)(src + 0x02);
    cm_fn_00417c10((u32 *)(dst + 0x04), local, (u16 *)(src + 0x04));
    for (i = 0; i < 4; i++) {
        dst[0x0c + i] = src[0x0c + i];
    }
    dst[0x10] = src[0x10];
    *(u16 *)(dst + 0x11) = *(u16 *)(src + 0x11);
    bits = dst[0x13];
    bits = (u8)(((src[0x13] ^ bits) & 1) ^ bits);
    dst[0x13] = bits;
    bits = (u8)(((src[0x13] ^ bits) & 2) ^ bits);
    dst[0x13] = bits;
    bits = (u8)(((src[0x13] ^ bits) & 4) ^ bits);
    dst[0x13] = bits;
    return dst;
}
