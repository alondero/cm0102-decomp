#include "cmre.h"

/* Stack arguments by offset from the entry stack pointer (cdecl, ret 0):
   arg_04 owner whose dword table at +0x5c is indexed by arg_14 (signed word, stride 4);
   arg_08 owner whose dword at +0x18 points to a dword array;
   arg_0c word: the routine does nothing when it is zero;
   arg_10 dword index into that array;
   arg_14 word index into the +0x5c table; 9999 selects the no-lookup path. */

void cm_fn_00934480(u32 arg_04, u32 arg_08, u16 arg_0c, u32 arg_10, u16 arg_14)
{
    u32 array;
    u32 slot;
    u32 value;
    u32 limit;
    u32 result;

    if (arg_0c == 0u) {
        return;
    }

    array = *(volatile u32 *)(arg_08 + 0x18u);
    slot = array + arg_10 * 4u;
    value = *(volatile u32 *)slot;
    result = value;

    if (arg_14 != 9999u) {
        s32 index = (s32)(s16)arg_14;

        limit = *(volatile u32 *)(arg_04 + 0x5cu + (u32)index * 4u);
        if ((s32)value > (s32)limit) {
            u32 lowered = value - 0x60u;

            if ((s32)limit > (s32)lowered) {
                *(volatile u32 *)slot = (limit + 0x20u) & 0xffffffc0u;
                return;
            }
            result = lowered;
        } else {
            u32 raised = value + 0x60u;

            result = ((s32)limit < (s32)raised) ? limit : raised;
        }
    }

    *(volatile u32 *)slot = (result + 0x20u) & 0xffffffc0u;
}
