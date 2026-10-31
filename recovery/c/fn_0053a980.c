/* Preferred-image VA 0x0053A980, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: three stack arguments (no ECX/EDX input, ret 0, so cdecl):
   [esp+4] = destination record, [esp+8] = unused by this routine,
   [esp+0xc] = source record. Twenty-one bytes are copied, each from a source
   offset to a destination offset, in the order below. The copied bytes are
   not interpreted; field meanings remain unknown. No return value. */
#include "cmre.h"

void CM_CDECL cm_fn_0053a980(void *dst, u32 arg2_unused, const void *src)
{
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;

    (void)arg2_unused;
    d[0x0e] = s[0x00];
    d[0x0f] = s[0x03];
    d[0x12] = s[0x06];
    d[0x13] = s[0x09];
    d[0x14] = s[0x0c];
    d[0x15] = s[0x0f];
    d[0x16] = s[0x12];
    d[0x1a] = s[0x15];
    d[0x1c] = s[0x18];
    d[0x1d] = s[0x1b];
    d[0x1f] = s[0x1e];
    d[0x20] = s[0x21];
    d[0x22] = s[0x24];
    d[0x11] = s[0x27];
    d[0x10] = s[0x2a];
    d[0x17] = s[0x2d];
    d[0x18] = s[0x30];
    d[0x19] = s[0x33];
    d[0x1b] = s[0x36];
    d[0x1e] = s[0x39];
    d[0x21] = s[0x3c];
}
