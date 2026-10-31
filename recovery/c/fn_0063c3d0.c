/* Preferred-image VA 0063c3d0/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern void CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

/* THIS4/RET10/fullEAX=0or1. No indexed direct callers; preserve the full
   explicit EAX result. AE1080 is written as a DWORD after each lookup.
   Both original lookups and the intervening pointer calculation remain;
   a null second lookup still proceeds to the original read at address 57. */
u32 CM_THISCALL cm_fn_0063c3d0(u32 self, u32 club, u32 person,
                              u32 output, u32 reason)
{
    u32 receiver, id, table, entry, base, mapped, original_nation, other_nation;
    u32 text, kind;
    receiver = D(0x00de1f54u);
    id = D(club);
    table = D(receiver + 0x91cu);
    if ((s8)B(table + id * 16u) >= 5) {
        if (output != 0) {
            kind = (u32)(s32)(s8)B(club + 0x52u);
            cm_fn_0066f4e0(output, 0x009c4534u, kind, kind, club + 0x38u);
        }
        if (reason != 0) W(reason) = 0x1e;
        return 0;
    }
    receiver = D(0x00ae1050u);
    entry = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = entry;
    if (entry == 0) return 1;
    id = D(entry + 4u);
    base = D(0x00ae23b4u);
    mapped = base + id * 581u;
    if (mapped == 0) return 1;
    receiver = D(0x00ae1050u);
    entry = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = entry;
    if (entry != 0) {
        id = D(entry + 4u);
        base = D(0x00ae23b4u);
        mapped = base + id * 581u;
    } else {
        mapped = 0;
    }
    original_nation = D(club + 0x57u);
    other_nation = D(mapped + 0x57u);
    if (original_nation != other_nation) return 1;
    if (output != 0) {
        text = D(self + 0xau);
        kind = (u32)(s32)(s8)B(text + 0x37u);
        cm_fn_0066f4e0(output, 0x009ea168u, kind, kind, text + 4u);
    }
    if (reason != 0) W(reason) = 0x1d;
    return 0;
}
