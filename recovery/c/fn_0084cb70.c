/* fn_0084cb70 -- CM0102 3.9.68, preferred-image VA 0x0084cb70 (image base
   0x00400000), 67 bytes, one dword stack argument ("ret 4").
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Shape: the routine exits at once when the dword at [this+0x28] is zero or
   when the low byte of the stack argument, as a signed byte, is outside
   0..9. Otherwise ESI addresses slot this + index * 4; a null slot exits.
   A live item receives three virtual calls through its vtable, each with the
   item pointer reloaded from the slot and pushed as the first stack
   argument: +0x48 (one word), +0x34 (item, 0) and +0x08 (one word). The slot
   is then cleared. The routine ends with pop esi; ret 4.

   Shape of the three indirect calls.  The words pushed at 0084cb93,
   0084cb99/0084cb9b and 0084cba3 are never removed by this function; the
   only pops are "pop esi" (0084cbaf) and "ret 4" (0084cbb0).  The stack can
   only balance if the three callees pop the 4 + 8 + 4 bytes pushed for them,
   so every callee takes its arguments on the stack and pops them itself: the
   item pointer is the first argument and the 0 pushed at 0084cb99 is the
   second one at 0084cb9e.

   ECX and EDX carry the dispatch base only.  0084cb91 loads the item's
   vtable word into ECX, 0084cb9c reloads that word into EDX and 0084cba4
   reloads it into ECX again; the receiver of each call is the item pointer
   that was pushed for it, so no register receiver is invented here.

   ECX at 0084cb9e is the residue the callee at 0084cb94 left in that
   register: no instruction of this function establishes it, the callee's
   own contract is unknown, and plain C cannot read a register back.  That
   value is therefore not fabricated (no null, no reuse of the dispatch
   word).  What the instructions do establish for 0084cb9e is the dispatch
   base in EDX, the two pushed stack arguments and the callee cleanup, and
   that is exactly what this call reproduces.

   The result of the function is EAX after the call at 0084cba6, i.e. that
   callee's return value, and 0 on the early exits, exactly as the listing
   leaves EAX.  The item pointer loaded at 0084cba1 is the pre-call value
   of EAX; it is not the result and is never returned as one. */

#include "cmre.h"

/* Indirect slot shapes derived from the pushes and from the callee cleanup
   the stack accounting requires (see the comment above). */
typedef u32 (CM_STDCALL *cm_slot_1_t)(void *item);
typedef u32 (CM_STDCALL *cm_slot_2_t)(void *item, u32 arg2);

u32 CM_THISCALL cm_fn_0084cb70(void *self, u32 arg1)
{
    volatile u32 *slot;
    u32 eax;
    u32 al;
    u32 item;
    u32 ecx_word;
    u32 edx_word;
    cm_slot_1_t target;

    /* 0084cb70 mov eax, dword ptr [ecx + 0x28] */
    eax = *(volatile u32 *)((u8 *)self + 0x28);
    /* 0084cb74 test eax, eax / 0084cb76 je 0x84cbaf (EAX = 0) */
    if (eax == 0) {
        return 0;
    }

    /* 0084cb78 mov al, byte ptr [esp + 8]: only AL is replaced, the rest
       of EAX keeps the word read at 0084cb70. */
    al = (u8)arg1;
    eax = (eax & 0xffffff00u) | (u32)al;
    /* 0084cb7c test al, al / 0084cb7e jl 0x84cbaf */
    if ((s8)al < 0) {
        return eax;
    }
    /* 0084cb80 cmp al, 0xa / 0084cb82 jge 0x84cbaf (signed compare; AL is
       0..127 here because the jl above already left) */
    if ((s8)al >= 10) {
        return eax;
    }

    /* 0084cb84 movsx eax, al / 0084cb87 lea esi, [ecx + eax*4] */
    slot = (volatile u32 *)((u8 *)self + (u32)(s32)(s8)al * 4u);
    /* 0084cb8a mov eax, dword ptr [ecx + eax*4] */
    eax = *slot;
    /* 0084cb8d test eax, eax / 0084cb8f je 0x84cbaf (EAX = 0) */
    if (eax == 0) {
        return 0;
    }
    item = eax;

    /* 0084cb91 mov ecx, dword ptr [eax]: the dispatch base word. */
    ecx_word = *(volatile u32 *)item;
    /* 0084cb93 push eax / 0084cb94 call dword ptr [ecx + 0x48]: the item
       pointer is the only argument and the callee pops it. */
    target = (cm_slot_1_t)((volatile u32 *)ecx_word)[0x48 / 4];
    (void)target((void *)item);

    /* 0084cb97 mov eax, dword ptr [esi]: the slot is reloaded, the callee
       above may have changed it. */
    item = *slot;
    /* 0084cb99 push 0 / 0084cb9b push eax */
    /* 0084cb9c mov edx, dword ptr [eax]: the dispatch base word.  ECX at
       0084cb9e is the residue the callee at 0084cb94 left in it; nothing in
       this function establishes that value and it is not a receiver here,
       so it is not fabricated. */
    edx_word = *(volatile u32 *)item;
    /* 0084cb9e call dword ptr [edx + 0x34]: item pointer first, the 0
       pushed at 0084cb99 second, both popped by the callee. */
    {
        cm_slot_2_t target2 = (cm_slot_2_t)((volatile u32 *)edx_word)[0x34 / 4];
        (void)target2((void *)item, 0);
    }

    /* 0084cba1 mov eax, dword ptr [esi] / 0084cba3 push eax */
    item = *slot;
    /* 0084cba4 mov ecx, dword ptr [eax]: the dispatch base word. */
    ecx_word = *(volatile u32 *)item;
    /* 0084cba6 call dword ptr [ecx + 8]: EAX after this call is the result
       of the function; the item pointer of 0084cba1 is only the pre-call
       value of EAX and is not returned. */
    target = (cm_slot_1_t)((volatile u32 *)ecx_word)[8 / 4];
    eax = target((void *)item);
    /* 0084cba9 mov dword ptr [esi], 0 */
    *slot = 0;

    return eax;
}
