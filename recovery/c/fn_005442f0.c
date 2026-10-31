/* WIP: the listing tail-jumps to 0x00945640 (__ftol) with its calculated value
   in implicit x87 ST0. Plain C has no supported way to pass that live ST0 value
   to the original boundary. Do not substitute a guessed integer or floating
   parameter. The original ends at the __ftol tail jump. */
#include "cmre.h"
