#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0094c322(u32, u32);
extern u32 CM_CDECL cm_fn_00952061(u32, u32, u32);
extern u32 CM_CDECL cm_fn_009531a6(u32, u32, u32);

static u32 digit_00952128(u8 c)
{
    u32 table;
    if ((s32)D(0x00ad91dc) > 1) return cm_fn_0094c322(c, 4);
    table = D(0x00ad8fd0);
    return B(table + (u32)c * 2u) & 4u;
}

/* Preferred-image VA00952128/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL7/full EAX flags0/1/2/4. Scratch5c uses original local adjacency;
   actual twelve-DWORD dispatch table retained. Original conversion helpers.
   The rounding byte is L17, exactly as the original, not a repaired carry.
   Final D[L26] upper16 are unobserved: only AX is stored, then EAX becomes
   flags. Load WORD there; do not initialize the adjacent unwritten bytes. */
u32 CM_CDECL cm_fn_00952128(volatile u32 output_arg, volatile u32 end_arg,
                           volatile u32 input_arg, volatile u32 scale_arg,
                           volatile u32 adjust_arg, volatile u32 decimal_arg,
                           volatile u32 implicit_exponent_arg)
{
    u32 scratch[0x5c / 4];
    u32 l = (u32)scratch, cursor, state = 0, pointer, count, exponent;
    u32 target, seen, value, out, low, middle, high, exponent_word, flags;
    u8 c;
    cursor = input_arg;
    D(l + 0x50) = l;
    D(l + 0x34) = 0;
    D(l + 0x44) = 1;
    D(l + 0x58) = 0;
    D(l + 0x4c) = 0;
    D(l + 0x38) = 0;
    D(l + 0x3c) = 0;
    D(l + 0x30) = 0;
    D(l + 0x2c) = 0;
    D(l + 0x40) = 0;
    D(l + 0x54) = 0;
    D(l + 0x48) = 0;
    input_arg = cursor;
    do {
        c = B(cursor);
        if (c != 0x20 && c != 9 && c != 10 && c != 13) break;
        ++cursor;
    } while (1);
next_character:
    c = B(cursor);
    ++cursor;
    if (state > 11) goto dispatch_end;
    target = D(0x009525c9 + state * 4u);
    switch (target) {
    case 0x00952192: goto state_zero;
    case 0x009521e1: goto state_one;
    case 0x00952238: goto state_two;
    case 0x00952262: goto state_three;
    case 0x009522bd: goto state_four;
    case 0x00952334: goto state_five;
    case 0x0095236a: goto state_six;
    case 0x009523b4: goto state_seven;
    case 0x00952393: goto state_eight;
    case 0x00952418: goto state_nine;
    case 0x00952402: goto dispatch_end;
    case 0x009523ce: goto state_eleven;
    default: __builtin_unreachable(); /* Pinned original dispatch targets. */
    }
state_zero:
    if ((s8)c >= '1' && (s8)c <= '9') goto integer_again;
    if (c == B(0x00ad91e0)) goto decimal_first;
    if (c == '+') {
        D(l + 0x34) = 0;
        state = 2;
        goto next_character;
    }
    if (c == '-') {
        D(l + 0x34) = 0x8000;
        state = 2;
        goto next_character;
    }
    if (c == '0') {
        state = 1;
        goto next_character;
    }
    goto preceding_end;
state_one:
    D(l + 0x4c) = 1;
    if ((s8)c >= '1' && (s8)c <= '9') goto integer_again;
    if (c == B(0x00ad91e0)) {
        state = 4;
        goto next_character;
    }
    if (c == '+' || c == '-') goto implicit_exponent;
    if (c == '0') {
        state = 1;
        goto next_character;
    }
    goto exponent_marker;
state_two:
    if ((s8)c >= '1' && (s8)c <= '9') goto integer_again;
    if (c == B(0x00ad91e0)) goto decimal_first;
    if (c == '0') {
        state = 1;
        goto next_character;
    }
    goto saved_end;
state_three:
    D(l + 0x4c) = 1;
    while (digit_00952128(c) != 0) {
        count = D(l + 0x58);
        if (count < 25) {
            pointer = D(l + 0x50);
            D(l + 0x58) = count + 1;
            c = (u8)(c - '0');
            D(l + 0x50) = pointer + 1;
            B(pointer) = c;
        } else {
            value = D(l + 0x54);
            D(l + 0x54) = value + 1;
        }
        c = B(cursor);
        ++cursor;
    }
    if (c == B(0x00ad91e0)) {
        state = 4;
        goto next_character;
    }
    goto exponent_sign;
state_four:
    D(l + 0x4c) = 1;
    D(l + 0x38) = 1;
    if (D(l + 0x58) == 0) {
        while (c == '0') {
            value = D(l + 0x54);
            D(l + 0x54) = value - 1;
            c = B(cursor);
            ++cursor;
        }
    }
    while (digit_00952128(c) != 0) {
        count = D(l + 0x58);
        if (count < 25) {
            pointer = D(l + 0x50);
            D(l + 0x58) = count + 1;
            c = (u8)(c - '0');
            D(l + 0x50) = pointer + 1;
            value = D(l + 0x54);
            D(l + 0x54) = value - 1;
            B(pointer) = c;
        }
        c = B(cursor);
        ++cursor;
    }
exponent_sign:
    if (c == '+' || c == '-') goto implicit_exponent;
exponent_marker:
    if (((s8)c > 'C' && (s8)c <= 'E') || ((s8)c > 'c' && (s8)c <= 'e')) {
        state = 6;
        goto next_character;
    }
    goto preceding_end;
state_five:
    D(l + 0x38) = 1;
    if (digit_00952128(c) == 0) goto saved_end;
    state = 4;
    --cursor;
    goto next_character;
state_six:
    input_arg = cursor - 2;
    if ((s8)c >= '1' && (s8)c <= '9') goto exponent_again;
    if (c == '+') {
        state = 7;
        goto next_character;
    }
    if (c == '-') goto negative_exponent;
    if (c == '0') {
        state = 8;
        goto next_character;
    }
    goto saved_end;
state_seven:
    if ((s8)c >= '1' && (s8)c <= '9') goto exponent_again;
    if (c == '0') {
        state = 8;
        goto next_character;
    }
    goto saved_end;
state_eight:
    D(l + 0x3c) = 1;
    while (c == '0') {
        c = B(cursor);
        ++cursor;
    }
    if ((s8)c >= '1' && (s8)c <= '9') goto exponent_again;
    goto preceding_end;
state_nine:
    D(l + 0x3c) = 1;
    exponent = 0;
    while (digit_00952128(c) != 0) {
        exponent = (u32)(s32)(s8)c + exponent * 10u - '0';
        if ((s32)exponent > 0x1450) {
            exponent = 0x1451;
            break;
        }
        c = B(cursor);
        ++cursor;
    }
    do {
        D(l + 0x40) = exponent;
        if (digit_00952128(c) == 0) break;
        c = B(cursor);
        ++cursor;
    } while (1);
    goto preceding_end;
state_eleven:
    if (implicit_exponent_arg != 0) {
        input_arg = cursor - 1;
        if (c == '+') {
            state = 7;
            goto next_character;
        }
        if (c == '-') goto negative_exponent;
        goto saved_end;
    }
    state = 10;
    --cursor;
dispatch_end:
    if (state == 10) goto finish;
    goto next_character;
negative_exponent:
    D(l + 0x44) = 0xffffffffu;
    state = 7;
    goto next_character;
decimal_first:
    state = 5;
    goto next_character;
integer_again:
    state = 3;
    --cursor;
    goto next_character;
exponent_again:
    state = 9;
    --cursor;
    goto next_character;
implicit_exponent:
    --cursor;
    state = 11;
    goto next_character;
saved_end:
    cursor = input_arg;
    goto finish;
preceding_end:
    --cursor;
finish:
    pointer = end_arg;
    seen = D(l + 0x4c);
    D(pointer) = cursor;
    if (seen == 0) {
        low = exponent_word = high = middle = 0;
        D(l + 0x48) = 4;
        goto write_output;
    }
    count = D(l + 0x58);
    if (count > 24) {
        c = B(l + 0x17);
        if ((s8)c >= 5) {
            c = B(l + 0x17);
            B(l + 0x17) = (u8)(c + 1);
        }
        D(l + 0x58) = 24;
        pointer = D(l + 0x50);
        --pointer;
        value = D(l + 0x54);
        D(l + 0x54) = value + 1;
    } else pointer = D(l + 0x50);
    if (D(l + 0x58) == 0) {
        low = exponent_word = high = middle = 0;
        goto check_range;
    }
    do {
        --pointer;
        if (B(pointer) != 0) break;
        value = D(l + 0x58);
        D(l + 0x58) = value - 1;
        value = D(l + 0x54);
        D(l + 0x54) = value + 1;
    } while (1);
    count = D(l + 0x58);
    cm_fn_00952061(l, count, l + 0x1c);
    exponent = D(l + 0x40);
    if ((s32)D(l + 0x44) < 0) exponent = 0u - exponent;
    exponent += D(l + 0x54);
    if (D(l + 0x3c) == 0) exponent += adjust_arg;
    if (D(l + 0x38) == 0) exponent -= decimal_arg;
    if ((s32)exponent > 0x1450) {
        D(l + 0x30) = 1;
        goto provisional_output;
    }
    if ((s32)exponent < (s32)0xffffebb0u) {
        D(l + 0x2c) = 1;
        goto provisional_output;
    }
    value = scale_arg;
    cm_fn_009531a6(l + 0x1c, exponent, value);
    low = D(l + 0x1c);
    middle = D(l + 0x1e);
    high = D(l + 0x22);
    exponent_word = W(l + 0x26);
    goto check_range;
provisional_output:
    middle = input_arg;
    high = input_arg;
    exponent_word = input_arg;
    low = input_arg;
check_range:
    if (D(l + 0x30) != 0) {
        middle = 0;
        exponent_word = 0x7fff;
        high = 0x80000000u;
        low = 0;
        D(l + 0x48) = 2;
    } else if (D(l + 0x2c) != 0) {
        low = exponent_word = high = middle = 0;
        D(l + 0x48) = 1;
    }
write_output:
    out = output_arg;
    exponent_word |= D(l + 0x34);
    D(out + 6) = high;
    D(out + 2) = middle;
    W(out + 0xa) = (u16)exponent_word;
    flags = D(l + 0x48);
    W(out) = (u16)low;
    return flags;
}
