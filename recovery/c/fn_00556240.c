#include "cmre.h"

typedef void (CM_THISCALL *dispatch_fn)(u32 self, u32 arg);

void CM_THISCALL cm_fn_00556240(u32 self, u32 arg)
{
    u32 offset;
    for (offset = 0; offset < 0xacu; offset += 4u) {
        u32 table = *(volatile u32 *)self;
        u32 object = *(volatile u32 *)(table + offset);
        if (object != 0) {
            u32 vtable = *(volatile u32 *)object;
            ((dispatch_fn)*(volatile u32 *)(vtable + 4u))(object, arg);
        }
    }
}
