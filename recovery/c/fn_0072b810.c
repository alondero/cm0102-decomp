/* Preferred-image VA 0072b810/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00547840(u32, u32);

/* THIS2/RET8/fullEAX. Preserve initial ECX saved in the score local;
   EBX==0 skips that comparison until a selected score has been written.
   The original second formal is the outer counter, independent of cached
   reference. The 100th draw still loads the global player base before exit.
   RNG and relationship calls bind to original code, including x87 behavior. */
u32 CM_THISCALL cm_fn_0072b810(u32 self, volatile u32 club_argument,
                              volatile u32 reference_argument)
{
    u32 reference = reference_argument, best = 0, best_score = self;
    u32 tries, value, base, candidate, owner, nation, scale, score, threshold;
    if (reference == 0 && club_argument == 0) return 0;
    reference_argument = 150;
    do {
        tries = 0;
        for (;;) {
            value = D(0x00ae2364u);
            value = cm_fn_0090cfc0(value);
            ++tries;
            base = D(0x00ae23bcu);
            candidate = base + value * 110u;
            if ((s32)tries >= 100) goto next;
            if ((s8)B(candidate + 0x18u) < 37) continue;
            if (B(candidate + 0x22u) < 50) continue;
            if (D(candidate + 0x69u) == 0) continue;
            if (D(candidate + 0x61u) != 0 && D(candidate + 0x39u) != 0) continue;
            owner = club_argument;
            if (D(owner + 0xcfu) == candidate) continue;
            break;
        }
        nation = D(candidate + 0x1au);
        scale = B(candidate + 0x22u);
        scale *= (u32)(s32)(s16)W(nation + 0x8eu);
        score = cm_fn_0090cfc0(scale);
        if (reference == 0) {
            owner = club_argument;
            nation = D(candidate + 0x1au);
            if (nation != D(owner + 0x53u)) goto next;
            score += score;
        } else {
            nation = D(candidate + 0x1au);
            value = D(reference + 0x1au);
            if (nation == value) {
                score += score;
            } else {
                threshold = cm_fn_0090cfc0(100) + 0x1964u;
                nation = D(candidate + 0x1au);
                if ((s32)(s16)W(nation + 0x8eu) < (s32)threshold) goto next;
                value = D(candidate + 0x69u);
                if ((s16)W(value + 0xcu) < 6000) goto next;
                if (cm_fn_00547840(reference, candidate) != 0) goto next;
            }
        }
        if (best == 0 || (s32)score > (s32)best_score) {
            best_score = score;
            best = candidate;
        }
next:
        --reference_argument;
    } while (reference_argument != 0);
    return best;
}
