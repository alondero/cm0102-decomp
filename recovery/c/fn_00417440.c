/* Preferred-image VA 00417440, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *date);
extern u32 CM_THISCALL cm_fn_00417590(u32 self, u32 input);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00417440(u32 self, volatile u32 descriptor, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u8 *name, *extension;
    u32 saved_descriptor, value, allocation, version, line, optional;
    u16 count, word;
    frame.state = 0xffffffffu;
    frame.handler = 0x00953dceu;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D((u32)work) = self;
    cm_fn_0054a110((void *)(self + 0x38u));
    saved_descriptor = descriptor;
    frame.state = 0;
    if (saved_descriptor == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x30u;
        goto error;
    }
    W(self + 0x36u) = 0xffff;
    count = (u16)(s16)(s8)B(saved_descriptor + 13u);
    W(self + 6u) = count;
    W(self + 4u) = 0;
    allocation = (u32)cm_fn_00944e46((u32)(s32)(s16)count * 14u);
    D(self) = allocation;
    if (allocation == 0) {
        name = work + 4;
        extension = work + 0x104;
        line = 0x3eu;
        goto error;
    }
    value = D(saved_descriptor);
    D(self + 0x28u) = value;
    value = D(saved_descriptor + 4u);
    D(self + 0x2cu) = value;
    value = D(saved_descriptor + 8u);
    D(self + 0x30u) = value;
    word = W(saved_descriptor + 12u);
    W(self + 0x34u) = word;
    optional = input;
    if (optional != 0)
        cm_fn_00417590(self, optional);
    goto done;
error:
    cm_fn_00944cff(0x009880b0u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return self;
}
