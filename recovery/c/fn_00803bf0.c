/* Preferred-image VA 0x00803bf0, base 0x00400000; executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep full incidental EAX, including the original helper/error returns.
   00564080 saves the incoming ECX after its preserving stack probe and ret 0xc.
   0077ac30 returns full EAX 0/1; its third stack argument can be forwarded
   whole to an indirect callback, so preserve the original partial-AX DWORD. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00564080(u32 self, u32 first, u32 second,
                                     u32 buffer);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 buffer, u32 length,
                                     u32 recipient);
/* Its incidental EAX is subsequently returned by the late error path. */
extern u32 CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                  s32 flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);

u32 CM_THISCALL cm_fn_00803bf0(u32 self)
{
    u8 scratch[0x200];
    u32 record = self + 0x37feu;
    u32 length, capacity, buffer, slot, recipient, receiver, result, text;

    CM_GLOBAL(volatile u32, self + 0x3802u) = 0;
    length = CM_GLOBAL(volatile u32, record + 4u);
    capacity = CM_GLOBAL(volatile u32, record + 8u);
    if (length + 1u > capacity) {
        cm_fn_00944cff(0x009c3734u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x100, scratch, 0x85u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
        buffer = CM_GLOBAL(volatile u32, record);
        CM_GLOBAL(volatile u8, buffer + length) = 0x15;
        length = CM_GLOBAL(volatile u32, record + 4u);
        CM_GLOBAL(volatile u32, record + 4u) = length + 1u;
    }

    slot = (u32)(s32)CM_GLOBAL(volatile s16, self + 0x382au);
    recipient = (slot & 0xffff0000u) |
        CM_GLOBAL(volatile u16, self + ((slot * 3u) << 8) + 0x7d4u);
    if ((u16)recipient == 0) {
        receiver = CM_GLOBAL(volatile u32, self + 0x3840u);
        return cm_fn_00564080(self + 0x3844u, receiver, 0, record);
    }

    length = CM_GLOBAL(volatile u32, self + 0x3802u);
    buffer = CM_GLOBAL(volatile u32, record);
    receiver = CM_GLOBAL(volatile u32, self + 0x3840u);
    result = cm_fn_0077ac30(receiver, buffer, length, recipient);
    if (result == 0) {
        cm_fn_00944cff(0x00a7ddbcu, 0, 0, scratch, scratch + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch, scratch + 0x100, 0xd47u);
        result = cm_fn_005e8290((const char *)0x009870e0u,
                                (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    }
    return result;
}
