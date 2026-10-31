/* Preferred-image VA 005abf10, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_005aa2d0(u32 self, u32 year);
extern u32 CM_THISCALL cm_fn_005a9cf0(u32 self, u32 year, u32 parent, u32 zero);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005abf10(u32 self, volatile u32 first, volatile u32 second,
                              volatile u32 third, volatile s8 fourth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u32 previous, year, child, allocation, result, version, row, lane, index;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095977eu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    year = first;
    if ((u16)year != W(self + 0x1130u)) {
        do {
            child = D(self + 0x112cu);
            if (child != 0) {
                if (W(child + 0x1130u) != (u16)year)
                    cm_fn_005aa2d0(child, year);
            } else {
                allocation = (u32)cm_fn_00944cf1(0x1184u);
                D((u32)work) = allocation;
                frame.state = 0;
                result = allocation != 0 ? cm_fn_005a9cf0(allocation, year, self, 0) : 0;
                frame.state = 0xffffffffu;
                D(self + 0x112cu) = result;
                if (result == 0) {
                    cm_fn_00944cff(0x009cc208u, 0, 0, work + 0x104, work + 4);
                    version = cm_fn_0090d130();
                    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x681u);
                    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
                    D(0x00b67a34u) = 0;
                    result = 0;
                    goto done;
                }
            }
            self = D(self + 0x112cu);
        } while ((u16)year != W(self + 0x1130u));
    }
    row = second;
    lane = third;
    index = (u32)(s32)fourth;
    row &= 0xffffu;
    lane &= 0xffu;
    result = D(self + (row * 3u + lane) * 4u);
    result = D(result + index * 4u);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
