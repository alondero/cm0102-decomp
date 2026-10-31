#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_004a4a30(u32, u32);
extern u32 CM_THISCALL cm_fn_008e8a60(u32, u32, u32);

u32 CM_THISCALL cm_fn_007937f0(u32 self, u32 person, u32 match)
{
    u32 object = D(match + 0x14);
    if (object) {
        u32 key = D(object);
        if (key == D(0x009cf8c0) || key == D(0x009cf8c8) || key == D(0x009cf8c4)) {
            u32 input = D(person + 0x61);
            if (input) {
                u32 table = D(0x00adadfc);
                u32 entry = D(table + key * 4u);
                u32 receiver = D(entry + 8);
                u32 found = cm_fn_004a4a30(receiver, input);
                if (found) {
                    u32 other = D(person + 0x39);
                    if (other) {
                        u32 value = D(found + 8);
                        if (value != D(other))
                            return (value & 0xffffff00u) | 1u;
                    }
                }
            }
        }
    }
    return cm_fn_008e8a60(self, person, match);
}

