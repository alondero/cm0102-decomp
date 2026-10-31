#include "cmre.h"

/* Object (ECX) fields by byte offset: 0x00 base pointer P, 0x08 signed element
   count. Element i (stride 0x17) has a dword at P+0x08+0x17*i compared with the
   key, and a word at P+0x11+0x17*i that is searched in the byte list at
   0x00a17be0 (signed-byte values, list ends at 0xff) and returned on a match.
   Stack argument: object with a dword key at +0x00 and a dword at +0x39 that
   must be nonzero. */
#define OBJ_U32(p, off) (*(volatile u32 *)((p) + (off)))

u16 CM_THISCALL cm_fn_007357c0(u32 self, u32 arg)
{
    s32 index;
    s32 count;
    u32 base;
    u32 key;
    u32 record;
    u32 scan;
    u16 word;
    u8 c;

    if (arg != 0u && OBJ_U32(arg, 0x39) != 0u) {
        count = (s32)OBJ_U32(self, 0x08);
        index = 0;
        if (count > 0) {
            base = OBJ_U32(self, 0x00);
            key = OBJ_U32(arg, 0x00);
            do {
                record = base + 0x11u + (u32)index * 0x17u;
                if (*(volatile u32 *)(record - 9u) == key) {
                    if (CM_GLOBAL(volatile u8, 0x00a17be0) != 0xffu) {
                        word = *(volatile u16 *)record;
                        scan = 0x00a17be0u;
                        do {
                            c = *(volatile u8 *)scan;
                            scan = scan + 1u;
                            if (word == (u16)(s16)(s8)c) {
                                return *(volatile u16 *)(base + 0x11u + (u32)index * 0x17u);
                            }
                        } while (*(volatile u8 *)scan != 0xffu);
                    }
                }
                index = index + 1;
            } while (index < count);
        }
    }
    return 0u;
}
