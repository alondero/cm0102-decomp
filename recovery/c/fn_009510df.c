/* Preferred-image VA 0x009510df, image base 0x00400000, CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The two direct call sites consume ST(0); their integer return paths write
   EAX independently. The volatile double preserves the native FSTP/FLD pair. */
#include "cmre.h"

double CM_CDECL cm_fn_009510df(double value)
{
    volatile double rounded = __builtin_rint(value);
    return rounded;
}
