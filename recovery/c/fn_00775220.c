#include "cmre.h"

/* Preferred-image VA 0x00775220, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_THISCALL cm_fn_006a3770(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00775220(u32 self, volatile u32 first, volatile u32 second)
{
    u8 scratch[0x204];
    u32 local = (u32)scratch, nation = first, incoming, id, base, slot, old;
    u32 value, special0, special1, other, text, manager;
    if (nation == 0) goto error;
    incoming = second;
    if (incoming == 0) goto error;
    id = D(nation);
    base = D(self + 4);
    slot = base + id * 110u + 4;
    D(local) = slot;
    old = D(slot);
    if (old == 0xffffffffu) {
        value = D(incoming);
        D(slot) = value;
        goto notify;
    }
    base = D(0x00ae23d0);
    special0 = D(0x009cf7a0);
    other = base + old * 107u;
    special1 = D(0x009cf79c);
    if (other != 0) {
        value = D(other);
        if (value == special1 || value == special0 ||
            value == D(0x009cf7a4) || value == D(0x009cf7a8) ||
            value == D(0x009cf7ac) || value == D(0x009cf790) ||
            value == D(0x009cf8a0) || value == D(0x009cf78c) ||
            value == D(0x009cf964)) {
            base = D(self + 8);
            incoming = second;
            value = D(incoming);
            D(base + id * 4u) = value;
            return base;
        }
        slot = D(local);
    }
    incoming = second;
    value = D(incoming);
    if (value == special1 || value == special0 ||
        value == D(0x009cf7a4) || value == D(0x009cf7a8) ||
        value == D(0x009cf7ac) || value == D(0x009cf790) ||
        value == D(0x009cf8a0) || value == D(0x009cf78c) ||
        value == D(0x009cf964)) {
        base = D(self + 8);
        D(base + id * 4u) = old;
        nation = first;
        base = D(self + 4);
        id = D(nation);
        incoming = second;
        value = D(incoming);
        D(base + id * 110u + 4) = value;
    } else {
        D(slot) = value;
        manager = D(0x00b67a44);
        if (manager == 0) return manager;
        nation = first;
        goto call_notify;
    }
notify:
    manager = D(0x00b67a44);
    if (manager == 0) return manager;
call_notify:
    value = cm_fn_0053b3d0(nation);
    manager = D(0x00b67a44);
    return cm_fn_006a3770(manager, value, 1);
error:
    cm_fn_00944cff(0x00a5b2a4, 0, 0, local + 0x104, local + 4);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x104, local + 4, 0x1bb2);
    value = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return value;
}
