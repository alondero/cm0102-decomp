#include "cmre.h"

/* Object at 0x00b74450 is loaded into ECX before every helper call below.
   The helper at 0x007ff270 takes that object as self plus three stack
   arguments (slot index, value, flag). */
#define OBJ_B74450 ((u32)0x00b74450u)

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flag);

void cm_fn_0048f8e0(s32 *param_1, u16 param_2, u16 param_3, s32 param_4)
{
    u32 table;
    u32 index;
    u32 entry;
    u32 uVar3;
    u32 v8;
    u32 v9;
    u32 v11;
    u32 v12;

    table = CM_GLOBAL(volatile u32, 0x00adadfc);
    index = (u32)*(volatile s32 *)param_1;
    entry = *(volatile u32 *)(table + index * 4u);
    if (cm_fn_007fe600(OBJ_B74450, 0x0048fcd0u, 0x004965d0u, 0u, 0x004985e0u, 0u) != 0) {
        cm_fn_007ff270(OBJ_B74450, 0u, entry, 0u);
        if (param_4 == 0) {
            uVar3 = 3u;
        } else {
            uVar3 = 2u;
        }
        cm_fn_007ff270(OBJ_B74450, 1u, uVar3, 0u);
        cm_fn_007ff270(OBJ_B74450, 2u, 0xfffffffeu, 0u);
        cm_fn_007ff270(OBJ_B74450, 3u, 0xfffffffeu, 0u);
        cm_fn_007ff270(OBJ_B74450, 4u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 5u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 6u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 7u, 0u, 0u);
        v8 = (param_4 == 0) ? 0u : (u32)param_2;
        cm_fn_007ff270(OBJ_B74450, 8u, v8, 0u);
        v9 = (param_4 == 0) ? 0u : (u32)param_3;
        cm_fn_007ff270(OBJ_B74450, 9u, v9, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0au, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0bu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0cu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0du, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0eu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x0fu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x10u, 0u, 0u);
        v11 = (param_4 != 0) ? 0u : (u32)param_2;
        cm_fn_007ff270(OBJ_B74450, 0x11u, v11, 0u);
        v12 = (param_4 != 0) ? 0u : (u32)param_3;
        cm_fn_007ff270(OBJ_B74450, 0x12u, v12, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x13u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x14u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x15u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x16u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x17u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x18u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x19u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1au, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1bu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1cu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1du, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x23u, 3u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x24u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x25u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x26u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x27u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1eu, 0x11u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x1fu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x20u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x21u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x22u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x28u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x29u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2au, 0xffffffffu, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2bu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2cu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2du, 1u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2eu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x2fu, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x30u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x31u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x32u, 0xffffffffu, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x33u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x34u, 0u, 0u);
        cm_fn_007ff270(OBJ_B74450, 0x35u, 0u, 0u);
    }
}
