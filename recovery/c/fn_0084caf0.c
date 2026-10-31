#include "cmre.h"

typedef void (CM_THISCALL *release_fn)(u32 vtable, u32 object);

void CM_THISCALL cm_fn_0084caf0(u32 self)
{
    u32 object = *(volatile u32 *)(self + 0x28u);
    if (object != 0) {
        u32 vtable = *(volatile u32 *)object;
        ((release_fn)*(volatile u32 *)(vtable + 8u))(vtable, object);
    }
    *(volatile u32 *)(self + 0x28u) = 0;
}
