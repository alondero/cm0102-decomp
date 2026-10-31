/* Preferred-image VA 008ae8d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_STDCALL cm_fn_00944c52(u32 socket, u32 buffer, u32 length, u32 flags);
extern u32 CM_STDCALL cm_fn_00944c4c(void);
extern u32 CM_STDCALL cm_fn_00944c3a(u32 socket);

u32 CM_THISCALL cm_fn_008ae8d0(u32 self, u32 index,
                              volatile u32 buffer, volatile u32 length)
{
    u32 sockets, slot, socket, address, size, result, error, total = 0;
    sockets = D(self);
    if (sockets == 0) return 0xfffffffeu;
    slot = (u32)(s32)(s16)index * 4u;
    socket = D(sockets + slot);
    if (socket == 0xffffffffu) return 0xffffffffu;
    address = buffer;
    if (address == 0) return 0xffffffffu;
    size = length;
    if ((s32)size <= 0) return 0xffffffffu;
    result = cm_fn_00944c52(socket, address, size, 0);
    for (;;) {
        if (result != 0xffffffffu) return result;
        error = cm_fn_00944c4c();
        if (error != 0x2733u) break;
        if ((s32)result >= 0) {
            total += result;
            if ((s32)total >= (s32)size) return size;
        }
        address = buffer;
        sockets = D(self);
        socket = D(sockets + slot);
        result = cm_fn_00944c52(socket, address + total, size - total, 0);
    }
    sockets = D(self);
    if (sockets != 0) {
        socket = D(sockets + slot);
        if (socket != 0xffffffffu) {
            cm_fn_00944c3a(socket);
            sockets = D(self);
            D(sockets + slot) = 0xffffffffu;
        }
    }
    return 0xfffffffeu;
}
