#include "cmre.h"

/* WIP: the second 4a3250 call leaves a value in x87 ST0 which is consumed by
   the no-argument 945640 conversion helper. Plain C has no supported way to
   pass that live ST0 value across the original helper boundary. */
