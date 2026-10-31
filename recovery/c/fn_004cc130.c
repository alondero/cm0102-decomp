#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Both indexed direct callers consume only AL (VA00484444/0048444f).
   Preserve the original null-record reads at 00000035 and 0000005e. */
u8 CM_THISCALL cm_fn_004cc130(u32 self, u32 person, u32 club)
{
    u8 scratch[0x200];
    u32 index, limit, base, record, contract, value, other, flags, text;
    if (club == 0) {
        cm_fn_00944cff(0x009a8190, 0, 0, (u32)scratch + 0x100, (u32)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x100, (u32)scratch, 0xbd8);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 1;
    }
    if (person == 0)
        goto fallback;
    index = D(person);
    limit = D(0x00ae2364);
    if ((s32)index >= (s32)limit)
        goto fallback;
    base = D(0x00ae2d0c);
    record = D(base + index * 79u + 4);
    if ((s32)record < 0)
        goto fallback;
    base = D(self);
    contract = base + record * 80u;
    if (contract == 0)
        goto fallback;
    value = D(contract + 4);
    other = D(club);
    if (value == other)
        goto fallback;
    other = cm_fn_00540a50(club, 0, 1);
    if (other != 0) {
        value = D(contract + 4);
        flags = D(other);
        if (value == flags)
            goto fallback;
    }
    other = cm_fn_00540a50(club, 0, 1);
    if (other != 0)
        return 1;
    index = D(person);
    limit = D(0x00ae2364);
    if ((s32)index >= (s32)limit) {
        record = 0;
    } else {
        base = D(0x00ae2d0c);
        value = D(base + index * 79u);
        if ((s32)value < 0)
            record = 0;
        else {
            base = D(self);
            record = base + value * 80u;
        }
    }
    flags = B(record + 0x35);
    return (u8)(1u + ((flags & 0x40u) != 0));
fallback:
    return B(person + 0x5e) & 0xf3u;
}
