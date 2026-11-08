/* Preferred-image VA 0x00950f9b, image base 0x00400000, CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Integer argument bits preserve the input high word before x87 quieting.
   The native caller consumes ST(0) and overwrites incidental integer outputs.
   Disable SSE copies so both builds retain FLD/FSTP before the word store. */
#include "cmre.h"

__attribute__((target("no-sse,no-sse2")))
double CM_CDECL cm_fn_00950f9b(unsigned long long input_bits, u32 exponent)
{
    union { unsigned long long bits; double number; } input;
    volatile union { double number; u16 word[4]; } output;
    u16 original_high = (u16)(input_bits >> 48);

    input.bits = input_bits;
    output.number = input.number;
    output.word[3] = (u16)(((exponent + 0x3feu) << 4) |
                           (original_high & 0x800fu));
    return output.number;
}
