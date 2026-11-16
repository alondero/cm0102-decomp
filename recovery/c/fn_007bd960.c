#include "cmre.h"

/* fn_007bd960 - preferred-image VA 0x007bd960 (CM 01/02 3.9.68).
 *
 * The original saves the receiver (ECX) on the stack before it reads the two
 * receiver bytes:
 *
 *     007bd960  push ecx                    ; [E-4] = the receiver
 *     007bd961  mov  dl, byte ptr [ecx+1a0]
 *     007bd967  push ebx                    ; [E-8] = EBX
 *     007bd968  mov  bl, byte ptr [ecx+1a1]
 *
 * E is the stack pointer the callee is entered with.  When the receiver
 * object overlaps that stack, the two byte reads observe the pushed register
 * bytes instead of the object fields.  A rebuilt C frame has another layout,
 * so the overlap is reproduced here: a receiver field that falls inside the
 * pushed ECX slot reads the pushed byte.
 */

long double CM_THISCALL cm_fn_007bd960(u32 self)
{
    u32 cfa;               /* the DWARF CFA: the caller's stack pointer */
    u32 frame;             /* the frame pointer the prologue kept */
    u32 entry_stack;       /* E: the stack pointer the callee was entered with */
    u32 pushed_ecx;        /* E-4: the original's "push ecx" slot */
    u32 e_cfa;             /* E derived from the CFA */
    u32 e_frame;           /* E derived from the frame pointer */
    u8 dl;                 /* DL, the cursor byte */
    u8 bl;                 /* BL, the count byte */
    u32 acc;               /* ESI, the accumulator (its low 16 bits are used) */
    s32 remaining;         /* EDI, the loop counter */
    u32 eax;               /* EAX */
    u32 ebp;               /* EBP */
    u32 addr;              /* the table byte address */
    u32 table;             /* the table byte */

    /* The two receiver bytes, read in the original's order. */
    dl = *(volatile u8 *)(self + 0x1a0u);
    bl = *(volatile u8 *)(self + 0x1a1u);

    /* Find E.  On x86 the DWARF CFA is the caller's stack pointer, that is
       E+4, because the call pushed the return address at E.  A prologue that
       keeps a frame pointer stores the old EBP at E-4, so EBP+4 is E too.
       Both forms are used when the build provides them. */
    cfa = 0u;
#if defined(__has_builtin)
#  if __has_builtin(__builtin_dwarf_cfa)
    cfa = (u32)__builtin_dwarf_cfa();
#  endif
#endif
#if defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wframe-address"
#endif
    frame = (u32)__builtin_frame_address(0);
#if defined(__GNUC__)
#  pragma GCC diagnostic pop
#endif
    e_cfa = cfa ? (cfa - 4u) : 0u;
    e_frame = frame + 4u;
    if (e_cfa != 0u && (e_cfa - e_frame) <= 4u && (e_frame - e_cfa) <= 4u)
        entry_stack = e_frame;     /* the frame pointer confirms the CFA */
    else if (e_cfa != 0u)
        entry_stack = e_cfa;
    else
        entry_stack = e_frame;
    pushed_ecx = entry_stack - 4u;

    /* "push ecx" left the receiver in [E-4]; a receiver field that overlaps
       that slot reads the pushed byte instead of the field.  The original's
       "push ebx" at [E-8] is not observable in a rebuilt frame, so only the
       ECX slot is reproduced. */
    if ((self + 0x1a0u) - pushed_ecx < 4u)
        dl = (u8)((self >> (8u * ((self + 0x1a0u) - pushed_ecx))) & 0xffu);
    if ((self + 0x1a1u) - pushed_ecx < 4u)
        bl = (u8)((self >> (8u * ((self + 0x1a1u) - pushed_ecx))) & 0xffu);

    acc = 0u;                          /* xor esi, esi */
    remaining = (s32)(s8)bl;           /* movsx edi, bl */
    if (remaining > 0) {
        do {
            eax = (u32)(s32)(s8)dl;            /* movsx eax, dl */
            ebp = eax + eax * 4u;              /* lea ebp, [eax + eax*4] */
            eax = eax + self;                  /* add eax, ecx */
            addr = eax + ebp * 8u + 0x1bu;     /* the table byte address */
            table = (u32)*(volatile u8 *)addr; /* movzx ax, byte [addr] */
            eax = (eax & 0xffff0000u) | table; /* movzx replaces AX only */
            acc = acc + eax;                   /* add esi, eax */

            dl = (u8)(dl + 1u);                /* inc dl */
            if (dl == 10u)                     /* cmp dl, 0xa */
                dl = 0u;                       /* xor dl, dl */
            --remaining;                       /* dec edi */
        } while (remaining != 0);
    }

    if (bl == 0u)                              /* test bl, bl / je */
        return (long double)CM_GLOBAL(volatile double, 0x009689a0u);

    /* movsx ecx, si / movsx edx, bl / fild / fild / fdivp st(1) */
    return (long double)(s32)(s16)(u16)acc / (long double)(s32)(s8)bl;
}
