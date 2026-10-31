#include "cmre.h"

/* Preferred-image VA 0x008ac3d0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The signed multiply/shift sequence divides a signed WORD by eleven,
   truncating toward zero, before its low BYTE is compared with minute caps. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_006cdd00(u32, u32);
extern u32 CM_THISCALL cm_fn_006cdd50(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006d6f70(u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008ac3d0(u32 self, volatile u32 first, u32 second)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, base, team, offset, in_index, out_index;
    u32 incoming, outgoing, team_record, team_raw, text, key, manager, first_raw;
    s32 elapsed, mode;
    u8 minute, value, old_count, marker, team_byte;
    s8 slot = (s8)first;
    if (slot < 0 || slot > 19) goto error;
    if ((s8)second < 0 || (s8)second > 19) goto error;
    base = D(self + 4);
    if (base == 0) return 0;
    team = (u32)(s32)(s8)B(self + 0xde9);
    offset = team * 20u;
    in_index = (u32)(s32)(s8)first + offset;
    out_index = (u32)(s32)(s8)second + offset;
    incoming = base + in_index * 446u + 0x4796;
    outgoing = base + out_index * 446u + 0x4796;
    cm_fn_006cdd00(base, incoming);
    base = D(self + 4);
    elapsed = (s16)W(base + 0x8ed0);
    minute = (u8)(elapsed / 11);
    B(incoming + 0x1f) = minute;
    base = D(self + 4);
    mode = (s16)W(base + 0x8ed4);
    if (mode == 0x1ef) {
        value = (s8)minute < 45 ? minute : 45;
        B(incoming + 0x1f) = value;
    } else if (mode == 0x3de) {
        value = (s8)minute < 90 ? minute : 90;
        B(incoming + 0x1f) = value;
    } else if (mode == 0x483) {
        value = (s8)minute < 105 ? minute : 105;
        B(incoming + 0x1f) = value;
    } else if (mode == 0x528) {
        value = (s8)minute < 120 ? minute : 120;
        B(incoming + 0x1f) = value;
    }
    value = B(outgoing + 0x19);
    if (value == 0xfe) value = B(outgoing + 0x1a);
    B(incoming + 0x19) = value;
    base = D(self + 4);
    cm_fn_006cdd50(base, outgoing, 0xffffffffu);
    minute = B(incoming + 0x1f);
    B(outgoing + 0x19) = 0xfc;
    B(outgoing + 0x20) = minute;
    B(outgoing + 0x1e) = 1;
    team = (u32)(s32)(s8)B(self + 0xde9);
    offset = team * 208u;
    base = D(self + 4);
    old_count = B(base + offset + 0x9049);
    team_record = base + offset + 0x9032;
    B(team_record + 0x17) = (u8)(old_count - 1u);
    marker = B(outgoing + 0x22);
    team_byte = B(self + 0xde9);
    team_raw = (team_record & 0xffffff00u) | team_byte;
    base = D(self + 4);
    key = (s8)marker > -1 ? 0x20d7 : 0x1f5b;
    first_raw = first;
    manager = D(base + 0x8f7a);
    cm_fn_006d6f70(manager, key, base, first_raw, team_raw, second, team_raw, 0);
    return 1;
error:
    cm_fn_00944cff(0x00a8b94c, 0, 0, local + 0x100, local);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x2055);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
