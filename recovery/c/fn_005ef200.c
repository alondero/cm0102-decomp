#include "cmre.h"

/* Thiscall: ECX is the destination object (this) and one stack argument is the
   source object (a). The routine copies source bytes 0x00..0x63 to this in
   ascending order (byte loops, 50 bytes then 50 bytes), then copies dwords at
   offsets 0x64 and 0x68 from source to this, reads the source dword at 0x6c
   and stores it to this at 0x6c just before the call. The callee is the
   sub-object at this+0x70, called with an 8-byte local buffer and the source's
   sub-object pointer a+0x70 (two stack arguments, popped by the callee). The
   routine returns this. */

extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 out, u32 src);

u32 CM_THISCALL cm_fn_005ef200(u32 self, u32 src)
{
    u8 local[8];
    u32 k;
    u32 value;

    k = 0u;
    do {
        *(volatile u8 *)(self + k) = *(volatile u8 *)(src + k);
        k = k + 1u;
    } while (k < 0x32u);

    do {
        *(volatile u8 *)(self + k) = *(volatile u8 *)(src + k);
        k = k + 1u;
    } while (k < 0x64u);

    *(volatile u32 *)(self + 0x64u) = *(volatile u32 *)(src + 0x64u);
    *(volatile u32 *)(self + 0x68u) = *(volatile u32 *)(src + 0x68u);
    value = *(volatile u32 *)(src + 0x6cu);
    *(volatile u32 *)(self + 0x6cu) = value;

    cm_fn_00417c10(self + 0x70u, (u32)&local, src + 0x70u);
    return self;
}
