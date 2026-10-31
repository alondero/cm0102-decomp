/* WIP: the caller must read the ESP relocated by cm_fn_00945610 after its
   page-by-page stack probes. Plain C has no supported current-ESP access; an
   alloca substitute would change the probe reads, faults, and returned buffer.
   Keep the original helper as an explicit boundary under C-BOUNDARIES.md. */
#include "cmre.h"
