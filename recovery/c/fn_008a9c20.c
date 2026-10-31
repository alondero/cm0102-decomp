/* WIP: original REP MOVSD at VA 0x008a9c4c and 0x008a9c83, REP MOVSW at
   0x008a9c5b/0x008a9c85, and REP MOVSB at 0x008a9c5d/0x008a9c87 use the
   incoming direction flag. The plain-C boundary does not transport DF, so
   forward C copies cannot represent DF-set inputs. */
#include "cmre.h"
