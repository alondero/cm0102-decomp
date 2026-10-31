/* Preferred-image VA 0x00690730 (image base 0x00400000). Stdcall shape:
   two stack arguments, `ret 8`, ECX not read before written. No direct
   caller reads EAX after the call, so the routine is void. Each callee's first parameter is
   its thiscall ECX object (the brief's prototypes); the listing's pushes give
   the stack arguments that follow it. */
#include "cmre.h"

extern int CM_THISCALL cm_fn_004cf5a0(int *param_1,int *param_2);
extern u32 CM_THISCALL cm_fn_007c5fe0(int param_1,int param_2,u32 param_3,char param_4,char param_5,u32 param_6,int param_7 );
extern u32 CM_THISCALL cm_fn_00843e10(int *param_1,int *param_2,int *param_3,char param_4,char param_5);
extern u32 CM_THISCALL cm_fn_00896c60(int param_1,int param_2,int *param_3,int param_4,int param_5);

void CM_STDCALL cm_fn_00690730(u32 param_1, u32 param_2)
{
    u8 keep;
    u8 slot;
    u32 obj;
    s32 state;
    int result;

    if (*(u32 *)(param_1 + 0x39u) == param_2) {
        state = *(s8 *)(param_1 + 0x3du);
        if (state == 12) {
            *(u8 *)(param_1 + 0x3du) = 5;
            keep = 1;
        } else if (state == 13) {
            *(u8 *)(param_1 + 0x3du) = 6;
            keep = 1;
        } else if (state == 15) {
            *(u8 *)(param_1 + 0x3du) = 8;
            keep = 1;
        } else {
            *(u8 *)(param_1 + 0x3du) = 0xb;
            keep = 0;
        }

        obj = CM_GLOBAL(u32, 0xb74350u);
        cm_fn_007c5fe0((int)obj, (int)param_1, 0xffffffffu, 0, 0, 0xffffffffu, 0);

        obj = CM_GLOBAL(u32, 0xde1f54u);
        obj = CM_GLOBAL(u32, obj + 0x856u);
        cm_fn_00843e10((int *)obj, (int *)param_1, (int *)param_2, 4, 2);

        obj = CM_GLOBAL(u32, 0xde1d08u);
        cm_fn_00896c60((int)obj, (int)param_2, (int *)param_1, 0, 1);

        obj = CM_GLOBAL(u32, 0xde1d08u);
        cm_fn_00896c60((int)obj, (int)param_2, (int *)param_1, 1, 1);

        slot = 0;
        do {
            if (*(u32 *)(param_2 + 0xd7u + (u32)slot * 4u) == param_1) {
                *(u32 *)(param_2 + 0xd7u + (u32)slot * 4u) = 0;
                break;
            }
            slot++;
        } while (slot < 0x32);

        if (!keep) {
            *(u32 *)(param_1 + 0x39u) = 0;
        }
    }

    if (*(u32 *)(param_1 + 0x24u) == param_2) {
        state = *(s8 *)(param_1 + 0x28u);
        if (state == 12) {
            *(u8 *)(param_1 + 0x28u) = 5;
            keep = 1;
        } else if (state == 13) {
            *(u8 *)(param_1 + 0x28u) = 6;
            keep = 1;
        } else if (state == 15) {
            *(u8 *)(param_1 + 0x28u) = 8;
            keep = 1;
        } else {
            *(u8 *)(param_1 + 0x28u) = 0xb;
            keep = 0;
        }

        obj = CM_GLOBAL(u32, 0xde1d08u);
        cm_fn_00896c60((int)obj, (int)param_2, (int *)param_1, 0, 1);

        obj = CM_GLOBAL(u32, 0xde1d08u);
        cm_fn_00896c60((int)obj, (int)param_2, (int *)param_1, 1, 1);

        slot = 0;
        do {
            if (*(u32 *)(param_2 + 0xd7u + (u32)slot * 4u) == param_1) {
                *(u32 *)(param_2 + 0xd7u + (u32)slot * 4u) = 0;
                break;
            }
            slot++;
        } while (slot < 0x32);

        if (!keep) {
            *(u32 *)(param_1 + 0x24u) = 0;
        }
    }

    obj = CM_GLOBAL(u32, 0xae1050u);
    result = cm_fn_004cf5a0((int *)obj, (int *)param_1);
    if (result != 0) {
        *(u8 *)((u32)result + 0x4eu) = 4;
    }
}
