#include "cmre.h"

/* Preferred-image VA0085C260/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full EAX; wrapper007E6720 forwards it. L is the original
   228-byte scratch area. Date constructors and all selectors stay original. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_005c6710(u32, u32);
extern u8 CM_THISCALL cm_fn_005fa730(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0085c800(u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0085de20(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0085c260(u32 self, volatile u32 flags_arg,
                               volatile u32 object_arg, volatile u32 explicit_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x228];
    u32 L = (u32)scratch, object, flags, choice, p, q, value, id, base;
    u32 first, second, key, club_a, club_b, competition, a, b, result;
    u16 word_a, word_b;
    u8 outcome;
    frame.state = 0xffffffff;
    frame.handler = 0x9629d1;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(L);
    object = object_arg;
    choice = 0;
    frame.state = 0;
    D(L + 8) = 0;
    if (object == 0) {
        cm_fn_00944cff(0xa835b0, 0, 0, (void *)(L + 0x128), (void *)(L + 0x28));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0x128, L + 0x28, 0x1f0);
        result = cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        goto done;
    }
    word_a = W(object + 0x28);
    word_b = W(object + 0x2a);
    D(object + 0x18) = 0;
    D(object + 4) = 0xffffffff;
    p = cm_fn_00549ef0(L + 0x14, word_b, word_a);
    B((u32)&frame.state) = 1;
    result = cm_fn_00417c10(L, L + 0x20, p);
    flags = flags_arg;
    B((u32)&frame.state) = 0;
    if (flags == 0x8000) {
        p = D(object + 0x1c);
        id = D(0x9d0264);
        if (D(p) == id) goto use_four;
        p = D(object + 0x20);
        if (D(p) == id) goto use_four;
        base = D(0xae23b4);
        p = D(base + id * 0x245u + 0x69);
        D(object + 0x18) = p;
        value = D(p);
        D(object + 4) = value;
        result = value;
        goto done;
use_four:
        flags = 4;
    } else if (flags != 4) goto other_flags;
    p = D(object + 0x14);
    if (p == 0) goto choose;
    p = D(p + 0x5d);
    if (p == 0) goto choose;
    value = D(p);
    id = D(0x9cf2e4);
    if (value != id) goto choose;
    p = D(object + 0x1c);
    value = cm_fn_0053b900(p);
    if (value != 0) goto choose;
    if (D(0xa835a4) == 0xffffffff) goto choose;
    B(object + 0x3c) = 1;
    id = D(0x9cf510);
    base = D(0xae23a8);
    p = D(base + id * 290u + 0x80);
    D(object + 0x18) = p;
    value = D(p);
    D(object + 4) = value;
    result = value;
    goto done;
other_flags:
    if (flags != 0) goto choose;
    if (W(L + 2) < 2003) {
        p = D(object + 0x1c);
        value = cm_fn_0053b900(p);
        if (value != 0) {
            p = D(object + 0x1c);
            p = D(p + 0x53);
            value = D(p);
            id = D(0x9cf2e4);
            if (value == id) flags = 0x20;
        }
    }
    p = D(object + 0x1c);
    q = D(p + 0x53);
    if (q == 0) goto general;
    id = D(0x9cf2fc);
    if (D(q) != id) goto general;
    if (B(object + 0x42) == 0xff) goto general;
    value = D(object);
    if (value != D(0x9cf6e8)) goto general;
    if (D(p + 0x69) == 0) goto general;
    base = D(0xae23a8);
    choice = D(base + id * 290u + 0x80);
    cm_fn_0054a110(L + 0x18);
    p = D(self);
    B((u32)&frame.state) = 2;
    if (p == 0) goto capacity;
    value = D(choice);
    D(L + 0x14) = value;
    cm_fn_00417c10(L + 0x18, L + 0x20, L);
    a = D(self + 4);
    b = D(self);
    value = cm_fn_00946464(L + 0x14, b, a, 12, 0x85e380);
    if (value == 0) goto capacity;
    B((u32)&frame.state) = 0;
reset_choice:
    choice = 0;
general:
    B(object + 0x3c) = 0;
    if (flags == 0x20) goto check_club;
    p = D(object + 0x1c);
    if (p == choice) goto check_club;
    if (D(p + 0x69) == choice) goto check_club;
    p = D(p + 0x53);
    if (p != choice) {
        value = D(p);
        id = D(0x9cf360);
        if (value == id || value == D(0x9cf300)) {
            a = D(object + 0x14);
            p = D(0xae2c74);
            value = cm_fn_005c6710(p, a);
            if (value != 0) goto force_twenty;
        }
    }
    p = D(object + 0x1c);
    p = D(p + 0x69);
    D(object + 0x18) = p;
    value = D(p);
    D(object + 4) = value;
    result = p;
    goto done;
capacity:
    p = D(object + 0x1c);
    B((u32)&frame.state) = 0;
    p = D(p + 0x69);
    if ((s32)D(p + 0x3c) >= 40000) goto reset_choice;
    B(object + 0x3c) = 1;
    id = D(0x9cf2fc);
    base = D(0xae23a8);
    p = D(base + id * 290u + 0x80);
    D(object + 0x18) = p;
    value = D(p);
    B(object + 0x3c) = 1;
    D(object + 4) = value;
remember:
    p = D(object + 0x18);
    result = cm_fn_0085de20(self, p, L);
    goto done;
check_club:
    p = D(object + 0x1c);
    value = cm_fn_0053b900(p);
    result = value;
    if (value == 0) goto no_choice;
force_twenty:
    flags = 0x20;
choose:
    B(object + 0x3c) = 1;
    if ((flags & 0x10) != 0) {
        value = explicit_arg;
        result = value;
        if (value == 0xffffffff) goto no_choice;
        D(object + 4) = value;
        base = D(0xae23b0);
        p = base + value * 78u;
        D(object + 0x18) = p;
        result = p;
        goto done;
    }
    if (flags == 1) goto no_choice;
    competition = D(object + 0x14);
    word_a = W(object + 0x2c);
    id = D(competition);
    p = D(0xb5e134);
    outcome = cm_fn_005fa730(p, id, word_a, L + 0xc, L + 0x10, 1);
    if (outcome == 2) {
        value = D(L + 0x10);
        base = D(0xae23a8);
        second = base + value * 290u;
        choice = second;
        if ((flags & 0x80) != 0 || (flags & 0x2a00) != 0) {
            if (flags & 0x80) flags = 0x40;
            else if (flags & 0x200) flags = 0x80;
            else if (flags & 0x800) flags = 0x100;
            else if (flags & 0x2000) flags = 0x200;
            value = D(L + 0xc);
            first = base + value * 290u;
            D(L + 8) = first;
        } else if ((flags & 0x40) != 0 || (flags & 0x5500) != 0) {
            if (flags & 0x40) flags = 4;
            else if (flags & 0x100) flags = 0x40;
            else if (flags & 0x400) flags = 0x80;
            else if (flags & 0x1000) flags = 0x100;
            else if (flags & 0x4000) flags = 0x200;
            value = D(L + 0xc);
            D(L + 8) = second;
            choice = base + value * 290u;
        }
    } else if (outcome == 1) {
        value = D(L + 0xc);
        base = D(0xae23a8);
        choice = base + value * 290u;
    } else choice = D(competition + 0x5d);
    if ((flags & 0x20) != 0) {
        p = D(object + 0x1c);
        value = cm_fn_0053b900(p);
        if (value != 0) {
            p = D(object + 0x1c);
            choice = D(p + 0x53);
        }
    }
    if ((flags & 4) != 0 && choice != 0) {
        p = D(choice + 0x80);
        if (p != 0) {
            D(object + 0x18) = p;
            value = D(p);
            D(object + 4) = value;
            goto remember;
        }
        p = D(object + 0x1c);
        value = cm_fn_0053b900(p);
        flags = value != 0 ? 0x20 : 0x40;
    }
    club_b = D(object + 0x20);
    club_a = D(object + 0x1c);
    competition = D(object + 0x14);
    key = D(competition + 0x59);
    p = cm_fn_0085c800(self, flags, key, choice, L, club_a, club_b, object);
    D(object + 0x18) = p;
    if (p == 0) {
        value = D(L + 8);
        if (value != 0) {
            if (flags == 0x40) flags = 0x400;
            else if (flags == 0x80) flags = 0x800;
            else if (flags == 0x100) flags = 0x1000;
            else if (flags == 0x200) flags = 0x2000;
            club_b = D(object + 0x20);
            club_a = D(object + 0x1c);
            competition = D(object + 0x14);
            key = D(competition + 0x59);
            p = cm_fn_0085c800(self, flags, key, value, L, club_a, club_b, object);
            D(object + 0x18) = p;
        }
    }
    p = D(object + 0x18);
    result = p;
    if (p == 0) D(object + 4) = 0xffffffff;
    else { value = D(p); D(object + 4) = value; }
    goto done;
no_choice:
    D(object + 0x18) = choice;
    D(object + 4) = 0xffffffff;
done:
    value = frame.prev;
    CM_FS(volatile u32, 0) = value;
    return result;
}
