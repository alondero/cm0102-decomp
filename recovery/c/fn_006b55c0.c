#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_006b5ce0(u32, u32);
extern u32 CM_FASTCALL cm_fn_006b5a70(u32);
extern u32 CM_THISCALL cm_fn_006b98c0(u32, u32);
extern u32 CM_CDECL cm_fn_006b6570(s16, u8);
extern u32 CM_STDCALL cm_fn_006d5cd0(u32);
extern u32 CM_THISCALL cm_fn_006d78b0(u32, u32);
extern u32 CM_THISCALL cm_fn_006d8c70(u32, u32);
extern u32 CM_STDCALL cm_fn_00729ff0(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0090c510(u32 incoming_ecx);

/* Preferred-image VA006b55c0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4, full Boolean EAX. The two partial argument pushes to
   6d5cd0 are masked to 15 bits there; 6b6570 reads only WORD/BYTE.
   The saved receiver is kept independently of the reused EDI value.
   90c510 jumps to947299, whose two PUSH ECX slots are read after the
   import boundary; forward the actual ECX, including REP's zero count. */
u32 CM_THISCALL cm_fn_006b55c0(u32 self, u32 row)
{
    u32 state, owner, receiver, edi, value, pointer, index, slot, now, limit;
    u32 first, second, ignored, clock_ecx;
    s32 threshold, score;
    u8 flags, left, right;

    if (row == 0) {
        value = cm_fn_006b5ce0(self, 0);
        if (value != 0) cm_fn_006b5a70(self);
        return 0;
    }
    state = D(row + 0x5b);
    owner = D(row + 0x57);
    flags = B(state);
    if (flags != 0) {
        left = B(state + 0x107);
        right = B(state + 0x105);
        if (left != right) {
            index = (u32)(s32)(s16)W(0x00b77c7a);
            slot = state + index * 4u + 0x85;
            value = D(slot);
            if (value != 0) return 0;
            D(slot) = 1;
            left = B(state + 0x107);
            B(state + 0x107) = (u8)(left + 1u);
            clock_ecx = value;
            goto stamp;
        }
    }
    edi = self;
    left = B(state + 0x108);
    if (left != 0) goto reset;
    if ((flags & 8u) != 0) {
        receiver = D(owner + 0x8f7a);
        value = cm_fn_006d8c70(receiver, state + 5);
        if ((u8)value == 0) goto reset;
    }
    receiver = D(owner + 0x8f7a);
    left = B(receiver + 0x2db0);
    if (left == 0x14) edi = 0;
    else edi = D(state + 0x109);
    now = cm_fn_0090c510(receiver);
    limit = D(state + 0x10d);
    limit += edi;
    if (now < limit) {
        edi = self;
        goto reset;
    }
    flags = B(state);
    if ((flags & 8u) != 0) {
        receiver = D(owner + 0x8f7a);
        pointer = cm_fn_006d78b0(receiver, 0);
        if (pointer != 0) {
            value = W(pointer);
            receiver = D(owner + 0x8f7a);
            /* ECX is unused by this original stdcall helper. */
            value = cm_fn_006d5cd0(value);
            if ((u8)value == 2) D(state + 0x115) = 1;
            D(state + 1) = pointer;
            B(state) = 8;
            for (index = 0; index != 16; ++index)
                D(state + 0x85 + index * 4u) = 0;
            index = (u32)(s32)(s16)W(0x00b77c7a);
            D(state + 0x85 + index * 4u) = 1;
            B(state + 0x107) = 1;
            D(state + 0x109) = 0;
            clock_ecx = index;
            goto stamp;
        }
    }
    value = D(state + 0x111);
    edi = 0;
    if (value != 0) {
        D(state + 0x115) = 0;
        value = cm_fn_006b98c0(owner, 1);
        B(row + 0x67) = (u8)value;
        if ((u8)value == 2 || (u8)value == 8) {
            value = D(self + 0x25);
            D(self + 0x25) = value + 1u;
        }
        receiver = D(owner + 0x8f7a);
        left = B(receiver + 5);
        if (left != 0) {
            pointer = cm_fn_006d78b0(receiver, 0);
            if (pointer != 0) {
                owner = D(owner + 0x8f7a);
                value = W(pointer);
                value = cm_fn_006d5cd0(value);
                if ((u8)value == 2) D(state + 0x115) = 1;
                D(state + 1) = pointer;
                B(state) = 8;
                for (index = 0; index != 16; ++index)
                    D(state + 0x85 + index * 4u) = 0;
                index = (u32)(s32)(s16)W(0x00b77c7a);
                D(state + 0x85 + index * 4u) = 1;
                B(state + 0x107) = 1;
                D(state + 0x109) = 0;
                now = cm_fn_0090c510(0);
                D(state + 0x111) = 0;
                D(state + 0x10d) = now;
                return 1;
            }
        }
    }
    B(state) = 0;
    left = B(row + 0x67);
    if (left == 2) {
        B(state) = 0x40;
        D(state + 0x109) = edi;
    }
    left = B(row + 0x67);
    if (left == 8) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 0x40u;
    }
    left = B(row + 0x67);
    if (left == 4) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 0x20u;
    }
    pointer = D(row + 0x5f);
    threshold = (s32)(s8)B(owner + 0xf5bd);
    score = B(pointer + 0x11f);
    if (threshold > score) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 1u;
    }
    pointer = D(row + 0x5f);
    threshold = (s32)(s8)B(owner + 0xf5f3);
    score = B(pointer + 0x744);
    if (threshold > score) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 2u;
    }
    value = D(state + 0x115);
    if (value != edi) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 0x10u;
    }
    left = B(owner + 0x8eb2);
    if (left != 0) {
        flags = B(state);
        D(state + 0x109) = edi;
        B(state) = flags | 0x80u;
    }
    threshold = (s32)(s16)W(owner + 0x8ed0);
    pointer = D(row + 0x5f);
    threshold /= 11;
    score = (s32)(s16)W(pointer + 0x4d);
    if (threshold > score) {
        flags = B(state);
        B(state) = flags | 4u;
        pointer = D(row + 0x5f);
        value = (u32)(s32)(s16)W(pointer + 0x51);
        D(state + 0x109) = 0x226u - value * 50u;
    }
    flags = B(state);
    D(state + 0x111) = 1;
    if (flags == 0) return 0;
    for (index = 0; index != 16; ++index)
        D(state + 0x85 + index * 4u) = 0;
    index = (u32)(s32)(s16)W(0x00b77c7a);
    D(state + 0x85 + index * 4u) = 1;
    B(state + 0x107) = 1;
    now = cm_fn_0090c510(0);
    D(state + 0x10d) = now;
    pointer = D(row + 0x5f);
    ignored = D(0x00b6b7c0);
    (void)ignored;
    cm_fn_00729ff0(owner, pointer, 1);
    receiver = D(self + 0x34);
    state = D(receiver + 0x5f);
    if (state != 0) {
        pointer = D(row + 0x5f);
        value = W(pointer + 0x4f);
        first = cm_fn_006b6570((s16)value, 1);
        value = W(state + 0x4f);
        second = cm_fn_006b6570((s16)value, 1);
        if ((s8)first <= (s8)second) {
            receiver = D(self + 0x34);
            pointer = D(row + 0x5f);
            state = D(receiver + 0x5f);
            value = W(pointer + 0x4d);
            if ((s16)value <= (s16)W(state + 0x4d)) return 1;
        }
    }
    D(self + 0x34) = row;
    return 1;

stamp:
    now = cm_fn_0090c510(clock_ecx);
    D(state + 0x10d) = now;
    return 1;
reset:
    value = cm_fn_006b5ce0(edi, 0);
    if (value != 0) cm_fn_006b5a70(edi);
    return 0;
}
