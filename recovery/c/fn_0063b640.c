/* Preferred-image VA 0063b640, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_00768290(u32 self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 stream);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_00767af0(u32 self, u32 zero);
extern void CM_THISCALL cm_fn_0063b790(u32 self, u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0063b640(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x208];
    u32 index, base, stream, value, sign, allocation, list, version;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b679u;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D((u32)work + 4u) = self;
    cm_fn_00768290(self);
    D(self) = 0x0096c17cu;
    index = D(0x009cf2dcu);
    base = D(0x00ae23a8u);
    stream = input;
    frame.state = 0;
    D(self + 0x13u) = 12;
    B(self + 0x12u) = 1;
    D(self + 14u) = base + index * 290u;
    if (stream != 0) {
        value = D(0x00dd7110u);
        sign = (s32)value < 0 ? 0xffffffffu : 0;
        value = (value ^ sign) - sign;
        if ((s32)value >= 0x3e028 && cm_fn_0076a280(self, stream) != 0)
            goto done;
    }
    allocation = (u32)cm_fn_00944cf1(15);
    D((u32)work) = allocation;
    B((u32)&frame.state) = 1;
    list = allocation != 0 ? cm_fn_00767af0(allocation, 0) : 0;
    B((u32)&frame.state) = 0;
    D(self + 10u) = list;
    if (list == 0) {
        cm_fn_00944cff(0x009ea0f4u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x57u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        cm_fn_0063b790(self, stream);
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return self;
}
