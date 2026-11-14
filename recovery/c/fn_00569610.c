/* fn_00569610 - Championship Manager 01/02 3.9.68 (CM0102/Game/cm0102.exe).
 *
 * Original body: preferred-image VA 0x00569610..0x005697d2 (450 bytes),
 * body SHA-256 b8c6f77d931977a245603c1fc360b97c5d9332aa86117cf60b14a01ae2b3d9f7.
 * Original image SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
 * preferred image base 0x00400000 (file offset = VA - 0x00400000).
 *
 * Entry: ECX is the receiver, there are no physical stack arguments and the
 * routine ends in a bare RET at 0x005697d1.  The only direct call site is
 * 0x0055f6ef; its caller overwrites the returned EAX at 0x0055f6f4
 * ("mov eax,[esi]"), so the return word is dead, but the path dependent EAX
 * is reproduced here exactly anyway:
 *   - both windows inside their range  -> EAX = 0
 *   - element count <= 0               -> EAX = 1 (the non-zero window flag)
 *   - loop finished                    -> EAX = number of performed iterations
 *
 * Frame: "sub esp,0x114" at 0x00569610, "push esi" at 0x00569616.  L is the
 * ESP value right after the SUB, so L = entry_ESP - 0x114.  Original locals:
 *   L+0x00  flag_100   (0x0056965e)  1 when the 0x00ae2a2c 100-wide window moved
 *   L+0x04  flag_1000  (0x00569636)  1 when the 0x00ae2a28 1000-wide window moved
 *   L+0x08  receiver   (0x00569619)  reloaded at 0x0056974b for cm_fn_0055f300
 *   L+0x0c  counter    (0x00569691)  loop counter
 *   L+0x10  scratch    (0x005696ec)  format destination and cm_fn_005eed50 arg1
 * The scratch block reaches L+0x114, so it is 0x104 (260) bytes.
 *
 * All four direct callees stay explicit original boundaries:
 *   0x0090c510  E9 thunk to 0x00947299, regparm(3) entry EAX/EDX/ECX
 *   0x00944c9f  cdecl varargs formatter, five physical words, caller cleanup
 *   0x005eed50  ECX receiver plus two physical words, callee cleanup (ret 8)
 *   0x0055f300  ECX receiver plus four physical words, callee cleanup (ret 0x10)
 */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))

/* Volatile views of object fields reached through a runtime address.  The
 * original loads them with MOV, so the reads stay ordered and are not
 * cached across the calls that may rewrite the object. */
#define OD(a) (*(volatile u32 *)(u32)(a))
#define OW(a) (*(volatile u16 *)(u32)(a))
#define OB(a) (*(volatile u8 *)(u32)(a))

/* 0x0090c510 is an E9 thunk to 0x00947299: the original bytes at
 * 0x0090c510 are e9 84 ad 03 00, a plain JMP (0x0090c510 + 5 + 0x0003ad84
 * = 0x00947299) that preserves every register.  The accepted register
 * boundary is regparm(3): EAX, EDX and ECX on entry.
 *
 * Both leading words are DEAD INPUTS of the callee, and that is established
 * from the original callee instructions, not from a (void) cast in any
 * accepted C:
 *   EAX - the first instruction of 0x00947299 that touches EAX is
 *          LEA EAX,[EBP-8] at 0x0094729e (8d 45 f8), which overwrites EAX
 *          before any read on every path, so the incoming EAX is never read.
 *   EDX - the first read of EDX in the body is SBB EDX,DWORD PTR
 *          [0x00de2ff4] at 0x009472c0 (1b 15 f4 2f de 00), and that read
 *          happens after CALL 0x00945e40 at 0x009472b2 (e8 89 eb ff ff).
 *          That callee is __allmul and it DEFINES EDX on every path: both
 *          of its multiplies are 32-bit MUL, which writes EDX:EAX and zeroes
 *          EDX (0x00945e54 "mul ecx" and 0x00945e5a/0x00945e62/0x00945e6c).
 *          The EDX that reaches 0x009472c0 is therefore the product high
 *          word, never the caller's garbage.
 * The C therefore passes 0 in the two dead slots; that is a faithful
 * realisation of proven-dead inputs, not an invented register default.
 * cm_fn_00947299 reads only ECX (the receiver) as a live input.
 * Only the low EAX of the 64-bit result is consumed (0x00569628). */
extern unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_0090c510(
    u32 eax_unused, u32 edx_unused, u32 ecx_receiver);

/* 0x00944c9f: destination at EBP+8, format at EBP+0xc, varargs pointer at
 * EBP+0x10, bare RET, caller cleans the five physical words (0x005696fb). */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

/* 0x005eed50: ECX receiver captured in EBP, physical arg1 at entry ESP+4 and
 * arg2 later, both exits are "ret 8".  Its return registers are overwritten
 * before use, so no return contract is invented. */
extern u32 CM_THISCALL cm_fn_005eed50(u32 self, u32 input, u32 update);

/* 0x0055f300: ECX receiver (MOV EBP,ECX at 0x0055f302), four physical stack
 * arguments, both exits are "ret 0x10".  Its internal POP ECX at 0x0055f327
 * reloads its own saved-ECX slot, so it does not touch the caller's argument
 * home and does not restore the incoming ECX. */
extern u32 CM_THISCALL cm_fn_0055f300(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_00569610(u32 self)
{
    u32 now, base_1000, base_100, flag_1000, flag_100;
    u32 index, element, element_flags, object;
    s32 limit;
    u32 scan, tail, left, right, diff;
    u32 offset, context;
    u32 arg1, arg2, arg3, arg4;
    u16 word_low, word_high;
    u8 left_byte, right_byte;
    u8 scratch[0x104];

    /* 0x0056961d: the clock thunk; only the low EAX is used afterwards. */
    now = (u32)cm_fn_0090c510(0u, 0u, self);

    /* 0x00569622..0x0056964d: 1000-wide window over the 0x00ae2a28 base.
     * JB/JAE are unsigned compares; ECX is bumped by 0x3e8 with 32-bit wrap. */
    base_1000 = D(0x00ae2a28u);
    if (now < base_1000 || now >= base_1000 + 1000u) {
        D(0x00ae2a28u) = now;
        flag_1000 = 1;
    } else {
        flag_1000 = 0;
    }

    /* 0x0056964d..0x00569675: 100-wide window over the 0x00ae2a2c base. */
    base_100 = D(0x00ae2a2cu);
    if (now < base_100 || now >= base_100 + 100u) {
        D(0x00ae2a2cu) = now;
        flag_100 = 1;
    } else {
        flag_100 = 0;
    }

    /* 0x00569675..0x00569683: with both windows unchanged there is nothing
     * to do; EAX holds 0 on this exit. */
    if (flag_1000 == 0 && flag_100 == 0)
        return 0;

    /* 0x00569689..0x00569699: the element count is a signed 16-bit word at
     * self+0x12e9a0; JLE leaves EAX holding the non-zero window flag (1). */
    if ((s16)W(self + 0x12e9a0u) <= 0)
        return 1;

    /* 0x005696a2: first element of the array at self+0xba9de. */
    element = self + 0xba9deu;
    index = 0;                       /* 0x00569691 */

    for (;;) {
        /* 0x005696a8..0x005696ab: the element flags word and the object
         * pointer, both reloaded on every iteration. */
        element_flags = OD(element - 0x74u);
        object = element - 0x80u;

        /* 0x005696ae: TEST BH,8 is the EBX mask 0x800 (bit 11). */
        if ((element_flags & 0x800u) != 0 && flag_1000 != 0) {
            /* 0x005696c3..0x005696d8: scan for the first 0x00 or 0x01 byte.
             * The byte load at 0x005696c3 stays live: XOR EAX,EAX clears only
             * EAX, and CMP CL,1 / TEST CL,CL consume the loaded byte. */
            scan = 0;
            for (;;) {
                left_byte = OB(element + scan);
                if (left_byte == 1 || left_byte == 0)
                    break;
                scan++;
            }

            /* 0x005696da..0x005696e4: a 0x01 separator becomes a 0x00. */
            if (OB(element + scan) == 1) {
                OB(element + scan) = 0;
                scan++;
            }

            /* 0x005696e5: ESI = element + scan, the text behind the
             * separator. */
            tail = element + scan;

            /* 0x005696e8..0x005696fb: "%s%c%s" (literal 0x0098b4c8) into the
             * frame scratch block; arguments are destination, literal, tail,
             * the immediate 1 and the element base, in that order. */
            cm_fn_00944c9f((u32)scratch, 0x0098b4c8u, (u32)tail, 1u, (u32)element);

            /* 0x005696fe..0x0056972b: compare the element text with the
             * formatted tail, two bytes per step.  The mismatch value is the
             * SBB pair at 0x00569724/0x00569726: -1 when the left byte is the
             * unsigned smaller one, +1 otherwise; only zero/non-zero is used
             * at 0x00569729. */
            left = element;
            right = tail;
            diff = 0;
            for (;;) {
                left_byte = OB(left);
                right_byte = OB(right);
                if (left_byte != right_byte) {
                    diff = (left_byte < right_byte) ? 0xffffffffu : 1u;
                    break;
                }
                if (left_byte == 0) {
                    diff = 0;
                    break;
                }
                left_byte = OB(left + 1u);
                right_byte = OB(right + 1u);
                if (left_byte != right_byte) {
                    diff = (left_byte < right_byte) ? 0xffffffffu : 1u;
                    break;
                }
                left += 2u;
                right += 2u;
                if (left_byte == 0) {
                    diff = 0;
                    break;
                }
            }

            /* 0x0056972b..0x00569754: a non-zero difference updates the
             * object.  0x00569754 restores ESI to the receiver. */
            if (diff != 0) {
                /* 0x0056972d..0x00569736: the same frame buffer is the
                 * physical arg1 of cm_fn_005eed50; the receiver is EBP. */
                cm_fn_005eed50(object, (u32)scratch, 0);

                /* 0x0056973b..0x0056974f: the four words of the object are
                 * read in the original order (0x1c, 0x18, 0x14, 0x10) and
                 * pushed in that order, so the callee sees them as
                 * arg1..arg4 = +0x10, +0x14, +0x18, +0x1c. */
                arg4 = OD(object + 0x1cu);
                arg3 = OD(object + 0x18u);
                arg2 = OD(object + 0x14u);
                arg1 = OD(object + 0x10u);
                cm_fn_0055f300(self, arg1, arg2, arg3, arg4);
            }
        }

        /* 0x00569758: TEST BH,0x10 is the EBX mask 0x1000 (bit 12). */
        if ((element_flags & 0x1000u) != 0 && flag_100 != 0) {
            /* 0x00569765..0x00569771: swap the two words of the object. */
            word_low = OW(object + 0x72u);
            word_high = OW(object + 0x78u);
            OW(object + 0x72u) = word_high;
            OW(object + 0x78u) = word_low;

            /* 0x00569775..0x00569790: table slot lookup, index taken from the
             * signed 16-bit global 0x00b77c7a and scaled by 3 then by 256. */
            offset = (u32)((s32)(s16)W(0x00b77c7au) * 3);
            offset = offset << 8;
            context = OD(0x00b74c4cu + offset);
            if (OD(context + 0x12f545u) != 0) {
                /* 0x00569792..0x005697a4: the same four object words and the
                 * same argument order as the 0x0056974f call. */
                arg4 = OD(object + 0x1cu);
                arg3 = OD(object + 0x18u);
                arg2 = OD(object + 0x14u);
                arg1 = OD(object + 0x10u);
                cm_fn_0055f300(self, arg1, arg2, arg3, arg4);
            }
        }

        /* 0x005697a9..0x005697c1: advance to the next element and reload the
         * signed count word before the signed compare; the counter is
         * incremented first, so a stable positive count of N runs N times.
         *
         * Signedness is taken from the original bytes, not chosen to silence
         * the compiler.  At 0x005697b3 the count is loaded with MOVSX
         * ECX,WORD PTR [ESI+0x12e9a0] (0f bf 8e a0 e9 12 00), so ECX is a
         * sign-extended 16-bit value in a signed 32-bit register; the loop
         * test at 0x005697c1 is JL (0f 8c), a signed less-than.  CMP at
         * 0x005697bb (3b c1) therefore compares two signed 32-bit operands:
         * the 32-bit counter EAX against the sign-extended count ECX.  Both
         * sides of the C compare are consequently signed: the counter is cast
         * to s32 and the limit is held in an s32 local, so the C compare is
         * the same signed comparison the original performs.  The unsigned
         * reading of the count is rejected by the original itself: the
         * entry test at 0x00569699 is JLE (0f 8e) on the same word, so the
         * count is signed there as well. */
        element += 0x18cu;
        limit = (s32)(s16)W(self + 0x12e9a0u);
        index += 1u;
        if (!((s32)index < limit))
            break;
    }

    /* 0x005697a9..0x005697c1: the final EAX is the incremented counter. */
    return index;
}
