/* fn_0090b080 - VA 0x0090b080 (preferred image base 0x00400000), 30 bytes,
   CM0102 3.9.68, pinned binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI evidence from the original instructions (preferred-image VAs of that
   pinned binary):
   - 0x0090b081 `mov esi, ecx` reads ECX before it is written, so ECX carries
     the receiver: __thiscall.
   - 0x0090b09b `ret 4` pops 4 stack bytes: one 4-byte stack argument.
   - 0x0090b080 `push esi` leaves ESP = entry_esp-4, so 0x0090b088
     `test byte ptr [esp + 8], 1` reads entry_esp+4 - the low byte of that
     4-byte argument.  The optional call is taken when bit 0 of that byte is
     set.
   - 0x0090b080 `push esi` / 0x0090b09a `pop esi` are the wrapper's only frame
     slot: the frame is 4 bytes deep and holds a copy of the receiver for the
     whole body.
   - 0x0090b083 calls 0x0090b0a0 with the receiver still in ECX.  That callee
     takes its receiver from ECX (0x0090b0b7 `mov esi, ecx`) and pops no
     stack bytes itself (0x0090b168 `ret`), so the pushed ESI at 0x0090b080 is
     a register save, not an argument for it.
   - 0x0090b08f `push esi`, 0x0090b090 `call 0x00944c94`, 0x0090b095
     `add esp, 4`: 0x00944c94 is cdecl with one stack argument, the receiver.
   - 0x0090b098 `mov eax, esi`: the result is the receiver on both paths.
   - Order: the 0x0090b0a0 call (0x0090b083) precedes the flag test
     (0x0090b088), and the 0x00944c94 call (0x0090b090) is the optional second
     call.  This function reads and writes no memory of its own.

   BOUNDARY - harness-observed stack-layout divergence, not a defect of this
   translation (same pinned binary and preferred-image VAs):
   The free path reached from 0x0090b0a0 passes stack words to the free
   helper: `cmre explain fn_0090b080 --c --case 392` records a 4-byte stack
   read and the matching KERNEL32.dll!HeapFree call with the same value, for
   consecutive stack words (0x0203efcc upward in that case, ending in the
   shared fault on 0x0203f000).  The two sides free different words:
     -O2 (difference at compared-event index 10): the original frees 0x03000030
   - the receiver, which its own 4-byte frame (0x0090b080 `push esi`) leaves on
   the stack - while the recovered build frees 0x7fffffff, the stack argument
   the harness passed, out of a compiler-generated frame slot; the callee's
   recorded frame writes are displaced by 8 bytes.
     -O0 (difference at compared-event index 6): the original frees 0x33255e1e
   and the recovered build 0x53544b00, again stack words; the frame writes are
   displaced by 24 bytes and the recovered EBP is the compiler's frame pointer
   (0x0203effc) where the original keeps the incoming EBP (0x33255e1e), the
   original never writing EBP in its 4-byte frame.
   The cause is the frame depth, not the arguments: no value this wrapper
   passes is mis-passed, and both sides fault at the same address with the
   same final registers apart from the stack displacement and, at -O0, the
   compiler's frame pointer in EBP.  A plain-C source
   cannot reproduce the original's frame here: the freed word is the receiver
   copy inside the original's saved-register slot, a property of the original
   prologue (0x0090b080) rather than a value this wrapper computes, and no
   plain-C construct binds a compiler-generated frame slot to a chosen value
   at a chosen offset - at -O0 the compiler always emits a frame pointer plus
   callee-saved saves, so the original's 4-byte frame is not reproducible at
   all.  Zeroing that slot, adding a stack argument or adding a local would
   change the original's behaviour.
   The harness classifies exactly these two cases (392 and 445 of 600) as
   stack_layout rather than mismatches, and `cmre test fn_0090b080 --c`
   passes at -O0 and -O2 with coverage 1.0 and no mismatch.  No behavioural
   repair is claimed for those two cases; the wrapper's own behaviour - call
   order, the byte-wide flag test, EAX = receiver, `ret 4` - is reproduced
   exactly. */

#include "cmre.h"

extern void CM_THISCALL cm_fn_0090b0a0(u32 self);
extern u32 CM_CDECL cm_fn_00944c94(u32 block);

u32 CM_THISCALL cm_fn_0090b080(u32 self, u32 flags)
{
    cm_fn_0090b0a0(self);        /* 0x0090b083 */
    if ((u8)flags & 1u)          /* 0x0090b088: low byte of the stack argument */
        cm_fn_00944c94(self);    /* 0x0090b08f..0x0090b095 */
    return self;                 /* 0x0090b098 */
}
