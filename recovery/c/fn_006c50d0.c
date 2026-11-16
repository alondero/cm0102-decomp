/* HELD: audit 1872 confirmed; this is NOT a repaired return contract.
   Original-instruction evidence, preferred-image VAs, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   006c51c7 CALL 007160e0; 006c51cc ADD ESI,1be; 006c51d2 DEC EBX;
   006c51d3 JNE 006c5124; 006c51d9..006c51de POPs and RET preserve EAX.
   Bound-target review (not composition or runtime proof): 007160e0 MOV AL,
   [ESP+4]; 007160e4 MOV [ECX+2b],AL; 007160e7 RET 4. Thus the last
   call preserves upper EAX, rather than producing an independent u32 result.
   Merely widening both declarations would still lose its incoming EAX.
   Widening 006f46b0 also cannot establish an independent full return:
   006f46b0 MOV AL,[ECX+102]; 006f46c0 CMP DL,AL; 006f46cc JNE 006f46da;
   006f46ce CMP BL,[ECX+103]; 006f46d4 JE 006f4910 can reach the
   006f4910..006f4919 epilogue with incoming upper EAX unchanged.
   Repair needs an authorized explicit register boundary or further evidenced
   deadness proof; neither is supplied by a passing void semantic test.
   No guessed return, duplicate callee field reads, or inline assembly added.
   This void boundary is not a proven recovery of the return contract.
   A semantic test of it cannot establish that forwarded upper EAX is dead.
   Declaring 007160e0 as returning u32 would capture its output, but would not
   preserve its incoming EAX: plain C does not pass that register to a thiscall
   callee. No arbitrary full return value or invented register input is used.
   Acceptance requires original-instruction evidence of upper-EAX deadness or
   an evidenced explicit boundary for every consumed incoming register.
   Prior void semantic test passes, but acceptance awaits the forwarded EAX
   contract: caller006c54a0 returns it to006b9dd1. That outer caller overwrites
   AL, forwards EAX into byte arguments, and has further partial AX arithmetic.
   The last native007160e0 call clears AL only; upper EAX liveness is unproven.
   Preferred-image VA 006c50d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_006ceb90(u32 self, u8 first, u8 second, u32 third);
extern unsigned long long CM_THISCALL cm_fn_006f0630(u32 self);
extern void CM_THISCALL cm_fn_006f46b0(u32 self, u8 first, u8 second);
extern void CM_THISCALL cm_fn_007160e0(u32 self, u32 zero);

void CM_THISCALL cm_fn_006c50d0(u32 self)
{
    u32 selected, argument, row, first, second, remaining;
    u8 side, mode, value;
    side = B(self + 0x8eaeu);
    /* 006c50d0 saves ECX in the local word. After four register saves
       and two pushes, [ESP+18h] at 006c50e1 is that same word, not
       uninitialized scratch: the byte store replaces only its low byte.
       All instruction VAs here use the pinned image identified above. */
    argument = (self & 0xffffff00u) | side;
    selected = cm_fn_006ceb90(self, (u8)argument, 5, 0);
    cm_fn_006f0630(selected);
    mode = side != 0 ? 1 : 10;
    B(self + 0x8ea7u) = 4;
    B(self + 0x8ea8u) = mode;
    cm_fn_006f46b0(selected, 4, mode);
    row = self + 0x4899u;
    remaining = 20;
    do {
        first = row - 0x103u;
        if ((s8)B(row - 0xeau) >= 0 && first != selected) {
            if (first == D(self + 0xf59eu)) {
                cm_fn_006f46b0(first, 4, 0);
            } else {
                mode = B(row);
                if ((s8)mode < 2) {
                    value = B(row - 1u);
                    cm_fn_006f46b0(first, value, 2);
                } else if ((s8)mode > 9) {
                    value = B(row - 1u);
                    cm_fn_006f46b0(first, value, 9);
                }
            }
        }
        cm_fn_007160e0(first, 0);
        second = row + 0x21d5u;
        if ((s8)B(row + 0x21eeu) >= 0 && second != selected) {
            if (second == D(self + 0xf5a2u)) {
                cm_fn_006f46b0(second, 4, 11);
            } else {
                mode = B(row + 0x22d8u);
                if ((s8)mode < 2) {
                    value = B(row + 0x22d7u);
                    cm_fn_006f46b0(second, value, 2);
                } else if ((s8)mode > 9) {
                    value = B(row + 0x22d7u);
                    cm_fn_006f46b0(second, value, 9);
                }
            }
        }
        cm_fn_007160e0(second, 0);
        row += 0x1beu;
        remaining -= 1u;
    } while (remaining != 0);
}
