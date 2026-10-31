#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005b57c0(u32 self, s8 value);
extern u32 CM_THISCALL cm_fn_005b5910(u32 self, s8 value, u32 *entry);
extern s8 CM_THISCALL cm_fn_006d4330(u32 self, s8 group, s8 slot);

void CM_THISCALL cm_fn_006bbf60(u32 self)
{
    u32 group = 0;
    u32 group_base = self + 0x91e2u;
    u32 row = self + 0x47afu;

    while ((s32)group < 2) {
        u32 slot = 0;
        while ((s32)slot < 0x14) {
            s8 first = *(volatile s8 *)row;
            if (first >= 0) {
                u32 flags = cm_fn_005b57c0(group_base, first);
                if ((u8)flags & 0x10u) {
                    s8 second = *(volatile s8 *)row;
                    flags = cm_fn_005b57c0(group_base, second);
                    if ((u8)flags & 4u) {
                        s8 result = cm_fn_006d4330(self, (s8)group, (s8)slot);
                        u32 index = (u32)(s32)result + (group == 0 ? 0x14u : 0u);
                        u32 offset = index * 446u + 0x47ffu;
                        s8 final_value = *(volatile s8 *)row;
                        u32 entry = *(volatile u32 *)(self + offset);
                        (void)cm_fn_005b5910(group_base, final_value, (u32 *)entry);
                    }
                }
            }
            row += 0x1beu;
            slot++;
        }
        group++;
        group_base += 0x18e3u;
    }
}
