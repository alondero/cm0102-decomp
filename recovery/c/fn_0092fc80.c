/* Preferred-image VA 0092fc80, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_008af520(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i, u32 j);
extern u32 CM_THISCALL cm_fn_0092fe80(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i, u32 j, u32 k);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0092fc80(u32 self, volatile u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u32 previous, key, allocation, option, first, second, index = 0, result, table, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096605cu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    key = D(self + 0x13u);
    W(self + 8u) = 2;
    allocation = (u32)cm_fn_00944e46(8);
    D(self + 4u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00ad58c4u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x5du);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    allocation = (u32)cm_fn_00944cf1(0x2bu);
    D((u32)work) = allocation;
    option = argument;
    frame.state = 0;
    if (allocation != 0) {
        first = D(0x009cf79cu);
        second = D(0x009cfc60u);
        index = 1;
        result = cm_fn_0092fe80(allocation, key, 0, second, 9000, 7000, first, 0, 0, 1, option, 1);
    } else {
        result = 0;
    }
    table = D(self + 4u);
    frame.state = 0xffffffffu;
    D(table) = result;
    allocation = (u32)cm_fn_00944cf1(0x25du);
    D((u32)work) = allocation;
    frame.state = 1;
    if (allocation != 0) {
        first = D(0x009cf79cu);
        second = D(0x009cfc64u);
        result = cm_fn_008af520(allocation, key, index, second, 8500, 6750, first, 0, 0, option, 1);
    } else {
        result = 0;
    }
    table = D(self + 4u);
    D(table + 4u) = result;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
