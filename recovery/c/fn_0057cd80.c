/* WIP: VA 0x0057cd94 and 0x0057cda6 call __ftol (0x00945640) with the
   preceding long-double result still in ST0. Its original FISTP conversion
   also temporarily changes the x87 control word. Plain C cannot transport
   that implicit ST0 value across the typed call boundary. */
#include "cmre.h"
