/* Preferred-image VA 008c2060, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full EAX at error exits and the null-record exit. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 list);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008c2060(u32 self, s32 index, u32 person_id)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, result, table, record, list, head, node, all_clear = 1;
    if (index < 0) {
        name = work + 0x100;
        extension = work;
        line = 0xdc3u;
        goto diagnostic;
    }
    if (index >= (s32)D(self + 4u)) {
        name = work;
        extension = work + 0x100;
        line = 0xdc8u;
        goto diagnostic;
    }
    table = D(self);
    record = D(table + (u32)index * 4u);
    if (record == 0)
        return (u32)index;
    list = D(record + 4u);
    head = D(list + 2u);
    D(list + 0xau) = head;
    list = D(record + 4u);
    node = cm_fn_0068d1b0(list);
    if (node != 0) {
        do {
            if (D(node) == person_id)
                B(node + 4u) = 1;
            if (B(node + 4u) != 0)
                all_clear = 0;
            list = D(record + 4u);
            node = cm_fn_0068d1b0(list);
        } while (node != 0);
        if (all_clear == 0)
            return 0;
    }
    B(record + 0x2cu) = B(record + 0x2cu) == 7 ? 8 : 6;
    return 0;
diagnostic:
    cm_fn_00944cff(0x00a8d514u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
