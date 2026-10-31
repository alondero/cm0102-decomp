#include "cmre.h"

/* WIP after two failed invocations (one build, one semantic). O0/O2 semantic
   runs have inconclusive count 0 and fail on memory page 0x00ae2000 in cases
   286, 326 and 348. The listing-derived call maps and L+4/L+0x104 scratch
   geometry match; no instruction-backed cause for the output-page difference
   is established. */

extern u32 CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                  void *output_a, void *output_b);
extern u32 cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 format, u32 message, u32 mode);
extern void CM_THISCALL cm_fn_00687430(u32 self, u32 index_word,
                                      u32 item, u32 zero1, u32 zero2);
extern void CM_THISCALL cm_fn_00684230(u32 self);

u32 CM_THISCALL cm_fn_0091ec00(u32 self)
{
    volatile u8 scratch[0x204];
    volatile u16 *self_words = (volatile u16 *)self;
    u32 allocation;
    u32 format_text;
    u32 table;
    u32 item;
    u32 index_word;
    u8 index;
    s16 limit;
    s16 signed_index;

    self_words[0x3e / 2] = 5;
    allocation = cm_fn_00944e46(0x127);
    *(volatile u32 *)(self + 0xb1) = allocation;
    if (allocation == 0) {
        (void)cm_fn_00944cff(0x00ad4afc, 0, 0,
                             (void *)(scratch + 0x104),
                             (void *)(scratch + 4));
        format_text = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                             format_text, (u32)(scratch + 0x104),
                             (u32)(scratch + 4), 0x24b);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    index = 0;
    limit = (s16)self_words[0x3e / 2];
    scratch[0] = index;
    if (limit > 0) {
        for (;;) {
            table = *(volatile u32 *)(self + 0xef);
            item = *(volatile u32 *)(table +
                         (u32)(s32)(s8)index * 8u);
            index_word = *(volatile u32 *)scratch;
            cm_fn_00687430(self, index_word, item, 0, 0);
            index = (u8)(index + 1u);
            signed_index = (s16)(s8)index;
            limit = (s16)self_words[0x3e / 2];
            scratch[0] = index;
            if (signed_index >= limit)
                break;
        }
    }
    cm_fn_00684230(self);
    return 1;
}
