#include "cmre.h"

extern u32 CM_CDECL cm_fn_005b73a0(u32 value, u16 *saved_self, u16 *first_arg_slot);

u32 CM_THISCALL cm_fn_005b2e80(u32 *self, u32 value, u8 *scan)
{
    union { u32 value; u16 words[2]; } saved_self_slot = { (u32)self };
    union { u32 value; u16 words[2]; } first_arg_slot = { value };
    u32 helper_result;
    u32 current_self;
    u32 original_self = (u32)self;
    u32 eax_value;
    u32 edi_value;
    u32 scan_address;
    u8 match_index;
    u16 field;
    s32 i;
    s32 j;

    helper_result = cm_fn_005b73a0(value, saved_self_slot.words, first_arg_slot.words);
    current_self = saved_self_slot.value;
    eax_value = helper_result;
    for (i = 0; i < 11; ++i) {
        field = *(volatile u16 *)(original_self + 0x12du + (u32)i * 2u);
        eax_value = (eax_value & 0xffff0000u) | (u32)field;
        edi_value = eax_value & current_self;
        if ((u16)edi_value != 0) {
            eax_value &= 0x200u;
            if ((u16)eax_value != 0)
                break;
        }
    }
    if (i == 11)
        return 0;
    match_index = (u8)i;

    if (scan == 0)
        return 1;
    scan_address = (u32)scan + 0x19u;
    for (j = 0; j < 20; ++j) {
        u8 byte = *(volatile u8 *)scan_address;
        if (byte == match_index)
            return 1;
        if (byte == 0xfeu && *(volatile u8 *)(scan_address + 1u) == match_index)
            return 1;
        scan_address += 0x1beu;
    }
    return 0;
}
