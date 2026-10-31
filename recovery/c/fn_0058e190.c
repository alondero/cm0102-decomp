/* Preferred-image VA 0x0058E190, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX = self) with one stack pointer (record); RET 4 pops it.
   self's first dword is a table base. record's dword at +0 is a row index and its
   signed word at +0x8e is a level. Row address = base + index*0x2c, and its signed
   word at +0x10 is the value. Returns 3 when level < 3000 and value < 50; else 2
   when level < 6000 and value < 15; else 1 when value < 21, 2 when value < 61, and
   otherwise 3 plus 1 when value > 100. The return is 8-bit (AL). Reads follow the
   listing's order and branch structure. Field and table meanings remain unknown. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_0058e190(u8 *self, u8 *record)
{
    s16 level;
    s16 value;
    u32 index;
    u8 *table;

    level = *(s16 *)(record + 0x8e);
    if (level < 3000) {
        index = *(u32 *)record;
        table = *(u8 **)self;
        if (*(s16 *)(table + (index * 0x2cu) + 0x10) < 0x32) {
            return 3;
        }
    }
    if (level < 6000) {
        index = *(u32 *)record;
        table = *(u8 **)self;
        if (*(s16 *)(table + (index * 0x2cu) + 0x10) < 0xf) {
            return 2;
        }
    }
    index = *(u32 *)record;
    table = *(u8 **)self;
    value = *(s16 *)(table + (index * 0x2cu) + 0x10);
    if (value < 0x15) {
        return 1;
    }
    if (value < 0x3d) {
        return 2;
    }
    return (u8)((value > 100) + 3);
}
