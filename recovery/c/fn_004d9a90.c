#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae770(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);

static u32 text_4d9a90(u32 target)
{
    switch (target) {
    case 0x004d9bbb: return 0x009ae5c0;
    case 0x004d9bc5: return 0x009ae568;
    case 0x004d9bcf: return 0x009ae508;
    case 0x004d9bd9: return 0x009ae4b8;
    case 0x004d9be3: return 0x009ae450;
    case 0x004d9bed: return 0x009ae3ec;
    case 0x004d9bf7: return 0x009ae39c;
    case 0x004d9c1a: return 0x009ae358;
    case 0x004d9c24: return 0x009ae2fc;
    case 0x004d9c2e: return 0x009ae29c;
    case 0x004d9c38: return 0x009ae248;
    case 0x004d9c42: return 0x009ae1dc;
    case 0x004d9c4c: return 0x009ae174;
    case 0x004d9c56: return 0x009ae120;
    case 0x004d9c7c: return 0x009ae0d4;
    case 0x004d9c86: return 0x009ae074;
    case 0x004d9c90: return 0x009ae00c;
    case 0x004d9c9a: return 0x009adfb4;
    case 0x004d9ca4: return 0x009adf44;
    case 0x004d9cae: return 0x009aded8;
    case 0x004d9cb8: return 0x009ade80;
    case 0x004d9cde: return 0x009ade38;
    case 0x004d9ce5: return 0x009adddc;
    case 0x004d9cec: return 0x009add78;
    case 0x004d9cf3: return 0x009add24;
    case 0x004d9cfa: return 0x009adcb8;
    case 0x004d9d01: return 0x009adc50;
    case 0x004d9d08: return 0x009adc00;
    case 0x004d9d33: return 0x009adbd8;
    case 0x004d9d3a: return 0x009adb9c;
    case 0x004d9d41: return 0x009adb58;
    case 0x004d9d48: return 0x009adb24;
    case 0x004d9d4f: return 0x009adad8;
    case 0x004d9d56: return 0x009ada90;
    case 0x004d9d5d: return 0x009ada5c;
    default: __builtin_unreachable(); /* Six pinned tables, each guarded by <=6. */
    }
}

/* Preferred-image VA004d9a90/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full incidental EAX; no input ECX. Scratch148/name300L1c.
   Retain all eleven getter calls, actual six DWORD dispatch tables, the
   unconditional append after an out-of-range role, and the club link's
   original +4 text address. All callees are original boundaries. */
u32 CM_STDCALL cm_fn_004d9a90(u32 event, volatile u32 mode, volatile u32 text_object)
{
    u32 scratch[0x148 / 4];
    u32 l = (u32)scratch, value, person, enabled, old_flags;
    u32 first, second, third, receiver, flags, table, index, target, format;
    u32 base, club, offset, eax;
    value = cm_fn_00785830(event, 0);
    D(l + 0x18) = value;
    person = cm_fn_00785830(event, 2);
    value = cm_fn_00785830(event, 3);
    D(l + 8) = value;
    value = cm_fn_00785830(event, 4);
    D(l + 0xc) = value;
    value = cm_fn_00785830(event, 5);
    D(l + 4) = value;
    enabled = cm_fn_00785830(event, 6);
    value = cm_fn_00785830(event, 7);
    D(l) = value;
    old_flags = cm_fn_00785830(event, 8);
    value = cm_fn_00785830(event, 9);
    D(l + 0x10) = value;
    value = cm_fn_00785830(event, 10);
    D(l + 0x14) = value;
    cm_fn_00785830(event, 11);
    if (mode != 0) {
        third = D(l + 4);
        second = D(l + 0xc);
        first = D(l + 8);
        cm_fn_0053e140(first, second, third, l + 0x1c, 300, 1, person);
        cm_fn_0066f4e0(0x00de1f64, 0x009ae604, l + 0x1c);
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        if (enabled != 0) {
            flags = D(l + 0x10);
            if (flags != 0xffffffffu) {
                if (old_flags == flags) table = 0x004d9f5c;
                else if ((u8)flags & 2u) table = 0x004d9ed0;
                else if ((u8)flags & 1u) table = 0x004d9eec;
                else if ((u8)old_flags & 1u) table = 0x004d9f08;
                else if ((u8)old_flags & 2u) table = 0x004d9f24;
                else table = 0x004d9f40;
                index = D(l) - 1u;
                if (index <= 6u) {
                    target = D(table + index * 4u);
                    format = text_4d9a90(target);
                    cm_fn_0066f4e0(0x00de1f64, format);
                }
                cm_fn_004ae770(receiver, 0x00de1f64);
                value = D(l + 0x14);
                if (value != 0) {
                    if (B(l + 0x10) & 1u) format = 0x009ada0c;
                    else {
                        value = D(l);
                        if (value == 1) format = 0x009ad9e4;
                        else if (value == 3 || value == 2) format = 0x009ad8e8;
                        else format = 0x009ad990;
                    }
                    cm_fn_0066f4e0(0x00de1f64, format);
                    cm_fn_004ae770(receiver, 0x00de1f64);
                }
            }
        }
        value = D(l + 0x18);
        base = D(0x00ae23b4);
        club = base + value * 0x245u;
        cm_fn_004ae8a0(receiver, club + 4u, 0x7d5, club);
        second = D(l + 0xc);
        first = D(l + 8);
        offset = person * 110u;
        third = D(l + 4);
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(offset + base, first, second, third, 0);
    } else {
        third = D(l + 4);
        second = D(l + 0xc);
        first = D(l + 8);
        cm_fn_0053e140(first, second, third, l + 0x1c, 300, 5, person);
        cm_fn_0066f4e0(0x00de1f64, 0x009ad8b8, l + 0x1c);
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        offset = person * 110u;
        third = D(l + 4);
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(offset + base, first, second, third, 0);
    }
    if (eax == 0) return eax;
    base = D(0x00ae23bc);
    receiver = text_object;
    return cm_fn_004ae8a0(receiver, l + 0x1c, 0x7d7, offset + base);
}
