#include "cmre.h"

/* fn_006cd3d0: thiscall (ECX = object, read before written) with seven stack arguments
   (ret 0x1c). Preferred-image VA 0x006cd3d0, image base 0x00400000.
   Object fields: 0x12 byte counter incremented on a result of 1; 0x2b signed kind byte
   (<= 1 passes); 0x19 signed byte (>= 0 passes); 0xf5ae dword set to the chosen record.
   The table is indexed by the signed byte argument 1 (44-byte rows of record pointers),
   with a signed byte count at row + 0x58.
   The 0x0070dfd0 call pushes param_8, param_7, param_4 and param_5 in that order. Its third
   slot receives the integer bits of param_4 (the lifted prototype names it float, but the
   original pushes an integer register there), so it is declared u32 here. */

extern u8 CM_THISCALL cm_fn_0070dfd0(int param_1, short param_2, u32 param_3, char param_4, char param_5);

u8 CM_THISCALL cm_fn_006cd3d0(int param_1, s8 param_2, int param_3, u32 param_4, u32 param_5,
                              s8 param_6, u32 param_7, u32 param_8)
{
    s32 edi;
    u8 local_5;
    u32 i;
    u32 *elem;
    u32 rec;
    u8 r;

    edi = (s32)param_2;
    local_5 = 0;
    i = 0;

    if ((s8)*(volatile s8 *)((u32)param_3 + (u32)edi + 0x58u) > 0) {
        elem = (u32 *)((u32)param_3 + (u32)edi * 0x2cu);
        do {
            rec = *(volatile u32 *)elem;
            if ((s8)*(volatile s8 *)(rec + 0x2bu) <= 1 && (s8)*(volatile s8 *)(rec + 0x19u) >= 0) {
                if (param_6 == 2) {
                    local_5 = (u8)(local_5 + 1u);
                } else {
                    r = cm_fn_0070dfd0((int)rec, (short)param_5, param_4, (char)param_7, (char)param_8);
                    if ((s8)r == 1) {
                        *(volatile u8 *)(rec + 0x12u) = (u8)(*(volatile u8 *)(rec + 0x12u) + 1u);
                        *(volatile u32 *)((u32)param_1 + 0xf5aeu) = rec;
                        return 1u;
                    }
                    if ((s8)r == 2) {
                        *(volatile u32 *)((u32)param_1 + 0xf5aeu) = rec;
                        return 2u;
                    }
                }
            }
            i = i + 1u;
            elem = elem + 1;
        } while ((s32)i < (s32)*(volatile s8 *)((u32)param_3 + (u32)edi + 0x58u));
    }

    return local_5;
}
