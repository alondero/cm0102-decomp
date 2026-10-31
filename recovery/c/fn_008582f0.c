/* Preferred-image VA 008582f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 record, u8 place);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008582f0(u32 self)
{
    u8 work[0x204];
    u32 allocation, object, table, index, counter, value, receiver, version, line;
    u8 slot;
    void *name, *extension;
    if (D(self + 8u) == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x519u;
        goto diagnostic;
    }
    allocation = (u32)cm_fn_00944e46((u32)(s32)(s16)W(self + 0x3eu) * 4u);
    if (allocation == 0) {
        name = work + 4;
        extension = work + 0x104;
        line = 0x521u;
        goto diagnostic;
    }
    slot = 0xffu;
    B((u32)work + 3u) = slot;
    for (;;) {
        if (slot == 0xffu) {
            object = self;
        } else {
            table = D(self + 0xcu);
            object = D(table + (u32)(s32)(s8)slot * 4u);
        }
        counter = 0;
        if ((s16)W(object + 0x3eu) > 0) {
            do {
                index = (u32)(s32)(s16)counter;
                table = D(object + 0xb1u);
                counter += 1u;
                value = D(table + index * 59u);
                D(allocation + index * 4u) = value;
            } while ((s16)counter < (s16)W(object + 0x3eu));
        }
        value = (u32)(s32)(s16)W(object + 0x3eu);
        cm_fn_00945333(allocation, value, 4, 0x004b07d0u);
        counter = 0;
        if ((s16)W(object + 0x3eu) > 0) {
            do {
                u8 place = (u8)((u8)counter * 4u + 1u);
                index = (u32)(s32)(s16)counter;
                value = D(allocation + index * 4u);
                receiver = D(self + 8u);
                cm_fn_004a2540(receiver, value, place);
                counter += 1u;
            } while ((s16)counter < (s16)W(object + 0x3eu));
        }
        slot = B((u32)work + 3u);
        slot = (u8)(slot + 1u);
        B((u32)work + 3u) = slot;
        if ((s8)slot >= 3)
            break;
        slot = B((u32)work + 3u);
    }
    return (u32)cm_fn_009452ca(allocation);
diagnostic:
    cm_fn_00944cff(0x00a828f8u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    value = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return value;
}
