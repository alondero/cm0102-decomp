/* Independent source review complete; actual compiler/differential/control validation pending. */
/* Parent normalization: use frozen project cmre.h below; remove copied header guard/types. */

#include "cmre.h"
typedef u32 uintptr_t; /* Fixed32-bit C lane; avoid unsupported external-header dependency. */

extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1,
                                    u32 arg2,
                                    u32 arg3,
                                    void *arg4,
                                    void *arg5);
extern void CM_CDECL cm_fn_005e8290(const char *param_1,
                                     const char *param_2,
                                     int param_3);

typedef u16 cm_u16_alias __attribute__((may_alias, aligned(1)));
typedef u32 cm_u32_alias __attribute__((may_alias, aligned(1)));

#define CM_ADDR(va) ((u32)(uintptr_t)&CM_GLOBAL(u8, (va)))

#define CM_FMT_9870E8 CM_ADDR(0x009870e8)
#define CM_FMT_9870E0 CM_ADDR(0x009870e0)
#define CM_FMT_98806C CM_ADDR(0x0098806c)
#define CM_FMT_98E8D4 CM_ADDR(0x0098e8d4)
#define CM_FMT_98FE94 CM_ADDR(0x0098fe94)
#define CM_FMT_99CC58 CM_ADDR(0x0099cc58)
#define CM_FMT_99CCC8 CM_ADDR(0x0099ccc8)
#define CM_FMT_A78310 CM_ADDR(0x00a78310)
#define CM_FMT_A7849C CM_ADDR(0x00a7849c)
#define CM_FMT_A784A8 CM_ADDR(0x00a784a8)
#define CM_FMT_A784B4 CM_ADDR(0x00a784b4)
#define CM_FMT_A784C0 CM_ADDR(0x00a784c0)
#define CM_FMT_A784D0 CM_ADDR(0x00a784d0)

#define CM_READ_BYTE(base, offset) \
    (*(volatile u8 *)(const void *)((const u8 *)(base) + (offset)))

#define CM_READ_WORD(base, offset) \
    (*(volatile cm_u16_alias *)(const void *) \
        ((const u8 *)(base) + (offset)))

static void
cm_fn_007c2750_original_copy(volatile u8 *start, void *destination)
{
    volatile u8 *scan;
    volatile u8 *dest;
    u32 count;
    u32 length;
    u32 dwords;
    u32 trailing;
    uintptr_t scan_address;
    uintptr_t source_address;
    u32 i;

    scan = start;
    count = 0xffffffffu;

    for (;;) {
        u8 value;

        count = count - 1u;
        value = *scan;
        scan++;

        if (value == 0u) {
            break;
        }
        if (count == 0u) {
            break;
        }
    }

    /*
     * If N non-NUL bytes precede the NUL, SCASB performed N+1 iterations:
     *
     *   final ECX = FFFFFFFFh - (N+1)
     *   NOT ECX   = N+1
     *   final EDI = start + (N+1) - (N+1) = start
     *
     * The copied extent therefore includes the terminating NUL.
     */
    length = ~count;
    scan_address = (uintptr_t)scan;
    source_address = scan_address - (uintptr_t)length;
    dest = (volatile u8 *)destination;

    dwords = length >> 2;
    for (i = 0u; i < dwords; i++) {
        u32 value;

        value = *(volatile cm_u32_alias *)
            (const void *)(source_address + (uintptr_t)i * 4u);
        *(volatile cm_u32_alias *)
            (void *)(dest + (uintptr_t)i * 4u) = value;
    }

    trailing = length & 3u;
    for (i = 0u; i < trailing; i++) {
        u8 value;

        value = *(const volatile u8 *)
            (const void *)(source_address + (uintptr_t)dwords * 4u + i);
        dest[(uintptr_t)dwords * 4u + i] = value;
    }
}

static float
cm_fn_007c2750_read_multiplier(void)
{
    /* Original FMUL reads one volatile single-precision value. */
    return CM_GLOBAL(volatile float, 0x00968960);
}

u32 CM_STDCALL
cm_fn_007c2750(u8 *param_1,
                char param_2,
                char *param_3,
                int param_4,
                int param_5,
                int param_6,
                int param_7)
{
    /*
     * L is the stable local-allocation base after SUB ESP,20c.
     * B is the body ESP after the three saved-register pushes: B=L-0Ch.
     *
     * Consequently:
     *   [B+10h]   = [L+04h]  = scratch+04
     *   [B+14h]   = [L+08h]  = scratch+08
     *   [B+18h]   = [L+0Ch]  = scratch+0Ch
     *   [B+118h]  = [L+10Ch] = scratch+10Ch
     *
     * The two adjacent destination buffers occupy scratch+0Ch through
     * scratch+20Bh and are not split into separate arrays.
     */
    volatile u8 scratch[0x20c];
    volatile u8 *buffer_a;
    volatile u8 *buffer_b;
    volatile u8 *data;
    u32 buffer_a_address;
    u32 buffer_b_address;
    s32 signed_index;
    u32 table_index;
    u32 jump_target;

    buffer_a = &scratch[0x0c];
    buffer_b = &scratch[0x10c];
    buffer_a_address = (u32)(uintptr_t)buffer_a;
    buffer_b_address = (u32)(uintptr_t)buffer_b;

    if (param_3 == (char *)0 || param_4 < 0x33) {
        char *returned_name;

        /* Physical CALL 007c2fbb: CM_CDECL 00944cff. */
        (void)cm_fn_00944cff(CM_FMT_A78310,
                             0u,
                             0u,
                             (void *)(uintptr_t)buffer_a,
                             (void *)(uintptr_t)buffer_b);

        /*
         * Physical CALL 007c2fd5: CM_CDECL 0090D130.
         * 0090D130 consumes no register or stack argument.
         */
        returned_name = cm_fn_0090d130();

        /* Physical CALL 007c2fe5: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f(CM_ADDR(0x00ae24d0),
                             CM_FMT_9870E8,
                             (u32)(uintptr_t)returned_name,
                             buffer_a_address,
                             buffer_b_address,
                             0x000007ffu);

        /* Physical CALL 007c2ff6: CM_CDECL 005E8290. */
        cm_fn_005e8290((const char *)(uintptr_t)CM_ADDR(0x009870e0),
                      (const char *)(uintptr_t)CM_ADDR(0x00ae24d0),
                      0);

        CM_GLOBAL(u32, 0x00b67a34) = 0u;
        return 0u;
    }

    if ((u8)param_2 == 0x11u) {
        cm_fn_007c2750_original_copy(
            &CM_GLOBAL(u8, 0x0099cc4c), param_3);
    } else {
        cm_fn_007c2750_original_copy(
            &CM_GLOBAL(u8, 0x0098ba0c), param_3);
    }

    if (param_1 == (u8 *)0) {
        char *returned_name;

        /* Physical CALL 007c27c0: CM_CDECL 00944cff. */
        (void)cm_fn_00944cff(CM_FMT_A78310,
                             0u,
                             0u,
                             (void *)(uintptr_t)buffer_b,
                             (void *)(uintptr_t)buffer_a);

        /* Physical CALL 007c2f6c: CM_CDECL 0090D130. */
        returned_name = cm_fn_0090d130();

        /* Physical CALL 007c2f7c: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f(CM_ADDR(0x00ae24d0),
                             CM_FMT_9870E8,
                             (u32)(uintptr_t)returned_name,
                             buffer_b_address,
                             buffer_a_address,
                             0x00000811u);

        /* Physical CALL 007c2f8c: CM_CDECL 005E8290. */
        cm_fn_005e8290((const char *)(uintptr_t)CM_ADDR(0x009870e0),
                      (const char *)(uintptr_t)CM_ADDR(0x00ae24d0),
                      0);

        CM_GLOBAL(u32, 0x00b67a34) = 0u;
        return 0u;
    }

    data = (volatile u8 *)param_1;

    /*
     * Original dispatch:
     *   MOVSX ECX,DL
     *   DEC ECX
     *   CMP ECX,10h
     *   JA default
     *   JMP DWORD PTR [ECX*4+007c3014h]
     *
     * ECX is not a receiver.  The guarded table read is explicit here.
     */
    signed_index = (s32)(s8)(u8)param_2 - 1;
    if ((u32)signed_index > 0x10u) {
        goto cm_default;
    }

    table_index = (u32)signed_index;
    jump_target = *(volatile u32 *)(uintptr_t)
        (0x007c3014u + table_index * 4u);

    switch (jump_target) {
    case 0x007c27f3u:
        goto cm_case_01;
    case 0x007c2845u:
        goto cm_case_02;
    case 0x007c288cu:
        goto cm_case_03;
    case 0x007c28d3u:
        goto cm_case_04;
    case 0x007c2984u:
        goto cm_case_05;
    case 0x007c29e3u:
        goto cm_case_06;
    case 0x007c2a42u:
        goto cm_case_07;
    case 0x007c2aa1u:
        goto cm_case_08;
    case 0x007c2b00u:
        goto cm_case_09;
    case 0x007c2b5fu:
        goto cm_case_10;
    case 0x007c2bbeu:
        goto cm_case_11;
    case 0x007c2c1du:
        goto cm_case_12;
    case 0x007c2d30u:
        goto cm_case_13;
    case 0x007c2dc5u:
        goto cm_case_14;
    case 0x007c2e29u:
        goto cm_case_15;
    case 0x007c2eadu:
        goto cm_case_16;
    case 0x007c2c7cu:
        goto cm_case_17;
    default:
        goto cm_default;
    }

cm_case_01: {
    u8 second;

    second = CM_READ_BYTE(data, 1);
    if (second != 0u && param_5 != 0) {
        u32 difference;

        difference = (u32)CM_READ_BYTE(data, 0) - (u32)second;

        /* Physical CALL 007c2815: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_99CC58,
                             difference,
                             (u32)second);
        return 1u;
    } else {
        u8 value;

        value = CM_READ_BYTE(data, 0);
        if (value == 0u && param_7 == 0) {
            return 1u;
        }

        /* Physical CALL 007c2c66: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_98806C,
                             (u32)value);
        return 1u;
    }
}

cm_case_02: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2845. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 2);
        if (param_6 != 0) {
            /* Physical CALL 007c286c: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
            return 1u;
        }

        /* Physical CALL 007c2c66: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_98806C,
                             (u32)value);
        return 1u;
    }
    return 1u;
}

cm_case_03: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C288C. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 3);
        if (param_6 != 0) {
            /* Physical CALL 007c28b3: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
            return 1u;
        }

        /* Physical CALL 007c2c66: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_98806C,
                             (u32)value);
        return 1u;
    }
    return 1u;
}

cm_case_04: {
    u8 fourth;
    u8 captured_count;
    u8 fifth;

    fourth = CM_READ_BYTE(data, 4); /* Original 007C28D3. */
    if (fourth != 0u && param_5 != 0) {
        if (param_6 != 0) {
            captured_count = CM_READ_BYTE(data, 0); /* 007C28EC, before fifth. */
            fifth = CM_READ_BYTE(data, 5);          /* 007C28F1. */
            /* Physical CALL 007c28fc: logical values fourth,fifth,count. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784C0,
                                 (u32)fourth,
                                 (u32)fifth,
                                 (u32)captured_count);
        } else {
            fifth = CM_READ_BYTE(data, 5); /* Original 007C291A. */
            /* Physical CALL 007c2925: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_99CC58,
                                 (u32)fourth,
                                 (u32)fifth);
        }
        return 1u;
    }

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C293B. */
    if (captured_count != 0u || param_7 != 0) {
        if (param_6 != 0) {
            /* Physical CALL 007c2962: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)fourth,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2c66: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)fourth);
        }
    }
    return 1u;
}

cm_case_05: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2984. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 6);
        if (param_6 != 0) {
            /* Physical CALL 007c29ab: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c29cd: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_06: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C29E3. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 7);
        if (param_6 != 0) {
            /* Physical CALL 007c2a0a: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2a2c: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_07: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2A42. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 8);
        if (param_6 != 0) {
            /* Physical CALL 007c2a69: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2a8b: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_08: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2AA1. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 9);
        if (param_6 != 0) {
            /* Physical CALL 007c2ac8: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2aea: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_09: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2B00. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 10);
        if (param_6 != 0) {
            /* Physical CALL 007c2b27: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2b49: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_10: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2B5F. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 11);
        if (param_6 != 0) {
            /* Physical CALL 007c2b86: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2ba8: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_11: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2BBE. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 12);
        if (param_6 != 0) {
            /* Physical CALL 007c2be5: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2c07: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_12: {
    u8 captured_count;

    captured_count = CM_READ_BYTE(data, 0); /* Original 007C2C1D. */
    if (captured_count != 0u || param_7 != 0) {
        u8 value;

        value = CM_READ_BYTE(data, 13);
        if (param_6 != 0) {
            /* Physical CALL 007c2c44: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784D0,
                                 (u32)value,
                                 (u32)captured_count);
        } else {
            /* Physical CALL 007c2c66: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98806C,
                                 (u32)value);
        }
    }
    return 1u;
}

cm_case_13: {
    s16 first;
    s16 numerator_word;
    u32 numerator_bits;
    u32 divisor_bits;
    long double value;
    u32 converted;
    u8 captured_count;

    first = (s16)CM_READ_WORD(data, 0x10);
    if (first <= 0) {
        return 1u;
    }

    if (param_6 != 0) {
        numerator_word = (s16)CM_READ_WORD(data, 0x12);
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)numerator_word;
        numerator_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_bits;
        captured_count = CM_READ_BYTE(data, 0); /* Original 007C2D50, before FMUL. */
        value = value * (long double)cm_fn_007c2750_read_multiplier(); /* FMUL 007C2D55; cast 007C2D64. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)first;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = value / (long double)(s32)divisor_bits;
        /* ADR0006 compiler cast at original __ftol site 007C2D64. */
        converted = (u32)(long long)value;

        /* Physical CALL 007C2D70: converted value then captured count. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_A784A8,
                             converted,
                             (u32)captured_count);
    } else {
        numerator_word = (s16)CM_READ_WORD(data, 0x12);
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)numerator_word;
        numerator_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_bits;
        value = value * (long double)cm_fn_007c2750_read_multiplier(); /* FMUL 007C2D95; cast 007C2DA3. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)first;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = value / (long double)(s32)divisor_bits;
        /* ADR0006 compiler cast at original __ftol site 007C2DA3. */
        converted = (u32)(long long)value;

        /* Physical CALL 007C2DAF: converted value only. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_98E8D4,
                             converted);
    }
    return 1u;
}

cm_case_14: {
    u8 divisor;
    s16 numerator_word;
    u32 divisor_bits;
    long double value;
    volatile double spilled;

    divisor = CM_READ_BYTE(data, 0);
    if (divisor == 0u) {
        return 1u;
    }

    numerator_word = (s16)CM_READ_WORD(data, 0x14);

    if (param_6 != 0) {
        /* MOV [B+14h],ECX; PUSH ECX; FIDIV [ESP+18h]. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x08] =
            (u32)divisor;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x08];
        value = (long double)(s32)numerator_word /
                 (long double)(s32)divisor_bits;
        spilled = (double)value;

        /* Physical CALL 007c2e5b: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_A7849C,
                             spilled,
                             (u32)divisor);
    } else {
        /* MOV [B+10h],ECX; SUB ESP,8; FIDIV [ESP+18h]. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)divisor;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_word /
                 (long double)(s32)divisor_bits;
        spilled = (double)value;

        /* Physical CALL 007c2e13: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_99CCC8,
                             spilled);
    }
    return 1u;
}

cm_case_15: {
    u8 divisor;
    s16 numerator_word;
    u32 divisor_bits;
    long double value;
    volatile double spilled;

    divisor = CM_READ_BYTE(data, 0);
    if (divisor == 0u) {
        return 1u;
    }

    numerator_word = (s16)CM_READ_WORD(data, 0x16);

    if (param_6 != 0) {
        /* MOV [B+14h],ECX; PUSH ECX; FIDIV [ESP+18h]. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x08] =
            (u32)divisor;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x08];
        value = (long double)(s32)numerator_word /
                 (long double)(s32)divisor_bits;
        spilled = (double)value;

        /* Physical CALL 007c2e5b: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_A7849C,
                             spilled,
                             (u32)divisor);
    } else {
        /* MOV [B+10h],ECX; SUB ESP,8; FIDIV [ESP+18h]. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)divisor;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_word /
                 (long double)(s32)divisor_bits;
        spilled = (double)value;

        /* Physical CALL 007c2e97: CM_CDECL 00944c9f. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_99CCC8,
                             spilled);
    }
    return 1u;
}

cm_case_16: {
    s16 first;
    s16 numerator_word;
    u32 numerator_bits;
    u32 divisor_bits;
    long double value;
    u32 converted;
    u8 captured_count;

    first = (s16)CM_READ_WORD(data, 0x18);
    if (first <= 0) {
        return 1u;
    }

    if (param_6 != 0) {
        numerator_word = (s16)CM_READ_WORD(data, 0x1a);
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)numerator_word;
        numerator_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_bits;
        captured_count = CM_READ_BYTE(data, 0); /* Original 007C2EC9, before FMUL. */
        value = value * (long double)cm_fn_007c2750_read_multiplier(); /* FMUL 007C2ECE; cast 007C2EDD. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)first;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = value / (long double)(s32)divisor_bits;
        /* ADR0006 compiler cast at original __ftol site 007C2EDD. */
        converted = (u32)(long long)value;

        /* Physical CALL 007C2EE9: converted value then captured count. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_A784A8,
                             converted,
                             (u32)captured_count);
    } else {
        numerator_word = (s16)CM_READ_WORD(data, 0x1a);
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)numerator_word;
        numerator_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = (long double)(s32)numerator_bits;
        value = value * (long double)cm_fn_007c2750_read_multiplier(); /* FMUL 007C2F0E; cast 007C2F1C. */
        *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
            (u32)(s32)first;
        divisor_bits =
            *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
        value = value / (long double)(s32)divisor_bits;
        /* ADR0006 compiler cast at original __ftol site 007C2F1C. */
        converted = (u32)(long long)value;

        /* Physical CALL 007C2F28: converted value only. */
        (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                             CM_FMT_98E8D4,
                             converted);
    }
    return 1u;
}

cm_case_17: {
    u8 divisor;
    s16 numerator_word;
    u32 divisor_bits;
    long double value;
    volatile double spilled;

    divisor = CM_READ_BYTE(data, 0);
    if (divisor != 0u) {
        numerator_word = (s16)CM_READ_WORD(data, 0x0e);

        if (param_6 != 0) {
            /*
             * MOV [B+14h],ECX
             * PUSH ECX
             * SUB ESP,8
             * FIDIV [ESP+20h]
             *
             * PUSH/SUB leaves ESP=B-0Ch, so [ESP+20h]=B+14h.
             * The divisor is the stored DWORD at L+08h.
             */
            *(volatile cm_u32_alias *)(void *)&scratch[0x08] =
                (u32)divisor;
            divisor_bits =
                *(volatile cm_u32_alias *)(const void *)&scratch[0x08];
            value = (long double)(s32)numerator_word /
                     (long double)(s32)divisor_bits;
            spilled = (double)value;

            /* Physical CALL 007c2cae: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_A784B4,
                                 spilled,
                                 (u32)divisor);
        } else {
            /*
             * MOV [B+10h],ECX
             * SUB ESP,8
             * FIDIV [ESP+18h]
             *
             * SUB leaves ESP=B-08h, so [ESP+18h]=B+10h.
             */
            *(volatile cm_u32_alias *)(void *)&scratch[0x04] =
                (u32)divisor;
            divisor_bits =
                *(volatile cm_u32_alias *)(const void *)&scratch[0x04];
            value = (long double)(s32)numerator_word /
                     (long double)(s32)divisor_bits;
            spilled = (double)value;

            /* Physical CALL 007c2cea: CM_CDECL 00944c9f. */
            (void)cm_fn_00944c9f((u32)(uintptr_t)param_3,
                                 CM_FMT_98FE94,
                                 spilled);
        }
    } else {
        cm_fn_007c2750_original_copy(
            &CM_GLOBAL(u8, 0x0099cc4c), param_3);
    }
    return 1u;
}

cm_default: {
    char *returned_name;

    /* Physical CALL 007c2f52: CM_CDECL 00944cff. */
    (void)cm_fn_00944cff(CM_FMT_A78310,
                         0u,
                         0u,
                         (void *)(uintptr_t)buffer_a,
                         (void *)(uintptr_t)buffer_b);

    /* Physical CALL 007c2f6c: CM_CDECL 0090D130. */
    returned_name = cm_fn_0090d130();

    /* Physical CALL 007c2f7c: CM_CDECL 00944c9f. */
    (void)cm_fn_00944c9f(CM_ADDR(0x00ae24d0),
                         CM_FMT_9870E8,
                         (u32)(uintptr_t)returned_name,
                         buffer_a_address,
                         buffer_b_address,
                         0x0000091bu);

    /* Physical CALL 007c2f8c: CM_CDECL 005E8290. */
    cm_fn_005e8290((const char *)(uintptr_t)CM_ADDR(0x009870e0),
                  (const char *)(uintptr_t)CM_ADDR(0x00ae24d0),
                  0);

    CM_GLOBAL(u32, 0x00b67a34) = 0u;
    return 0u;
}
}
