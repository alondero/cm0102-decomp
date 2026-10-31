/* Preferred-image VA 0041c790, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_007fee80(u32 self);
extern void CM_THISCALL cm_fn_007feeb0(u32 self, u32 path);
extern u32 CM_CDECL cm_fn_0041ced0(u32 record, u32 lane);
extern u32 CM_CDECL cm_fn_0041cad0(u32 record, u32 lane);
extern u32 CM_CDECL cm_fn_0041c9d0(u32 record, u32 lane);
extern u32 CM_CDECL cm_fn_0090bde0(u32 prefix, u32 suffix);
extern void CM_CDECL cm_fn_0041bfb0(void);

u32 CM_CDECL cm_fn_0041c790(volatile u32 first, volatile u32 second, volatile u32 third)
{
    u32 path, record, lane, related;
    path = cm_fn_007fee80(0x00b74450u);
    if (B(path) != 0x5c) {
        path = cm_fn_007fee80(0x00b74450u);
        if (B(path) != 0)
            return 0;
    }
    if (D(0x00989858u) == 0) {
        cm_fn_007feeb0(0x00b74450u, 0x00989adcu);
        return 1;
    }
    record = first;
    lane = second;
    if (record != 0) {
        path = cm_fn_0041ced0(record, lane);
        if (path != 0)
            goto found;
        if (third != 0)
            return 0;
        related = D(record + 0x39u);
        if (related != 0) {
            path = cm_fn_0041cad0(related, lane);
            if (path != 0)
                goto found;
        }
        record = D(record + 0x1au);
        if (record != 0) {
            path = cm_fn_0041c9d0(record, lane);
            if (path != 0)
                goto found;
        }
    } else if (third != 0) {
        return 0;
    }
    path = cm_fn_0041ced0(0, lane);
    if (path != 0)
        goto found;
    cm_fn_0041bfb0();
    return 1;
found:
    path = cm_fn_0090bde0(0x009899e0u, path);
    cm_fn_007feeb0(0x00b74450u, path);
    return 1;
}
