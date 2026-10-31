#include "cmre.h"

/* WIP: the conditional path leaves a computed value in x87 ST0 and calls
   0x00945640 (__ftol). Plain C has no supported way to pass that live ST0
   value across the original boundary. No guessed integer or floating
   argument is substituted here. */
