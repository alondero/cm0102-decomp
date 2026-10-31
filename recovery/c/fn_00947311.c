/* Preferred-image VA 0x00947311, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl. Four pushes (ebx, ebp, esi, edi), one stack argument at [esp+0x14]
   (a byte pointer), plain ret so the caller pops. Result in EAX as a signed 32-bit value.
   Behaviour: skip leading class-8 characters, take an optional '+' or '-', read
   class-4 characters as decimal digits (value = value*10 + (c - '0')), and negate
   when the first character before the optional sign was '-'.
   Character classes: when the global at 0x00AD91DC is at most 1, the class is read from
   the table pointed to by the global at 0x00AD8FD0 at byte index 2*c (mask 8 for
   skip, mask 4 for digit). Otherwise the caller-side test calls the original routine
   0x0094C322 with pushes (c, 8) or (c, 4); it is cdecl and the caller pops its two
   arguments (add esp via pop ecx, pop ecx). Its behaviour is an explicit boundary.
   The table lookup in the listing uses `mov al, byte` with a full-register `and`
   that keeps only the masked bit, so the result is the masked table byte. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094c322(u32 ch, u32 cls);

s32 CM_CDECL cm_fn_00947311(u8 *p)
{
    u32 c;
    u32 first;
    u32 flag;
    u32 value;

    for (;;) {
        if (CM_GLOBAL(s32, 0x00ad91dc) < 2) {
            flag = CM_GLOBAL(u8 *, 0x00ad8fd0)[(u32)*p * 2] & 8;
        } else {
            flag = cm_fn_0094c322((u32)*p, 8);
        }
        if (flag == 0) {
            break;
        }
        p++;
    }

    c = (u32)*p;
    p++;
    first = c;
    if (c == 0x2d || c == 0x2b) {
        c = (u32)*p;
        p++;
    }

    value = 0;
    for (;;) {
        if (CM_GLOBAL(s32, 0x00ad91dc) < 2) {
            flag = CM_GLOBAL(u8 *, 0x00ad8fd0)[c * 2] & 4;
        } else {
            flag = cm_fn_0094c322(c, 4);
        }
        if (flag == 0) {
            break;
        }
        value = (c - 0x30) + value * 10;
        c = (u32)*p;
        p++;
    }

    if (first == 0x2d) {
        value = (u32)(0u - value);
    }
    return (s32)value;
}
