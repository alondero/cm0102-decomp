/* Preferred-image VA 00849f00/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00849ce0(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_0053b620(u8);
extern u8 CM_THISCALL cm_fn_00849d60(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);

/* THIS3/RETc/u8: all five indexed callers consume AL. Preserve the early
   output BYTE and AE1080 DWORD stores, plus the original reloads after calls.
   53b620 consumes only BYTE; 849d60 receives the original ECX self value
   and two stack arguments/RET8, and its result is forwarded at AL width. */
u8 CM_THISCALL cm_fn_00849f00(u32 self, u32 person, u32 club, u32 output)
{
    u32 entry, receiver, id, base, mapped, value, other, flag;
    if (club == 0) return 7;
    flag = D(person + 0x61u) == 0;
    entry = cm_fn_00849ce0(self, club, person, flag, 0);
    if (entry != 0) goto found;
    if (D(person + 0x69u) != 0) {
        entry = cm_fn_00849ce0(self, club, person, 1, 0);
        if (entry != 0) goto found;
    }
    if (output != 0) B(output) = B(person + 0x3du);
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
    if (mapped == club) goto existing;
    value = cm_fn_00600a50(club, 1, 1, 0, 0);
    if (value != 0) {
        flag = B(person + 0x3du);
        if (cm_fn_0053b620((u8)flag) != 0)
            return cm_fn_00849d60(self, person, club);
    }
    if (D(person + 0x39u) == club) goto existing;
    value = cm_fn_00540a50(club, 0, 1);
    other = D(person + 0x39u);
    if (other != value) return 7;
existing:
    receiver = D(0x00ae1050u);
    entry = cm_fn_004cf5f0(receiver, person, club);
    if (entry == 0) return 7;
    return B(entry + 0x4fu) >> 4;
found:
    if (output != 0) B(output) = B(entry);
    return (B(entry + 0x24u) >> 1) & 15u;
}
