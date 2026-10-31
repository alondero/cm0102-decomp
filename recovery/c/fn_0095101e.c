/* WIP: `cmre brief fn_0095101e` fails before producing a C draft with
   KeyError: WIDTH[3] in the converter. `cmre show 0x0095101e --asm` still
   provides the listing: this is an x87 routine using FCOMP/FNSTSW/SAHF,
   subnormal bit normalization, and the explicit 0x00950f9b helper boundary.
   No test was run and no numerical behavior is inferred here. */
#include "cmre.h"
