#include "cmre.h"
extern u32 CM_STDCALL cm_fn_009463af(u32, u32, u32, u32, u32);
u32 CM_THISCALL cm_fn_00453be0(u32 self)
{
    cm_fn_009463af(self + 0x11fu, 0x625, 2, 0x00453c30, 0x00453c10);
    return self;
}
