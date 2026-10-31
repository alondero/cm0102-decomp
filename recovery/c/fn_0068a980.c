/* Preferred-image VA 0x0068a980, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, edi), four stack arguments, ret 0x10,
   no return value. Stack arguments in order: arg1 byte (dl), arg2 byte (bl),
   arg3 byte (cl), arg4 dword (ebp, tested against zero).
   If the 16-bit count at +0x3e is greater than zero, loop index i (16-bit,
   signed) runs from 0 while i < count. Each element is 59 (0x3b) bytes at
   base = dword [object+0xb1], element = base + i * 0x3b, and its dword at +0x00
   points to a record whose byte at +0x64 is updated:
   - block A runs unless arg2 == -3 or byte element+0x37 == arg2;
   - block B runs only if arg3 != -3 and byte element+0x37 == arg3;
   - in each block, with record = dword element+0x00: if arg4 == 0 the byte
     record+0x64 is set to arg1 when it is signed-less-than arg1; otherwise it is
     set when arg1 is signed-less-than it. The two blocks use the same rule.
   Comparisons are signed. Field meanings remain unknown. */
#include "cmre.h"

void CM_THISCALL cm_fn_0068a980(u8 *object, s8 arg1, s8 arg2, s8 arg3, s32 arg4)
{
    s16 index;
    u8 *element;
    u8 *record;

    index = 0;
    if (*(s16 *)(object + 0x3e) > 0) {
        do {
            if (arg2 != -3) {
                element = *(u8 **)(object + 0xb1) + (s32)index * 0x3b;
                if (*(s8 *)(element + 0x37) != arg2) {
                    record = *(u8 **)element;
                    if (arg4 == 0) {
                        if (*(s8 *)(record + 0x64) < arg1) {
                            *(s8 *)(record + 0x64) = arg1;
                        }
                    } else if (arg1 < *(s8 *)(record + 0x64)) {
                        *(s8 *)(record + 0x64) = arg1;
                    }
                }
            }
            if (arg3 != -3) {
                element = *(u8 **)(object + 0xb1) + (s32)index * 0x3b;
                if (*(s8 *)(element + 0x37) == arg3) {
                    record = *(u8 **)element;
                    if (arg4 == 0) {
                        if (*(s8 *)(record + 0x64) < arg1) {
                            *(s8 *)(record + 0x64) = arg1;
                        }
                    } else if (arg1 < *(s8 *)(record + 0x64)) {
                        *(s8 *)(record + 0x64) = arg1;
                    }
                }
            }
            index = (s16)(index + 1);
        } while (index < *(s16 *)(object + 0x3e));
    }
}
