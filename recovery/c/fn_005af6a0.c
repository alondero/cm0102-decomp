#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);

void CM_THISCALL cm_fn_005af6a0(volatile u32 *self)
{
    u32 i;
    for (i = 0; i != 16; ++i) {
        u32 item = self[i];
        if (item) {
            u32 data = *(volatile u32 *)item;
            if (data) {
                cm_fn_009452ca(data);
                *(volatile u32 *)item = 0;
            }
            cm_fn_00944c94((void *)item);
        }
    }
}
