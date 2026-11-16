/* fn_008a9c20 - preferred-image VA 0x008a9c20 (CM0102 3.9.68, image base 0x00400000).
 *
 * Recovered from the original listing only; no invented behaviour.
 *
 * Object layout (EBX = this, ECX on entry):
 *   +0x0e40  u32  guard word; non-zero -> return 0 without touching memory
 *   +0x0e44  block (copy source, also the callee's receiver)
 *   +0x4a5f  u32  set to 1 on the success path
 *   +0x4a63  block (copy destination)
 *   +0xc4de  u32  set to 1 before the callee call, cleared to 0 on the failure path
 *
 * Block transfer (0x008a9c4c/0x008a9c83): REP MOVSD with ECX = 0xf07 dword
 * elements (0x3c1c bytes), followed by one MOVSW (0x008a9c5b/0x008a9c85) and one
 * MOVSB (0x008a9c5d/0x008a9c87).  The trailing moves carry no REP prefix and no
 * ECX count: they move exactly one word and one byte.  The accesses are volatile
 * and of the original element width so that the access sizes and the fault
 * widths stay those of the original instructions.
 *
 * Direction flag remains unresolved: the original string instructions follow
 * DF, while the plain C copy below only models the clear-DF case. Neither the
 * listing nor a single tested entry establishes that DF is always clear.
 */
#include "cmre.h"

/* Evidenced callee (recovery ledger): thiscall, receiver in ECX, three stack
   words pushed by the caller and popped by the callee (ret 0xc). */
extern u32 CM_THISCALL cm_fn_005b5290(u32 self, u32 mode, u32 param_3, u32 param_4);

#define CM_BLOCK_DWORDS 0xf07u
#define CM_BLOCK_BYTES  0x3c1cu            /* CM_BLOCK_DWORDS * 4 */

/* 32-bit object accesses of the original width (the object fields are not
   naturally aligned, so they are read and written one dword at a time). */
static u32 cm_load_u32(const u8 *p)
{
    return *(const volatile u32 *)p;
}

static void cm_store_u32(u8 *p, u32 v)
{
    *(volatile u32 *)p = v;
}

/* The original block transfer: 0xf07 dword elements, then one word, then one
   byte, moved in the original order and with the original element widths. */
static void cm_copy_block(u8 *dst, const u8 *src)
{
    u32 i;

    for (i = 0; i < CM_BLOCK_DWORDS; i++)                          /* REP MOVSD */
        *(volatile u32 *)(dst + i * 4u) = *(const volatile u32 *)(src + i * 4u);
    *(volatile u16 *)(dst + CM_BLOCK_BYTES) =                      /* MOVSW */
        *(const volatile u16 *)(src + CM_BLOCK_BYTES);
    *(volatile u8 *)(dst + CM_BLOCK_BYTES + 2u) =                  /* MOVSB */
        *(const volatile u8 *)(src + CM_BLOCK_BYTES + 2u);
}

u32 CM_THISCALL
cm_fn_008a9c20(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    u8 *obj = (u8 *)self;
    u8 *src = obj + 0x0e44u;
    u8 *dst = obj + 0x4a63u;
    u32 ret;

    /* 0x008a9c25: mov eax, [ebx + 0xe40] ; test eax, eax ; je 0x008a9c39 */
    if (cm_load_u32(obj + 0x0e40u) != 0u)
        return 0u;                        /* 0x008a9c33: xor eax, eax */

    /* 0x008a9c39..0x008a9c5d: copy the block to obj + 0x4a63 */
    cm_copy_block(dst, src);

    /* 0x008a9c67: mov dword ptr [ebx + 0xc4de], esi (esi = 1) */
    cm_store_u32(obj + 0xc4deu, 1u);

    /* 0x008a9c6d: call 0x005b5290 with ECX = obj + 0xe44 and the three stack
       words arg1 (mode), arg2, arg3 pushed right to left. */
    ret = cm_fn_005b5290((u32)src, arg1, arg2, arg3);

    /* 0x008a9c72: test eax, eax ; jne 0x008a9c95 */
    if (ret == 0u) {
        /* 0x008a9c76..0x008a9c87: copy the block back from obj + 0x4a63 */
        cm_copy_block(src, dst);
        /* 0x008a9c8a: mov dword ptr [ebx + 0xc4de], eax (eax = 0) */
        cm_store_u32(obj + 0xc4deu, 0u);
        return 0u;
    }

    /* 0x008a9c95: mov dword ptr [ebx + 0x4a5f], esi (esi = 1)
       0x008a9c9b: mov eax, esi */
    cm_store_u32(obj + 0x4a5fu, 1u);
    return 1u;
}
