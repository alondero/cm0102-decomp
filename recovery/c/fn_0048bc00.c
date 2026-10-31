/* Preferred-image VA 0048bc00, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both direct callers overwrite EAX (008889d6, 0088a393). */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 slot, u32 value, u32 zero);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);

void CM_CDECL cm_fn_0048bc00(u32 first, u32 second)
{
    u8 work[0x1c];
    u32 club, candidate, cursor, remaining, slot, mask = 0, result, id;
    s8 count = 0;
    if (first == 0 || second == 0)
        return;
    club = D(second + 0x39u);
    if (club == 0)
        return;
    if (cm_fn_007fe600(0x00b74450u, 0x0048bd20u, 0x0048c290u, 1, 0x005a8f60u, 0) == 0)
        return;
    candidate = D(club + 0xd3u);
    if (candidate != 0 && D(candidate + 0x69u) != 0 && first != candidate) {
        D((u32)work + 4u) = candidate;
        count = 1;
    }
    cursor = club + 0x19fu;
    for (remaining = 5; remaining != 0; --remaining) {
        candidate = D(cursor);
        if (candidate != 0 && D(candidate + 0x69u) != 0 && first != candidate) {
            slot = (u32)(s32)count;
            ++count;
            D((u32)work + 4u + slot * 4u) = candidate;
        }
        cursor += 4;
    }
    if (count == 0)
        return;
    for (slot = 0; (s32)slot < (s32)count; ++slot) {
        if (second == D((u32)work + 4u + slot * 4u))
            mask = 1u << (slot & 31u);
    }
    id = D(first);
    cm_fn_007ff270(0x00b74450u, 0, id, 0);
    cm_fn_007ff270(0x00b74450u, 1, mask, 0);
    result = cm_fn_00540a50(club, (u32)work, 1);
    if (result != 0 && D((u32)work) == 0)
        club = result;
    id = D(club);
    cm_fn_007ff270(0x00b74450u, 2, id, 0);
}
