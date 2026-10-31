#include "cmre.h"

/* Preferred-image VA 0075b6e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch e4: counter BYTE L0, message L4. Setter 785790 observes only
   index BYTE, including its diagnostic path; keep the original DWORD read
   of the counter slot. 757fa0's full EAX is forwarded despite its draft's
   BYTE return. 7dab90 is THIS3/RET12, 735870 is THIS3/RET12. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u8, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00757fa0(u32);
extern u32 CM_THISCALL cm_fn_007dab90(u32, u32, u32, u32);
extern u16 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_00735870(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0075b6e0(u32 self, u32 person,
                              volatile u32 club_arg, volatile u8 mode_arg,
                              volatile u8 flag_arg, volatile u32 other_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe4];
    u32 l = (u32)scratch, message = l + 4;
    u32 eax, base, value, saved_value, club, owner, cf, other, raw_index;
    u8 index;
    frame.state = 0xffffffffu; frame.handler = 0x0095d9db;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    eax = frame.prev;
    if (person == 0) goto done;
    eax = D(person + 0x1a);
    if (eax == 0) goto done;
    eax = D(person + 0x39);
    if (eax == 0) goto done;
    base = D(eax + 0xcf);
    if (base == 0) goto done;
    base = D(eax + 0x69);
    if (base == 0) goto done;
    eax = D(person + 0x61);
    if (eax == 0) goto done;
    eax = other_arg;
    if (eax == 0 && mode_arg == 2) goto done;

    cm_fn_0077bbe0(message, 0xbe5, 0);
    value = D(person);
    frame.state = 0;
    cm_fn_00785790(message, 0, value);
    base = D(person + 4); value = D(base + 0x33);
    cm_fn_00785790(message, 1, value);
    base = D(person + 8); value = D(base + 0x33);
    cm_fn_00785790(message, 2, value);
    base = D(person + 0xc); value = D(base + 0x33);
    cm_fn_00785790(message, 3, value);
    base = D(person + 0x39); value = D(base);
    cm_fn_00785790(message, 4, value);
    value = cm_fn_0090cfc0(2);
    cm_fn_00785790(message, 5, value);
    saved_value = cm_fn_00757fa0(person);
    cm_fn_00785790(message, 6, saved_value);
    club = club_arg;
    base = D(club + 0x53);
    if (base != 0) value = D(base);
    else value = 0xffffffffu;
    owner = D(0x00b74430);
    value = cm_fn_007dab90(owner, club, saved_value, value);
    cm_fn_00785790(message, 7, value);
    base = D(person + 0x61);
    value = (u32)(s32)(s16)cm_fn_005b7d20(base);
    cm_fn_00785790(message, 8, value);
    value = (u32)(s32)(s8)mode_arg;
    cm_fn_00785790(message, 9, value);
    base = D(person + 0x39);
    saved_value = 0;
    value = cm_fn_00600a50(base, 0, 1, 0, 0);
    if (value == 0) {
        base = D(person + 0x39);
        cf = D(base + 0xcf);
        saved_value = cm_fn_00735870(self, cf, person, 0x82);
    }
    index = 11;
    cm_fn_00785790(message, 10, saved_value);
    if (mode_arg == 2) {
        other = other_arg;
        value = D(other);
        cm_fn_00785790(message, 11, value);
        base = D(other + 4); value = D(base + 0x33);
        cm_fn_00785790(message, 12, value);
        base = D(other + 8); value = D(base + 0x33);
        cm_fn_00785790(message, 13, value);
        base = D(other + 0xc);
        index = 15;
        value = D(base + 0x33);
        cm_fn_00785790(message, 14, value);
    }
    value = (s16)W(club + 0x80) > 0x1c52;
    raw_index = index++;
    cm_fn_00785790(message, (u8)raw_index, value);
    base = D(person + 0x39);
    raw_index = index++;
    cf = D(base + 0xcf);
    value = D(cf);
    cm_fn_00785790(message, (u8)raw_index, value);
    base = D(cf + 4);
    raw_index = index++;
    value = D(base + 0x33);
    cm_fn_00785790(message, (u8)raw_index, value);
    base = D(cf + 8);
    raw_index = index++;
    value = D(base + 0x33);
    B(l) = index;
    cm_fn_00785790(message, (u8)raw_index, value);
    base = D(cf + 0xc);
    raw_index = D(l);
    value = D(base + 0x33);
    cm_fn_00785790(message, (u8)raw_index, value);
    value = (u32)(s32)(s8)flag_arg;
    ++index;
    cm_fn_00785790(message, index, value);
    base = D(person + 0x39);
    owner = D(0x00b7431c);
    eax = (u32)cm_fn_007861e0(owner, message, base);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
