/* Preferred-image VA 0x005D8B80, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ECX is not read; ret 0), two stack arguments: key (dword) and
   text (pointer, stored in EAX on entry to the search). Returns EAX = 0 or 1.
   Records start at 0x00AE2D28 and are 0x68 bytes apart; the scan covers record
   addresses p with p < 0x00AE3138 (both scans use the same range).
   Pass 1: if no record has dword p+0 equal to key, return 1 (EAX keeps its
   initial value).
   Pass 2: for each record in the same range whose dword p+0 equals key, compare
   the NUL-terminated string at p+4 with text. The compare reads byte pairs in the
   listing's order (record byte, text byte; stop at a NUL), and equal means the two
   strings match through their terminator. A match returns 1; if no record matches,
   the routine returns 0.
   Field meanings remain unknown. The record and text accesses are volatile so -O2
   keeps the listing's read order. */
#include "cmre.h"

u32 CM_CDECL cm_fn_005d8b80(u32 key, u8 *text)
{
    u32 record;
    u32 found;
    u8 *pa;
    u8 *pb;
    u8 ca;
    u8 cb;
    u32 equal;

    found = 0u;
    for (record = 0x00ae2d28u; record < 0x00ae3138u; record += 0x68u) {
        if (*(volatile u32 *)record == key) {
            found = 1u;
            break;
        }
    }
    if (found == 0u) {
        return 1u;
    }

    for (record = 0x00ae2d28u; record < 0x00ae3138u; record += 0x68u) {
        if (*(volatile u32 *)record != key) {
            continue;
        }
        pa = (u8 *)(record + 4u);
        pb = text;
        equal = 0u;
        for (;;) {
            ca = *(volatile u8 *)pa;
            if (ca != *(volatile u8 *)pb) {
                break;
            }
            if (ca == 0u) {
                equal = 1u;
                break;
            }
            cb = *(volatile u8 *)(pa + 1);
            if (cb != *(volatile u8 *)(pb + 1)) {
                break;
            }
            if (cb == 0u) {
                equal = 1u;
                break;
            }
            pa += 2;
            pb += 2;
        }
        if (equal != 0u) {
            return 1u;
        }
    }
    return 0u;
}
