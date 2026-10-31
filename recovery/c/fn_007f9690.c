/* Faithful C recovery at preferred-image VA 007f9690. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0068d080(u32 *self);
extern u32 CM_THISCALL cm_fn_0068d0d0(void *self, u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_007f9690(void *self, u32 key_pointer, u32 cleanup_object)
{
    u32 result = 0;
    if (cleanup_object != 0)
        cm_fn_0068d080((u32 *)cleanup_object);
    if (key_pointer == 0)
        return 0;

    u32 base = (u32)self;
    s32 count = *(volatile s32 *)(base + 0x64u);
    u32 offset = 0;
    s32 i = 0;
    while (count > 0 && i < count) {
        u32 records = *(volatile u32 *)(base + 0x5cu);
        u32 record = records + offset;
        u32 id = *(volatile u32 *)record;
        u32 key = *(volatile u32 *)key_pointer;
        if (id == key) {
            if (cleanup_object != 0) {
                u32 index = *(volatile u32 *)(record + 4u);
                u32 table = CM_GLOBAL(volatile u32, 0x00ae23bc);
                u32 address = table + index * 0x6eu;
                cm_fn_0068d0d0((void *)cleanup_object, address, 0x6eu);
            }
            result = 1;
        }
        count = *(volatile s32 *)(base + 0x64u);
        ++i;
        offset += 0x11u;
    }
    return result;
}
