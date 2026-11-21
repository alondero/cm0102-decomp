#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))


extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 id);

extern u32 CM_THISCALL cm_fn_00803320(u32 self, u32 value);

extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 id, u32 value, u32 mode);

u32 CM_CDECL cm_fn_0046e460(u32 param_1)
{
    s16 selection = (s16)(u16)param_1;
    s32 active;
    s32 index;
    u32 table;
    u32 loaded;
    u32 candidate;
    u32 value;
    u16 mode;

    if (selection == -1) {

        loaded = 0;
    } else {
        u32 got;
        u32 mapped;


        got = cm_fn_007ff010(0x00b74450u, 0x18u);

        mapped = cm_fn_00803320(0x00b74450u, got);


        cm_fn_007ff270(0x00b74450u, 0x17u, mapped, 0u);

        active = (s32)(s16)W(0x00b77c7au);

        index = (s32)selection;

        table = D(0x00b74c4cu + (u32)active * 768u);

        loaded = D(table + (u32)index * 396u + 0xba966u);
    }


    mode = (u16)loaded;
    if (mode == 0) {

        mode = W(0x00dd6bdeu);
    }

    if (mode != 0xbu)
        return 0;


    if (selection != -1) {
        active = (s32)(s16)W(0x00b77c7au);
        index = (s32)selection;
        table = D(0x00b74c4cu + (u32)active * 768u);

        candidate = D(table + (u32)index * 396u + 0xba9a6u);
    } else {

        candidate = 0;
    }

    value = cm_fn_007ff010(0x00b74450u, 0x16u);

    if (candidate == value)
        return 0;


    if (selection != -1) {
        active = (s32)(s16)W(0x00b77c7au);
        index = (s32)selection;
        table = D(0x00b74c4cu + (u32)active * 768u);
        value = D(table + (u32)index * 396u + 0xba9a6u);
    } else {

        value = 0;
    }


    cm_fn_007ff270(0x00b74450u, 0x16u, value, 0u);

    return 0xfffffff5u;
}
