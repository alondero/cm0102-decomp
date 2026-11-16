/* fn_007ca490 - preferred-image VA 0x007ca490 (image base 0x00400000), 80 bytes.
 *
 * Shape: with `this` kept in ESI, the routine calls vtable slot +0xa4 of
 * the object [this->f0c + 4], then slot +0xa4 of the table entry
 * dword[0xadadfc][dword[0x9cf85c]]. It then calls the thiscall 0x00689c80
 * on `this` with the stack words ([this->f0c + 4], that table entry, 1, 1,
 * -1, -1), the table entry and object both reloaded from memory, and returns
 * with pop esi; ret 4.
 *
 * Entry: ECX is read before written, so ECX is the receiver (`this`); `ret 4`
 * pops one stack word, so the entry also carries one stack argument.
 */
#include "cmre.h"

/* 0x00689c80: thiscall, six stack words - source, target, 1, 1, -1, -1.
 * The last two are byte words: the original pushes the dword -1
 * (0xffffffff), so the byte the callee reads is 0xff. */
extern u32 CM_THISCALL cm_fn_00689c80(u32 self, volatile u32 source_arg, volatile u32 target_arg, volatile u32 clear_arg, volatile u32 pair_arg, volatile u8 expected3, volatile u8 expected2);

/* The vtable slot at +0xa4 is a thiscall member that also consumes two
 * stack words: it reads [ESP+4] (instruction 0x00685d38) and [ESP+8], and
 * returns with `ret 8`, so it pops both words.  The words are the caller's
 * own stack, not values the listing pushes; see the call-boundary note in
 * cm_fn_007ca490 below. */
typedef u32 (CM_THISCALL *cm_slot_a4)(u32 self, u32 word0, u32 word1);

u32 CM_THISCALL cm_fn_007ca490(u32 self, u32 arg)
{
    u32 p, object, table, base, index, target;

    /* 0x007ca493..0x007ca49b: vtable slot +0xa4 of [this->f0c + 4].
     *
     * Call boundary.  The original's frame holds, at the slot call,
     *   [ESP+4] = the ESI saved by `push esi` (the incoming ESI), and
     *   [ESP+8] = this function's caller's return address,
     * and the callee pops both (`ret 8`), so the original's stack pointer
     * advances by 8 per slot call.  A compiler-generated C frame does not
     * reproduce those two words: the earlier translation left 0 in [ESP+4],
     * the callee took its invalid-argument path and showed a MessageBoxA
     * that the original never shows.  The words are therefore passed
     * explicitly:
     *   word0 - the saved incoming ESI is a register value plain C cannot
     *            name; the receiver of this method is the pointer the
     *            translation can supply that the callee may dereference at
     *            +0x42 (see 0x00685ed8 in the callee).
     *   word1 - __builtin_return_address(0) is exactly the word the
     *            original exposes there (the caller's return address). */
    p = *(volatile u32 *)(self + 0xc);
    object = *(volatile u32 *)(p + 4);
    table = *(volatile u32 *)object;
    ((cm_slot_a4)*(volatile u32 *)(table + 0xa4))(object, self, (u32)__builtin_return_address(0));

    /* 0x007ca4a1..0x007ca4b1: the same slot on the object at
     * [0x00adadfc + 0x009cf85c * 4].  Here the original's frame exposes
     *   [ESP+4] = the stack word this function pops with `ret 4`
     *              (the `arg` parameter), and
     *   [ESP+8] = the caller's next stack word,
     * so word0 is `arg` exactly; word1 is not nameable and the callee does
     * not dereference it (the original passes a non-dereferenced word). */
    base = CM_GLOBAL(volatile u32, 0x00adadfc);
    index = CM_GLOBAL(volatile u32, 0x009cf85c);
    object = *(volatile u32 *)(base + index * 4);
    table = *(volatile u32 *)object;
    ((cm_slot_a4)*(volatile u32 *)(table + 0xa4))(object, arg, 0);

    /* 0x007ca4b7..0x007ca4d7: reload the globals, then call 0x00689c80
     * with source = [this->f0c + 4], target = [0x00adadfc + 0x009cf85c*4]
     * and the words 1, 1, -1, -1. */
    base = CM_GLOBAL(volatile u32, 0x00adadfc);
    index = CM_GLOBAL(volatile u32, 0x009cf85c);
    target = *(volatile u32 *)(base + index * 4);
    p = *(volatile u32 *)(self + 0xc);
    object = *(volatile u32 *)(p + 4);
    return cm_fn_00689c80(self, object, target, 1, 1, 0xff, 0xff);
}
