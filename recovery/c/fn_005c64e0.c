#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(u8 *self);

void CM_THISCALL cm_fn_005c64e0(u8 *self, const volatile u32 *key_pointer)
{
    u32 object = (u32)self;
    u32 *list = (u32 *)(u32)*(volatile u32 *)(object + 0x34d8u);
    if (list != 0) {
        s32 count = *(volatile s32 *)(object + 0x34e0u);
        if (count > 0) {
            s32 index = 0;
            u32 offset = 0;
            do {
                list = (u32 *)(u32)*(volatile u32 *)(object + 0x34d8u);
                u32 address = (u32)list + offset + 8u;
                u32 value = *(volatile u32 *)(u32)address;
                u32 key = *key_pointer;
                if (value == key)
                    *(volatile u32 *)(u32)address = 0xffffffffu;
                count = *(volatile s32 *)(object + 0x34e0u);
                ++index;
                offset += 0x14u;
            } while (index < count);
        }
    }

    u32 head = *(volatile u32 *)(object + 0x34c8u);
    if (head != 0) {
        u32 value = *(volatile u32 *)(head + 2u);
        *(volatile u32 *)(head + 0x0au) = value;
        u32 iterator = *(volatile u32 *)(object + 0x34c8u);
        u32 entry = cm_fn_0068d1b0((u8 *)(u32)iterator);
        while (entry != 0) {
            u32 stored_pointer = *(volatile u32 *)(entry + 0x0cu);
            if (stored_pointer == (u32)key_pointer)
                *(volatile u32 *)(entry + 0x0cu) = 0;
            iterator = *(volatile u32 *)(object + 0x34c8u);
            entry = cm_fn_0068d1b0((u8 *)(u32)iterator);
        }
    }
}
