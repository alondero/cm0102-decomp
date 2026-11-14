/* fn_007bdee0 -- CM 01/02 3.9.68, preferred-image VA 0x007bdee0 (base 0x00400000).
 *
 * Original shape: cdecl, seven stack arguments, one 0x11fc-byte alloca frame
 * (mov eax,0x11fc / call 0x00945610 __alloca_probe), three saved registers
 * (ebx/esi/edi), a single variadic cdecl callee cm_fn_0066f4e0 (sprintf-like)
 * and no meaningful return value (EAX is dead at every ret).
 *
 * Frame model.  L is the base of the alloca'd area, i.e. esp right after the
 * three register pushes; the frame occupies L+0x0000 .. L+0x11fb and the seven
 * arguments live at L+0x1200 .. L+0x1218.  Every local below is expressed with
 * its original frame offset; the three 100-byte row bases deliberately overlap
 * because the original does:
 *
 *   L+0x0002  byte   count2   (original [esp+0x0e])
 *   L+0x0003  byte   count3   (original [esp+0x0f])
 *   L+0x0004  row A  stride 100, indexed by BL   (original [esp+0x10])
 *   L+0x0068  row A[1]
 *   L+0x00cc  row B  stride 100, indexed by count2(original [esp+0xd8])
 *   L+0x0130  row B[1]
 *   L+0x0194  row C  stride 100, indexed by count3(original [esp+0x1a0])
 *   L+0x01f8  row C[1]
 *   L+0x025c  row C[2]
 *   L+0x0644  buffer 1        (original [esp+0x650])
 *   L+0x0a2c  buffer 2        (original [esp+0xa38])
 *   L+0x0e14  buffer 3        (original [esp+0xe20])
 *
 * Argument map (all read through the original's own stack slots):
 *   arg1 [L+0x1200] u32 flags      read as dword at [esp+0x120c] and, with two
 *                                   arguments pushed, at [esp+0x1214]
 *   arg2 [L+0x1204] u32 flags2     read as byte/dword at [esp+0x1210]
 *   arg3 [L+0x1208] u16 score      read as word at [esp+0x1214]
 *   arg4 [L+0x120c] u8  age        read as byte at [esp+0x1218]
 *   arg5 [L+0x1210] ptr destination read as dword at [esp+0x121c] -> EDI
 *   arg6 [L+0x1214] u32            read as dword at [esp+0x1220]
 *   arg7 [L+0x1218] u32            read as dword at [esp+0x1224] -> ESI
 *
 * The bit tests at 007be01f..007be12d come from arg1 (the listing reloads
 * [esp+0x1214] with the two call arguments still pushed, i.e. L+0x1200);
 * the 0x0cb2/0x186a comparisons at 007be169 come from arg3; the byte
 * comparisons at 007be177/007be19e/007be1bf/007be2da come from arg4.
 */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(void *destination, const char *format, ...);

#define CM_FRAME_SIZE 0x11fcu

#define CM_OFF_COUNT2 0x0002u /* byte at original [esp+0x0e] */
#define CM_OFF_COUNT3 0x0003u /* byte at original [esp+0x0f] */
#define CM_OFF_ROW_A 0x0004u  /* 100-byte rows indexed by BL */
#define CM_OFF_ROW_B 0x00ccu  /* 100-byte rows indexed by count2 */
#define CM_OFF_ROW_C 0x0194u  /* 100-byte rows indexed by count3 */
#define CM_ROW_STRIDE 100

#define CM_OFF_BUF1 0x0644u
#define CM_OFF_BUF2 0x0a2cu
#define CM_OFF_BUF3 0x0e14u

void CM_CDECL cm_fn_007bdee0(u32 volatile flags1, u32 volatile flags2,
                             u32 volatile score_word, u32 volatile age_byte,
                             volatile u8 *destination, u32 volatile argument6,
                             u32 volatile argument7)
{
    u8 frame[CM_FRAME_SIZE];
    u8 count1; /* the original's BL register */
    u32 argument7_cached;
    u32 row;

    /* 007bdef4 xor bl,bl ; 007bdef8/007bdefc clear the two byte counters */
    count1 = 0u;
    frame[CM_OFF_COUNT2] = 0u;
    frame[CM_OFF_COUNT3] = 0u;

    /* 007bdef6 test edi,edi ; je 007be635 (plain epilogue, no store) */
    if (destination == (volatile u8 *)0)
        return;

    /* 007bdf06..007bdf1a: low word of arg1 and of arg2 both zero -> *dest = 0 */
    if ((u16)flags1 == 0u && (u16)flags2 == 0u) {
        *destination = 0u;
        return;
    }

    /* 007bdf20..007bdf76: (arg1 & 0x100) or (arg1 & 0x400) -> row C[0] */
    if ((flags1 & 0x100u) != 0u || (flags1 & 0x400u) != 0u) {
        const char *format;
        if ((flags1 & 0x100u) != 0u)
            format = ((flags1 & 0x400u) != 0u) ? (const char *)0x00a7598cu
                                              : (const char *)0x00a7596cu;
        else
            format = (const char *)0x00a75950u;
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_C), format);
        frame[CM_OFF_COUNT3] = 1u;
    }

    /* 007bdf7b..007bdfcc: (arg1 & 0x200) -> row C[count3], then count3++ */
    if ((flags1 & 0x200u) != 0u) {
        row = CM_OFF_ROW_C +
              (u32)((s32)((s8)frame[CM_OFF_COUNT3]) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row),
                             ((flags1 & 4u) != 0u) ? (const char *)0x00a7591cu
                                                   : (const char *)0x00a75900u);
        frame[CM_OFF_COUNT3] = (u8)(frame[CM_OFF_COUNT3] + 1u);
    }

    /* 007bdfd0..007be01d: (arg1 & 0x1000) else (arg1 & 0x20) -> row A[0] */
    if ((flags1 & 0x1000u) != 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_A),
                             ((flags1 & 0x20u) != 0u) ? (const char *)0x00a758f4u
                                                       : (const char *)0x00a758e8u);
        count1 = 1u;
    } else if ((flags1 & 0x20u) != 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_A),
                             (const char *)0x00a758d0u);
        count1 = 1u;
    }

    /* 007be01f..007be12d: the remaining bit tests all read arg1's low byte */
    if ((flags1 & 2u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a758c8u);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 1u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a758bcu);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 4u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a758a0u);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 8u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a75890u);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 0x10u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a75880u);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 0x40u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a75874u);
        count1 = (u8)(count1 + 1u);
    }
    if ((flags1 & 0x80u) != 0u && count1 < 2u) {
        row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a7586cu);
        count1 = (u8)(count1 + 1u);
    }

    /* 007be153..007be200: (arg2 & 2) group, arg3 score and arg4 age */
    if (((u8)flags2 & 2u) != 0u) {
        s16 score = (s16)(u16)score_word;
        s8 age = (s8)(u8)age_byte;
        if (score < (s16)0x0cb2 && age < (s8)0x17) {
            (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                                 (const char *)0x00a75820u);
            frame[CM_OFF_COUNT2] = 1u;
        } else if (score < (s16)0x186a && age < (s8)0x17) {
            (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                                 (const char *)0x00a757c4u);
            frame[CM_OFF_COUNT2] = 1u;
        } else if (age > (s8)0x19) {
            (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                                 (const char *)0x00a7579cu);
            frame[CM_OFF_COUNT2] = 1u;
        } else if (count1 < 2u) {
            row = CM_OFF_ROW_A + (u32)((s32)((s8)count1) * (s32)CM_ROW_STRIDE);
            (void)cm_fn_0066f4e0((void *)(frame + row),
                                 (const char *)0x00a75788u);
            count1 = (u8)(count1 + 1u);
        }
    }

    /* 007be202..007be229: (arg2 & 0x10) */
    if (((u8)flags2 & 0x10u) != 0u && frame[CM_OFF_COUNT2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                             (const char *)0x00a75780u);
        frame[CM_OFF_COUNT2] = 1u;
    }

    /* 007be22e..007be25d: (arg2 & 0x800) */
    if ((flags2 & 0x800u) != 0u && frame[CM_OFF_COUNT2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                             (const char *)0x00a75770u);
        frame[CM_OFF_COUNT2] = 1u;
    }

    /* 007be262..007be289: (arg2 & 0x20) */
    if (((u8)flags2 & 0x20u) != 0u && frame[CM_OFF_COUNT2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                             (const char *)0x00a75760u);
        frame[CM_OFF_COUNT2] = 1u;
    }

    /* 007be28e..007be2c4: (arg2 & 0x40) -> row B[count2], count2++ */
    if (((u8)flags2 & 0x40u) != 0u && frame[CM_OFF_COUNT2] < 2u) {
        row = CM_OFF_ROW_B +
              (u32)((s32)((s8)frame[CM_OFF_COUNT2]) * (s32)CM_ROW_STRIDE);
        (void)cm_fn_0066f4e0((void *)(frame + row), (const char *)0x00a75750u);
        frame[CM_OFF_COUNT2] = (u8)(frame[CM_OFF_COUNT2] + 1u);
    }

    /* 007be2c8..007be308: (arg2 & 4), age decides the format */
    if (((u8)flags2 & 4u) != 0u && frame[CM_OFF_COUNT2] == 0u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                             ((s8)(u8)age_byte < (s8)0x15)
                                 ? (const char *)0x00a75728u
                                 : (const char *)0x00a7570cu);
        frame[CM_OFF_COUNT2] = 1u;
    }

    /* 007be30d..007be358: (arg2 & 8) -> row B[0] or row B[count2] */
    if (((u8)flags2 & 8u) != 0u && frame[CM_OFF_COUNT2] < 2u) {
        if (frame[CM_OFF_COUNT2] == 0u) {
            (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_B),
                                 (const char *)0x00a756f0u);
        } else {
            row = CM_OFF_ROW_B +
                  (u32)((s32)((s8)frame[CM_OFF_COUNT2]) * (s32)CM_ROW_STRIDE);
            (void)cm_fn_0066f4e0((void *)(frame + row),
                                 (const char *)0x00a756d4u);
        }
        frame[CM_OFF_COUNT2] = (u8)(frame[CM_OFF_COUNT2] + 1u);
    }

    /* 007be35c..007be403: count3 dispatch */
    argument7_cached = argument7;
    if (frame[CM_OFF_COUNT3] == 0u) {
        frame[CM_OFF_BUF2] = 0u;
    } else if (frame[CM_OFF_COUNT3] == 1u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF2),
                             (const char *)0x00a75604u,
                             (void *)(unsigned long)argument7_cached,
                             (void *)(frame + CM_OFF_ROW_C));
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF3),
                             (const char *)0x00a755e0u,
                             (void *)(frame + CM_OFF_ROW_C));
    } else if (frame[CM_OFF_COUNT3] == 2u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF2),
                             (const char *)0x00a75680u,
                             (void *)(unsigned long)argument7_cached,
                             (void *)(frame + CM_OFF_ROW_C),
                             (void *)(frame + CM_OFF_ROW_C + CM_ROW_STRIDE));
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF3),
                             (const char *)0x00a7563cu,
                             (void *)(frame + CM_OFF_ROW_C),
                             (void *)(frame + CM_OFF_ROW_C + CM_ROW_STRIDE));
    }

    /* 007be40d..007be456: count1 (BL) dispatch */
    if (count1 == 0u) {
        frame[CM_OFF_BUF1] = 0u;
    } else if (count1 == 1u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF1),
                             (const char *)0x00a75548u,
                             (void *)(frame + CM_OFF_ROW_A));
    } else if (count1 == 2u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_BUF1),
                             (const char *)0x00a75578u,
                             (void *)(frame + CM_OFF_ROW_A),
                             (void *)(frame + CM_OFF_ROW_A + CM_ROW_STRIDE));
    }

    /* 007be460..007be4b4: count2 dispatch */
    if (frame[CM_OFF_COUNT2] == 0u) {
        frame[CM_OFF_ROW_C + 2u * CM_ROW_STRIDE] = 0u;
    } else if (frame[CM_OFF_COUNT2] == 1u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE),
                             (const char *)0x00a754b4u,
                             (void *)(frame + CM_OFF_ROW_B));
    } else if (frame[CM_OFF_COUNT2] == 2u) {
        (void)cm_fn_0066f4e0((void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE),
                             (const char *)0x00a754e4u,
                             (void *)(frame + CM_OFF_ROW_B),
                             (void *)(frame + CM_OFF_ROW_B + CM_ROW_STRIDE));
    }

    /* 007be4be..007be631: final selection */
    if (count1 != 0u) {
        if (frame[CM_OFF_COUNT2] == 0u) {
            if (frame[CM_OFF_COUNT3] != 0u) {
                (void)cm_fn_0066f4e0((void *)destination,
                                     (const char *)0x00a753d0u, 0x64u, 0x64u,
                                     0x64u, argument6, argument7_cached,
                                     (void *)(frame + CM_OFF_BUF1),
                                     (void *)(frame + CM_OFF_BUF3));
            } else {
                (void)cm_fn_0066f4e0((void *)destination,
                                     (const char *)0x00a75360u, 0x64u, 0x64u,
                                     argument7_cached,
                                     (void *)(frame + CM_OFF_BUF1));
            }
        } else if (frame[CM_OFF_COUNT3] != 0u) {
            (void)cm_fn_0066f4e0((void *)destination, (const char *)0x00a7522cu,
                                 0x64u, 0x64u, 0x64u, argument7_cached,
                                 (void *)(frame + CM_OFF_BUF1),
                                 (void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE),
                                 argument6, (void *)(frame + CM_OFF_BUF3));
        } else {
            (void)cm_fn_0066f4e0((void *)destination, (const char *)0x00a75150u,
                                 0x64u, 0x64u, 0x64u,
                                 (s32)(s8)frame[CM_OFF_COUNT3], argument6,
                                 argument7_cached,
                                 (void *)(frame + CM_OFF_BUF1),
                                 (void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE));
        }
        return;
    }

    if (frame[CM_OFF_COUNT2] != 0u) {
        if (frame[CM_OFF_COUNT3] != 0u) {
            (void)cm_fn_0066f4e0((void *)destination, (const char *)0x00a750b0u,
                                 (void *)(frame + CM_OFF_BUF2),
                                 (void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE));
        } else {
            (void)cm_fn_0066f4e0((void *)destination, (const char *)0x00a75048u,
                                 argument6,
                                 (void *)(frame + CM_OFF_ROW_C + 2u * CM_ROW_STRIDE));
        }
    } else if (frame[CM_OFF_COUNT3] != 0u) {
        (void)cm_fn_0066f4e0((void *)destination, (const char *)0x00a74fecu,
                             (void *)(frame + CM_OFF_BUF2));
    } else {
        /* 007be632 mov byte ptr [edi],0 */
        *destination = 0u;
    }
}
