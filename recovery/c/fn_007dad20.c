#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);

u32 CM_THISCALL cm_fn_007dad20(u32 self, volatile u8 type, u16 id,
                               volatile u32 output, volatile u32 flag_output)
{
    u32 destination = output;
    u32 result, flag_pointer, label, table, target, index, list, value, node;
    u32 field, scanned;
    u8 kind, flag;
    int second_list;
    if (destination == 0) {
        flag_pointer = flag_output;
        B(flag_pointer) = 0;
        return flag_pointer;
    }
    kind = type;
    if (id == 0xffff) {
        index = (u32)(s32)(s8)kind - 1u;
        if (index <= 6u) {
            target = D(0x007daef8u + index * 4u);
            (void)target;
        }
        switch (kind) {
        case 2: label = 0x00a24284; break;
        case 3: label = 0x00a794cc; break;
        case 4: label = 0x00a241f4; break;
        case 5: label = 0x00a794b8; break;
        default: label = 0x00a242cc; break;
        }
        result = cm_fn_00944c9f(destination, 0x00991944, label);
        flag_pointer = flag_output;
        B(flag_pointer) = 0;
        return result;
    }
    second_list = kind == 2 || kind == 4;
    field = self + (second_list ? 4u : 0u);
    list = D(field);
    scanned = 0;
    value = D(list + 2);
    D(list + 0xa) = value;
    list = D(field);
    if ((s16)W(list) > 0) {
        do {
            node = cm_fn_0068d1b0(list);
            if (node == 0)
                break;
            if (W(node) == id) {
                destination = output;
                result = cm_fn_00944c9f(destination, 0x00991944, node + 2);
                flag = B(node + 0xd3);
                flag_pointer = flag_output;
                B(flag_pointer) = flag;
                if (second_list)
                    return (result & 0xffffff00u) | flag;
                return flag_pointer;
            }
            list = D(field);
            ++scanned;
            value = (u32)(s32)(s16)W(list);
        } while ((s32)scanned < (s32)value);
    }
    kind = type;
    index = (u32)(s32)(s8)kind - 1u;
    table = second_list ? 0x007daf14u : 0x007daf30u;
    if (index <= 6u) {
        target = D(table + index * 4u);
        (void)target;
    }
    switch (kind) {
    case 2: label = 0x00a24284; break;
    case 3: label = 0x00a794cc; break;
    case 4: label = 0x00a241f4; break;
    case 5: label = 0x00a794b8; break;
    default: label = 0x00a242cc; break;
    }
    destination = output;
    result = cm_fn_00944c9f(destination, 0x00991944, label);
    flag_pointer = flag_output;
    B(flag_pointer) = 0;
    return second_list ? flag_pointer : result;
}
