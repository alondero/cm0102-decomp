/* Preferred-image VA 007f7490, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007f6cd0(u32 self, u32 raw_kind, u32 manager);
extern u32 CM_THISCALL cm_fn_00804540(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_007af600(u32 self, u32 record);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007f7490(u32 self, volatile u32 first, volatile u32 second)
{
    u8 work[0x204];
    u32 record, manager, active, receiver, raw_kind, result, version, line;
    u8 index, total, kind;
    void *name, *extension;
    record = first;
    if (record == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x231u;
        goto error;
    }
    manager = second;
    if (manager == 0) {
        active = (u32)(s32)(s16)W(0x00b77c7au);
        manager = D(0x00b74c26u + active * 768u);
        if (manager == 0) {
            name = work + 4;
            extension = work + 0x104;
            line = 0x23bu;
            goto error;
        }
    }
    index = B(manager);
    total = B(0x00ae2364u);
    index = (u8)(index - total + 16u);
    if ((s8)index < 0 || (s8)index > 15)
        return 0;
    receiver = D(self + (u32)(s32)(s8)index * 4u + 12u);
    if (receiver != 0)
        return cm_fn_00804540(receiver, record);
    kind = 0;
    B((u32)work) = kind;
    for (;;) {
        raw_kind = D((u32)work);
        result = cm_fn_007f6cd0(self, raw_kind, manager);
        if (result != 0 && B(result + 10u) == 1)
            break;
        kind = (u8)(kind + 1u);
        B((u32)work) = kind;
        if ((s8)kind >= 10)
            return 0;
    }
    record = first;
    return cm_fn_007af600(result, record);
error:
    cm_fn_00944cff(0x00a7b094u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
