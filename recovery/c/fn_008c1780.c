#include "cmre.h"

extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, s32 param_3);
extern u32 CM_THISCALL cm_fn_0068d1b0(u8 *self);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* Thiscall with one stack argument (ret 4). The object's fields are used by offset:
   +0x92c sub-object (passed as ECX to 0x68d1b0), +0x92e value copied to +0x936.
   Scratch offsets: the 0x200-byte frame buffer at offset 0 (arg5 of 0x944cff) and
   offset 0x100 (arg4 of 0x944cff). The sole direct caller consumes AL only. */
u8 CM_THISCALL cm_fn_008c1780(u8 *self, u32 arg)
{
    u8 scratch[0x200];
    char *text;
    u32 club;
    u32 entry;
    u32 value;
    u32 cursor;
    u32 base;
    u32 index_value;
    u8 count;

    count = 0;
    if (arg == 0) {
        cm_fn_00944cff(0x00a8d514U, arg, arg, (void *)(scratch + 0x100), (void *)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0U, 0x009870e8U, (u32)text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x0c04U);
        cm_fn_005e8290((const char *)0x009870e0U,
                             (const char *)&CM_GLOBAL(u8, 0x00ae24d0), (s32)arg);
        CM_GLOBAL(volatile u32, 0x00b67a34) = arg;
        return 0;
    }

    club = cm_fn_00540a50(arg, 0, 1);
    value = *(volatile u32 *)(self + 0x92e);
    *(volatile u32 *)(self + 0x936) = value;
    cursor = cm_fn_0068d1b0(self + 0x92c);
    while (cursor != 0) {
        index_value = *(volatile u32 *)cursor;
        base = CM_GLOBAL(volatile u32, 0x00ae23bc);
        entry = *(volatile u32 *)(base + 0x39U + index_value * 0x6eU);
        if (entry == arg || (entry == club && club != 0)) {
            count = (u8)(count + 1);
        }
        cursor = cm_fn_0068d1b0(self + 0x92c);
    }
    return count;
}
