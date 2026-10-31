#include "cmre.h"
extern void CM_CDECL cm_fn_00945b13(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945b00(u32, u32);
u32 CM_CDECL cm_fn_0054d580(u32 directory, u32 name)
{
    cm_fn_00945b13(0x00de1f64, 0, directory, name, 0);
    return cm_fn_00945b00(0x00de1f64, 0x0098f928);
}
