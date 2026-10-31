#include "cmre.h"

/* Thiscall with one stack argument (ret 4): the receiver is ECX and the stack argument is
   the key. It returns a record pointer. */
extern u32 CM_THISCALL cm_fn_004a4850(u8 *self, u32 key);

/* Thiscall with five stack arguments (ret 0x14). The first argument is a key dword; its
   low byte is later overwritten by a byte from a matching table record (the stack slot
   is reused as a local). The record pointer from 0x004a4850 receives three bytes at +0x73,
   +0x74 and +0x75. The table base is at this+0xb1, its count (signed 16-bit) at this+0x3e,
   and each record is 59 (0x3b) bytes, with the key at offset 0 and a byte at offset 0x37.
   Arguments 2 to 5 are bytes; the byte checks use unsigned ranges 2..5. */
void CM_THISCALL cm_fn_00664400(u8 *param_1, u32 param_2, u32 param_3, u32 param_4,
                                u32 param_5, u32 param_6)
{
    u32 slot;
    u32 rec;
    s16 m;
    s16 i;
    u32 table = 0;
    int found;
    u8 dl;
    u8 cl;

    slot = param_2;
    rec = cm_fn_004a4850(*(u8 **)(param_1 + 8), slot);
    m = (s16)*(u16 *)(param_1 + 0x3e);
    found = 0;
    i = 0;
    if (m > 0) {
        table = *(u32 *)(param_1 + 0xb1);
        do {
            if (*(u32 *)(table + (u32)(s32)i * 59U) == slot) {
                found = 1;
                break;
            }
            i++;
        } while ((s16)i < m);
    }
    if (found) {
        slot = (slot & 0xffffff00U) | *(u8 *)(table + (u32)(s32)i * 59U + 0x37U);
    }
    if ((u8)param_3 == 0xff) {
        if ((u8)slot == 1) {
            return;
        }
        dl = (u8)param_5;
        if (dl >= 2 && dl <= 5) {
            dl = 2;
        }
        cl = (u8)param_6;
        if (cl >= 2 && cl <= 5) {
            *(volatile u8 *)(rec + 0x73) = (u8)param_4;
            *(volatile u8 *)(rec + 0x74) = dl;
            *(volatile u8 *)(rec + 0x75) = 5;
        } else {
            *(volatile u8 *)(rec + 0x74) = dl;
            *(volatile u8 *)(rec + 0x73) = (u8)param_4;
            *(volatile u8 *)(rec + 0x75) = cl;
        }
    } else {
        dl = (u8)param_5;
        cl = (u8)param_6;
        *(volatile u8 *)(rec + 0x74) = dl;
        *(volatile u8 *)(rec + 0x73) = (u8)param_4;
        *(volatile u8 *)(rec + 0x75) = cl;
    }
}
