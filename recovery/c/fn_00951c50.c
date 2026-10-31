/* WIP after two failed semantic test invocations. The locale path now
 * preserves full EAX/EBX across byte loads and passes the retained DWORD
 * values to 009465fc, matching the original partial-register operations.
 * Case 75 still faults at a different nested-helper stack address; case 387
 * still differs in returned EAX (original 1, C -1). Do not normalize either
 * result without further instruction evidence.
 */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_009465fc(u32 value);

static u8 cm_ascii_fold(u8 value)
{
    return value >= 0x41u && value < 0x5bu ? (u8)(value + 0x20u) : value;
}

s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first,
                              const volatile u8 *second)
{
    if (CM_GLOBAL(volatile u32, 0x00de3048u) == 0) {
        for (;;) {
            u8 right = *second++;
            u8 left = *first++;
            if (left == right) {
                if (left == 0)
                    return 0;
                continue;
            }
            u8 folded_left = cm_ascii_fold(left);
            u8 folded_right = cm_ascii_fold(right);
            if (folded_left == folded_right)
                continue;
            return folded_left < folded_right ? -1 : 1;
        }
    }

    u32 eax = 0xffu;
    u32 ebx = 0;
    for (;;) {
        u8 right = *second++;
        u8 left = *first++;
        eax = (eax & 0xffffff00u) | right;
        ebx = (ebx & 0xffffff00u) | left;
        if ((u8)eax == (u8)ebx) {
            if ((u8)eax == 0)
                return (s32)eax;
            continue;
        }
        ebx = cm_fn_009465fc(ebx);
        eax = cm_fn_009465fc(eax);
        if ((u8)ebx == (u8)eax) {
            if ((u8)eax == 0)
                return (s32)eax;
            continue;
        }
        return (u8)ebx < (u8)eax ? -1 : 1;
    }
}
