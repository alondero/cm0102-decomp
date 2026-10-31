/* Preferred-image VA 0x005FA730, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object; its first dword points at a table of
   0x22-byte entries and its word at +4 is the entry count). Five stack
   arguments, ret 0x14, returns a byte in AL.
   Scan: an entry matches when its first dword equals arg1 and its word at +4
   equals arg2 or is zero. Kind byte +0x1E < 0x80 selects the indexed pair at
   entry + kind*8 + 6 / + 10. Otherwise, kind 0xFD, kind 0xFF, or arg5 != 0
   selects the pair at +6 / +10. Other entries are skipped.
   On a hit, *arg3 receives the first dword, *arg4 the second; the return is
   (*arg3 != -1) + (second != -1), re-reading *arg3 after the *arg4 store.
   No match: *arg3 and *arg4 untouched, return 0. The loop bound is the count
   read once at entry. Field meanings remain unknown. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_005fa730(u8 *object, s32 arg1, s16 arg2, s32 *arg3, s32 *arg4, s32 arg5)
{
    u8 *entry;
    s16 count;
    s16 index;
    s32 slot;
    s32 value;
    u8 kind;

    entry = (u8 *)*(volatile u32 *)object;
    count = *(volatile s16 *)(object + 4);
    index = 0;
    if (count > 0) {
        do {
            if (*(s32 *)entry == arg1 &&
                (*(s16 *)(entry + 4) == arg2 || *(s16 *)(entry + 4) == 0)) {
                kind = entry[0x1e];
                if (kind < 0x80) {
                    slot = (s8)kind;
                    *arg3 = *(s32 *)(entry + slot * 8 + 6);
                    value = *(s32 *)(entry + slot * 8 + 10);
                    goto store_value;
                }
                if (kind == 0xfd || kind == 0xff || arg5 != 0) {
                    *arg3 = *(s32 *)(entry + 6);
                    value = *(s32 *)(entry + 10);
                    goto store_value;
                }
            }
            index = (s16)(index + 1);
            entry += 0x22;
        } while (index < count);
    }
    return 0;

store_value:
    *arg4 = value;
    return (u8)((*arg3 != -1) + (value != -1));
}
