#include "cmre.h"

/* Preferred-image VA 0x0093a240, image base 0x00400000, 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cases follow the byte selector at VA 0x0093a3bc and jump table 0x0093a380.
   Comparison cases write a Boolean but leave the original left operand in EAX.
   Unknown operators consume both operands without replacing either one. */
extern u32 CM_CDECL cm_fn_00944760(s32 a, s32 b);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))

u32 CM_CDECL cm_fn_0093a240(u32 context)
{
    u32 stack = U32_AT(context, 0x20);
    u32 opcode = U32_AT(context, 4);
    u32 right = U32_AT(stack - 4u, 0);
    u32 left = U32_AT(stack - 8u, 0);
    u32 result = left;
    u32 stored, target, selector, index = opcode - 0x50u;

    stack -= 8;
    if (index > 0x3cu) {
        U32_AT(context, 0x20) = stack;
        return result;
    }
    selector = CM_GLOBAL(volatile u8, 0x0093a3bcu + index);
    target = CM_GLOBAL(volatile u32, 0x0093a380u + selector * 4u);
    switch (target) {
    case 0x0093a273: stored = (s32)left < (s32)right; break;
    case 0x0093a286: stored = (s32)left <= (s32)right; break;
    case 0x0093a299: stored = (s32)left > (s32)right; break;
    case 0x0093a2ac: stored = (s32)left >= (s32)right; break;
    case 0x0093a2bf: stored = left == right; break;
    case 0x0093a2d2: stored = left != right; break;
    case 0x0093a2e5: stored = result = left != 0 && right != 0; break;
    case 0x0093a2fe: stored = result = left != 0 || right != 0; break;
    case 0x0093a314: stored = result = left + right; break;
    case 0x0093a322: stored = result = left - right; break;
    case 0x0093a346: stored = result = (s32)(left << 6) / (s32)right; break;
    case 0x0093a330: stored = result = cm_fn_00944760((s32)left, (s32)right); break;
    case 0x0093a359: stored = result = (s32)left < (s32)right ? right : left; break;
    case 0x0093a36b: stored = result = (s32)left > (s32)right ? right : left; break;
    default:
        U32_AT(context, 0x20) = stack;
        return result;
    }
    U32_AT(stack, 0) = stored;
    U32_AT(context, 0x20) = stack + 4u;
    return result;
}
