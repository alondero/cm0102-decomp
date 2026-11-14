/* fn_008adbb0 - plain C reconstruction of the routine at preferred-image
 * VA 0x008adbb0 (cm0102.exe 3.9.68, image base 0x00400000).
 *
 * LISTING (VA, instruction), complete behaviour:
 *   008adbb0  mov ecx, dword ptr [esp+4]   ; selector = first stack argument
 *   008adbb4  mov eax, dword ptr [esp+8]   ; value    = second stack argument
 *   008adbb8..008add82  32 entries, K = 1,2,4,...,0x80000000:
 *       "cmp ecx, K / jne next ; test eax, eax / jne next ; mov al, IMM ; ret"
 *       A hit returns AL = index of K in the power-of-two series (0x00..0x1f),
 *       and is reachable only when eax == 0, so the chain is folded into
 *       "if (value == 0) compare selector".
 *   008add83  test ecx, ecx / jne 0x8ade71  ; second chain needs ecx == 0
 *   008add93..008ade70  16 entries, K = 1,2,4,...,0x8000:
 *       "cmp eax, K / jne next ; mov al, IMM ; ret"
 *       A hit returns AL = 0x20 + index of K.  The ecx==0 test is repeated
 *       before every entry; ecx is unchanged, so one guard is equivalent.
 *   008ade71  or al, 0xff
 *   008ade73  ret 0
 *   Every exit writes AL only, so EAX keeps the upper 24 bits of the second
 *   stack argument that "mov eax, dword ptr [esp+8]" loaded; the routine
 *   writes no other register.
 *
 * RETURN WIDTH AND THE CALLER-READ EDX (resolves the held concern):
 *   The original never writes EDX ("registers read before written: none";
 *   the only writes are ECX, EAX, AL).  EDX after the call is therefore the
 *   inherited caller value, and the direct caller at 0x0089de36 reads it.
 *   `cmre test` requires a 64-bit declaration for that caller, and its
 *   differential run shows the expected exit EDX is exactly the entry EDX
 *   (case 0: entry 0x9f6a8cc7 -> original exit 0x9f6a8cc7).  A 64-bit
 *   integer return is therefore the honest boundary type: its low half is
 *   the EAX the routine produces and its high half is the unchanged EDX.
 *
 *   Plain C cannot name EDX, so the inherited register state is kept as an
 *   explicit boundary parameter, as the rules require for forwarded
 *   registers: `regparm(2)` places the two leading parameters in EAX and
 *   EDX (the incoming register file) and leaves the two original stack
 *   arguments as parameters 3 and 4, which the compiler reads at
 *   [esp+4] / [esp+8] - the addresses the original uses - and cleans up
 *   with a plain "ret" (no stack pop), matching "ret 0".  No inline
 *   assembly, no code-generation shaping: only the declaration states
 *   which incoming registers are part of this boundary.
 *
 *   (CM_FASTCALL would also expose EDX, but GCC's fastcall pops the stack
 *   arguments with "ret 8", leaving exit ESP eight bytes above the
 *   original's, which the semantic run rejects.)
 */
#include "cmre.h"

unsigned long long __attribute__((regparm(2)))
cm_fn_008adbb0(u32 in_eax, u32 in_edx, u32 selector, u32 value)
{
    /* Power-of-two series shared by both chains: 1 .. 0x8000 (first 16),
     * extended to 0x80000000 for the first chain. */
    static const u32 first[] = {
        1u, 2u, 4u, 8u, 0x10u, 0x20u, 0x40u, 0x80u,
        0x100u, 0x200u, 0x400u, 0x800u, 0x1000u, 0x2000u, 0x4000u, 0x8000u,
        0x10000u, 0x20000u, 0x40000u, 0x80000u, 0x100000u, 0x200000u,
        0x400000u, 0x800000u, 0x1000000u, 0x2000000u, 0x4000000u, 0x8000000u,
        0x10000000u, 0x20000000u, 0x40000000u, 0x80000000u
    };
    u32 al;

    (void)in_eax;                 /* incoming EAX is not an input of this routine */

    /* Chain one: each entry needs eax == 0, so the test is hoisted. */
    if (value == 0) {
        for (u32 i = 0; i < 32u; ++i) {
            if (selector == first[i]) {
                al = i;                       /* mov al, index */
                goto done;
            }
        }
    }

    /* Chain two is guarded by "test ecx, ecx" and compares eax with 1..0x8000. */
    if (selector == 0) {
        for (u32 i = 0; i < 16u; ++i) {
            if (value == first[i]) {
                al = 0x20u + i;               /* mov al, 0x20 + index */
                goto done;
            }
        }
    }

    al = 0xffu;                       /* 008ade71: or al, 0xff */

done:
    /* Only AL was written; EAX keeps the upper 24 bits of the second argument. */
    return ((unsigned long long)in_edx << 32) | ((value & 0xffffff00u) | al);
}
