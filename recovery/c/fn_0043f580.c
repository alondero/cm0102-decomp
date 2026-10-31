#include "cmre.h"

/* WIP: original leaves a live x87 ST0 (FILD m32 at 0x0043F588 times FMUL m64 at 0x0043F58E) across the direct call to 0x00945640 (__ftol-like, no-argument, consumes ST0). Plain C has no transport that keeps ST0 live across that call, and C that reconverts the value would replace the callee. The second conversion path at 0x0043F5FA (FILD m64) and 0x0043F5FE (FDIV m64) uses x87 forms outside cmre/x87_transport.py's supported list (FILD m32, FSTP m64, FMUL m64, FCOMP m64, FNSTSW AX). No test has been run. Needs a parent decision on original_x87 transport for these sites. */
