#include "cmre.h"

/* WIP: original VA 0x0093de35 executes REP STOSD without clearing DF. The
   0x95-DWORD initialization direction and resulting memory accesses depend on
   incoming EFLAGS.DF. Plain C cannot preserve that state here; no forward fill
   is assumed. */
