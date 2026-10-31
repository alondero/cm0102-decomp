#include "cmre.h"

/* Arguments (cdecl, caller pops):
   arg1 flag word (EBP+8), tested bytewise and masked with 0x1f;
   arg2 pointer to a double that the routine reads and rewrites (EBP+0xc);
   arg3 flag word (EBP+0x10), tested bytewise.
   Constants: double at 0x009689a0 is the comparison point, doubles at
   0x00ad9800 and 0x00ad9810 are the two magnitudes. */

extern void CM_CDECL cm_fn_00952f05(u32 arg1);
extern double CM_CDECL cm_fn_0095101e(double arg1, u32 arg2);

typedef union {
    double d;
    u32 w[2];   /* w[0] is the low dword (EBP-0xc), w[1] the high dword (EBP-8) */
} cm_double_bits;

#define CONST_POINT CM_GLOBAL(double, 0x009689a0u)
#define CONST_MAG_A CM_GLOBAL(double, 0x00ad9800u)
#define CONST_MAG_B CM_GLOBAL(double, 0x00ad9810u)

u32 CM_CDECL cm_fn_00952c85(u32 arg1, u32 arg2, u32 arg3)
{
    u32 mask = arg1 & 0x1fu;
    u32 c;
    u32 sticky;
    u32 edx_flag;
    s32 exp_out;
    s32 exp_rel;
    double value;
    double result;
    double *target;
    cm_double_bits local;
    u32 lo;
    u32 hi;
    u32 b6;
    u32 nb;
    u32 n;

    if ((arg1 & 8u) != 0u && (arg3 & 1u) != 0u) {
        cm_fn_00952f05(1u);
        mask &= ~8u;
        goto tail;
    }
    if ((arg1 & 4u) != 0u && (arg3 & 4u) != 0u) {
        cm_fn_00952f05(4u);
        mask &= ~4u;
        goto tail;
    }
    if ((arg1 & 1u) != 0u && (arg3 & 8u) != 0u) {
        cm_fn_00952f05(8u);
        c = arg3 & 0xc00u;
        target = (double *)arg2;
        value = *target;
        if (c == 0u) {
            *target = (value > CONST_POINT) ? CONST_MAG_A : -CONST_MAG_A;
        } else if (c == 0x400u) {
            *target = (value > CONST_POINT) ? CONST_MAG_B : -CONST_MAG_A;
        } else if (c == 0x800u) {
            *target = (value > CONST_POINT) ? CONST_MAG_A : -CONST_MAG_B;
        } else {
            *target = (value > CONST_POINT) ? CONST_MAG_B : -CONST_MAG_B;
        }
        mask &= ~1u;
        goto tail;
    }

    /* Block entered when the 0x02 and 0x10 conditions both hold. */
    if ((arg1 & 2u) == 0u) {
        goto tail;
    }
    if ((arg3 & 0x10u) == 0u) {
        goto tail;
    }
    sticky = ((arg1 & 0x10u) != 0u) ? 1u : 0u;

    target = (double *)arg2;
    value = *target;
    if (!(value < CONST_POINT) && !(value > CONST_POINT)) {
        sticky = 1u;
        goto after_store;
    }

    /* 0x0095101e always stores the exponent through its pointer argument. */
    result = cm_fn_0095101e(value, (u32)&exp_out);
    exp_rel = exp_out - 0x600;
    local.d = result;

    if (exp_rel >= -1074) {
        edx_flag = (result >= CONST_POINT) ? 0u : 1u;
        lo = local.w[0];
        hi = local.w[1];
        /* Rewrite the high byte at EBP-6 to (low nibble | 0x10) and clear EBP-5. */
        b6 = (hi >> 16) & 0xffu;
        nb = (b6 & 0xfu) | 0x10u;
        hi = (hi & 0x0000ffffu) | (nb << 16);
        if (exp_rel < -1021) {
            n = (u32)(-1021) - (u32)exp_rel;
            while (n != 0u) {
                if ((lo & 1u) != 0u) {
                    sticky = 1u;
                }
                lo >>= 1;
                if ((hi & 1u) != 0u) {
                    lo |= 0x80000000u;
                }
                hi >>= 1;
                n--;
            }
        }
        if (edx_flag != 0u) {
            hi ^= 0x80000000u;
        }
        local.w[0] = lo;
        local.w[1] = hi;
    } else {
        sticky = 1u;
        local.d = 0.0;
    }

    *target = local.d;

after_store:
    if (sticky != 0u) {
        cm_fn_00952f05(0x10u);
    }
    mask &= ~2u;

tail:
    if ((arg1 & 0x10u) != 0u && (arg3 & 0x20u) != 0u) {
        cm_fn_00952f05(0x20u);
        mask &= ~0x10u;
    }
    return (mask == 0u) ? 1u : 0u;
}
