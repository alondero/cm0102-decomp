#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_00547d30(u32 object, u32 target);

u32 CM_STDCALL cm_fn_00776380(u32 object)
{
    u32 nation = *(volatile u32 *)(object + 0x1au);
    u32 target = cm_fn_0053b3d0(nation);
    return cm_fn_00547d30(object, target) != 0;
}
