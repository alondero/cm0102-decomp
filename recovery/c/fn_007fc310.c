#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007f6cd0(u32 self, s8 kind, u32 record);
extern u32 CM_THISCALL cm_fn_007b14a0(u32 self, u32 flag);

u32 CM_THISCALL cm_fn_007fc310(u32 self, u32 record)
{
    u32 i;
    for (i = 0; i < 10; ++i) {
        u32 entry = cm_fn_007f6cd0(self, (s8)i, record);
        cm_fn_007b14a0(entry, 1);
    }
    return 1;
}
