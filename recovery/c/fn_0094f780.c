#include "cmre.h"

/* WIP: the final REP STOSD at original VA 0x0094f802 depends on incoming
 * EFLAGS.DF. The C boundary does not transport DF, so a forward or backward
 * C loop would assume machine state that the original routine preserves. */
