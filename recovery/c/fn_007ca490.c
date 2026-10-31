#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00689c80(u32 self, volatile u32 source, volatile u32 target, volatile u32 clear, volatile u32 pair, volatile u8 expected3, volatile u8 expected2);
typedef u32 (CM_THISCALL *slot_fn)(u32);

u32 CM_THISCALL cm_fn_007ca490(u32 self, u32 unused)
{
    u32 p, object, table, base, index, target;
    (void)unused;
    p = *(volatile u32 *)(self + 0xc);
    object = *(volatile u32 *)(p + 4);
    table = *(volatile u32 *)object;
    ((slot_fn)*(volatile u32 *)(table + 0xa4))(object);
    base = CM_GLOBAL(volatile u32, 0x00adadfc);
    index = CM_GLOBAL(volatile u32, 0x009cf85c);
    object = *(volatile u32 *)(base + index * 4);
    table = *(volatile u32 *)object;
    ((slot_fn)*(volatile u32 *)(table + 0xa4))(object);
    base = CM_GLOBAL(volatile u32, 0x00adadfc);
    index = CM_GLOBAL(volatile u32, 0x009cf85c);
    target = *(volatile u32 *)(base + index * 4);
    p = *(volatile u32 *)(self + 0xc);
    object = *(volatile u32 *)(p + 4);
    return cm_fn_00689c80(self, object, target, 1, 1, 0xff, 0xff);
}
