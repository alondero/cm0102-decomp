#include "cmre.h"
extern u32 CM_STDCALL cm_fn_009462bb(u32, u32, u32, u32);
u32 CM_THISCALL cm_fn_00453bc0(u32 self)
{
    return cm_fn_009462bb(self + 0x11fu, 0x625, 2, 0x00453c10);
}
