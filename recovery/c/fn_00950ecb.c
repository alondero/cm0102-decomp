#include "cmre.h"

extern void CM_CDECL cm_fn_009529d2(u32 *output, u32 *codepage_slot,
                                    u32 mode, u32 operation,
                                    unsigned long long *record_value,
                                    unsigned long long *record_result);
extern u32 CM_CDECL cm_fn_00952c85(u32 mode, double *record_value,
                                   u32 codepage);
extern void CM_CDECL cm_fn_00952e9c(s32 value);
extern u32 CM_CDECL cm_fn_00952ec2(u32 *record);
extern void CM_CDECL cm_fn_00952ee2(u32 codepage, u32 value);

void CM_CDECL cm_fn_00950ecb(s32 operation, u32 *record,
                             u16 *codepage_pointer)
{
    volatile u32 *codepage_argument_slot = (volatile u32 *)&codepage_pointer;
    u16 codepage_word = *(volatile u16 *)codepage_pointer;
    *codepage_argument_slot = (u32)codepage_word;

    u32 record_state = *(volatile u32 *)record;
    u32 mode;
    switch (record_state) {
    case 1: mode = 8; break;
    case 2: mode = 4; break;
    case 3: mode = 0x11; break;
    case 4: mode = 0x12; break;
    case 5: mode = 8; break;
    case 7:
        *(volatile u32 *)record = 1;
        goto cleanup;
    case 8: mode = 0x10; break;
    default: goto cleanup;
    }

    if (cm_fn_00952c85(mode, (double *)((u8 *)record + 0x18),
                       *codepage_argument_slot) == 0u) {
        volatile struct {
            u32 output[10];
            double value;
            u32 gap[2];
            u32 flags;
            u32 tail[7];
        } scratch;
        if (operation == 0x10 || operation == 0x16 || operation == 0x1d) {
            u32 flags = scratch.flags;
            double source_value = *(volatile double *)((u8 *)record + 0x10);
            scratch.value = source_value;
            flags = (flags & 0xffffffe3u) | 3u;
            scratch.flags = flags;
        } else {
            u32 flags = scratch.flags;
            flags &= 0xfffffffeu;
            scratch.flags = flags;
        }

        cm_fn_009529d2((u32 *)(void *)&scratch.output[0],
                       (u32 *)(void *)codepage_argument_slot,
                       mode, (u32)operation,
                       (unsigned long long *)((u8 *)record + 8),
                       (unsigned long long *)((u8 *)record + 0x18));
    }

cleanup:
    cm_fn_00952ee2(*codepage_argument_slot, 0xffffu);
    if (*(volatile u32 *)record != 8u &&
        CM_GLOBAL(volatile u32, 0x00ad9978u) == 0u &&
        cm_fn_00952ec2(record) != 0u)
        return;
    cm_fn_00952e9c(*(volatile s32 *)record);
}
