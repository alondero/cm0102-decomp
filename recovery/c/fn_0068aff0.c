/* Preferred-image VA 0x0068aff0, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver and six stack DWORDs, ret 0x18. Retain the full incidental EAX:
   initializer result for a nonpositive count, FFFFFFFF when both slots are
   resolved, otherwise the incremented traversal count. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0054a130(u32 self, u32 day, u32 month,
                                     u32 year, u32 value);
extern u32 CM_CDECL cm_fn_00545770(u32 team);

/* The original compares alternating bytes, advancing both addresses by two
   only after the second byte matches. Volatile loads retain its fault order. */
static int same_name(u32 left, u32 right)
{
    for (;;) {
        u8 a = CM_GLOBAL(volatile u8, left);
        u8 b = CM_GLOBAL(volatile u8, right);
        if (a != b)
            return 0;
        if (a == 0)
            return 1;
        a = CM_GLOBAL(volatile u8, left + 1u);
        b = CM_GLOBAL(volatile u8, right + 1u);
        if (a != b)
            return 0;
        left += 2u;
        right += 2u;
        if (a == 0)
            return 1;
    }
}

u32 CM_THISCALL cm_fn_0068aff0(u32 self, u32 competition, u32 day,
                               u32 month, u32 year, u32 name1, u32 name2)
{
    u32 result, offset = 0, index = 0, table, team, name, value;
    s32 count;

    result = cm_fn_0054a130(self, day, month, year, 0xffffffffu);
    CM_GLOBAL(volatile u32, self + 0xcu) = 0xffffffffu;
    CM_GLOBAL(volatile u32, self + 8u) = 0xffffffffu;
    count = CM_GLOBAL(volatile s16, competition + 0x3eu);
    if (count <= 0)
        return result;

    for (;;) {
        table = CM_GLOBAL(volatile u32, competition + 0xb1u);
        team = CM_GLOBAL(volatile u32, table + offset);
        name = cm_fn_00545770(team);
        if (same_name(name, name1)) {
            table = CM_GLOBAL(volatile u32, competition + 0xb1u);
            team = CM_GLOBAL(volatile u32, table + offset);
            value = CM_GLOBAL(volatile u32, team);
            CM_GLOBAL(volatile u32, self + 8u) = value;
        }

        table = CM_GLOBAL(volatile u32, competition + 0xb1u);
        team = CM_GLOBAL(volatile u32, table + offset);
        name = cm_fn_00545770(team);
        if (same_name(name, name2)) {
            table = CM_GLOBAL(volatile u32, competition + 0xb1u);
            team = CM_GLOBAL(volatile u32, table + offset);
            value = CM_GLOBAL(volatile u32, team);
            CM_GLOBAL(volatile u32, self + 0xcu) = value;
        }

        value = CM_GLOBAL(volatile u32, self + 8u);
        if (value != 0xffffffffu &&
            CM_GLOBAL(volatile u32, self + 0xcu) != 0xffffffffu)
            return 0xffffffffu;
        offset += 0x3bu;
        count = CM_GLOBAL(volatile s16, competition + 0x3eu);
        ++index;
        if ((s32)index >= count)
            return index;
    }
}
