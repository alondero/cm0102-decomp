#include "cmre.h"
/* Preferred-image VA 0x008b4770, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EDX:EAX is observed by caller 008b4626. The selector-512 path
   overwrites only AL and leaves incoming EDX intact. Fastcall captures that
   register input while retaining the two original stack words and RET 8.
   00544380 reads only BYTE [ESP+8]; discarded upper bits of its pushed
   arguments do not enter its x87 calculation or tail-jumped conversion. */
extern u8 CM_CDECL cm_fn_00544380(u32, u8);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern unsigned long long CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static unsigned long long pair(u32 low, u32 high)
{
    return ((unsigned long long)high << 32) | low;
}

unsigned long long CM_FASTCALL cm_fn_008b4770(
    u32 incoming_ecx, u32 incoming_edx, u32 player, u32 kind)
{
    volatile u8 scratch[0x200];
    s32 selector = (s16)kind;
    u32 profile = D(player + 0x61u);
    u32 index, mapped, target, text, high;
    s32 first, second, third, fourth, fifth, sum;
    long long product;
    unsigned long long returned;
    (void)incoming_ecx;
    if (selector <= 32) {
        if (selector == 32) goto five_a;
        index = (u32)selector - 1u;
        if (index > 15u) goto failure;
        mapped = B(0x008b499cu + index);
        target = D(0x008b498cu + mapped * 4u);
        switch (target) {
        case 0x008b47b0: goto four_a;
        case 0x008b47f0: goto four_b;
        case 0x008b4839: goto five_a;
        default: /* The remaining pinned table target is 008b48ff. */
            goto failure;
        }
    }
    if (selector == 512)
        return pair(0x200u | B(player + 0x56u), incoming_edx);
    if (selector < 512) {
        if (selector == 64) goto five_b;
        if (selector == 128) goto two_a;
        goto failure;
    }
    if (selector == 2048) goto two_b;
    goto failure;

four_a:
    first = (s8)cm_fn_00544380(profile, B(profile + 0x39u));
    second = (s8)cm_fn_00544380(profile, B(profile + 0x2au));
    third = (s8)B(player + 0x56u);
    fourth = (s8)B(profile + 0x35u);
    sum = first + second + third + fourth;
    return pair((u32)(sum / 4), sum < 0 ? 3u : 0u);

four_b:
    first = (s8)cm_fn_00544380(profile, B(profile + 0x3eu));
    second = (s8)cm_fn_00544380(profile, B(profile + 0x39u));
    third = (s8)cm_fn_00544380(profile, B(profile + 0x32u));
    fourth = (s8)B(player + 0x56u);
    sum = first + second + third + fourth;
    return pair((u32)(sum / 4), sum < 0 ? 3u : 0u);

five_a:
    first = (s8)cm_fn_00544380(profile, B(profile + 0x37u));
    second = (s8)cm_fn_00544380(profile, B(profile + 0x32u));
    third = (s8)B(player + 0x56u);
    fourth = (s8)B(profile + 0x40u);
    fifth = (s8)B(profile + 0x3fu);
    sum = first + second + third + fourth + fifth;
    goto five_result;

five_b:
    first = (s8)cm_fn_00544380(profile, B(profile + 0x33u));
    second = (s8)cm_fn_00544380(profile, B(profile + 0x27u));
    fourth = (s8)B(profile + 0x40u);
    third = (s8)B(player + 0x56u);
    fifth = (s8)B(profile + 0x36u);
    sum = first + second + third + fourth + fifth;
five_result:
    product = (long long)sum * 0x66666667LL;
    high = (u32)((unsigned long long)product >> 32);
    return pair((u32)(sum / 5), high);

two_a:
    first = (s8)B(player + 0x56u);
    second = (s8)B(profile + 0x30u);
    sum = first + second;
    return pair((u32)(sum / 2), sum < 0 ? 0xffffffffu : 0u);

two_b:
    first = (s8)B(player + 0x56u);
    second = (s8)B(profile + 0x3bu);
    sum = first + second;
    return pair((u32)(sum / 2), sum < 0 ? 0xffffffffu : 0u);

failure:
    cm_fn_00944cff(0x00a8ccd0, 0, 0,
                   (u32)(scratch + 0x100), (u32)scratch);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   (u32)(scratch + 0x100), (u32)scratch, 0x945);
    returned = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return returned & 0xffffffffffffff00ULL;
}
