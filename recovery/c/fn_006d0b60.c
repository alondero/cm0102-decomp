#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern void CM_THISCALL cm_fn_006d6f70(u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_006cdd50(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_005fd480(u32, u32);
extern void CM_THISCALL cm_fn_006e1090(u32, u32);
extern void CM_FASTCALL cm_fn_006e1aa0(u32);
extern u8 CM_THISCALL cm_fn_00897590(u32, u32, u32, u32);
extern u8 CM_FASTCALL cm_fn_006e1a40(u32);
extern u32 CM_THISCALL cm_fn_00897760(u32, u32, s8, u32);
extern u32 CM_THISCALL cm_fn_006ce110(u32, s8);
extern u32 CM_THISCALL cm_fn_006bbb60(u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006bbf60(
    u32 incoming_eax, u32 unused_edx, u32 receiver);

/* Preferred-image VA006d0b60/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   FAST1/RET0/full incidental EAX. No scratch address escapes. Original
   DWORD loads of BYTE locals are narrowed only after full input-slot proof:
   6d6f70 arg3/4/5/6 BYTE; 6cdd50 flag BYTE; 6ce110 and 6bbb60 selector BYTE.
   6e1a40 has no stack arguments: the earlier push belongs to 897760 arg3.
   6bbf60 can preserve upper EAX from 6bbb60. Its incoming EDX is unobserved:
   before its first EDX definition the only calls are leaf 5b57c0, which
   neither reads nor forwards EDX. All callees remain original boundaries. */
u32 CM_FASTCALL cm_fn_006d0b60(u32 self)
{
    u8 side = 0;
    u32 eax;
    do {
        u32 group = side;
        u32 record = self + group * 0x22d8u + 0x4796u;
        u32 remaining = 20;
        u8 changed = 0;
        do {
            s8 state = (s8)B(record + 0x19);
            if ((state >= 0 || state == -3) && B(record + 0x1d) != 0) {
                u8 apply = B(record + 0x1c);
                ++changed;
                if (apply != 0) {
                    u8 source = B(record + 0x2e);
                    u8 kind = cm_fn_00714d30(record);
                    u32 receiver = D(self + 0x8f7a);
                    if (state >= 0) {
                        cm_fn_006d6f70(receiver, 0x1f56, self, source, kind,
                                     0xffffffffu, 0xffffffffu, 0);
                        u8 flag = B(self + group * 0xd0u + 0x9049);
                        cm_fn_006cdd50(self, record, (u8)((flag != 0) - 2));
                        u8 previous = B(record + 0x19);
                        B(record + 0x1a) = previous;
                        B(record + 0x19) = 0xfe;
                    } else {
                        cm_fn_006d6f70(receiver, 0x1f57, self, source, kind,
                                     0xffffffffu, 0xffffffffu, 0);
                        cm_fn_006cdd50(self, record, 0);
                    }
                    s32 value = (s16)W(self + 0x8ed0);
                    u8 quotient = (u8)(value / 11);
                    B(record + 0x22) = quotient;
                    s32 code = (s16)W(self + 0x8ed4);
                    if (code <= 0x483) {
                        if (code == 0x483) {
                            B(record + 0x22) = (s8)quotient < 0x69 ? quotient : 0x69;
                        } else if (code == 0x1ef) {
                            B(record + 0x22) = (s8)quotient < 0x2d ? quotient : 0x2d;
                        } else if (code == 0x3de) {
                            B(record + 0x22) = (s8)quotient < 0x5a ? quotient : 0x5a;
                        }
                    } else if (code == 0x528) {
                        B(record + 0x22) = (s8)quotient < 0x78 ? quotient : 0x78;
                    }
                } else {
                    u32 random = cm_fn_0090cfc0(4);
                    if (random == 0 || D(record + 0x55) != 0) {
                        u8 source = B(record + 0x2e);
                        u8 kind = cm_fn_00714d30(record);
                        u32 receiver = D(self + 0x8f7a);
                        cm_fn_006d6f70(receiver, 0x1f58, self, source, kind,
                                     0xffffffffu, 0xffffffffu, 0);
                    }
                }
                u32 other = D(record + 0x55);
                if (other != 0) {
                    u8 other_source = B(other + 0x2e);
                    u8 source = B(record + 0x2e);
                    u8 other_kind = cm_fn_00714d30(other);
                    u8 kind = cm_fn_00714d30(record);
                    u32 receiver = D(self + 0x8f7a);
                    cm_fn_006d6f70(receiver, 0x20f6, self, source, kind,
                                 other_source, other_kind, 0);
                }
                B(record + 0x1d) = 0;
            }
            record += 0x1be;
            --remaining;
        } while (remaining != 0);
        eax = remaining; /* Original MOV/DEC counter leaves full EAX zero. */
        if (changed == 0) goto next_side;
        u32 group_base = self + group * 0xd0u;
        eax = D(group_base + 0x904d);
        if (eax != 0) {
            u32 bound = D(0x00ae2364);
            u32 id = D(eax);
            if ((s32)id >= (s32)(bound - 16u) && (s32)id < (s32)bound &&
                B(eax + 0x5f) != 0) {
                u32 receiver = D(0x00b63c98);
                eax = (u32)cm_fn_005fd210(receiver, eax);
                if (eax == 0) {
                    eax = (eax & 0xffffff00u) | B(group_base + 0x904b);
                    if ((u8)eax == 0) goto next_side;
                    u32 owner = D(self + 0x4792);
                    eax = D(owner + (side == 0 ? 0x1cu : 0x20u));
                    eax = cm_fn_0053b900(eax);
                    if (eax != 0) goto next_side;
                    eax = D(group_base + 0x904d);
                    receiver = D(0x00b63c98);
                    eax = cm_fn_005fd480(receiver, eax);
                    if (eax != 0) goto next_side;
                }
            }
        }
        if (B(group_base + 0x9049) != 0) {
            cm_fn_006e1090(group_base + 0x9032, 0);
            u16 value = W(self + 0x8ed0);
            W(self + group * 2u + 0x8ecc) = value;
        } else {
            u32 receiver = group_base + 0x9032;
            cm_fn_006e1aa0(receiver);
            u32 owner = D(self + 0x4792);
            u8 mode_byte = B(owner + (side == 0 ? 0x40u : 0x41u));
            u32 object = D(owner + (side == 0 ? 0x1cu : 0x20u));
            u32 mode = mode_byte == 0;
            u32 manager = D(0x00de1d08);
            s8 selected = (s8)cm_fn_00897590(manager, object, mode, 0);
            u32 index = (u32)(s32)selected + group * 20u;
            u32 selected_record = self + index * 0x1beu + 0x4796u;
            if ((s8)B(selected_record + 0x19) < 0) {
                owner = D(self + 0x4792);
                mode_byte = B(owner + (side == 0 ? 0x40u : 0x41u));
                object = D(owner + (side == 0 ? 0x1cu : 0x20u));
                mode = mode_byte == 0;
                s8 chosen = (s8)cm_fn_006e1a40(receiver);
                manager = D(0x00de1d08);
                cm_fn_00897760(manager, object, chosen, mode);
            }
            D(self + (side == 0 ? 0xf59eu : 0xf5a2u)) = 0;
            cm_fn_006ce110(self, (s8)side);
            u16 value = W(self + 0x8ed0);
            W(self + group * 2u + 0x8ecc) = value;
        }
        eax = cm_fn_006bbb60(self, side);
        eax = cm_fn_006bbf60(eax, 0, self);
next_side:
        ++side;
        eax = (eax & 0xffffff00u) | side;
    } while ((s8)side < 2);
    B(self + 0x8eb5) = 0;
    return eax;
}
