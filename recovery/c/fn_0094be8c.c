/* Translation of preferred-image VA 0x0094be8c (101 bytes) from the
   3.9.68 primary executable (image base 0x00400000).

   Listing-derived behaviour:
     - the routine reads ECX before writing it (the scratch `push ecx`
       pushes) but never uses that value, so ECX carries no input; the
       double argument is the first stack slot, read at [ebp+8].
     - it calls cm_fn_009510f1 with the argument double and inspects bits
       0x90 of the returned word; a set bit returns 0.
     - it calls cm_fn_009510df with the argument double, stores the result
       and compares it with the argument (fcomp / fnstsw / sahf); an unequal
       comparison returns 0.
     - it divides the argument by the double global at 0x00968950, stores the
       quotient back into the argument slot, then calls cm_fn_009510df with
       that quotient and compares the result with the quotient: equal yields
       2, unequal yields 1.
     - the fdiv runs in x87 extended precision and is rounded to a double by
       the following fstp, hence the long double intermediate.
     - [ebp+8] is the caller's first argument slot, not a private local: the
       original writes the quotient back into it (0x0094becc), reloads it
       (0x0094becf) and compares against it (0x0094beda). The recovered C
       therefore keeps that slot itself as a volatile home and repeats the
       original's reads and its write-through through the home; a plain local
       copy would drop the store the original makes in the caller's frame. */
#include "cmre.h"

extern double CM_CDECL cm_fn_009510df(double value);
extern u32 CM_CDECL cm_fn_009510f1(double value);

/* The original ends in `leave; ret`: it leaves the stack argument to the
   caller, so the argument is not callee-cleaned. */
u32 CM_CDECL cm_fn_0094be8c(double param_2)
{
    u32 status;
    double result;
    double slot;
    long double quotient;
    u32 frame;
    volatile double *home;

    /* 0x0094be8c..0x0094be90: push ebp; mov ebp, esp. [ebp+8] is the
       caller's first argument slot, so the home below is that slot and not a
       local of this frame. */
    frame = (u32)(unsigned long)__builtin_frame_address(0);
    home = (volatile double *)(frame + 8u);

    /* 0x0094be91..0x0094bea2: the argument goes to cm_fn_009510f1; the
       returned word's bits 0x90 decide the early zero return. */
    status = cm_fn_009510f1(param_2);
    if ((status & 0x90) != 0) {
        return 0u;
    }

    /* 0x0094bea4..0x0094bebf: the slot goes to cm_fn_009510df and its result
       is compared with the slot (fcomp / fnstsw / sahf). fcomp sets ZF for
       equal and for unordered, so only a strictly ordered difference takes
       the branch to the zero return. */
    result = cm_fn_009510df(*home);
    slot = *home;
    if (result > slot || result < slot) {
        return 0u;
    }

    /* 0x0094bec1..0x0094becc: fld [ebp+8]; fdiv [0x968950]; fstp [ebp+8].
       The store rounds the extended quotient to a double and writes it into
       the caller's argument slot. */
    quotient = (long double)*home / (long double)CM_GLOBAL(double, 0x968950);
    *home = (double)quotient;

    /* 0x0094becf..0x0094bed5: the stored quotient is reloaded from the slot
       and passed to cm_fn_009510df. */
    slot = *home;
    result = cm_fn_009510df(slot);

    /* 0x0094beda..0x0094bee2: the result is compared with the slot again; an
       ordered difference returns 1, equality (and unordered) returns 2. */
    slot = *home;
    if (result > slot || result < slot) {
        return 1u;
    }
    return 2u;
}
