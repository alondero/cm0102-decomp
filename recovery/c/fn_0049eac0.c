#include "cmre.h"

/* fn_0049eac0: cdecl, two stack arguments (ret 0), no ECX read before write.
   Preferred-image VA 0x0049eac0, image base 0x00400000.
   Links an SEH frame {prev, handler 0x009563bb, state} in listing order and unlinks it on
   return. The 0xe0-byte scratch area is the receiver for three thiscalls and is passed by
   address as self. The global object at 0x00b7431c is the receiver of 0x00786b70. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(int *param_1, int *param_2, int param_3, int param_4);
extern void cm_fn_0048f4a0(int *param_1, u32 param_2, u32 param_3, int param_4);

void cm_fn_0049eac0(u32 param_1, u32 param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u32 iVar3;
    s8 cVar1;
    s16 sVar2;
    u8 bl;

    seh.state = -1;
    seh.handler = 0x009563bbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0077bbe0(scratch, 0u, 0u);
    seh.state = 0;
    cm_fn_00786b70((int *)CM_GLOBAL(u32, 0x00b7431c), (int *)param_1, (int)param_2, (int)scratch);
    iVar3 = (u32)cm_fn_00785830(scratch, 0u);
    bl = (u8)cm_fn_00785830(scratch, 4u);
    sVar2 = (s16)cm_fn_00785830(scratch, 5u);
    cVar1 = (s8)bl;
    cm_fn_0048f4a0((int *)(iVar3 * 0x6bu + CM_GLOBAL(u32, 0x00ae23d0)), (u32)(s32)cVar1,
                   (u32)(s32)sVar2, 0);

    CM_FS(u32, 0) = seh.prev;
}
