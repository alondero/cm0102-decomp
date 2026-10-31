#include "cmre.h"
/* Preferred-image VA 0x0076a280, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The flag read from scratch remains uninitialized unless the original read
   helper writes it. SEH state zero remains live after the optional constructor. */
extern u32 CM_CDECL cm_fn_00932c30(u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_00767af0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
typedef void (CM_THISCALL *reader)(u32, u32);

u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 stream)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 work = (u32)scratch, prev, table, target, allocation, value, result;
    char *version;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095dc7e;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    table = D(self);
    target = D(table);
    ((reader)target)(self, stream);
    result = cm_fn_00932c30(work, 4, 1, stream);
    if (result != 1) {
        cm_fn_00944cff(0x00a5b26c, 0, 0, scratch + 0x108, scratch + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x108, scratch + 8, 0x80b);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        result = 0;
    } else {
        value = D(work);
        if (value) {
            allocation = (u32)cm_fn_00944cf1(15);
            D(work + 4) = allocation;
            frame.state = 0;
            value = allocation ? cm_fn_00767af0(allocation, stream) : 0;
            D(self + 0xa) = value;
        }
        result = 1;
    }
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return result;
}
