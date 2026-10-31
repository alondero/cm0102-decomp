#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (this):
   0x86e pointer to a table of 8-byte entries; the flag byte at entry offset 0
   selects the three call groups (bit 8, bit 2, bit 4).
   Argument record fields by byte offset: 0x04 element pointer (passed to
   0x0068d1b0 and copied from +0x02 to +0x0a), 0x0c element index.
   Element records are 110 bytes, starting at the global at 0x00ae23bc. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define REC_U32(off) (*(volatile u32 *)(rec + (off)))
#define FLAGS_OF(idx) (*(volatile u8 *)(OBJ_U32(0x86e) + (u32)(idx) * 8u))
#define ELEM_ADDR(idx) (CM_GLOBAL(volatile u32, 0x00ae23bc) + (u32)(idx) * 110u)

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern void CM_THISCALL cm_fn_008cb0f0(int param_1, int *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_008cb340(int param_1, u32 *param_2, int param_3, u32 param_4, u32 param_5);
extern void CM_THISCALL cm_fn_008cb4f0(void *self, u32 *arg);
extern void CM_THISCALL cm_fn_008cb590(void *self, u32 *arg);
extern void CM_THISCALL cm_fn_008cb630(void *self, u32 *arg);

void CM_THISCALL cm_fn_008c91e0(u32 self, u32 rec)
{
    u32 idx;
    u32 base;
    u32 ptr;
    u32 elem;
    u32 copied;

    idx = REC_U32(0x0c);
    base = ELEM_ADDR(idx);
    if ((FLAGS_OF(idx) & 8) != 0) {
        cm_fn_008cb0f0((int)self, (int *)base, (int)rec);
        cm_fn_008cb4f0((void *)self, (u32 *)base);
    }

    idx = REC_U32(0x0c);
    if ((FLAGS_OF(idx) & 2) != 0) {
        cm_fn_008cb340((int)self, (u32 *)ELEM_ADDR(idx), -1, 2u, rec);
        cm_fn_008cb590((void *)self, (u32 *)base);
    }

    idx = REC_U32(0x0c);
    if ((FLAGS_OF(idx) & 4) != 0) {
        cm_fn_008cb340((int)self, (u32 *)ELEM_ADDR(idx), -1, 5u, rec);
        cm_fn_008cb630((void *)self, (u32 *)base);
    }

    elem = REC_U32(0x04);
    if (elem == 0) {
        return;
    }
    copied = *(volatile u32 *)(elem + 0x02u);
    *(volatile u32 *)(elem + 0x0au) = copied;
    ptr = cm_fn_0068d1b0((int)REC_U32(0x04));
    if (ptr == 0) {
        return;
    }

    do {
        idx = *(volatile u32 *)ptr;
        if ((FLAGS_OF(idx) & 8) != 0) {
            cm_fn_008cb0f0((int)self, (int *)ELEM_ADDR(idx), (int)rec);
            cm_fn_008cb4f0((void *)self, (u32 *)base);
        }

        idx = *(volatile u32 *)ptr;
        if ((FLAGS_OF(idx) & 2) != 0) {
            cm_fn_008cb340((int)self, (u32 *)ELEM_ADDR(idx), -1, 2u, rec);
            idx = *(volatile u32 *)ptr;
            cm_fn_008cb590((void *)self, (u32 *)ELEM_ADDR(idx));
        }

        idx = *(volatile u32 *)ptr;
        if ((FLAGS_OF(idx) & 4) != 0) {
            cm_fn_008cb340((int)self, (u32 *)ELEM_ADDR(idx), -1, 5u, rec);
            idx = *(volatile u32 *)ptr;
            cm_fn_008cb630((void *)self, (u32 *)ELEM_ADDR(idx));
        }

        ptr = cm_fn_0068d1b0((int)REC_U32(0x04));
    } while (ptr != 0);
}
