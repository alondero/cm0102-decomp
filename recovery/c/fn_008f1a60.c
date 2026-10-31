/* Preferred-image VA 0x008f1a60, image base 0x00400000.
   Cdecl with two stack inputs (arg1 at entry+4, arg2 at entry+8). The routine links
   an SEH record at fs:[0] whose {prev, handler, state} fields sit directly above a
   0xe0-byte scratch area: scratch+0xe0 is the record. Every buffer address the
   listing passes (ECX and the stack argument) is that scratch base. Global 0x00b7431c
   supplies the receiver for cm_fn_00786b70 and 0x00de1f54 the receiver for
   cm_fn_008c61e0; 0x00b74450 is the immediate receiver for cm_fn_007fe600 and
   cm_fn_007ff270. Return is EAX: 0 when cm_fn_007fe600 returns 0, otherwise the EAX
   left by the last cm_fn_007ff270 call. */
#include "cmre.h"

struct cm_fn_008f1a60_seh {
    u32 prev;
    u32 handler;
    u32 state;
};

struct cm_fn_008f1a60_frame {
    u8 scratch[0xe0];
    volatile struct cm_fn_008f1a60_seh seh;
};

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(int *param_1, int *param_2, int param_3, int param_4);
extern int CM_THISCALL cm_fn_008c61e0(int *param_1, int param_2);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern u32 CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

u32 CM_CDECL cm_fn_008f1a60(u32 arg1, u32 arg2)
{
    struct cm_fn_008f1a60_frame blk;
    u32 ecx;
    u32 eax;
    u32 ebx;

    blk.seh.state = 0xffffffffu;
    blk.seh.handler = 0x00964cfbu;
    blk.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&blk.seh;

    cm_fn_0077bbe0((void *)blk.scratch, 0u, 0u);

    ecx = CM_GLOBAL(u32, 0x00b7431cu);
    blk.seh.state = 0u;
    cm_fn_00786b70((int *)ecx, (int *)arg1, (int)arg2, (int)blk.scratch);

    eax = (u32)cm_fn_00785830((void *)blk.scratch, 9u);
    ecx = CM_GLOBAL(u32, 0x00de1f54u);
    ebx = (u32)cm_fn_008c61e0((int *)ecx, (int)eax);

    eax = cm_fn_007fe600(0x00b74450u, 0x008f2010u, 0x008f22c0u, 1u, 0u, 0u);
    if (eax != 0) {
        cm_fn_007ff270((int)0x00b74450u, 0, (char *)arg1, 0);
        cm_fn_007ff270((int)0x00b74450u, 1, (char *)arg2, 0);
        eax = cm_fn_007ff270((int)0x00b74450u, 2, (char *)ebx, 0);
    }

    ecx = blk.seh.prev;
    CM_FS(u32, 0) = ecx;
    return eax;
}
