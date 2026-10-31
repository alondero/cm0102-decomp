/* Preferred-image VA 0x0094F4E0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: CRT _strcmp, cdecl, two stack arguments read from [esp+4] (s1) and
   [esp+8] (s2), no stack bytes popped. Reads go through a 4-byte-aligned loop with
   an unaligned prefix (one byte, then one word) handled first. Each comparison reads
   the byte from s1 then the byte from s2, stopping at the first difference or NUL.
   A difference returns -1 when the s1 byte is below the s2 byte as unsigned values
   (sbb/shl/inc sequence) and +1 otherwise; equal strings return 0. The aligned loop
   reads a whole dword from s1, so the fault address matches the original. */
#include "cmre.h"

s32 CM_CDECL cm_fn_0094f4e0(u8 *s1, u8 *s2)
{
    u8 *p = s1;
    u8 *q = s2;
    u32 w;
    u16 h;
    u8 a;
    u8 b;

    if (((u32)p & 3u) != 0) {
        if (((u32)p & 1u) != 0) {
            a = *p;
            p++;
            b = *q;
            if (a != b) {
                return (a < b) ? -1 : 1;
            }
            q++;
            if (a == 0) {
                return 0;
            }
            if (((u32)p & 2u) == 0) {
                goto aligned;
            }
        }
        h = *(u16 *)p;
        p += 2;
        a = (u8)h;
        b = q[0];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        if (a == 0) {
            return 0;
        }
        a = (u8)(h >> 8);
        b = q[1];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        if (a == 0) {
            return 0;
        }
        q += 2;
    }

aligned:
    for (;;) {
        w = *(u32 *)p;
        a = (u8)w;
        b = q[0];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        if (a == 0) {
            return 0;
        }
        a = (u8)(w >> 8);
        b = q[1];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        if (a == 0) {
            return 0;
        }
        a = (u8)(w >> 16);
        b = q[2];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        if (a == 0) {
            return 0;
        }
        a = (u8)(w >> 24);
        b = q[3];
        if (a != b) {
            return (a < b) ? -1 : 1;
        }
        p += 4;
        q += 4;
        if (a == 0) {
            return 0;
        }
    }
}
