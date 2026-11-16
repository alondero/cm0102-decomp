/* fn_00938cc0 (VA 0x00938cc0, preferred image base 0x00400000) -- plain C
   reconstruction of the 113-byte body ending at 0x00938d30.

   Receiver / ECX transport.  Every indirect call in the listing is a
   `call dword ptr [mem]` that runs with a live ECX the original never
   re-materialises at the call site:

     call [esi+0x46]  (0x00938cfa)  ECX = the value loaded at 0x00938cf1, i.e.
                                    second_table[index] (also pushed as the last
                                    stack argument)
     call [ebx+0x18]  (0x00938d05)  ECX is not written after 0x00938cf1: the
                                    value carried out of the previous indirect
                                    call.  The argument home is reloaded into
                                    ECX only afterwards (0x00938d08).
     call [esi+0x42]  (0x00938d15)  ECX = the value reloaded at 0x00938d08 from
                                    [esp+0x2c], which with six outstanding
                                    pushes is the caller-owned argument home
                                    [entry esp+4]; its live content is the
                                    first indirect result
     call [esi+0x42]  (0x00938d26)  ECX = the popped stack cursor computed at
                                    0x00938cc8/0x00938cce and stored to
                                    [esi+0x20] at 0x00938cd6 (nothing rewrites
                                    ECX on this branch)

   A cdecl call hands the callee whatever ECX the compiler happens to leave --
   the earlier draft forwarded the caller's incoming ECX and the callback
   faulted.  The slots are therefore declared regparm(3): the first three
   integer arguments reach the callee in EAX, EDX and ECX, the remaining ones
   are pushed right to left, exactly the listing's push order for the same
   values.  regparm keeps cdecl argument ownership, so the pushes before each
   call belong to the caller, matching the original's own `add esp, 0x18` and
   `add esp, 0x10` cleanup (the callees pop nothing).

   Register values at each call site, from the listing:
     call [esi+0x46]: EAX = first_table[index] (0x00938cf4),
                      EDX = first_table (0x00938cee, not written again),
                      ECX = second_table[index] (0x00938cf1)
     call [ebx+0x18]: EAX = first indirect result (0x00938cfd pushes it and
                      0x00938d01 spills it),
                      ECX = carried out of the first indirect call
     call [esi+0x42]: EAX = the difference computed at 0x00938d0f,
                      ECX = the first indirect result (0x00938d08)
     else path:       EAX = 0 (0x00938d20), ECX = popped cursor

   EDX is written by the listing only at 0x00938cee, on the flag branch.  At
   the second and third boundaries it is whatever the preceding indirect
   callee left; on the else path it is the caller's incoming EDX.  Plain C
   cannot name either, so the boundary takes EDX as an explicit argument and
    currently substitutes values instead of transporting those live registers.
    This is an unresolved fidelity defect, not evidence that EDX is dead.
    The same unresolved boundary loses post-first-call ECX at 0x00938d05.

   The store at 0x00938d01 (`mov dword ptr [esp+0x2c], eax`) targets the
   caller-owned argument home.  Its in-function effect is the value reloaded
    into ECX at 0x00938d08. The intervening callback may change that home;
    the live reload, not a cached first result, supplies the subtraction and
    third callback's ECX. */

#include "cmre.h"

/* regparm(3): EAX, EDX, ECX then the pushed arguments, caller-cleaned. */
typedef u32 (__attribute__((regparm(3))) *cm_cb_reg3)(u32 a, u32 b, u32 c,
                                                      u32 d, u32 e, u32 f);
typedef u32 (__attribute__((regparm(3))) *cm_cb_reg3x4)(u32 a, u32 b, u32 c,
                                                        u32 d, u32 e, u32 f,
                                                        u32 g);

u32 CM_CDECL cm_fn_00938cc0(volatile u32 self_home)
{
    u32 self = self_home;
    u32 cursor;
    u32 callbacks;
    u32 points;
    u32 index;
    u32 flag;

    cursor = *(volatile u32 *)(self + 0x20);
    callbacks = *(volatile u32 *)(self + 0x28);
    cursor -= 4;
    points = *(volatile u32 *)(self + 8);
    *(volatile u32 *)(self + 0x20) = cursor;
    callbacks += 0x58;
    index = *(volatile u32 *)cursor;
    flag = *(volatile u8 *)(self + 4);
    *(volatile u32 *)(self + 0x34) = index;
    *(volatile u32 *)(self + 0x30) = index;

    if (flag & 1u) {
        u32 second_table = *(volatile u32 *)(points + 4);
        u32 first_table = *(volatile u32 *)points;
        u32 second = *(volatile u32 *)(second_table + index * 4);
        u32 first = *(volatile u32 *)(first_table + index * 4);
        u32 first_result;
        u32 mapped;
        u32 reloaded_home;

        first_result = ((cm_cb_reg3)*(volatile u32 *)(self + 0x46))
                           (first, first_table, second, self, first, second);
        /* 0x00938d01: entry ESP+4, not the object pointed to by self. */
        self_home = first_result;
        mapped = ((cm_cb_reg3)*(volatile u32 *)(callbacks + 0x18))
                      (first_result, first_table, second, self, first_result, 0);
        reloaded_home = self_home; /* 0x00938d08, after the second callback */
        return ((cm_cb_reg3x4)*(volatile u32 *)(self + 0x42))
                    (mapped - reloaded_home, first_table, reloaded_home,
                     self, points, index, mapped - reloaded_home);
    }

    return ((cm_cb_reg3x4)*(volatile u32 *)(self + 0x42))
               (0, points, cursor, self, points, index, 0);
}
