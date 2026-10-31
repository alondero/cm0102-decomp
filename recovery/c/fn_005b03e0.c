#include "cmre.h"
/* Preferred-image VA 0x005b03e0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller 005aff94 stores AL at 005aff9b, then overwrites EAX
   at 005aff9e. Only the returned low byte is live. */
extern u32 CM_STDCALL cm_fn_007c6e30(u32, u32);
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile s16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

u8 CM_STDCALL cm_fn_005b03e0(u32 left, u32 right)
{
    u32 initial_profile, right_tag, left_tag, pointer, left_club, right_club;
    u32 first, second, receiver;
    s32 quotient, age_limit;
    s16 elapsed;
    u8 same_tag = 0, shared_tag = 0, shared_field57 = 0;
    u8 right_in_left_tag = 0, left_in_right_tag = 0;
    long double product;
    initial_profile = D(right + 0x61u);
    if (initial_profile == 0) return 3;
    right_tag = D(right + 0x1au);
    left_tag = D(left + 0x1au);
    if (left_tag == right_tag) same_tag = 1;
    pointer = D(right + 0x24u);
    if (pointer != 0 && pointer == D(left + 0x24u)) return 3;
    left_club = D(left + 0x39u);
    if (left_club != 0) {
        right_club = D(right + 0x39u);
        if (right_club != 0) {
            first = D(left_club + 0x53u);
            second = D(right_club + 0x53u);
            if (first == second) shared_tag = 1;
        }
    }
    if (shared_tag == 0) {
        pointer = D(left + 0x24u);
        if (pointer != 0) {
            right_club = D(right + 0x39u);
            if (right_club != 0) {
                first = D(pointer + 0x53u);
                second = D(right_club + 0x53u);
                if (first == second) shared_tag = 1;
            }
        }
    }
    right_club = D(right + 0x39u);
    if (right_club != 0 && D(right_club + 0x53u) == left_tag)
        right_in_left_tag = 1;
    if (left_club != 0) {
        if (D(left_club + 0x53u) == right_tag) left_in_right_tag = 1;
        if (right_club != 0) {
            first = D(left_club + 0x57u);
            second = D(right_club + 0x57u);
            if (first == second) shared_field57 = 1;
        }
    }
    if (right != 0 && right_tag != 0) {
        quotient = (s32)W(right_tag + 0x8eu) / 10;
        first = B(right + 0x22u);
        product = (long double)quotient * (long double)(s32)first;
        if (product > (long double)CM_GLOBAL(volatile double, 0x0096a790)) return 3;
        if (W(initial_profile + 0xdu) > 0x1482) return 3;
    }
    if (shared_field57 != 0 && (s8)B(right + 0x18u) > 19) return 3;
    if (left_club != 0 && right != 0) {
        receiver = D(0x00b74350);
        (void)receiver;
        elapsed = (s16)cm_fn_007c6e30(right, left_club);
        if (elapsed != -1 && elapsed < 2) return 3;
    }
    if (shared_tag != 0) {
        age_limit = 21;
    } else if (right_in_left_tag != 0) {
        age_limit = 23;
    } else if (same_tag != 0 || left_in_right_tag != 0) {
        age_limit = 23;
    } else {
        pointer = D(left + 0x24u);
        if (pointer != 0) {
            pointer = D(pointer + 0x53u);
            if (pointer != 0) {
                if ((s32)D(pointer + 0x88u) < 1000) {
                    if (pointer == D(right + 0x1au)) return 1;
                }
            }
        }
        return 0;
    }
    pointer = D(right + 0x61u);
    if (pointer != 0) {
        if (W(pointer + 0xbu) > 0x109a) return 3;
    } else {
        pointer = D(right + 0x69u);
        if (pointer != 0 && W(pointer + 0xau) > 0x109a) return 3;
    }
    if ((s8)B(right + 0x18u) > age_limit) return 2;
    pointer = D(left + 0x39u);
    if (pointer == 0 || right == 0) return 1;
    receiver = D(0x00b74350);
    (void)receiver;
    elapsed = (s16)cm_fn_007c6e30(right, pointer);
    if (elapsed != -1 && elapsed < 3) return 2;
    return 1;
}
