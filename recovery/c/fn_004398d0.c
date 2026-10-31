/* Preferred-image VA 004398d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern u32 CM_CDECL cm_fn_004c08f0(u32 array, u32 count);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 record, u8 place);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004398d0(u32 self)
{
    u8 work[0x200];
    u32 allocation, count, counter, index, table, value, receiver, version, line;
    void *name, *extension;
    if (D(self + 8u) == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x307u;
        goto diagnostic;
    }
    count = (u32)(s32)(s16)W(self + 0x3eu);
    allocation = (u32)cm_fn_00944e46(count * 4u);
    if (allocation == 0) {
        name = work;
        extension = work + 0x100;
        line = 0x30fu;
        goto diagnostic;
    }
    counter = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        do {
            index = (u32)(s32)(s16)counter;
            table = D(self + 0xb1u);
            counter += 1u;
            value = D(table + index * 59u);
            D(allocation + index * 4u) = value;
        } while ((s16)counter < (s16)W(self + 0x3eu));
    }
    count = (u32)(s32)(s16)W(self + 0x3eu);
    cm_fn_004c08f0(allocation, count);
    value = D(allocation);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, value, 1);
    value = D(allocation + 4u);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, value, 2);
    value = D(allocation + 8u);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, value, 3);
    value = D(allocation + 12u);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, value, 3);
    counter = 5;
    if ((s16)W(self + 0x3eu) >= (s16)counter) {
        do {
            index = (u32)(s32)(s16)(counter - 1u);
            receiver = D(self + 8u);
            value = D(allocation + index * 4u);
            cm_fn_004a2540(receiver, value, (u8)counter);
            counter += 1u;
        } while ((s16)counter <= (s16)W(self + 0x3eu));
    }
    return (u32)cm_fn_009452ca(allocation);
diagnostic:
    cm_fn_00944cff(0x0098afd8u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    value = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return value;
}
