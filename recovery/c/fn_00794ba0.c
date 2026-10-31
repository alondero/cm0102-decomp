#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);

void CM_THISCALL cm_fn_00794ba0(u32 self)
{
    u32 outer, inner, cursor = self;
    for (outer = 0; outer < 16u; ++outer) {
        for (inner = 0; inner < 2u; ++inner) {
            u32 list = *(volatile u32 *)cursor;
            if (list != 0) {
                cm_fn_0068d040(list);
                cm_fn_00944c94((void *)list);
            }
            cursor += 4u;
        }
    }
}
