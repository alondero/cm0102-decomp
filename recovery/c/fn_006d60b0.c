/* Preferred-image VA006D60B0, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, two stack DWORDs, RET8. The logical result is signed16:
   sole caller006D6FFD uses AX, and every forwarded high half is dead before
   an ordinary value use (large-rng-return-proof/review.json, local evidence).
   Native callees remain explicit. RNG uses one stack bound plus physical
   EAX/EDX prefix registers; EDX:EAX return projection is not a source-type claim.
   Root contains integer instructions only. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u8 CM_THISCALL cm_fn_006d58f0(volatile u8 *self);
extern u8 CM_STDCALL cm_fn_006cb960(s8 first, s8 second, s8 third);
extern __attribute__((regparm(2))) unsigned long long CM_CDECL
    cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);

s16 CM_THISCALL cm_fn_006d60b0(u32 self, u32 dst, u32 context)
{
    s16 incoming, current, count, result;
    u32 src, index, rng_eax, rng_edx, value, bound;
    u8 phase, flag, a, b, c, d;

    incoming = (s16)W(dst);
    if (context) {
        phase = B(context + 0x8eab);
        flag = cm_fn_006d58f0((volatile u8 *)context);
        a = B(context + 0x8ea6);
        b = B(context + 0x8eb9);
        cm_fn_006cb960(0, (s8)b, (s8)a);
        a = B(context + 0x8ea6);
        b = B(context + 0x8eb9);
        cm_fn_006cb960(1, (s8)b, (s8)a);
    } else {
        phase = (u8)context; /* Original untouched argument-slot low byte. */
        flag = 0;
    }
    count = (s16)W(self + 8);
    if (count == (s16)W(self + 6)) return 0;
    if (!count) index = 11;
    else index = (u32)(s32)(s8)(u8)(B(self + 8) - 1u);
    rng_eax = index;
    rng_edx = index * 7u;
    src = self + index * 14u + 0x30u;
    current = (s16)W(src);
    if (incoming == 0x1f6f) {
        a = phase;
        b = B(dst + 6);
        if (a == b) return 0x21bb;
    } else if (incoming == 0x1f92 &&
               (current == 0x1f6c || current == 0x1f6e)) return (s16)0xe064;
    if (current == 0x1f7d) {
        a = B(dst + 6);
        b = B(src + 6);
        if (a != b && incoming == 0x1f46) return (s16)0xde63;
    }
    if ((incoming > 0x1f86 && incoming < 0x1fbe) ||
        (incoming == 0x1f5f && flag)) {
        if (current == 0x1f7e) {
            W(src) = 0x219c;
            value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 2);
            W(src + 2) = (u16)value;
            return 0;
        }
    }
    if ((incoming > 0x1f86 && incoming < 0x1fbe) && current == 0x1f40) {
        W(src) = 0x219b;
        return 0;
    }
    if (current == 0x1f44) {
        if (incoming != 0x1f43) goto range_fallback;
        result = 0x213f; goto copy_all;
    }
    if (current == 0x1f69) {
        if (incoming != 0x1f9d) goto range_fallback;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 3);
        if (value) return 0;
        return 0x2198;
    }
    if (current >= 0x2021 && current <= 0x20d0 &&
        (incoming == 0x1f46 || incoming == 0x213e ||
         incoming == 0x1f60 || incoming == 0x1f5f)) {
        a = B(src + 6);
        b = B(dst + 6);
        if (b != a && ((flag == 255 && a == 1) || (flag == 1 && !a)))
            return (s16)0xde88;
    }
    if (current == 0x1f6f) {
        if (incoming != 0x1f46) goto range_fallback;
        a = B(dst + 6);
        b = B(src + 6);
        if (a == b || !flag) return 0;
        b = B(dst + 5);
        B(dst + 8) = a;
        B(dst + 7) = b;
        a = B(src + 5); B(dst + 5) = a;
        a = B(src + 6); B(dst + 6) = a;
        return 0x2177;
    }
    if (current == 0x2111 || current == 0x210f ||
        current == 0x1fe6 || current == 0x1fe7) {
        if (incoming != 0x1f46) goto range_fallback;
        a = B(src + 5); b = B(dst + 5);
        if (a != b) return 0;
        c = B(src + 6); d = B(dst + 6);
        if (c != d) return 0;
        if (current == 0x2111) result = 0x2140;
        else if (current == 0x210f) result = 0x2141;
        else result = 0x2142;
        goto copy_cached_first;
    }
    if ((current == 0x213e || current == 0x1f71) && incoming == 0x1f46) {
        a = B(src + 5); b = B(dst + 5);
        rng_eax = (rng_eax & 0xffffff00u) | a;
        if (a == b) {
            c = B(src + 6); d = B(dst + 6);
            rng_edx = (rng_edx & 0xffffff00u) | d;
            if (c == d) { result = 0x1f46; goto copy_cached_first; }
        }
    }
    if (current == 0x213e) {
        if (incoming != 0x1f41) goto chance_five;
        result = current; goto copy_all;
    }
    if (current == 0x1f41) {
        if (incoming != 0x1f46) goto range_fallback;
        result = 0x1f46; goto copy_all;
    }
    if (current == 0x2113) {
        if (incoming < 0x1f86 || incoming > 0x1fbe) goto dead_pair;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 3);
        if (value) goto dead_pair;
        a = B(src + 5); b = B(dst + 5);
        if (a != b) goto dead_pair;
        c = B(src + 6); d = B(dst + 6);
        if (c != d) goto dead_pair;
        result = incoming; goto copy_cached_first;
    }
    if (current == 0x1f47) {
        if (incoming == 0x1f44 || incoming == 0x1f43) {
            a = B(src + 5); b = B(dst + 5);
            if (a != b) return 0;
            c = B(src + 6); d = B(dst + 6);
            if (c != d) return 0;
            result = incoming; goto copy_cached_first;
        }
        if (incoming != 0x1f45 && incoming != 0x20de) goto range_fallback;
        a = B(src + 5); b = B(dst + 5);
        if (a != b) goto range_fallback;
        c = B(src + 6); d = B(dst + 6);
        if (c != d) goto range_fallback;
        result = incoming; goto copy_cached_first;
    }
    if (current == 0x1f4d) {
        if (incoming != 0x1f4a) goto range_fallback;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 2);
        if (!value) return 0;
        a = B(src + 5); b = B(dst + 5);
        if (a != b) return 0;
        c = B(src + 6); d = B(dst + 6);
        if (c != d) goto compare_wildcard;
        c = B(src + 7); d = B(dst + 7);
        if (c != d) goto compare_wildcard;
        c = B(src + 8); d = B(dst + 8);
        if (c == d) goto matched_wildcard;
compare_wildcard:
        if (a != b) return 0;
        c = B(src + 6); d = B(dst + 6);
        if (c != d) return 0;
        c = B(src + 7);
        if (c != 255) { d = B(dst + 7); if (d != 255) return 0; }
matched_wildcard:
        result = 0x1f4a; goto copy_cached_first;
    }
    if (current == 0x1f43) {
        if (incoming != 0x1f9d) goto range_fallback;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 2);
        if (!value) return 0;
        result = current; goto copy_all;
    }
    if ((current == 0x1f62 || current == 0x1f63 || current == 0x2150) &&
        incoming == 0x1f46) {
        a = B(src + 5); b = B(dst + 5);
        if (a == b) {
            c = B(src + 6); d = B(dst + 6);
            if (c == d) { result = current; goto copy_cached_first; }
        }
    }
    if (current == 0x1f5d) {
        if (incoming != 0x1f6f) goto range_fallback;
        result = 0x2143; goto copy_all;
    }
    if (current >= 0x2021 && current <= 0x20d0 && incoming == 0x1f46) {
        a = B(src + 7); b = B(dst + 5);
        if (a == b) {
            a = B(src + 8); b = B(dst + 6);
            if (a == b) { result = current; goto copy_all; }
        }
    }
    if (current == 0x1f60) {
        if (incoming != 0x1f6f) goto range_fallback;
        result = incoming; goto copy_all;
    }
    if (current == 0x1f46) {
        if (incoming != 0x1f70) goto range_fallback;
        result = incoming; goto copy_all;
    }
    if (current == 0x1f84 || current == 0x1f85) {
        if (incoming != 0x1f75) goto range_fallback;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 3);
        if (!value) return 0;
        result = current == 0x1f84 ? 0x2144 : 0x2145; goto copy_all;
    }
    if (current == 0x1f80) {
        if (incoming != 0x1f46) goto range_fallback;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 2);
        if (!value) { result = current; goto copy_all; }
        a = B(dst + 6); b = B(src + 6);
        if (b == a) { result = current; goto copy_all; }
        b = B(dst + 5);
        B(dst + 8) = a; B(dst + 7) = b;
        a = B(src + 5); B(dst + 5) = a;
        a = B(src + 6); B(dst + 6) = a;
        value = D(src + 9); D(dst + 9) = value;
        return 0x2146;
    }
    if (current == 0x1f5e) {
chance_five:
        if (incoming != 0x1f46) goto after_chance_five;
        value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 5);
        if (!value) goto after_chance_five;
        c = B(dst + 6); a = B(src + 6);
        if (a != c) goto after_chance_five;
        a = B(dst + 5); b = B(src + 5);
        if (b == a) goto after_chance_five;
        B(dst + 7) = a; B(dst + 8) = c;
        a = B(src + 5); B(dst + 5) = a;
        a = B(src + 6); B(dst + 6) = a;
        value = D(src + 9); D(dst + 9) = value;
        return 0x2147;
    }
after_chance_five:
    if (current == 0x2148 || current == 0x214a || current == 0x214c) {
        if (incoming != 0x1f60 && incoming != 0x1f5f) goto range_fallback;
        result = (s16)(current + 1); goto copy_all;
    }
    if (current >= 0x2021 && current <= 0x20d0) {
        if (incoming >= 0x2021 && incoming <= 0x20d0) {
            value = D(dst + 9);
            if ((s32)value >= 1 && (s32)value <= 20) {
                value = D(src + 9);
                if ((s32)value >= 1 && (s32)value <= 20) {
                    a = B(dst + 5); b = B(src + 7);
                    if (a == b) {
                        a = B(src + 5); b = B(dst + 7);
                        if (b == a) { result = 0x214e; goto copy_cached_first; }
                    }
                }
            }
        }
        if (incoming == 0x1f47) {
            value = D(src + 9);
            if ((s32)value >= 1 && (s32)value <= 20) {
                bound = 22u - value;
                value = (u32)cm_fn_0090cfc0(value, bound, bound);
                if (value) {
                    a = B(src + 7); b = B(dst + 5);
                    if (a == b) {
                        a = B(src + 8); b = B(dst + 6);
                        if (a == b) { result = current; goto copy_all; }
                    }
                }
            }
        }
    }
    if (current == 0x214f) {
        if (incoming != 0x1f92) goto range_fallback;
        result = current; goto copy_all;
    }
    if ((current == 0x213e || current == 0x1f71 || current == 0x1f72) &&
        incoming == 0x1f75) { result = 0x2151; goto copy_all; }
    if (current == 0x1f40) {
        if (incoming != 0x1f9d) goto range_fallback;
        result = incoming; goto copy_all;
    }
    if (current == 0x2021) {
        if (incoming == 0x1f62 || incoming == 0x1f63 || incoming == 0x2150)
            return (s16)0xdead;
        goto word_pair;
    }
    if (current == 0x1f55) {
        if (incoming == 0x1f40) return (s16)0xdeae;
        goto range_fallback;
    }
    if (current == 0x2113) {
dead_pair:
        if (incoming == 0x1f62 || incoming == 0x2150) return (s16)0xdead;
        goto range_fallback;
    }
    if (current >= 0x2021 && current <= 0x20d0) {
word_pair:
        if (incoming != 0x1f70) goto range_fallback;
        a = B(src + 6); b = B(dst + 6);
        if (a == b) return (s16)0xdea9;
        goto late_pairs;
    }
range_fallback:
    if (incoming >= 0x2076 && incoming <= 0x20d0 && current == 0x2113) {
        a = B(src + 5); b = B(dst + 5);
        if (a != b) return 0;
        a = B(src + 6); b = B(dst + 6);
        if (a != b) return 0;
        return (s16)0xdea7;
    }
late_pairs:
    if ((current == 0x215b || current == 0x215c || current == 0x215a ||
         current == 0x215e || current == 0x215f || current == 0x215d) &&
        incoming == 0x1f70) return (s16)0xdea9;
    if (current == 0x1f70) {
        if (incoming != 0x1f76) return 0;
        a = B(src + 6); b = B(dst + 6);
        if (a == b) return 0;
        result = 0x2165; goto copy_all;
    }
    if (current == 0x2150) {
        if (incoming != 0x1f75) return 0;
        result = 0x2151; goto copy_all;
    }
    if (current == 0x1f4b || current == 0x1f4c || current == 0x1f4e ||
        current == 0x1f4f || current == 0x1f50 || current == 0x1f51 ||
        current == 0x1f52 || current == 0x1f53) {
        if (incoming == 0x1f4b || incoming == 0x1f4c || incoming == 0x1f4e ||
            incoming == 0x1f4f || incoming == 0x1f50 || incoming == 0x1f51 ||
            incoming == 0x1f52 || incoming == 0x1f53) {
            value = (u32)cm_fn_0090cfc0(rng_eax, rng_edx, 2);
            if (value) return (s16)0xde97;
        }
    }
    if (current == 0x216e) {
        if (incoming != 0x1f44) return 0;
        a = B(src + 5); b = B(dst + 5);
        if (a != b) return 0;
        a = B(src + 6); b = B(dst + 6);
        if (a != b) return 0;
        return current;
    }
    if ((current == 0x1f80 || current == 0x2131 || current == 0x2130) &&
        incoming == 0x1f46) { result = current; goto copy_all; }
    if (current == 0x1f73 || (current >= 0x20b9 && current <= 0x20c4) ||
        (current >= 0x20c5 && current == 0x20d0) || current == 0x1f80) {
        if (incoming == 0x1f46 || incoming == 0x1f47) {
            result = current; goto copy_all;
        }
    }
    return 0;
copy_all:
    a = B(src + 5);
copy_cached_first:
    B(dst + 5) = a;
    a = B(src + 6); B(dst + 6) = a;
    a = B(src + 7); B(dst + 7) = a;
    a = B(src + 8); B(dst + 8) = a;
    value = D(src + 9); D(dst + 9) = value;
    return result;
}
