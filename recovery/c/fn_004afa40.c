#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);

u32 CM_THISCALL cm_fn_004afa40(volatile u32 *self)
{
    u32 pointer = self[0];
    if (pointer) {
        cm_fn_00944c94((void *)pointer);
        self[0] = 0;
    }
    pointer = self[1];
    if (pointer) {
        cm_fn_0068d040(pointer);
        cm_fn_00944c94((void *)pointer);
        self[1] = 0;
    }
    return 1;
}
