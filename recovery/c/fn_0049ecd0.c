/* Preferred-image VA 0049ecd0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   007e6f20 is RET4 and overwrites ECX before use, so its boundary is stdcall. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_STDCALL cm_fn_007e6f20(u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_0049ecd0(u32 record, u32 stage_out, u32 first_out, u32 second_out)
{
    u8 scratch[0x200];
    if (!record || !stage_out || !first_out || !second_out) {
        cm_fn_00944cff(0x0099c15c, 0, 0, scratch + 0x100, scratch);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x100, scratch, 0x2342);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    D(stage_out) = 0;
    D(first_out) = 0;
    D(second_out) = 0;
    u32 id = D(record);
    if ((s32)id <= -1) return 0;
    u32 table = D(0x00adadfc);
    u32 stage = D(table + id * 4u);
    if (!stage) return 0;
    u32 competition = D(record + 0x14);
    (void)D(0x00b74434);
    if (cm_fn_007e6f20(competition)) return 0;
    s8 index = (s8)B(record + 0x42);
    if (index > -1) {
        table = D(stage + 0xc);
        u32 child = D(table + (u32)(s32)index * 4u);
        D(stage_out) = child;
    } else {
        D(stage_out) = stage;
    }
    stage = D(stage_out);
    if (!stage || B(stage + 0x43) != 2) return 0;
    u8 row = 0;
    if ((s16)W(stage + 0x3e) > 0) {
        do {
            u32 entry = cm_fn_006826d0(stage, (u32)(s32)(s8)row);
            id = D(entry);
            u32 first = D(record + 0x1c);
            if (id == first) D(first_out) = entry;
            id = D(entry);
            u32 second = D(record + 0x20);
            if (id == second) D(second_out) = entry;
            stage = D(stage_out);
            row = (u8)(row + 1);
        } while ((s16)(s8)row < (s16)W(stage + 0x3e));
    }
    if (D(first_out) == 0) return 0;
    if (D(second_out) == 0) return 0;
    return 1;
}
