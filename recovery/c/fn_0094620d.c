#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094bef1(u32 pointer);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);

u32 CM_CDECL cm_fn_0094620d(u32 value)
{
    u32 length = cm_fn_0094bef1(CM_GLOBAL(volatile u32, 0x00de4638));
    u32 base = CM_GLOBAL(volatile u32, 0x00de4638);
    u32 cursor = CM_GLOBAL(volatile u32, 0x00de4634);
    if (length < cursor - base + 4u) {
        u32 size = cm_fn_0094bef1(base) + 0x10u;
        u32 current = CM_GLOBAL(volatile u32, 0x00de4638);
        u32 replacement = (u32)cm_fn_00945667(current, size);
        u32 delta;
        if (replacement == 0)
            return 0;
        cursor = CM_GLOBAL(volatile u32, 0x00de4634);
        delta = cursor - CM_GLOBAL(volatile u32, 0x00de4638);
        CM_GLOBAL(volatile u32, 0x00de4638) = replacement;
        cursor = replacement + (u32)((s32)delta >> 2) * 4u;
        CM_GLOBAL(volatile u32, 0x00de4634) = cursor;
    }
    *(volatile u32 *)cursor = value;
    CM_GLOBAL(volatile u32, 0x00de4634) += 4u;
    return value;
}
