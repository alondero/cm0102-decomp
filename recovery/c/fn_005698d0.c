#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);

void CM_THISCALL cm_fn_005698d0(volatile u32 *self)
{
    u32 pointer = self[1];
    if (pointer) {
        cm_fn_009452ca(pointer);
        self[1] = 0;
    }
    pointer = self[0];
    if (pointer) {
        cm_fn_0068d040(pointer);
        cm_fn_00944c94((void *)pointer);
        self[0] = 0;
    }
}
