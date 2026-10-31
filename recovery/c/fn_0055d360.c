#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007e8d60(u32 self, u32 key, volatile u32 second, volatile u32 third, volatile u32 fourth);

u32 CM_THISCALL cm_fn_0055d360(u32 self, u32 person, u32 key)
{
    u32 value, base, item, object;
    s32 type;
    if (person) {
        value = *(volatile u32 *)person;
        if ((s32)value >= CM_GLOBAL(volatile s32, 0x00ae2364))
            return 0;
    }
    value = *(volatile u32 *)key;
    base = CM_GLOBAL(volatile u32, 0x00adadfc);
    item = *(volatile u32 *)(base + value * 4);
    if (!item || !person)
        return 0;
    type = *(volatile s8 *)(item + 0x50);
    if (type == -1)
        return 0;
    base = *(volatile u32 *)self;
    object = *(volatile u32 *)(base + (u32)type * 4);
    if (!object)
        return 0;
    value = *(volatile u32 *)(item + 4);
    base = *(volatile u32 *)person;
    return cm_fn_007e8d60(object, base, 0, 0, value);
}
