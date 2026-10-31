/* WIP: untested draft. Parent reviewed the original call map; final source
   review and official cmre test/accept remain pending. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(void *destination,
                                    const char *format, ...);

void CM_CDECL cm_fn_007bdee0(u32 volatile flags1, u32 volatile flags2,
                             u32 volatile score_flags,
                             u32 volatile age_value,
                             volatile u8 *destination,
                             u32 volatile argument6,
                             u32 volatile argument7)
{
    volatile u8 local[0x11fcu];
    u8 primary_count = 0u;
    u8 *primary_row;
    u8 *secondary_row;
    u32 argument7_cached;

    local[2] = 0u;
    local[3] = 0u;
    if (destination == 0)
        return;

    if ((u16)flags1 == 0u && (u16)flags2 == 0u) {
        *destination = 0u;
        return;
    }

    if ((flags1 & 0x100u) != 0u || (flags1 & 0x400u) != 0u) {
        u32 format;
        if ((flags1 & 0x100u) != 0u)
            format = (flags1 & 0x400u) != 0u ? 0x00a7598cu : 0x00a7596cu;
        else
            format = 0x00a75950u;
        (void)cm_fn_0066f4e0((void *)(local + 0x194u),
                             (const char *)(unsigned long)format);
        local[3] = 1u;
    }

    if ((flags1 & 0x200u) != 0u) {
        s8 row = (s8)local[3];
        primary_row = (u8 *)(local + 0x194u + (s32)row * 100);
        (void)cm_fn_0066f4e0(primary_row,
                             (flags1 & 4u) != 0u
                                 ? (const char *)0x00a7591cu
                                 : (const char *)0x00a75900u);
        local[3] = (u8)(local[3] + 1u);
    }

    if ((flags1 & 0x1000u) != 0u) {
        (void)cm_fn_0066f4e0((void *)(local + 4u),
                             (flags1 & 0x20u) != 0u
                                 ? (const char *)0x00a758f4u
                                 : (const char *)0x00a758e8u);
        primary_count = 1u;
    } else if ((flags1 & 0x20u) != 0u) {
        (void)cm_fn_0066f4e0((void *)(local + 4u),
                             (const char *)0x00a758d0u);
        primary_count = 1u;
    }

    if ((score_flags & 2u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a758c8u);
        primary_count++;
    }
    if ((score_flags & 1u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a758bcu);
        primary_count++;
    }
    if ((score_flags & 4u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a758a0u);
        primary_count++;
    }
    if ((score_flags & 8u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a75890u);
        primary_count++;
    }
    if ((score_flags & 0x10u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a75880u);
        primary_count++;
    }
    if ((score_flags & 0x40u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a75874u);
        primary_count++;
    }
    if ((score_flags & 0x80u) != 0u && (s8)primary_count < 2) {
        primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
        (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a7586cu);
        primary_count++;
    }

    {
        u8 secondary_flags = (u8)flags2;
        s8 secondary_age = (s8)(u8)age_value;
        if ((secondary_flags & 2u) != 0u) {
            s16 score = (s16)(u16)score_flags;
            if (score < 0x0cb2 && secondary_age < 0x17) {
            (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                                 (const char *)0x00a75820u);
            local[2] = 1u;
            } else if (score < 0x186a && secondary_age < 0x17) {
            (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                                 (const char *)0x00a757c4u);
            local[2] = 1u;
            } else if (secondary_age > 0x19) {
            (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                                 (const char *)0x00a7579cu);
            local[2] = 1u;
            } else if ((s8)primary_count < 2) {
                primary_row = (u8 *)(local + 4u + (s32)(s8)primary_count * 100);
                (void)cm_fn_0066f4e0(primary_row, (const char *)0x00a75788u);
                primary_count++;
            }
        }
    }

    if (((u8)flags2 & 0x10u) != 0u && local[2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                             (const char *)0x00a75780u);
        local[2] = 1u;
    }
    if ((flags2 & 0x800u) != 0u && local[2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                             (const char *)0x00a75770u);
        local[2] = 1u;
    }
    if (((u8)flags2 & 0x20u) != 0u && local[2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                             (const char *)0x00a75760u);
        local[2] = 1u;
    }
    if (((u8)flags2 & 0x40u) != 0u && (s8)local[2] < 2) {
        s8 row = (s8)local[2];
        secondary_row = (u8 *)(local + 0xccu + (s32)row * 100);
        (void)cm_fn_0066f4e0(secondary_row, (const char *)0x00a75750u);
        local[2] = (u8)(local[2] + 1u);
    }
    if (((u8)flags2 & 4u) != 0u && local[2] == 0u) {
        s8 age = (s8)(u8)age_value;
        (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                             age < 0x15 ? (const char *)0x00a75728u
                                        : (const char *)0x00a7570cu);
        local[2] = 1u;
    }
    if (((u8)flags2 & 8u) != 0u && (s8)local[2] < 2) {
        if (local[2] == 0u) {
            (void)cm_fn_0066f4e0((void *)(local + 0xccu),
                                 (const char *)0x00a756f0u);
        } else {
            s8 row = (s8)local[2];
            secondary_row = (u8 *)(local + 0xccu + (s32)row * 100);
            (void)cm_fn_0066f4e0(secondary_row,
                                 (const char *)0x00a756d4u);
        }
        local[2] = (u8)(local[2] + 1u);
    }

    argument7_cached = argument7;
    if (local[3] == 0u) {
        local[0xa2cu] = 0u;
    } else if (local[3] == 1u) {
        (void)cm_fn_0066f4e0((void *)(local + 0xa2cu),
                             (const char *)0x00a75604u,
                             (void *)(unsigned long)argument7_cached,
                             (void *)(local + 0x194u));
        (void)cm_fn_0066f4e0((void *)(local + 0xe14u),
                             (const char *)0x00a755e0u,
                             (void *)(local + 0x194u));
    } else if (local[3] == 2u) {
        (void)cm_fn_0066f4e0((void *)(local + 0xa2cu),
                             (const char *)0x00a75680u,
                             (void *)(unsigned long)argument7_cached,
                             (void *)(local + 0x194u),
                             (void *)(local + 0x1f8u));
        (void)cm_fn_0066f4e0((void *)(local + 0xe14u),
                             (const char *)0x00a7563cu,
                             (void *)(local + 0x194u),
                             (void *)(local + 0x1f8u));
    }

    if (primary_count == 0u) {
        local[0x644u] = 0u;
    } else if (primary_count == 1u) {
        (void)cm_fn_0066f4e0((void *)(local + 0x644u),
                             (const char *)0x00a75548u,
                             (void *)(local + 4u));
    } else if (primary_count == 2u) {
        (void)cm_fn_0066f4e0((void *)(local + 0x644u),
                             (const char *)0x00a75578u,
                             (void *)(local + 4u),
                             (void *)(local + 0x68u));
    }

    if (local[2] == 0u) {
        local[0x25cu] = 0u;
    } else if (local[2] == 1u) {
        (void)cm_fn_0066f4e0((void *)(local + 0x25cu),
                             (const char *)0x00a754b4u,
                             (void *)(local + 0xccu));
    } else if (local[2] == 2u) {
        (void)cm_fn_0066f4e0((void *)(local + 0x25cu),
                             (const char *)0x00a754e4u,
                             (void *)(local + 0xccu),
                             (void *)(local + 0x130u));
    }

    if (primary_count != 0u) {
        if (local[2] == 0u) {
            if (local[3] != 0u) {
                (void)cm_fn_0066f4e0((void *)destination,
                                     (const char *)0x00a753d0u,
                                     0x64u, 0x64u, 0x64u, argument6,
                                     argument7_cached, (void *)(local + 0x644u),
                                     (void *)(local + 0xe14u));
            } else {
                (void)cm_fn_0066f4e0((void *)destination,
                                     (const char *)0x00a75360u,
                                     0x64u, 0x64u, argument7_cached,
                                     (void *)(local + 0x644u));
            }
        } else if (local[3] != 0u) {
            (void)cm_fn_0066f4e0((void *)destination,
                                 (const char *)0x00a7522cu,
                                 0x64u, 0x64u, 0x64u, argument7_cached,
                                 (void *)(local + 0x644u),
                                 (void *)(local + 0x25cu), argument6,
                                 (void *)(local + 0xe14u));
        } else {
            (void)cm_fn_0066f4e0((void *)destination,
                                 (const char *)0x00a75150u,
                                 0x64u, 0x64u, 0x64u,
                                 (s32)(s8)local[3], argument6, argument7_cached,
                                 (void *)(local + 0x644u),
                                 (void *)(local + 0x25cu));
        }
        return;
    }

    if (local[2] != 0u) {
        if (local[3] != 0u) {
            (void)cm_fn_0066f4e0((void *)destination,
                                 (const char *)0x00a750b0u,
                                 (void *)(local + 0xa2cu),
                                 (void *)(local + 0x25cu));
        } else {
            (void)cm_fn_0066f4e0((void *)destination,
                                 (const char *)0x00a75048u, argument6,
                                 (void *)(local + 0x25cu));
        }
    } else if (local[3] != 0u) {
        (void)cm_fn_0066f4e0((void *)destination,
                             (const char *)0x00a74fecu,
                             (void *)(local + 0xa2cu));
    } else {
        *destination = 0u;
    }
}
