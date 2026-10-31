/* Preferred-image VA 0x008aed80, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern int CM_STDCALL cm_fn_00944c6a(u32 socket, char *buffer, int length,
                                      int flags, void *from, int *from_length);

s32 CM_THISCALL cm_fn_008aed80(u32 self, char *buffer, s32 length,
                               void *from, int *from_length, volatile u16 *index_out)
{
    u32 index;
    if (!buffer || length <= 0 || !from)
        return 0;
    for (index = 0; index < 20; ++index) {
        u32 socket = CM_GLOBAL(volatile u32, self + 0x288 + index * 4);
        if (socket != 0xffffffffu) {
            s32 result = cm_fn_00944c6a(socket, buffer, length, 0, from, from_length);
            *index_out = (u16)index;
            if ((s16)result > 0)
                return (s16)result;
        }
    }
    return 0;
}
