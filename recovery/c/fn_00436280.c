/* Preferred-image VA 0x00436280, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller 00435540 overwrites EAX at 0043554c before reading it.
   Keep the unconditional first four placements (1,2,3,3), even for a short
   input count, and the original signed WORD counter comparisons. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
extern void CM_CDECL cm_fn_004c08f0(u32 array, s32 count);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 club, u8 position);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   s32 flags);

void CM_THISCALL cm_fn_00436280(u32 self)
{
    u8 scratch[0x200];
    u8 *name, *extension;
    u32 allocation, index, offset, rows, value, receiver, place, line, text;
    s32 count;

    if (CM_GLOBAL(volatile u32, self + 8u) == 0) {
        name = scratch + 0x100;
        extension = scratch;
        line = 0x307u;
        goto diagnostic;
    }
    count = CM_GLOBAL(volatile s16, self + 0x3eu);
    allocation = (u32)cm_fn_00944e46((u32)count << 2);
    if (allocation == 0) {
        name = scratch;
        extension = scratch + 0x100;
        line = 0x30fu;
        goto diagnostic;
    }

    index = 0;
    if (CM_GLOBAL(volatile s16, self + 0x3eu) > 0) {
        do {
            offset = (u32)(s32)(s16)index;
            rows = CM_GLOBAL(volatile u32, self + 0xb1u);
            ++index;
            value = CM_GLOBAL(volatile u32, rows + offset * 59u);
            CM_GLOBAL(volatile u32, allocation + offset * 4u) = value;
        } while ((s16)index < CM_GLOBAL(volatile s16, self + 0x3eu));
    }
    count = CM_GLOBAL(volatile s16, self + 0x3eu);
    cm_fn_004c08f0(allocation, count);

    value = CM_GLOBAL(volatile u32, allocation);
    receiver = CM_GLOBAL(volatile u32, self + 8u);
    cm_fn_004a2540(receiver, value, 1);
    value = CM_GLOBAL(volatile u32, allocation + 4u);
    receiver = CM_GLOBAL(volatile u32, self + 8u);
    cm_fn_004a2540(receiver, value, 2);
    value = CM_GLOBAL(volatile u32, allocation + 8u);
    receiver = CM_GLOBAL(volatile u32, self + 8u);
    cm_fn_004a2540(receiver, value, 3);
    value = CM_GLOBAL(volatile u32, allocation + 12u);
    receiver = CM_GLOBAL(volatile u32, self + 8u);
    cm_fn_004a2540(receiver, value, 3);

    place = 5;
    if (CM_GLOBAL(volatile s16, self + 0x3eu) >= 5) {
        do {
            offset = (u32)(s32)(s16)(place - 1u);
            /* This loop loads the receiver before the array value, unlike
               the four straight-line calls above. */
            receiver = CM_GLOBAL(volatile u32, self + 8u);
            value = CM_GLOBAL(volatile u32, allocation + offset * 4u);
            /* Original 004a25d0 observes only the argument BYTE. */
            cm_fn_004a2540(receiver, value, (u8)place);
            ++place;
        } while ((s16)place <= CM_GLOBAL(volatile s16, self + 0x3eu));
    }
    cm_fn_009452ca(allocation);
    return;

diagnostic:
    cm_fn_00944cff(0x0098af14u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                   name, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
}
