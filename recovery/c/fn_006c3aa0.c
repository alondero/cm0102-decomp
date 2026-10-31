#include "cmre.h"

/* Fastcall routine with the owner object in ECX (param_1 here). It walks two
   runs of 20 records. Each record is 0x1be bytes apart. Within a record, the
   signed byte at +0x19 gates a call to cm_fn_00717410 with the record as ECX.
   When that call's AX is not 1, the signed byte at +0x103 is folded into an
   accumulator:
     run 1 starts at +0x6a6e, accumulator starts at 6, keeps the maximum;
     the accumulator is then incremented (byte) and stored at +0x8eaf;
     run 2 starts at +0x4796, accumulator starts at 5, keeps the minimum;
     the accumulator is then decremented (byte) and stored at +0x8eb0. */
extern u32 CM_FASTCALL cm_fn_00717410(int param_1);

#define RECORD_STRIDE 0x1beu
#define RECORD_FLAG 0x19u
#define RECORD_VALUE 0x103u
#define RUN_LENGTH 20u

void CM_FASTCALL cm_fn_006c3aa0(int param_1)
{
    u32 self = (u32)param_1;
    u32 base;
    u32 record;
    u32 k;
    u8 acc;

    acc = 6u;
    base = self + 0x6a6eu;
    for (k = 0; k < RUN_LENGTH; k++) {
        record = base + k * RECORD_STRIDE;
        if (*(s8 *)(record + RECORD_FLAG) >= 0) {
            u32 r = cm_fn_00717410((int)record);
            if ((u16)r != 1u) {
                s8 v = *(s8 *)(record + RECORD_VALUE);
                if (v > (s8)acc) {
                    acc = (u8)v;
                }
            }
        }
    }

    acc = (u8)(acc + 1u);
    *(u8 *)(self + 0x8eafu) = acc;

    acc = 5u;
    base = self + 0x4796u;
    for (k = 0; k < RUN_LENGTH; k++) {
        record = base + k * RECORD_STRIDE;
        if (*(s8 *)(record + RECORD_FLAG) >= 0) {
            u32 r = cm_fn_00717410((int)record);
            if ((u16)r != 1u) {
                s8 v = *(s8 *)(record + RECORD_VALUE);
                if (v < (s8)acc) {
                    acc = (u8)v;
                }
            }
        }
    }

    acc = (u8)(acc - 1u);
    *(u8 *)(self + 0x8eb0u) = acc;
}
