#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_00775420(u32, u32);
u32 CM_THISCALL cm_fn_0057b730(u32 self)
{
    u32 object = D(self + 0xc);
    u32 receiver = D(object + 0xc);
    u32 table = D(receiver);
    ((u32 (CM_THISCALL *)(u32))D(table + 0x30))(receiver);
    object = D(self + 4);
    receiver = D(0x00b74318);
    return cm_fn_00775420(receiver, object);
}
