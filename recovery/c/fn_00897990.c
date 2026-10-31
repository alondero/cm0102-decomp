#include "cmre.h"

/* Preferred-image VA 0x00897990, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_00775790(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00897990(u32 self, volatile u32 club, volatile u32 kind,
                             volatile u32 side, volatile u32 last)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, incoming, result, slot, index, record_offset, field_offset;
    u32 base, player_offset, value, total, owner, manager, name, ext, line, text;
    s8 checked;
    incoming = club;
    if (incoming == 0) {
        name = local + 0x100; ext = local; line = 0x5a5;
        goto error;
    }
    checked = (s8)kind;
    if (checked < 0 || checked > 4) {
        name = local; ext = local + 0x100; line = 0x5ad;
        goto error;
    }
    checked = (s8)side;
    if (checked < 0 || checked > 1) {
        name = local; ext = local + 0x100; line = 0x5b5;
        goto error;
    }
    value = last;
    index = cm_fn_008982b0(self, incoming, value);
    if (index == 0xffffffffu) return 0;
    value = (u32)(s32)(s8)kind;
    field_offset = ((u32)(s32)(s8)side + value * 2u) * 4u;
    base = D(self + 0x601);
    record_offset = index * 145u;
    slot = base + record_offset + field_offset + 0x5e;
    index = D(slot);
    total = D(0x00ae2364) + 0x52du;
    if ((s32)index > (s32)total) {
        D(slot) = 0xffffffffu;
        return 0;
    }
    if ((s32)index <= -1) return 0;
    player_offset = index * 110u;
    base = D(0x00ae23bc);
    value = D(base + player_offset + 0x61);
    if (value == 0) {
        D(slot) = 0xffffffffu;
        return 0;
    }
    incoming = club;
    owner = D(base + player_offset + 0x39);
    if (owner == incoming) return base + player_offset;
    result = cm_fn_0053b900(incoming);
    if (result == 0) {
        incoming = club;
        manager = D(0x00b74318);
        result = cm_fn_00775790(manager, incoming);
        if (result == 0) {
            base = D(self + 0x601);
            D(base + record_offset + field_offset + 0x5e) = 0xffffffffu;
            return result;
        }
    }
    base = D(0x00ae23bc);
    return base + player_offset;
error:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
