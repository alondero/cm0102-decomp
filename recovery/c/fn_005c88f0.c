/* fn_005c88f0 (VA 0x005c88f0, 175 bytes). Thiscall with two stack arguments
   (ret 8) that returns its result in EAX. The loop is a record search over the
   list that starts at the object's pointer field at +0x34c8.

   Calls, all with the listing's receivers:
   - 0x0068d1b0: fastcall, the record pointer is ECX (set from [object+0x34c8]
     before each call, reloaded every time).
   - 0x004a7f20: thiscall with ECX = the current record; its two stack arguments
     are the two dwords at [param_2] and [param_2+4] (pushed in that listing order,
     so [param_2] is the lower stack argument). No caller cleanup follows the call,
     so the callee removes both. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004a7f20(u32 self, u32 arg2, u32 arg3);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_005c88f0(u32 param_1, u32 param_2, u32 param_3)
{
    u32 kept;
    u32 rec;
    u32 head;
    u32 hi;
    u32 lo;
    u32 r;

    kept = 0;
    head = *(volatile u32 *)(param_1 + 0x34c8);
    if (head == 0) {
        return 0;
    }
    *(volatile u32 *)(head + 0xa) = *(volatile u32 *)(head + 2);
    rec = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 0x34c8));

    while (rec != 0) {
        if (param_2 != 0) {
            hi = *(volatile u32 *)(param_2 + 4);
            lo = *(volatile u32 *)param_2;
            r = cm_fn_004a7f20(rec, lo, hi);
            if (r != 0) {
                if (*(volatile u32 *)(rec + 0xc) == param_3) {
                    if (kept != 0) {
                        return rec;
                    }
                    if (*(volatile u32 *)(rec + 0x10) != 0) {
                        if (*(volatile u32 *)(rec + 0x14) != 0) {
                            return rec;
                        }
                    }
                    kept = rec;
                }
            }
        } else {
            if (*(volatile u32 *)(rec + 0xc) == param_3) {
                if (*(volatile u32 *)(rec + 0x10) == 0 || *(volatile u32 *)(rec + 0x14) == 0) {
                    if (*(volatile u32 *)(rec + 0x8) != 0) {
                        return rec;
                    }
                }
            }
        }
        rec = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 0x34c8));
    }
    return kept;
}
