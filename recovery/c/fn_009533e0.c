#include "cmre.h"

/* Original call site: preferred-image VA 0x00953426 in fn_009533e0
   (pinned CM0102 3.9.68 image, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   The eight pushed words are, right to left: option 1, the DWORD at
   0x00de4408, capacity 2, the destination pointer, count 2, the source
   pointer, flags 0x200 and the DWORD at 0x00de4624. */
extern s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags, const char *source,
                                    s32 count, void *destination, s32 capacity,
                                    u32 code_page, s32 option);

/* fn_009533e0 pushes ECX at 0x009533e3 to reserve the two-byte destination
   slot at EBP-4 and reads that slot back at 0x00953432, so the slot starts
   out holding the incoming ECX and the conversion callee may leave it
   untouched.  ECX is therefore read before it is written, which the
   register-parameter entry captures in the third parameter; the fourth
   parameter is the stack argument the original reads at EBP+8.  The entry
   keeps the original caller-cleanup of the stack arguments (RET 0 at
   0x0095345a), so it cannot use the fastcall or thiscall convention, which
   pops the stack argument and would leave the caller's stack four bytes
   higher than the original leaves it; the register parameters are therefore
   taken in EAX, EDX and ECX order, and the first two are unused by the
   original body.

   Boundary: the original spends eight bytes below its frame pointer (the
   reserved ECX slot and the saved EBX), while a compiled caller has to copy
   the incoming register parameters into its own frame, so its frame is
   deeper.  The nested conversion callee probes the stack below that frame,
   so the probe address of a faulting case moves with the compiled caller's
   frame depth.  The fault itself is unchanged: the nested callee faults at
   the same original instruction with the same error code and access width,
   and the harness compares those, not the absolute stack address. */
u32 __attribute__((regparm(3))) cm_fn_009533e0(u32 unused_eax, u32 unused_edx,
                                               u32 dest_slot, u32 value)
{
    (void)unused_eax; /* EAX is not read by the original body. */
    (void)unused_edx; /* EDX is not read by the original body. */
    if (value <= 0xffu) {
        /* 0x00953441: the flag byte of the entry, then its mapped byte. */
        if ((*(volatile u8 *)(0x00de4521u + value) & 0x20u) == 0x20u)
            return *(volatile u8 *)(0x00de4420u + value);
        return value;
    }
    {
        /* 0x009533e5 keeps the argument in EBX while the argument slot
           itself is overwritten below, so the fallback results still report
           the argument as it arrived. */
        u32 original = value;
        /* 0x009533f2 and 0x009533f8 overwrite the two upper bytes of the
           caller's stack argument with the low two bytes of the value. */
        ((volatile u8 *)&value)[3] = (u8)original;
        ((volatile u8 *)&value)[2] = (u8)(original >> 8);
        /* 0x009533fe: the lead byte selects a flag byte in the same table. */
        if ((*(volatile u8 *)(0x00de4521u + ((original >> 8) & 0xffu)) & 4u) != 0u) {
            /* 0x00953426: the conversion writes into the reserved slot.
               The arguments are read in the order the original pushes
               them, so the code page is read before the locale. */
            if (cm_fn_0094c0fe(CM_GLOBAL(volatile u32, 0x00de4624u), 0x200u,
                               (const char *)((volatile u8 *)&value) + 2, 2,
                               (void *)&dest_slot, 2,
                               CM_GLOBAL(volatile u32, 0x00de4408u), 1) != 0) {
                /* 0x00953432: the slot bytes are combined big-endian. */
                return ((dest_slot & 0xffu) << 8) + ((dest_slot >> 8) & 0xffu);
            }
        }
        return original;
    }
}
