#include "cmre.h"

/* fn_0059fae0: stdcall, three stack arguments (ret 0xc), no ECX read before write.
   Preferred-image VA 0x0059fae0, image base 0x00400000.
   Layout: the SEH frame {prev, handler 0x009590eb, state} is linked before the
   parameter test and unlinked on every exit. The 0xe0-byte scratch area is the
   receiver object for three thiscalls; calls pass its address as self. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern int cm_fn_0090cfc0(int param_1);

void CM_STDCALL cm_fn_0059fae0(int param_1, s16 param_2, s16 param_3)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    s16 sVar1;

    seh.state = -1;
    seh.handler = 0x009590ebu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    if (param_1 != 0) {
        cm_fn_0077bbe0(scratch, 0x1788u, 0u);
        seh.state = 0;
        cm_fn_00785790(scratch, 1u, (u32)(s32)param_2);
        if (param_3 == 2) {
            sVar1 = 2;
        } else {
            sVar1 = (s16)cm_fn_0090cfc0(2);
        }
        cm_fn_00785790(scratch, 2u, (u32)(s32)sVar1);
        cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)scratch, (u32)param_1);
    }

    CM_FS(u32, 0) = seh.prev;
}
