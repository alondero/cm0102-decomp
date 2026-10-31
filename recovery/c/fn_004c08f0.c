/* Preferred-image VA 004c08f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
extern u32 CM_CDECL cm_fn_004c0720(u32 club);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_004c08f0(volatile u32 first, volatile u32 second)
{
    u8 work[0x204];
    u32 count, bytes, allocation, active, cursor, difference, remaining, value;
    u32 outer, next, saved, left, right, version, result;
    count = second;
    bytes = count * 4u;
    allocation = (u32)cm_fn_00944e46(bytes);
    active = allocation;
    D((u32)work) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x0099ec20u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x292du);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    /* count*4 wraps first; REP STOSB has zero remainder for this width. */
    cursor = allocation;
    remaining = bytes >> 2;
    while (remaining != 0) {
        D(cursor) = 0x60606060u;
        cursor += 4u;
        --remaining;
    }
    if ((s32)count > 0) {
        difference = first;
        cursor = allocation;
        difference -= allocation;
        remaining = count;
        do {
            value = D(difference + cursor);
            if (value != 0) {
                value = cm_fn_004c0720(value);
                active = D((u32)work);
                D(cursor) = value;
            } else {
                D(cursor) = 0;
            }
            cursor += 4u;
            --remaining;
        } while (remaining != 0);
    }
    if ((s32)(count - 1u) > 0) {
        outer = 1;
        do {
            if ((s32)outer < (s32)count) {
                difference = first;
                saved = D((u32)work);
                difference -= saved;
                next = active + 4u;
                count -= outer;
                do {
                    left = D(active);
                    right = D(next);
                    if ((s32)left < (s32)right) {
                        D(active) = right;
                        D(next) = left;
                        right = D(next + difference);
                        left = D(active + difference);
                        D(active + difference) = right;
                        D(next + difference) = left;
                    }
                    next += 4u;
                    --count;
                } while (count != 0);
                count = second;
            }
            ++outer;
            active += 4u;
        } while ((s32)(outer - 1u) < (s32)(count - 1u));
        active = D((u32)work);
    }
    return (u32)cm_fn_009452ca(active);
}
