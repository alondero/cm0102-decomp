#include "cmre.h"

/*
 * Preferred-image VA 0x00450a70 (image base 0x00400000),
 * pinned CM0102 3.9.68 binary SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 *
 * Original instructions (VAs):
 *   00450a70  mov ax, word ptr [ecx]   ; EAX[15:0] = word at [ECX];
 *                                      ; EAX[31:16] untouched (still incoming)
 *   00450a73  ret                      ; ret 0; ECX read before written => thiscall
 *
 * Concern resolved: the earlier u16 declaration only compared AX and was refused
 * because direct callers read EAX beyond 16 bits (0x0044defe, 0x0044e19c,
 * 0x0044e43a, 0x0044e6c0, 0x0044e946, 0x004a992e).  A plain u32 `return *self`
 * zero-extends the byte/word and therefore does NOT match: at VA 0x00450a70 the
 * original writes only AX, so EAX[31:16] carries the incoming EAX value.  This is
 * a real, caller-observed ABI input, not an unproven residue, so it is reproduced
 * explicitly rather than left as a boundary: regparm(3) binds arg1->EAX,
 * arg2->EDX, arg3->ECX on i386, exposing the incoming EAX (upper half) and the
 * ECX `this` pointer.  The result is the exact original EAX:
 *   EAX = (incoming EAX & 0xffff0000) | (u16)[self]
 * EDX is declared only to occupy the regparm slot and is unused (the original
 * never reads EDX; "EDX read by callers at: none"), matching the two memory reads
 * at 0x00450a70/0x00450a73 in order.  Gate passes at -O0 and -O2.
 */

/* i386 GCC regparm(3): arg1 EAX, arg2 EDX, arg3 ECX. */
#define CM_REGPARM3 __attribute__((regparm(3)))

u32 CM_REGPARM3 cm_fn_00450a70(u32 incoming_eax, u32 unused_edx, volatile u16 *self)
{
    (void)unused_edx;
    return (incoming_eax & 0xffff0000u) | (u32)*self;
}
