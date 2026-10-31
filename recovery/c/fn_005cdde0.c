#include "cmre.h"

/* Object (ECX): dword at 0x00 is the base address of a table of 0x49-byte records.
   Argument 1 points to a dword record index. Each record has a signed word at +6
   that is adjusted by argument 2 and then clamped to the range 1..10000. */

#define RECORD_SIZE 0x49u
#define RECORD_WORD_OFFSET 6u

void CM_THISCALL cm_fn_005cdde0(u32 self, u32 index_ptr, u16 delta)
{
    s16 value;
    u32 index;
    u32 base;
    u32 entry;

    if (index_ptr == 0u) {
        return;
    }

    index = *(volatile u32 *)index_ptr;
    base = *(volatile u32 *)self;
    entry = base + index * RECORD_SIZE + RECORD_WORD_OFFSET;
    *(volatile u16 *)entry = (u16)(*(volatile u16 *)entry + delta);

    /* Re-read the index and base: the store above may overlap them, and the
       listing reloads both (ECX is reused for the base). */
    index = *(volatile u32 *)index_ptr;
    base = *(volatile u32 *)self;
    entry = base + index * RECORD_SIZE + RECORD_WORD_OFFSET;
    value = (s16)*(volatile u16 *)entry;

    if (value < 1) {
        *(volatile u16 *)entry = 1u;
        return;
    }
    if (value > 10000) {
        *(volatile u16 *)entry = 10000u;
        return;
    }
    *(volatile u16 *)entry = (u16)value;
}
