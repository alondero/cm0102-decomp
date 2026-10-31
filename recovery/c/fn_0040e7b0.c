/* Preferred-image VA 0x0040E7B0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (one stack argument `value`, ret 0), 0/1 in EAX.
   1. If global dword [0x00AD9C5C] is 0 and the signed byte [0x00B64738] is
      positive: scan entries i = 0 .. limit-1 of 72-byte records at 0x00B63D64 + i*72.
      The first entry with a non-null pointer p (dword at p == [0x009CFA0C]) and
      dword [0x00B63D60 + i*72] == 0 sets [0x00AD9C5C] = dword at
      (dword [0x00B63D70 + i*72]). The scan stops there.
   2. obj = [0x00AD9C5C]. link = dword obj+0x0c. limit = signed word obj+0x3e.
      head = dword [link] (read before the limit test).
   3. If limit <= 0 return 0. Otherwise list = dword obj+0xa7 and, for i = 0 ..
      limit-1 (6-byte stride): return 1 when dword list+6i == value, or when
      dword (dword head+0xa7) + 6i == value (re-read each step). Else return 0.
   Field meanings remain unknown. Object accesses are volatile so -O2 keeps the
   listing's read order; a missing object faults at the original address. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0040e7b0(u32 value)
{
    u32 obj;
    u32 off;
    u32 entry;
    u32 target;
    u32 link;
    u32 head;
    u32 list;
    u32 row;
    s16 limit;
    s16 i;

    obj = CM_GLOBAL(u32, 0x00ad9c5c);
    if (obj == 0) {
        limit = (s16)(s8)CM_GLOBAL(s8, 0x00b64738);
        if (limit > 0) {
            i = 0;
            do {
                off = (u32)(s32)i * 72u;
                entry = *(volatile u32 *)(0x00b63d64u + off);
                if (entry != 0 && *(volatile u32 *)entry == CM_GLOBAL(u32, 0x009cfa0c)
                    && *(volatile u32 *)(0x00b63d60u + off) == 0) {
                    target = *(volatile u32 *)(0x00b63d70u + off);
                    obj = *(volatile u32 *)target;
                    CM_GLOBAL(u32, 0x00ad9c5c) = obj;
                    break;
                }
                i++;
            } while (i < limit);
        }
    }

    link = *(volatile u32 *)(obj + 0x0c);
    limit = *(volatile s16 *)(obj + 0x3e);
    head = *(volatile u32 *)link;
    if (limit <= 0) {
        return 0;
    }
    list = *(volatile u32 *)(obj + 0xa7);
    i = 0;
    do {
        off = (u32)(s32)i * 6u;
        if (*(volatile u32 *)(list + off) == value) {
            return 1;
        }
        row = *(volatile u32 *)(head + 0xa7);
        if (*(volatile u32 *)(row + off) == value) {
            return 1;
        }
        i++;
    } while (i < limit);
    return 0;
}
