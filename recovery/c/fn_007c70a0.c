/* Preferred-image VA 0x007c70a0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Listing (0x007c70a0):
     cmp dword ptr [esi],0 / je            -> *this == 0 : return
     mov eax,dword ptr [esi+0x18] / jle    -> signed count guard
     loop:  mov eax,[esi] ; mov eax,[edi+eax]            -> field +0
            call 0x9452ca (if field != 0)
            mov ecx,[esi] ; mov eax,[edi+ecx+0xe]         -> field +0x0e
            call 0x9452ca (if field != 0)
            mov eax,[esi+0x18] ; inc ebx ; add edi,0x12   -> count reload, index, byte step
     mov edx,[esi] ; call 0x9452ca                        -> final release of *this

   The loop keeps the original byte step 0x12 and the 0x0e offset inside the
   record, reloads *this and the signed count on every iteration, and keeps the
   two nested releases in listing order. The loop state is kept in plain
   locals and every object access is a volatile DWORD load so that the load
   order (and the fault site) matches the original.

   Physical entry ABI: EAX is passed through on the null branch at 0x007c70a6,
   EDX is unused at entry, and ECX is self (0x007c70a1). GCC i386 regparm(3)
   binds those registers in that order, with no stack arguments; cdecl retains
   the plain RET at 0x007c70f3. These are register inputs, not extra stack slots.
   On the non-null path 0x007c70e9 supplies EAX; the epilogue does not change it. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

u32 __attribute__((regparm(3), cdecl))
cm_fn_007c70a0(u32 entry_eax, u32 unused_edx, u32 self)
{
    volatile u32 *obj = (volatile u32 *)self;
    (void)unused_edx;

    if (obj[0] == 0)
        return entry_eax;

    if ((s32)obj[6] > 0) {
        u32 off = 0;
        s32 idx = 0;
        do {
            u32 f;

            f = CM_GLOBAL(volatile u32, obj[0] + off);
            if (f)
                cm_fn_009452ca(f);

            f = CM_GLOBAL(volatile u32, obj[0] + off + 0x0eu);
            if (f)
                cm_fn_009452ca(f);

            ++idx;
            off += 0x12u;
        } while (idx < (s32)obj[6]);
    }

    return (u32)cm_fn_009452ca(obj[0]);
}
